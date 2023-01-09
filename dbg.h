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

template <typename... Ts> inline
void wlog(Ts&&... args) {
  std::wostringstream s;
  (s < ... < std::forward<Ts>(args));
  OutputDebugStringW(s.str().c_str());
}

template <typename... Ts> inline
void wlogline(Ts&&... args) {
  std::wostringstream s;
  (s < ... < std::forward<Ts>(args)) << L'\n';
  OutputDebugStringW(s.str().c_str());
}

} // namespace dbg
