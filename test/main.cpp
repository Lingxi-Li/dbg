#include <dbg.hpp>

int main() {
    dbg::log(123, 3.14);
    dbg::logline(L"wstr", "str");
    dbg::log(L"END");
}
