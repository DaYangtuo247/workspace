#include <iostream>
#include <utility>  // std::move
#include <unordered_set>

void mightThrow() { throw std::runtime_error("This function might throw!"); }

void noThrow() noexcept {
    // This function won't throw
}

int main() {
    try {
        mightThrow();
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Caught an exception!\n";
    }

    if (noexcept(noThrow())) {
        std::cout << "noThrow() is noexcept.\n";
    }

    return 0;
}