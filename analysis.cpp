#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>

using namespace std;

// 容器类型的参数个数映射
std::map<std::string, int> containerCounts = {
    {"std::map", 2},
    {"std::unordered_map", 2},
    {"std::vector", 1},
    {"std::list", 1},
    {"std::set", 1},
    {"std::unordered_set", 1},
    {"std::tuple", -1},
    {"HASTMap", 2},
    {"HASTSet", 1},
    {"HSTVector", 1}
};

// 解析容器类型的参数个数
void parseContainer(const string& type, size_t& pos, int level) {
    // 跳过空白字符
    while (pos < type.size() && isspace(type[pos])) pos++;

    if (pos >= type.size()) return;

    // 容器名称部分（例如 std::map、std::vector 等）
    string containerName;
    while (pos < type.size() && (isalnum(type[pos]) || type[pos] == ':' || type[pos] == '_')) {
        containerName.push_back(type[pos]);
        pos++;
    }

    // 检查容器类型是否在映射表中
    auto it = containerCounts.find(containerName);
    if (it == containerCounts.end()) {
        cout << string(level, '\t') << "Unknown container: " << containerName << endl;
        return;
    }

    // 输出容器名称
    cout << string(level, '\t') << "Container: " << containerName << endl;

    // 获取容器的参数个数
    int numParams = it->second;

    // 处理容器内部的嵌套类型
    if (pos < type.size() && type[pos] == '<') {
        pos++;  // 跳过 '<'

        // 解析容器内的类型
        vector<string> innerTypes;
        int bracketLevel = 1;  // 记录当前嵌套层级
        string currentType;

        // 遍历整个容器内部的内容
        while (pos < type.size() && bracketLevel > 0) {
            char ch = type[pos];
            if (ch == '<') {
                bracketLevel++;  // 嵌套层级加一
            } else if (ch == '>') {
                bracketLevel--;  // 嵌套层级减一
            }

            // 累加当前字符到类型
            if (bracketLevel > 0 || isalnum(ch) || ch == ',' || ch == ' ' || ch == ':') {
                currentType.push_back(ch);
            }

            // 当我们遇到逗号时，就认为这是一个新的类型（key 和 value 类型的分隔符）
            if (bracketLevel == 1 && ch == ',') {
                innerTypes.push_back(currentType);
                currentType.clear();
            }

            pos++;
        }

        // 处理最后一个部分
        if (!currentType.empty()) {
            innerTypes.push_back(currentType);
        }

        // 根据容器的类型和参数个数解析
        if (numParams == 2) {
            // std::map 和 std::unordered_map 是二元容器（key, value）
            if (innerTypes.size() > 0) {
                cout << string(level + 1, '\t') << "Key: " << innerTypes[0] << endl;
                // 递归解析 key
                if (innerTypes[0].find('<') != string::npos) {
                    size_t innerPos = 0;
                    parseContainer(innerTypes[0], innerPos, level + 2);
                }
            }
            if (innerTypes.size() > 1) {
                cout << string(level + 1, '\t') << "Value: " << innerTypes[1] << endl;
                // 递归解析 value
                if (innerTypes[1].find('<') != string::npos) {
                    size_t innerPos = 0;
                    parseContainer(innerTypes[1], innerPos, level + 2);
                }
            }
        } else if (numParams == 1) {
            // std::vector、std::set 等是一元容器（element）
            if (innerTypes.size() > 0) {
                cout << string(level + 1, '\t') << "Element: " << innerTypes[0] << endl;
                // 递归解析 element
                if (innerTypes[0].find('<') != string::npos) {
                    size_t innerPos = 0;
                    parseContainer(innerTypes[0], innerPos, level + 2);
                }
            }
        } else if (numParams == -1) {
            // std::tuple 类型的参数个数由用户定义
            cout << string(level + 1, '\t') << "Tuple Elements:" << endl;
            for (size_t i = 0; i < innerTypes.size(); ++i) {
                cout << string(level + 2, '\t') << "Element " << i + 1 << ": " << innerTypes[i] << endl;
                // 递归解析 tuple 元素
                if (innerTypes[i].find('<') != string::npos) {
                    size_t innerPos = 0;
                    parseContainer(innerTypes[i], innerPos, level + 3);
                }
            }
        }
    }
}

int main() {
    // 测试案例
    vector<string> testCases = {
        "std::set<HSTKeyValIter const *,HSTKeyValIterLess>",
        "std::set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        "std::unordered_set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        "std::unordered_map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        "std::map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        "std::map<std::tuple<std::set<std::set<short,int>>, std::string, int>,int,less>",
        "std::tuple<std::set<std::map<short, int>>, std::string, int>",
        "std::tuple<int,unsigned short,float>",
        "std::vector<HSTVector<rt::nodegraph::Node*>>",
        "std::unordered_map<std::map<double, int>, HSTVector<rt::nodegraph::Node*>>"
        "HASTMap<HASTSet<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        "std::tuple<std::set<std::set<short, int>>, std::string, int>",
        "std::map<std::tuple<std::set<std::set<short,int>>, std::string,int>,int>",
        "std::unordered_map<std::map<double, int>, std::HSTVector<rt::nodegraph::Node>>",
    };

    // 遍历每个测试案例进行解析
    for (const auto& type : testCases) {
        cout << "正在解析类型: " << type << endl;
        size_t pos = 0;
        parseContainer(type, pos, 0);  // 从根容器开始解析
        cout << endl;
    }

    return 0;
}
