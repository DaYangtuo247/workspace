#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
std::vector<std::pair<size_t, size_t>> findMatchingBrackets(const std::string& type) {
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
    sort(matchingBrackets.begin(), matchingBrackets.end(), [](auto& a1, auto &a2){
        return a1.first < a2.first;
    });

    int i = 0;
    for (auto & t : matchingBrackets) {
        int rt = type.rfind(t.first, '<');
        cout << type.substr(i, t.first - rt);
        cout << "<" << t.first << ", " << t.second << ">, ";
    }

    return matchingBrackets;
}

int main() {
    // 测试用例
    std::vector<std::string> testCases = {
        "std::unordered_map<std::map<double, int>, std::HSTVector<rt::nodegraph::Node>>",
        "std::set<HSTKeyValIter const *,HSTKeyValIterLess>",
        "std::set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        "std::unordered_set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        "std::unordered_map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        "std::map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        "std::map<std::tuple<std::set<std::set<short,int>>, std::string, int>,int,less>",
        "std::tuple<std::set<std::map<short, int>>, std::string, int>",
        "std::tuple<int,unsigned short,float>",
        "HASTMap<HASTSet<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        "std::tuple<std::set<std::set<short, int>>, std::string, int>",
        "std::map<std::tuple<std::set<std::set<short,int>>, std::string, int>, int>"
    };

    for (const auto& type : testCases) {
        std::cout << "正在解析类型: " << type << std::endl;
        findMatchingBrackets(type);
        std::cout << std::endl;
    }

    return 0;
}
