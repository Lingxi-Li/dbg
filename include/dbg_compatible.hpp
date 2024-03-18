#pragma once
#include <Windows.h>

#include <sstream>
#include <utility>

template <typename T>
std::wostream& operator<(std::wostream& os, T&& v) {
    return os << std::forward<T>(v) << L' ';
}

namespace dbg {

    inline void break_into_debugger(bool condition = true) {
        if (IsDebuggerPresent()) return;
        if (!condition) return;
        while (!IsDebuggerPresent()) {
            Sleep(3 * 1000);
        }
        // Because `DebugBreak` is a call to a system function,
        // system debug symbols must be installed to ensure the
        // correct call stack information is displayed after breaking.
        // Otherwise, the call stack information displayed by the
        // debugger may be off by one frame. If you use `__debugbreak`,
        // symbols are not required.
        __debugbreak();
    }

    namespace impl {

        struct endl_guard {
            ~endl_guard() {
                OutputDebugStringW(L"\n");
            }
        };

        template <typename T>
        void pipe(std::wostringstream& os, T&& v) {
            os << std::forward<T>(v);
        }

        template <typename T, typename... Ts>
        void pipe(std::wostringstream& os, T&& v, Ts&&... args) {
            os << std::forward<T>(v) << L' ';
            pipe(os, std::forward<Ts>(args)...);
        }

    } // namespace impl

    template <typename... Ts>
    void log(Ts&&... args) {
        // In DebugView, '\n' triggers buffer flushing and
        // causes text to be displayed. DebugView has a
        // "Force Carriage Returns" option. It auto appends '\n'
        // for each OutputDebugString call, which may not
        // be what you want. The following static variable
        // flushes the buffer at process termination
        // regardless of the "Force Carriage Returns" option.
        static impl::endl_guard final_flush;
        std::wostringstream s;
        impl::pipe(s, std::forward<Ts>(args)...);
        OutputDebugStringW(s.str().c_str());
    }

} // namespace dbg
