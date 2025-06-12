#include "../utility.h"
#include "json.hpp"

using json = nlohmann::ordered_json;  // 有序

class containerConvert {
private:
    string TemplatePath; // 模板路径
    unordered_map<string, vector<string>> ContainerTypes; // 每个容器中存储的元素类型
public:
    static unordered_set<string> record; // 记录某个结构是否输出过
    json Template; // 容器模板
    stringstream result;  // 解析结果

private:
    // 清理字符串两边空白, 字符 '&' -> '*'
    string trimAndReplace(string str);
    // 获取结构体名称
    string getStructName(string str);
    // 判断str是不是为TN，即为T，T1，T2...，输入i用于判断是否 str == "Ti"
    bool isTN(string str, int i);
    // 获取容器类型外的信息，如指针，子类等
    string getTypeStruct(string & str);

public:
    containerConvert(string);

    // 解析str为json解析树
    json parseContainer(string str);
    // json解析为结构体输出，返回值为输出的结构体名称
    string generateStruct(const json& input);
    // 终端执行命令
    static string execCommand(string& command);
    // 清空文件，输出结构体正则匹配串
    void clearFileAndPrintf(const string& filename);
};

unordered_set<string> containerConvert::containerConvert::record = {};

containerConvert::containerConvert(string filePath) {
    // 获取模板
    ifstream f(filePath);
    if (!f.is_open()) {
        // cout << "Error: template.json not found!!!" << endl;
        throw logic_error("Error: template.json not found!!!");
    }
    Template = json::parse(f);
    this->TemplatePath = filePath;
}

// 清理字符串两边空白, 字符 '&' -> '*'
string containerConvert::trimAndReplace(string str) {
    int left = 0, right = str.size() - 1;
    while (left < str.size() && str[left] == ' ')
        left++;
    while (right > 0 && str[right] == ' ')
        right--;
    for (char& ch : str)
        ch = ch == '&' ? '*' : ch;
    return str.substr(left, right - left + 1);
}

// 获取结构体名称
string containerConvert::getStructName(string str) {
    int starCount = 0; // 统计指针个数
    // 去除类型
    while (str.back() == '*' || str.back() == ' ') {
        if(str.back() == '*') starCount++;
        str.pop_back();
    }
    replace(str.begin(), str.end(), ' ', '_');
    // 去除作用域
    size_t pos = str.rfind("::");
    if (pos != string::npos)
        str = str.substr(pos + 2);
    
    //添加指针信息
    if(starCount)
        return str + "_" + string(starCount, 'p');
    else
        return str;
}

// 判断str是不是为TN，即为T，T1，T2...，输入i用于判断是否 str == "Ti"
bool containerConvert::isTN(string str, int i) {
    smatch match;
    regex reg(R"(^T(|\d+)$)");
    if (regex_search(str, match, reg)) {
        if (match[1].length() == 0)
            return i == 1;
        else
            return stoi(match[1]) == i;
    }
    return false;
}

// 获取容器类型外的信息，如指针，子类等
string containerConvert::getTypeStruct(string & str) {
    auto it = find_if(str.begin(), str.end(), [](char ch) {
        return !(isalnum(ch) || ch == ':' || ch == '_');
    });

    if (it != str.end()) {
        string result(it, str.end());
        str = str.substr(0, distance(str.begin(), it));
        return result;
    }
    return "";
}

// 解析str为json解析树
json containerConvert::parseContainer(string str) {
    // 输入的是基本类型
    if (str.find('<') == string::npos) {
        // 去除指针后的const，例如 abc const * const
        smatch match;
        if (regex_match(str, match, regex(R"(^.+const$)"))) {
            str.erase(str.size() - 5, 5);
        }
        return str;
    }

    json res;
    int pos = 0;
    // 获取容器名
    string containerName;
    while (pos < str.size() && (isalnum(str[pos]) || str[pos] == ':' || str[pos] == '_' || str[pos] == ' ')) {
        containerName.push_back(str[pos]);
        pos++;
    }
    // 去除容器的const， 例如 const HSTVector
    smatch match;
    if (regex_match(containerName, match, regex(R"(^const [\w:]+$)"))) {
        containerName = containerName.substr(6);
    }

    // 检查容器是否在映射表中
    if (!Template.contains(containerName)) {
        stringstream msg;
        msg << "Unknown Type: " << containerName << ", Place add to template.json";
        // cout << msg.str();
        throw logic_error(msg.str());
    }

    // 容器是几元组
    uint32_t numParams = Template[containerName]["level"].get<int>(); // 注意，tuple为-1,这会让numParams无穷大

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
        if (pos != str.size()) {
            containerName += str.substr(pos);
        }

        currentType = trimAndReplace(currentType);

        // 最后一个类型
        if (currentType.size())
            innerTypes.push_back(currentType);

        // 修复当输入错误时，没有提示错误，例如输入"pair<int>"，应该报错
        if ((signed int)innerTypes.size() < (signed int)numParams) {
            stringstream msg;
            msg << "Input error, " << containerName << " lack T" << innerTypes.size() + 1 << " type.";
            // cout << msg.str();
            throw logic_error(msg.str());
        }

        // 记录当前容器存储的元素类型
        res[containerName]["CurParTypes"] = str;
        
        // 解析 一元组，二元组，tuple容器
        for (uint32_t i = 0; i < innerTypes.size() && i < numParams; ++i) {
            // 记录每个容器存储的元素类型
            ContainerTypes[str].push_back(innerTypes[i]);
            //构建 json 对象
            string containerName_new = containerName + "_T" + to_string(i + 1);
            res[containerName][containerName_new] = parseContainer(innerTypes[i]);
        }
    }
    return res;
}

// json解析为结构体输出，返回值为输出的结构体名称
string containerConvert::generateStruct(const json& input) {
    // 获取类型名（如 "HSTVector"）
    string type = trimAndReplace(input.begin().key());
    
    // 判断type是否是指针结构
    string is_ptr = getTypeStruct(type);

    // 确保类型在模板中存在
    if (!Template.contains(type)) {
        throw logic_error(input.dump(2) + "\nType not found in template.json.");
    }

    // 获取容器解析树
    json container_parse = input.begin().value();

    // 根据Template[struct] 生成struct结构
    json type_struct;

    // 如果存储类型是元组
    if (Template[type]["level"] == -1) {
        string el_key = Template[type]["struct"].begin().key(), el_value = Template[type]["struct"].begin().value();
        // 需要逆序输出，例如std::tuple
        if (Template[type].contains("reverse") && Template[type]["reverse"] == 1) {
            for (int j = container_parse.size() - 1; j >= 1; j--)
                type_struct[el_key + to_string(j)] = el_value + to_string(j);
        // 无需逆序, 例如boost::tuples::tuple
        } else {
            for (int j = 1; j < container_parse.size(); j++)
                type_struct[el_key + to_string(j)] = el_value + to_string(j);
        }
    } else {
        type_struct = Template[type]["struct"];
    }

    string abbreviation = Template[type]["abbreviation"].get<string>() + "_";
    // 生成的结构体名称
    string struct_name;
    // struct{}中每个变量的类型名称
    string inner_type_name;

    int i = 1; // Ti
    // 遍历容器解析树
    for (const auto& container_parse_it : container_parse.items()) {
        if (container_parse_it.key() == "CurParTypes")
            continue;
        
        json sub_type = container_parse_it.value();
        
        // 容器存储基本类型
        if (sub_type.is_string()) {
            inner_type_name = sub_type.get<string>();
        }
        // 容器存储对象
        else if (sub_type.is_object()) {
            inner_type_name = generateStruct(sub_type);
        }
        
        // 当前结构体加上子结构体的名称
        struct_name += getStructName(inner_type_name) + "_";

        // 按照template的struct结构处理
        for (const auto& el : type_struct.items()) {
            // 如果是 T, T1, T2, T3...., 修改为实际存储的类型
            if (isTN(el.value(), i))
                el.value() = inner_type_name;

            // Template中的struct的value存储的是模板容器, 需要二次解析
            else if (el.value().get<string>().find('<') != string::npos) {
                string inner_container = el.value();

                // 捕获是第几个模板
                regex reg("[ _,<]T(|\\d+)[ _,>*]+");
                smatch match;
                // 替换子容器的所有Tn为对应的类型
                while (regex_search(inner_container, match, reg)) {
                    int len = max((size_t)0, string(match[1]).size()) + 1;
                    int pos = match.position(1) - 1;
                    int target_idx = string(match[1]).size() == 0 ? 1 : stoi(match[1]);
                    if (target_idx > ContainerTypes[container_parse["CurParTypes"]].size()) {
                        stringstream msg;
                        msg << "Unknown Type T" << match[1] << ", Please check if the structure of " << type << " in template.json is correct.";
                        // cout << msg.str();
                        throw logic_error(msg.str());
                    }
                    string target = ContainerTypes[container_parse["CurParTypes"]][target_idx - 1];
                    inner_container.replace(pos, len, target);
                }
                containerConvert tp(this->TemplatePath);
                json tt = tp.parseContainer(inner_container);
                el.value() = tp.generateStruct(tt);
                this->result << tp.result.str();
            }
        }
        i++;
    }

    // 去掉末尾的 '_' 字符
    struct_name = struct_name.substr(0, struct_name.size() - 1);

    // 修改输出类型
    for (const auto& el : type_struct.items()) {
        // 修改指向自身的指针
        if (el.value() == type)
            el.value() = abbreviation + struct_name;
    }

    // 输出结构
    string output_struct_name = abbreviation + getStructName(struct_name);
    if (!record.count(output_struct_name)) {
        record.insert(output_struct_name);
        string current_parse_type = container_parse["CurParTypes"];
        
        // 输出的结构体名称，不能含有[ *]结尾
        while (current_parse_type.back() == '*' || current_parse_type.back() == ' ') {
            current_parse_type.pop_back();
        }
        
        result << "\033[0;32m// " << current_parse_type << "\033[0m" << endl;
        result << "struct " << output_struct_name << " {\n";
        for (const auto& el : type_struct.items())
            result << "  " << el.value().get<string>() << " " << el.key() << ";" << endl;
        result << "};\n";
    }
    return abbreviation + struct_name + is_ptr;
}

// 清空文件，输出结构体正则匹配串
void containerConvert::clearFileAndPrintf(const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        stringstream msg;
        msg << "not found: " << filename;
        // cout << msg.str();
        throw logic_error(msg.str());
    }
    // 备份文件
    ofstream backfile(filename + ".bak", ios::trunc);
    backfile << infile.rdbuf();
    backfile.close();
    infile.clear();
    infile.seekg(0, ios::beg);

    vector<string> structNames;
    regex structRegex(R"(struct\s+(.+?)\s*\{)");
    string line;

    while (getline(infile, line)) {
        smatch match;
        if (regex_search(line, match, structRegex))
            structNames.push_back(match[1]);
    }
    infile.close();

    cout << "^(^";
    for (auto it = structNames.begin(); it != structNames.end(); ++it) {
        if (it != structNames.end() - 1)
            cout << *it << "$|^";
        else
            cout << *it;
    }
    cout << "$)\n";

    ofstream outfile(filename, ios::trunc);
    outfile.close();
}

// 终端执行命令，获取命令输出
string containerConvert::execCommand(string& command) {
    // 打开管道
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw logic_error("popen() failed!");
    }

    char buffer[256];
    string result;

    // 读取命令输出
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    // 关闭管道
    int retCode = pclose(pipe);
    if (retCode != 0) {
        throw logic_error("Command execution failed with code: " + to_string(retCode));
    }

    // 去除第一行
    int pos = result.find('\n');
    result = result.substr(pos + 1);
    cout << result;

    // 去除控制字符
    regex ansi_escape(R"(\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~]))");
    result = regex_replace(result, ansi_escape, "");
    return result;
}
