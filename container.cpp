#include <iostream>
#include <regex>

#include "json.hpp"
#include "utility.h"

using json = nlohmann::json;

std::stack<int> st;
std::stack<char> op;

// 判断当前属于那种容器
bool exist(json& j, string tuple, string str) {
    for (const auto& item : j[tuple]) {
        if (item.contains("container") && item["container"] == str) {
            return true;
        }
    }
    return false;
}

std::vector<std::pair<size_t, size_t>> container_split(const std::string& type) {
    std::stack<size_t> bracketStack;  // 用栈存储 `<` 的下标
    std::vector<std::pair<size_t, size_t>> matchingBrackets;  // 存储匹配的 `<` 和 `>` 的下标对

    // 遍历字符串
    for (size_t i = 0; i < type.size(); ++i) {
        if (type[i] == '<') {
            // 如果是 `<`，将其位置压入栈
            bracketStack.push(i);
        } else if (type[i] == '>') {
            // 如果是 `>`，弹出栈顶的 `<` 并记录匹配对
            if (!bracketStack.empty()) {
                size_t start = bracketStack.top();
                bracketStack.pop();
                matchingBrackets.push_back({start, i});
            }
        }
    }
    reverse(matchingBrackets.begin(), matchingBrackets.end());
    return matchingBrackets;
}

json res;

// 递归容器
json parse_type(json& j, const std::string& type_str) {
    if (exist(j, "one_tuple", type_str)) {
        std::string container = decode(type_str);
        return parse_type(res[container], );

    } else if (exist(j, "two_tuple", type_str)) {

        return parse_type(res[left]) + param_type(res[right]);

    } else if (exist(j, "mutil_tuple", type_str)) {
        //count number

        return parse_type(i);

    } else {
        std::cout << "Unknown type: " << type_str << std::endl;
    }
    return j;
}

int main() {
    std::ifstream f("template.json");
    json j = json::parse(f);

    std::string container_type = "HSTVector<rt::nodegraph::Node>";

    auto vec = container_split(container_type);

    // 解析容器类型并输出
    json res = parse_type(j, container_type);

    cout << res.dump() << endl;

    return 0;
}




// "std::set<HSTKeyValIter const *,HSTKeyValIterLess>"
// "std::set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>"
// "std::unordered_set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>"
// "std::unordered_map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>"
// "std::map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>"
// "std::map<std::tuple<std::set<std::set<short,int>>, std::string, int>,int,less>"
// "std::tuple<std::set<std::map<short, int>>, std::string, int>"
// "std::tuple<int,unsigned short,float>"
// "HASTMap<HASTSet<HSTKeyValIter const &&>*,HSTKeyValIterLess>"
// "std::tuple<std::set<std::set<short, int>>, std::string, int>"
// "std::map<std::tuple<std::set<std::set<short,int>>, std::string, int>, int>"
// std::unordered_map<std::map<double, int>, std::HSTVector<rt::nodegraph::Node>>


