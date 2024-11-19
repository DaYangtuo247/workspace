#include <iostream>
using namespace std;

// 钩子函数：函数进入时被调用
extern "C" void __cyg_profile_func_enter(void *func, void *caller) __attribute__((no_instrument_function));
extern "C" void __cyg_profile_func_exit(void *func, void *caller) __attribute__((no_instrument_function));

// 钩子函数：函数进入时被调用
extern "C" void __cyg_profile_func_enter(void *func, void *caller) {
    printf("Entering function: %p, called by: %p\n", func, caller);
}

// 钩子函数：函数退出时被调用
extern "C" void __cyg_profile_func_exit(void *func, void *caller) {
    printf("Exiting function: %p, returned to: %p\n", func, caller);
}

int func2(int x) {
    for (int i = 0; i < x; i++) {
        cout << i << endl;
    }
    return true;
}

int founc1(void) {
    return func2(10);
}

int main() {
    if (founc1()) {
        cout << "implement success" << endl;
    } else {
        cout << "implement error" << endl;
    }

    return 0;
}
