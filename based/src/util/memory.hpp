#pragma once

#include <iostream>
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <future>

class memory_handle
{
public:
	/**
	 * \brief Constructs the object with a pointer
	 * \param p The pointer to initialize the object with, optional
	 */
	constexpr memory_handle(void* p = nullptr) :
		m_ptr(p)
	{}

	/**
	 * \brief Constructs the object with an integral pointer
	 * \param p The integral pointer to initialize the object with
	 */
	explicit memory_handle(std::uintptr_t p) :
		m_ptr(reinterpret_cast<void*>(p))
	{}

	/**
	 * \brief Retrieves the pointer as a pointer
	 * \return A pointer
	 */
	template <typename T>
	constexpr std::enable_if_t<std::is_pointer_v<T>, T> as()
	{
		return static_cast<T>(m_ptr);
	}

	/**
	 * \brief Retrieves the pointer as a reference
	 * \return A pointer
	 */
	template <typename T>
	constexpr std::enable_if_t<std::is_lvalue_reference_v<T>, T> as()
	{
		return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(m_ptr);
	}

	/**
	 * \brief Retrieves the pointer as an integral pointer
	 * \return An integral pointer
	 */
	template <typename T>
	constexpr std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> as()
	{
		return reinterpret_cast<T>(m_ptr);
	}

	/**
	 * \brief adds an offset to the pointer
	 * \param offset The offset to be added
	 * \return memory_handle
	 */
	template <typename T>
	constexpr memory_handle add(T offset)
	{
		return memory_handle(as<std::uintptr_t>() + offset);
	}

	/**
	 * \brief subtracted an offset to the pointer
	 * \param offset The offset to be subtracted
	 * \return memory_handle
	 */
	template <typename T>
	constexpr memory_handle sub(T offset)
	{
		return memory_handle(as<std::uintptr_t>() - offset);
	}

	/**
	 * \brief Gets the relative displacement of the pointer
	 * \return memory_handle
	 */
	constexpr memory_handle rip()
	{
		if (!m_ptr)
			return nullptr;
		return add(as<std::int32_t&>()).add(4U);
	}

	/**
	 * \brief Checks if the stored pointer is valid
	 * \return bool
	 */
	constexpr explicit operator bool() noexcept
	{
		return m_ptr;
	}
protected:
	void* m_ptr;
};

class memory_region
{
public:
	/**
	 * \brief Constructs the region
	 * \param base The base of the region
	 * \param size The size of the region
	 */
	constexpr explicit memory_region(memory_handle base, std::size_t size) :
		m_base(base),
		m_size(size)
	{}

	/**
	 * \brief Gets the base of the region
	 * \return memory_handle
	 */
	constexpr memory_handle base()
	{
		return m_base;
	}

	/**
	 * \brief Gets the end of the region
	 * \return memory_handle
	 */
	constexpr memory_handle end()
	{
		return m_base.add(m_size);
	}

	/**
	 * \brief Gets the size of the region
	 * \return std::size_t
	 */
	constexpr std::size_t size()
	{
		return m_size;
	}

	/**
	 * \brief Checks if p is within this region
	 * \param p The pointer
	 * \return bool
	 */
	constexpr bool contains(memory_handle p)
	{
		if (p.as<std::uintptr_t>() < m_base.as<std::uintptr_t>())
			return false;
		if (p.as<std::uintptr_t>() > m_base.add(m_size).as<std::uintptr_t>())
			return false;

		return true;
	}
protected:
	memory_handle m_base;
	std::size_t m_size;
};

class module : public memory_region
{
public:
	/**
	 * \brief Constructs the class with the main modue
	 */
	explicit module(std::nullptr_t) :
		module(static_cast<char*>(nullptr))
	{}

	/**
	 * \brief Constructs the class with a module name
	 * \param name The name of the module
	 */
	explicit module(const char* name) :
		module(GetModuleHandleA(name))
	{
	}

	/**
	 * \brief Constructs the class with a module base
	 */
	module(HMODULE hmod) :
		memory_region(hmod, 0)
	{
		auto dos_header = m_base.as<IMAGE_DOS_HEADER*>();
		auto nt_header = m_base.add(dos_header->e_lfanew).as<IMAGE_NT_HEADERS64*>();
		m_size = nt_header->OptionalHeader.SizeOfImage;
	}

	/**
	 * \brief Gets the DOS headers for the module
	 */
	IMAGE_DOS_HEADER* get_dos_headers()
	{
		return m_base.as<IMAGE_DOS_HEADER*>();
	}

	/**
	 * \brief Gets the NT headers for the module
	 */
	IMAGE_NT_HEADERS64* get_nt_headers()
	{
		return m_base.add(m_base.as<IMAGE_DOS_HEADER*>()->e_lfanew).as<IMAGE_NT_HEADERS64*>();
	}
private:
	/**
	 * \brief Transforms a relative virtual address to a virtual address
	 */
	template <typename t_return, typename t_offset>
	t_return get_rva(t_offset rva)
	{
		return m_base.add(rva).as<t_return>();
	}
};

class vmt_hook
{
public:
	vmt_hook(vmt_hook&& that) = delete;
	vmt_hook& operator=(vmt_hook&& that) = delete;
	vmt_hook(vmt_hook const&) = delete;
	vmt_hook& operator=(vmt_hook const&) = delete;

	explicit vmt_hook(void* obj, std::size_t num_funcs) :
		m_object(static_cast<void***>(obj)),
		m_num_funcs(num_funcs + 1),
		m_original_table(*m_object),
		m_new_table(std::make_unique<void* []>(m_num_funcs))
	{
		std::copy_n(m_original_table - 1, m_num_funcs, m_new_table.get());
	}

	void hook(std::size_t index, void* func)
	{
		m_new_table[index + 1] = func;
	}

	void unhook(std::size_t index)
	{
		m_new_table[index + 1] = m_original_table[index];
	}

	void enable()
	{
		*m_object = m_new_table.get() + 1;
	}

	void disable()
	{
		*m_object = m_original_table;
	}

	template <typename T>
	T get_original(std::size_t index)
	{
		return static_cast<T>(m_original_table[index]);
	}

private:
	void*** m_object;
	std::size_t m_num_funcs;

	void** m_original_table;
	std::unique_ptr<void* []> m_new_table;
};

class signature
{
public:
	struct element
	{
		std::uint8_t m_data{};
		bool m_wildcard{};
	};

	/**
	 * \brief Constructs the signature with an IDA pattern
	 * \param pattern The IDA pattern string
	 */
	explicit signature(const char* pattern)
	{
		auto to_upper = [](char c) -> char
		{
			return c >= 'a' && c <= 'z' ? static_cast<char>(c + ('A' - 'a')) : static_cast<char>(c);
		};

		auto is_hex = [&](char c) -> bool
		{
			switch (to_upper(c))
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				return true;
			default:
				return false;
			}
		};

		do
		{
			if (*pattern == ' ')
				continue;
			if (*pattern == '?')
			{
				m_elements.push_back(element{ {}, true });
				continue;
			}

			if (*(pattern + 1) && is_hex(*pattern) && is_hex(*(pattern + 1)))
			{
				char str[3] = { *pattern, *(pattern + 1), '\0' };
				auto data = std::strtol(str, nullptr, 16);

				m_elements.push_back(element{ static_cast<std::uint8_t>(data), false });
			}
		} while (*(pattern++));
	}

	/**
	 * \brief scans for the pattern in a memory region
	 * \param region The region to search in, default is the main module
	 * \return memory_handle
	 */
	memory_handle scan()
	{
		std::future<memory_handle> future_result = std::async(std::launch::async, [&]() {
			memory_region region = module(nullptr);

			auto compare_memory = [](std::uint8_t* data, element* elem, std::size_t num) -> bool
			{
				for (std::size_t i = 0; i < num; ++i)
				{
					if (!elem[i].m_wildcard)
						if (data[i] != elem[i].m_data)
							return false;
				}

				return true;
			};

			for (std::uintptr_t i = region.base().as<std::uintptr_t>(), end = region.end().as<std::uintptr_t>(); i != end; ++i)
			{
				if (compare_memory(reinterpret_cast<std::uint8_t*>(i), m_elements.data(), m_elements.size()))
				{
					return memory_handle(i);
				}
			}

			return memory_handle();
			});

		return future_result.get();
	}

private:
	std::vector<element> m_elements;
};



