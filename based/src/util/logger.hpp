#pragma once

#include <iostream>
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <memory>

namespace based::global
{
	enum class log_color : std::uint16_t
	{
		red = FOREGROUND_RED,
		green = FOREGROUND_GREEN,
		blue = FOREGROUND_BLUE,
		intensify = FOREGROUND_INTENSITY
	};

	inline log_color operator|(log_color a, log_color b)
	{
		return static_cast<log_color>(static_cast<std::underlying_type_t<log_color>>(a) | static_cast<std::underlying_type_t<log_color>>(b));
	}

	class logger
	{
	public:
		explicit logger() :
			m_file_path("C:\\based")
		{
			try
			{
				if (!std::filesystem::exists(m_file_path))
				{
					std::filesystem::create_directory(m_file_path);
				}
				else if (!std::filesystem::is_directory(m_file_path))
				{
					std::filesystem::remove(m_file_path);
					std::filesystem::create_directory(m_file_path);
				}

				m_file_path /= "based.log";
				m_file_out.open(m_file_path, std::ios_base::out | std::ios_base::app);
			}
			catch (std::filesystem::filesystem_error const&)
			{
			}

			if ((m_did_console_exist = AttachConsole(GetCurrentProcessId())) == false)
				AllocConsole();

			if ((m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE)) != nullptr)
			{
				//console size
				const auto console = GetConsoleWindow();
				RECT r;
				GetWindowRect(console, &r);
				MoveWindow(console, r.left, r.top, 550, 350, TRUE);

				//console font
				CONSOLE_FONT_INFOEX cfi{};
				cfi.cbSize = sizeof(cfi);
				cfi.nFont = 0;
				cfi.dwFontSize.X = 0;
				cfi.dwFontSize.Y = 14;
				cfi.FontFamily = FF_SWISS;
				cfi.FontWeight = FW_NORMAL;
				wcscpy_s(cfi.FaceName, L"Consolas");

				SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

				//console background
				SetLayeredWindowAttributes(console, NULL, 255, LWA_ALPHA);

				SetConsoleTitleA("based");
				SetConsoleOutputCP(CP_UTF8);

				m_console_out.open("CONOUT$", std::ios_base::out | std::ios_base::app);
			}
		}

		~logger()
		{
			if (!m_did_console_exist)
				FreeConsole();
		}

		template <typename... args>
		void log_to_console(log_color color, std::string_view type, std::string_view str, args&&... vars)
		{
			if (m_console_out && m_console_handle)
			{
				const auto now = std::chrono::system_clock::now();
				const auto now_c = std::chrono::system_clock::to_time_t(now);
				tm now_tm;
				localtime_s(&now_tm, &now_c);

				SetConsoleTextAttribute(m_console_handle, static_cast<std::uint16_t>(color));
				m_console_out << "[" << std::put_time(&now_tm, "%H:%M:%S") << "] " << type << " -> " << std::vformat(str, std::make_format_args(vars...)) << std::endl;
			}
		}

		template <typename... args>
		void log_to_file(std::string_view str, args&&... vars)
		{
			if (m_file_out)
			{
				m_file_out << std::vformat(str, std::make_format_args(vars...)) << std::endl;
			}
		}

		template <typename ...args>
		inline void log(std::string_view format, args &...vars)
		{
			std::string_view type = "info";
			log_to_console(log_color::blue | log_color::intensify, type, format, vars...);
			log_to_file(format, vars...);
		}

		template <typename ...args>
		inline void log_excp(std::string_view format, args &...vars)
		{
			std::string_view type = "excp";
			log_to_console(log_color::red | log_color::intensify, type, format, vars...);
			log_to_file(format, vars...);
		}

	private:
		bool m_did_console_exist{};

		HANDLE m_console_handle{};
		std::ofstream m_console_out;

		std::filesystem::path m_file_path;
		std::ofstream m_file_out;
	};

	inline std::unique_ptr<logger> console;
}