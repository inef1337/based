#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <format>
#include <string>
#include <Windows.h>
#include <Psapi.h>
#include <format>
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>

#include "logger.hpp"

/*
ExceptionInformation[0]: The type of access violation that caused the exception. This value is used only for EXCEPTION_ACCESS_VIOLATION exceptions. The possible values are:

0: The thread attempted to read inaccessible data.
1: The thread attempted to write to an inaccessible area of memory.
8: The thread attempted to execute data that is marked as non-executable.
ExceptionInformation[1]: The memory address that caused the exception. For an access violation, this is the address that the thread attempted to read from or write to.

ExceptionInformation[2]: This value is used only for EXCEPTION_IN_PAGE_ERROR exceptions. It indicates the type of operation that caused the page fault. The possible values are the same as for ExceptionInformation[0].

ExceptionInformation[3]: The address of the inaccessible data that caused the page fault. This value is used only for EXCEPTION_IN_PAGE_ERROR exceptions.

ExceptionInformation[4]: The NTSTATUS code for the exception. This value is used only for EXCEPTION_IN_PAGE_ERROR exceptions. It indicates the status of the operation that caused the page fault.

ExceptionInformation[5]: This value is used only for EXCEPTION_NONCONTINUABLE_EXCEPTION exceptions. It is a flag that indicates whether the exception is continuable (i.e., whether execution can continue after the exception is handled).

ExceptionInformation[6] through ExceptionInformation[14]: Reserved for future use.
*/

namespace based {

    enum class exception_access_vioaltion_types : ULONG_PTR {
        READ_INACCESSIBLE_DATA = 0,
        WRITE_INACCESSIBLE_DATA = 1,
        EXECUTE_NON_EXECUTABLE_DATA = 8
    };

    enum class exception_in_page_error_types : unsigned int {
        READ_ACCESS = 0,
        WRITE_ACCESS = 1,
        EXECUTE_ACCESS = 8,
        GUARD_PAGE = 0x80000001,
        DEP = 0x80000002
    };

    inline std::string get_stack_trace()
    {
        std::ostringstream stack_trace{};
        stack_trace << "[";
        constexpr int max_callers = 62;

        const auto capture_stack_back_trace = [](ULONG frames_to_skip, ULONG frames_to_capture, PVOID* call_stack, PULONG frames_captured) -> USHORT {
            typedef USHORT(WINAPI* capture_stack_back_trace_type)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
            static const auto capture_stack_back_trace_fn = reinterpret_cast<capture_stack_back_trace_type>(GetProcAddress(LoadLibraryA("kernel32.dll"), "RtlCaptureStackBackTrace"));
            if (!capture_stack_back_trace_fn) { return 0; }
            return capture_stack_back_trace_fn(frames_to_skip, frames_to_capture, call_stack, frames_captured);
        };

        PVOID call_stack[max_callers];
        const int count = capture_stack_back_trace(1, max_callers, call_stack, nullptr);

        for (int i = 0; i < count; i++) {
            const auto caller_addr = reinterpret_cast<uintptr_t>(call_stack[i]);
            const auto flags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT;
            HMODULE current_caller_module = nullptr;

            if (GetModuleHandleEx(flags, reinterpret_cast<LPCSTR>(caller_addr), &current_caller_module)) {
                char module_name[MAX_PATH];
                if (GetModuleBaseNameA(GetCurrentProcess(), current_caller_module, module_name, MAX_PATH)) {
                    stack_trace << "0x" << std::hex << (caller_addr - reinterpret_cast<uintptr_t>(current_caller_module)) << " [" << module_name << "]" << (i != count - 1 ? ", " : "");
                }
                else {
                    stack_trace << "0x" << std::hex << caller_addr << (i != count - 1 ? ", " : "");
                }
            }
            else {
                stack_trace << "0x" << std::hex << caller_addr << (i != count - 1 ? ", " : "");
            }
        }
        stack_trace << "]";

        return stack_trace.str();
    }

    inline LONG WINAPI exception_handler(EXCEPTION_POINTERS* exc_info) {

        const auto exc_code = exc_info->ExceptionRecord->ExceptionCode;
        const auto exc_address = exc_info->ExceptionRecord->ExceptionAddress;
        const auto exc_access_violation = exc_info->ExceptionRecord->ExceptionInformation[0];
        const auto exc_access_address = exc_info->ExceptionRecord->ExceptionInformation[1];
        const auto exc_in_page = exc_info->ExceptionRecord->ExceptionInformation[2];
        const auto* context = exc_info->ContextRecord;
        const auto stack_trace = get_stack_trace();

        global::console->log_excp("______________________________");
        global::console->log_excp("Game crashed with code 0x{}", exc_code);
        global::console->log_excp("Game crashed at {}", exc_address);
        switch (exc_code) {
        case EXCEPTION_ACCESS_VIOLATION:
            switch (exc_access_violation) {
            case static_cast<ULONG_PTR>(exception_access_vioaltion_types::READ_INACCESSIBLE_DATA):
                global::console->log_excp("Cause: EXCEPTION_ACCESS_VIOLATION-0");
                global::console->log_excp("Attempted to read from: 0x{}", exc_access_address);
                break;
            case static_cast<ULONG_PTR>(exception_access_vioaltion_types::WRITE_INACCESSIBLE_DATA):
                global::console->log_excp("Cause: EXCEPTION_ACCESS_VIOLATION-1");
                global::console->log_excp("Attempted to write to: 0x{}", exc_access_address);
                break;
            case static_cast<ULONG_PTR>(exception_access_vioaltion_types::EXECUTE_NON_EXECUTABLE_DATA):
                global::console->log_excp("Cause: EXCEPTION_ACCESS_VIOLATION-8 at 0x{}", exc_access_address);
                break;
            default:
                global::console->log_excp("Cause: EXCEPTION_ACCESS_VIOLATION-unknown_type at 0x{}", exc_access_address);
                break;
            }
            break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: global::console->log_excp("Cause: EXCEPTION_ARRAY_BOUNDS_EXCEEDED"); break;
        case EXCEPTION_BREAKPOINT: global::console->log_excp("Cause: EXCEPTION_BREAKPOINT"); break;
        case EXCEPTION_DATATYPE_MISALIGNMENT: global::console->log_excp("Cause: EXCEPTION_DATATYPE_MISALIGNMENT"); break;
        case EXCEPTION_FLT_DENORMAL_OPERAND: global::console->log_excp("Cause: EXCEPTION_FLT_DENORMAL_OPERAND"); break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO: global::console->log_excp("Cause: EXCEPTION_FLT_DIVIDE_BY_ZERO"); break;
        case EXCEPTION_FLT_INEXACT_RESULT: global::console->log_excp("Cause: EXCEPTION_FLT_INEXACT_RESULT"); break;
        case EXCEPTION_FLT_INVALID_OPERATION: global::console->log_excp("Cause: EXCEPTION_FLT_INVALID_OPERATION"); break;
        case EXCEPTION_FLT_OVERFLOW: global::console->log_excp("Cause: EXCEPTION_FLT_OVERFLOW"); break;
        case EXCEPTION_FLT_STACK_CHECK: global::console->log_excp("Cause: EXCEPTION_FLT_STACK_CHECK"); break;

        case EXCEPTION_FLT_UNDERFLOW: global::console->log_excp("Cause: EXCEPTION_FLT_UNDERFLOW"); break;
        case EXCEPTION_ILLEGAL_INSTRUCTION: global::console->log_excp("Cause: EXCEPTION_ILLEGAL_INSTRUCTION"); break;
        case EXCEPTION_IN_PAGE_ERROR:
            switch (exc_in_page) {
            case static_cast<unsigned int>(exception_in_page_error_types::READ_ACCESS):
                global::console->log_excp("Cause: EXCEPTION_IN_PAGE_ERROR-0");
                global::console->log_excp("Attempted to read from: 0x{}", exc_access_address);
                break;
            case static_cast<unsigned int>(exception_in_page_error_types::WRITE_ACCESS):
                global::console->log_excp("Cause: EXCEPTION_IN_PAGE_ERROR-1");
                global::console->log_excp("Attempted to write to: 0x{}", exc_access_address);
                break;
            case static_cast<unsigned int>(exception_in_page_error_types::EXECUTE_ACCESS):
                global::console->log_excp("Cause: EXCEPTION_IN_PAGE_ERROR-8 at 0x{}", exc_access_address);
                break;
            case static_cast<unsigned int>(exception_in_page_error_types::GUARD_PAGE):
                global::console->log_excp("Cause: EXCEPTION_IN_PAGE_ERROR-0x{}80000001 at 0x{}", exc_access_address);
                break;
            case static_cast<unsigned int>(exception_in_page_error_types::DEP):
                global::console->log_excp("Cause: EXCEPTION_IN_PAGE_ERROR-0x{}80000002 at 0x{}", exc_access_address);
                break;
            default:
                global::console->log_excp("Cause: EXCEPTION_IN_PAGE_ERROR-unknown_type at 0x{}", exc_access_address);
                break;
            }
            break;

        case EXCEPTION_INT_DIVIDE_BY_ZERO: global::console->log_excp("Cause: EXCEPTION_INT_DIVIDE_BY_ZERO"); break;
        case EXCEPTION_INT_OVERFLOW: global::console->log_excp("Cause: EXCEPTION_INT_OVERFLOW"); break;
        case EXCEPTION_INVALID_DISPOSITION: global::console->log_excp("Cause: EXCEPTION_INVALID_DISPOSITION"); break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: global::console->log_excp("Cause: EXCEPTION_NONCONTINUABLE_EXCEPTION"); break;
        case EXCEPTION_PRIV_INSTRUCTION: global::console->log_excp("Cause: EXCEPTION_PRIV_INSTRUCTION"); break;
        case EXCEPTION_STACK_OVERFLOW: global::console->log_excp("Cause: EXCEPTION_STACK_OVERFLOW"); break;
        default: global::console->log_excp("Cause: unknown exception"); break;
        }

        global::console->log_excp("______________________________");
        global::console->log_excp("RAX: 0x{}", context->Rax);
        global::console->log_excp("RSI: 0x{}", context->Rsi);
        global::console->log_excp("RBX: 0x{}", context->Rbx);
        global::console->log_excp("RDI: 0x{}", context->Rdi);
        global::console->log_excp("RCX: 0x{}", context->Rcx);
        global::console->log_excp("RBP: 0x{}", context->Rbp);
        global::console->log_excp("RDX: 0x{}", context->Rdx);
        global::console->log_excp("RSP: 0x{}", context->Rsp);
        global::console->log_excp("R8: 0x{}", context->R8);
        global::console->log_excp("R9: 0x{}", context->R9);
        global::console->log_excp("R10: 0x{}", context->R10);
        global::console->log_excp("R11: 0x{}", context->R11);
        global::console->log_excp("R12: 0x{}", context->R12);
        global::console->log_excp("R13: 0x{}", context->R13);
        global::console->log_excp("R14: 0x{}", context->R14);
        global::console->log_excp("R15: 0x{}", context->R15);
        global::console->log_excp("______________________________");
        global::console->log_excp("stack trace: {}", stack_trace);
        global::console->log_excp("______________________________");
        system("pause");

        return EXCEPTION_CONTINUE_SEARCH;
    }
}
