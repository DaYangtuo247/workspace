#include "../utility.h"
#include "json.hpp"

using json = nlohmann::ordered_json;  // 有序

class containerConvert {
private:
    json Template; // 容器模板
public:
    stringstream result;  // 解析结果

private:
    // 清理字符串两边空白, 字符 '&' -> '*'
    std::string trimAndReplace(std::string str);
    // 获取结构体名称
    string getStructName(string str);
    // 判断str是不是为TN，即为T，T1，T2...，输入i用于判断是否 str == "Ti"
    bool isTN(std::string str, int i);
    // 获取容器类型外的信息，如指针，子类等
    string getTypeStruct(string & str);

public:
    containerConvert(string);

    // 解析输入为json对象
    json parseContainer(std::string str);
    // json解析为结构体输出
    string generateStruct(const json& input);
    // 获取变量的容器名称
    std::string getVariableType(std::string& command);
    // 清空文件，输出结构体正则匹配串
    void outputClean(const std::string& filename);
};