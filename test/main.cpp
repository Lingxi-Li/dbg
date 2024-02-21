#include <dbg.hpp>

int main() {
    dbg::logf(L"Attempt {}: {}", 1, L"Success");
    dbg::loglinef(L"Attempt {}: {}", 1, L"Success");
    dbg::logf(L"Attempt {}: {}", 1, L"Success");
}
