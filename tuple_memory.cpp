#include <iostream>
#include <tuple>
#include <cstddef> // For offsetof

template<typename T>
void printOffset(const std::string& name, const T& value) {
    std::cout << name << " offset: " << reinterpret_cast<const char*>(&value) - reinterpret_cast<const char*>(&value) << std::endl;
}

int main() {
    // 创建一个 tuple
    std::tuple<int, double, char> myTuple(1, 3.14, 'a');

    // 获取每个元素的地址
    const char* tupleStart = reinterpret_cast<const char*>(&myTuple);

    // 输出每个元素的地址偏移量
    std::cout << "Address of tuple start: " << (tupleStart) << std::endl;
    std::cout << "Address of first element (int): " << (reinterpret_cast<const void*>(&std::get<0>(myTuple))) << std::endl;
    std::cout << "Address of second element (double): " << (reinterpret_cast<const void*>(&std::get<1>(myTuple))) << std::endl;
    std::cout << "Address of third element (char): " << (reinterpret_cast<const void*>(&std::get<2>(myTuple))) << std::endl;

    return 0;
}

