#include <windows.h>

#include <sstream>
#include <utility>

template <typename T> inline
std::wostream& operator<(std::wostream& os, T&& v) {
  return os << std::forward<T>(v) << L' ';
}

namespace dbg {

inline
void break_into_debugger(bool condition = true) {
  if (IsDebuggerPresent()) return;
  if (!condition) return;
  while (!IsDebuggerPresent()) {
    Sleep(3 * 1000);
  }
  DebugBreak();
}

namespace impl {

struct endl_guard {
  ~endl_guard() {
    OutputDebugStringW(L"\n");
  }
};

} // namespace impl

template <typename... Ts> inline
void log(Ts&&... args) {
  static impl::endl_guard final_flush; // for DebugView without "Force Carriage Returns"
  std::wostringstream s;
  (s < ... < std::forward<Ts>(args));
  OutputDebugStringW(s.str().c_str());
}

template <typename... Ts> inline
void logline(Ts&&... args) {
  impl::endl_guard endl;
  log(std::forward<Ts>(args)...);
}

} // namespace dbg
