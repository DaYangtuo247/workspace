#include <iostream>
#include <regex>

#include "json.hpp"
#include "utility.h"

using json = nlohmann::json;
json example;

std::string cleanwhile(std::string str) {
    int left = 0, right = str.size() - 1;
    while(left < str.size() && str[left] == ' ')
        left ++;
    while(right > 0 && str[right] == ' ')
        right --;
    return str.substr(left, right - left + 1);
}

json parseContainer(std::string str, int pos, int level) {
    json ans;
    while (pos < str.size() && isspace(str[pos]))
        pos++;

    if (pos > str.size())
        return ans;

    // 获取容器名
    std::string containerName;
    while (pos < str.size() && (isalnum(str[pos]) || str[pos] == ':' || str[pos] == '_')) {
        containerName.push_back(str[pos]);
        pos++;
    }

    // 检查容器是否在映射表中
    if (!example.contains(containerName)) {
        cout << "Unknown Type: " << containerName << endl;
        return ans;
    }

    int numParams = example[containerName]["level"].get<int>();

    // 输出容器名称
    cout << string(level, '\t') << "Container: " << containerName << endl;

    if (pos < str.size() && str[pos] == '<') {
        pos++;

        // 解析容器内的类型
        vector<string> innerTypes;
        int bracketLevel = 1;  // 记录当前嵌套层级
        string currentType;
        while (pos < str.size() && bracketLevel > 0) {
            char ch = str[pos];

            if (ch == '<')
                bracketLevel++;
            else if (ch == '>')
                bracketLevel--;

            // 累加当前字符到类型
            if (bracketLevel > 0 || isalnum(ch) || ch == ',' || ch == ' ' || ch == ':') {
                // 等于1 且 ch == ',' 说明当前容器结束，最后一个 ',' 不应该保存
                if (bracketLevel == 1 && ch == ',')
                    ;
                else
                    currentType.push_back(ch);
            }

            if (bracketLevel == 1 && ch == ',') {
                currentType = cleanwhile(currentType);
                innerTypes.push_back(currentType);
                currentType.clear();
            }
            pos++;
        }

        currentType = cleanwhile(currentType);

        if (currentType.size())
            innerTypes.push_back(currentType);

        // 一元组容器
        if (numParams == 1) {
            cout << string(level + 1, '\t') << "Element: " << innerTypes[0] << endl;
            ans[containerName] = innerTypes[0];
            if (innerTypes[0].find('<') != string::npos) {
                size_t innerpos = 0;
                ans[containerName] = parseContainer(innerTypes[0], innerpos, level + 2);
            }

        // 二元组容器
        } else if (numParams == 2) {
            cout << string(level + 1, '\t') << "Key: " << innerTypes[0] << endl;
            ans[containerName]["Key"] = innerTypes[0];
            if (innerTypes[0].find('<') != string::npos) {
                size_t innerpos = 0;
                ans[containerName]["Key"] = parseContainer(innerTypes[0], innerpos, level + 2);
            }
            cout << string(level + 1, '\t') << "Value: " << innerTypes[1] << endl;
            ans[containerName]["Value"] = innerTypes[1];
            if (innerTypes[1].find('<') != string::npos) {
                size_t innerpos = 0;
                ans[containerName]["Value"] = parseContainer(innerTypes[1], innerpos, level + 2);
            }

        // tuple容器
        } else if (numParams == -1) {
            // std::tuple 类型的参数个数由用户定义
            for (size_t i = 0; i < innerTypes.size(); ++i) {
                cout << string(level + 1, '\t') << "Element_" << i + 1 << ": " << innerTypes[i] << endl;
                ans[containerName][string("Element_") + to_string(i + 1)] = innerTypes[i];
                // 递归解析 tuple 元素
                if (innerTypes[i].find('<') != string::npos) {
                    size_t innerPos = 0;
                    ans[containerName][string("Element_") + to_string(i + 1)] = parseContainer(innerTypes[i], innerPos, level + 2);
                }
            }
        }
    }
    return ans;
}

int main() {
    std::ifstream f("./template.json");
    example = json::parse(f);

    // 测试案例
    vector<string> testCases = {
        "HSTVector<rt::nodegraph::Node*>",
        "std::vector<rt::nodegraph::Node*>",
        "std::vector<HSTVector<rt::nodegraph::Node*>>"
        // "std::set<HSTKeyValIter const *,HSTKeyValIterLess>",
        // "std::set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        // "std::unordered_set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        // "std::unordered_map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        // "std::map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        // "std::map<std::tuple<std::set<std::map<short,int>>, std::string, int>,int,less>",
        // "std::tuple<std::set<std::map<short, int>>, std::string, int>",
        // "std::tuple<int,unsigned short,float>",
        // "std::vector<HSTVector<rt::nodegraph::Node*>>",
        // "std::unordered_map<std::map<double, int>, HSTVector<rt::nodegraph::Node*>>",
        // "HASTMap<HASTSet<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        // "std::tuple<std::set<std::map<short, int>>, std::string, int>",
        // "std::map<std::tuple<std::set<std::map<short,int>>,std::string,int>,int>",
        // "std::tuple<unsigned int, int, double>",
        // "std::unordered_map<std::map<double, int>, HSTVector<rt::nodegraph::Node>>",
    };

    std::ofstream fd("output.txt", std::ios::trunc);
    std::stringstream ss;  // 用于缓冲输出内容

    if (!fd.is_open()) {
        cout << "file open error !" << endl;
        exit(1);
    }

    // 遍历每个测试案例进行解析
    for (const auto& type : testCases) {
        ss << "---------------------------------------------------------------------------------------------------\n";
        ss << "正在解析类型: " << type << endl;
        size_t pos = 0;
        json res = parseContainer(type, pos, 0);  // 从根容器开始解析
        ss << res.dump(2) << endl;
    }

    // 输出到文件
    fd << ss.str();

    // 同时输出到控制台
    cout << ss.str();

    fd.close();
    return 0;
}
