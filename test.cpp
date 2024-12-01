#include <iostream>
#include <string>
#include <algorithm>



int main() {
    std::string input = "rt::nodegraph::Node****&&&";
    std::string output = transform_type_name(input);
    std::cout << "Original: " << input << std::endl;
    std::cout << "Transformed: " << output << std::endl;

    return 0;
}
