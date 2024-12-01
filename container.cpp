#include "json.hpp"
#include "utility.h"

// using json = nlohmann::json; // 无序
using json = nlohmann::ordered_json; // 有序
json example;

// 清理字符串两边空白, 字符 '&' -> '*'
std::string trimAndReplace(std::string str) {
    int left = 0, right = str.size() - 1;
    while(left < str.size() && str[left] == ' ')
        left ++;
    while(right > 0 && str[right] == ' ')
        right --;
    for (char & ch : str)
        ch = ch == '&' ? '*' : ch;
    return str.substr(left, right - left + 1);
}

// 解析输入的字符串
json parseContainer(std::string str) {
    json ans;
    int pos = 0;
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

    // 容器是几元组
    int numParams = example[containerName]["level"].get<int>();


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
                currentType = trimAndReplace(currentType);
                innerTypes.push_back(currentType);
                currentType.clear();
            }
            pos++;
        }

        currentType = trimAndReplace(currentType);

        // 最后一个类型
        if (currentType.size())
            innerTypes.push_back(currentType);

        // 解析 一元组，二元组，tuple容器
        for (size_t i = 0; i < innerTypes.size() && i < numParams; ++i) {
            string containerName_new = containerName + "_T" + to_string(i + 1);
            ans[containerName][containerName_new] = innerTypes[i];
            if (innerTypes[i].find('<') != string::npos) {
                ans[containerName][containerName_new] = parseContainer(innerTypes[i]);
            }
        }
        
    }
    return ans;
}
