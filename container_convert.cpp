#include "json.hpp"
#include "utility.h"

// using json = nlohmann::json; // 无序
using json = nlohmann::ordered_json;  // 有序

json example;       // json模板
stringstream file;  // 输出到文件

// 清理字符串两边空白, 字符 '&' -> '*'
std::string trimAndReplace(std::string str) {
    int left = 0, right = str.size() - 1;
    while (left < str.size() && str[left] == ' ')
        left++;
    while (right > 0 && str[right] == ' ')
        right--;
    for (char& ch : str)
        ch = ch == '&' ? '*' : ch;
    return str.substr(left, right - left + 1);
}

// 解析输入的字符串为json对象
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

// 获取结构体名称
string getStructName(string str) {
    // 移除结尾的 '*' 或 '&'
    while (str.back() == '*' || str.back() == '&' || str.back() == ' ') {
        str.pop_back();
    }
    // 使用std::replace将空格替换为下划线
    std::replace(str.begin(), str.end(), ' ', '_');
    // 替换 "::" 为 "_"
    size_t pos = 0;
    while ((pos = str.find("::", pos)) != std::string::npos) {
        str.replace(pos, 2, "_");
        pos += 1;
    }
    return str;
}

// 判断str是不是为TN，即为T，T1，T2...，输入i用于判断是否 str == "Ti"
bool isTN(std::string str, int i) {
    if (str.size() == 1 && str[0] == 'T')
        return true;
    if (str.size() == 2 && str[0] == 'T' && isdigit(str[1])) {
        return str[1] - '0' == i;
    }
    return false;
}

// json解析为结构体输出
string generate_struct(const json& input, std::string prefix = "") {
    // 获取类型名（如 "HSTVector"）
    string type = input.begin().key();

    // 确保类型在模板中存在
    if (!example.contains(type)) {
        throw runtime_error("模板中未找到类型: \n" + input.dump(2));
    }

    // 获取存储类型
    json value = input.begin().value();

    // 容器中指向的结构体名称
    string sub_struct_name;

    // 生成的结构体名称
    string struct_name = prefix + example[type]["abbreviation"].get<std::string>() + "_";

    int i = 1;
    json output = example[type]["struct"];
    for (const auto& input_el : input.begin().value().items()) {
        json value_new = input_el.value();

        // 存在继承结构, 只执行一次，无需递归
        if (example[type].contains("public")) {
            json new_j = {{example[type]["public"], value}};
            sub_struct_name = generate_struct(new_j, prefix);
            struct_name += sub_struct_name + "_";

            // 修改输出类型
            for (const auto& el : output.items()) {
                // 如果是 T, T1, T2, T3...., 修改为实际存储的类型
                if (isTN(el.value(), i))
                    el.value() = sub_struct_name;
            }
            break;
        }
        // 容器存储基本类型, 直接输出为 struct
        else if (value_new.is_string()) {
            sub_struct_name = value_new.get<std::string>();
            struct_name += getStructName(value_new) + "_";
        }
        // 容器存储对象
        else if (value_new.is_object()) {
            sub_struct_name = generate_struct(value_new, prefix);
            struct_name += sub_struct_name + "_";
        }

        // 修改输出类型
        for (const auto& el : output.items()) {
            // 如果是 T, T1, T2, T3...., 修改为实际存储的类型
            if (isTN(el.value(), i))
                el.value() = sub_struct_name;
        }
        i++;
    }

    // 去掉末尾的 '_' 字符
    struct_name = struct_name.substr(0, struct_name.size() - 1);

    // 修改输出类型
    for (const auto& el : output.items()) {
        // 修改指向自身的指针
        if (el.value() == type)
            el.value() = struct_name;
    }

    // 输出结构
    cout << "struct " << struct_name << " {\n";
    file << "struct " << struct_name << " {\n";
    for (const auto& el : output.items()) {
        cout << "  " << el.value().get<std::string>() << " " << el.key() << ";" << endl;
        file << "  " << el.value().get<std::string>() << " " << el.key() << ";" << endl;
    }
    cout << "};\n";
    file << "};\n";
    return struct_name;
}

int main(int argc, char* argv[]) {
    // 测试案例
    vector<string> testCases = {
        // "HSTVector<rt::HARTRouteNetWrapper*>",
        "HSTSet<unsigned int>",
        "std::vector<std::pair<unsigned int,rt::HDRTNetGuide*>>",
        // "std::unordered_map<std::map<double, int>, std::HSTVector<rt::nodegraph::Node>>",
        // "HASTMap<HSTKeyValIter const &&,HSTKeyValIterLess>",
        // "std::map<unsigned int, double>",
        // "HSTVector<rt::nodegraph::Node*>",
        // "HASTMap<HASTSet<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        // "std::vector<rt::nodegraph::Node*>",
        // "std::vector<std::vector<rt::nodegraph::Node*>>",
        // "std::set<HSTKeyValIter const *,HSTKeyValIterLess>",
        // "std::set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        // "std::unordered_set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        // "std::unordered_map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        // "std::map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
        // "std::tuple<int,unsigned short,float>",
        // "std::tuple<std::set<std::map<short, int>>, std::string, int>",
        // "std::map<std::tuple<std::set<std::map<short,int>>, std::string, int>,int,less>",
        // "std::vector<HSTVector<rt::nodegraph::Node*>>",
        // "std::unordered_map<std::map<double, int>, HSTVector<rt::nodegraph::Node*>>",
        // "std::tuple<std::set<std::map<short, int>>, std::string, int>",
        // "std::map<std::tuple<std::set<std::map<short,int>>,std::string,int>,int>",
        // "std::tuple<unsigned int, int, double>",
        // "std::unordered_map<std::map<double, int>, HSTVector<rt::nodegraph::Node>>",
    };

    std::ifstream f("./template.json");
    example = json::parse(f);

    // 获取文件名和字符串
    std::string output_file = "output.txt";
    std::string content;

    // ./exe "需要解析的结构" "输出文件名"
    if (argc == 3) {
        output_file = argv[2];
        testCases.clear();
        testCases.push_back(argv[1]);
    }
    // ./exe "需要解析的结构"
    else if (argc == 2) {
        testCases.clear();
        testCases.push_back(argv[1]);
        cout << "output Path: " << output_file << endl;
    }

    std::ofstream fd(output_file, std::ios::trunc);
    if (!fd.is_open()) {
        cout << "file open error !" << endl;
        exit(1);
    }

    // 遍历每个测试案例进行解析
    for (const auto& type : testCases) {
        cout << "##########################################################################################\n";
        cout << "//Input Type: " << type << endl;
        file << "##########################################################################################\n";
        file << "//Input Type: " << type << endl;
        // 获取解析的json对象
        size_t pos = 0;
        json res = parseContainer(type);  // 用于将输入解析为json对象，保证没有错误

        cout << res.dump(2) << endl;
        cout << "------------------------------------------------------\n";
        file << res.dump(2) << endl;
        file << "------------------------------------------------------\n";

        // 将json对象解析为结构体
        generate_struct(res, "custom_");
        cout << endl;
        file << endl;
    }

    fd << file.str();

    fd.close();

    return 0;
}
