#include <iostream>
#include <string>

class MyString : public std::string {
public:
    // 公开父类的私有方法 (不能直接调用)
    using std::string::_M_replace_aux;
    // 这个函数需要进入库文件basic_string.h文件中， 将_M_replace_aux()函数修改为protected，即可生效
    // using 可以将基类的函数成员（对数据成员无效）提升为public权限，但是对于基类是private的无效
    // 因为在继承关系中，private是不会被继承的，因此即使使用using也无效
    
    // 使用父类 std::string 的所有构造函数
    using std::string::string;
};

int main() {
    MyString v35 = "";

    // 通过派生类访问_M_replace_aux
    v35._M_replace_aux(v35.size(), 0, 1, '.');
    v35.append("Hello");
    v35._M_replace_aux(v35.size(), 0, 1, '.');

    std::cout << v35 << std::endl;  // 输出: .ello

    return 0;
}
