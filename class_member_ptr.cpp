#include <iostream>
#include <functional>

class MyClass {
public:
    int * value;

    MyClass(int * v) : value(v) {}
    void print() const { std::cout << "Value: " << value << std::endl; }
};

int main() {
    int a = 10;
    MyClass obj(&a);
    std::function<bool (int)> abc;

    // 定义成员指针
    int * MyClass::*ptrToValue = &MyClass::value;

    // 使用 `.*` 访问成员
    std::cout << "Value through pointer: " << obj.*ptrToValue << std::endl;
    std::cout << "Value through pointer: " << *(obj.*ptrToValue) << std::endl;

    return 0;
}
