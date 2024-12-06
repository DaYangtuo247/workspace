#include "json.hpp"
#include "utility.h"

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

// 获取结构体名称
string getStructName(string str) {
    int starCount = 0; // 统计指针个数
    // 去除类型
    while (str.back() == '*' || str.back() == ' ') {
        if(str.back() == '*') starCount++;
        str.pop_back();
    }
    std::replace(str.begin(), str.end(), ' ', '_');
    // 去除作用域
    size_t pos = str.rfind("::");
    if (pos != std::string::npos)
        str = str.substr(pos + 2);
    
    //添加指针信息
    if(starCount)
        return str + "_" + string(starCount, 'p');
    else
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

// 获取容器类型外的信息，如指针，子类等
string getTypeStruct(string & str) {
    auto it = std::find_if(str.begin(), str.end(), [](char ch) {
        return !(std::isalnum(ch) || ch == ':' || ch == '_');
    });

    if (it != str.end()) {
        std::string result(it, str.end());
        str = str.substr(0, std::distance(str.begin(), it));
        return result;
    }
    return "";
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
        cout << "Unknown Type: " << containerName << ", Place add to template.json" << endl;
        exit(1);
    }

    // 容器是几元组
    uint32_t numParams = example[containerName]["level"].get<int>(); // 注意，tuple为-1,这会让numParams无穷大

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

        // 解析 一元组，二元组，tuple容器
        for (uint32_t i = 0; i < innerTypes.size() && i < numParams; ++i) {
            string containerName_new = containerName + "_T" + to_string(i + 1);
            ans[containerName][containerName_new] = innerTypes[i];
            if (innerTypes[i].find('<') != string::npos) {
                ans[containerName][containerName_new] = parseContainer(innerTypes[i]);
            }
        }
    }
    return ans;
}

// json解析为结构体输出
string generateStruct(const json& input) {
    // 获取类型名（如 "HSTVector"）
    string type = trimAndReplace(input.begin().key());
    
    // 判断type是否是指针结构
    string is_ptr = getTypeStruct(type);

    // 确保类型在模板中存在
    if (!example.contains(type)) {
        throw runtime_error(input.dump(2) + "\nType not found in template.json.");
    }

    // 获取存储类型
    json value = input.begin().value();

    json output;

    // 如果存储类型是元组
    if (type == "std::tuple") {
        string el_key = example[type]["struct"].begin().key(), el_value = example[type]["struct"].begin().value();
        for (int j = 1; j <= value.size(); j++)
            output[el_key + to_string(j)] = el_value + to_string(j);
    } else {
        output = example[type]["struct"];
    }

    // 容器中指向的结构体名称
    string sub_struct_name;
    // 生成的结构体名称
    string struct_name = example[type]["abbreviation"].get<std::string>() + "_";
    int i = 1; // Ti
    // 按照template结构处理
    for (const auto& input_el : value.items()) {
        json value_new = input_el.value();
        
        bool is_public = false;
        // 存在继承（迭代器）, 只执行一次，无需递归
        if (example[type].contains("public") || example[type].contains("iterator")) {
            json new_j;
            if (example[type].contains("public"))
                new_j = {{example[type]["public"], value}};
            else
                new_j = {{example[type]["iterator"], value}};
            sub_struct_name = generateStruct(new_j);
            is_public = true;
        }
        // 容器存储基本类型, 直接输出为 struct
        else if (value_new.is_string()) {
            sub_struct_name = value_new.get<std::string>();
        }
        // 容器存储对象
        else if (value_new.is_object()) {
            sub_struct_name = generateStruct(value_new);
        }
        
        // 当前结构体加上子结构体的名称
        struct_name += getStructName(sub_struct_name) + "_";

        // 修改输出类型
        for (const auto& el : output.items()) {
            // 如果是 T, T1, T2, T3...., 修改为实际存储的类型
            if (isTN(el.value(), i))
                el.value() = sub_struct_name;
        }
        if (is_public)
            break;
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
    file << "struct " << getStructName(struct_name) << " {\n";
    for (const auto& el : output.items())
        file << "  " << el.value().get<std::string>() << " " << el.key() << ";" << endl;
    file << "};\n";

    return struct_name + is_ptr;
}

// 清空文件，输出结构体正则匹配串
void processFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cout << "not found: " << filename << std::endl;
        return;
    }
    // 备份文件
    std::ofstream backfile(filename + ".bak", std::ios::trunc);
    backfile << infile.rdbuf();
    backfile.close();
    infile.clear();
    infile.seekg(0, std::ios::beg);

    std::vector<std::string> structNames;
    std::regex structRegex(R"(struct\s+(.+?)\s*\{)");
    std::string line;

    while (std::getline(infile, line)) {
        std::smatch match;
        if (std::regex_search(line, match, structRegex))
            structNames.push_back(match[1]);
    }
    infile.close();

    cout << "^(";
    for (auto it = structNames.begin(); it != structNames.end(); ++it) {
        if (it != structNames.end() - 1)
            cout << *it << "|";
        else
            cout << *it;
    }
    cout << ")\n";

    std::ofstream outfile(filename, std::ios::trunc);
    outfile.close();
}

int main(int argc, char* argv[]) {
    // 显示帮助信息
    if (argc == 1 || (argc >= 2 && string(argv[1]) == "-h")) {
        std::cout << "Options:\n";
        std::cout << "  -h                           Show this help message\n";
        std::cout << "  -d [output_file]             Generate [output_file].h.bak, clean [output_file].h\n";
        std::cout << "  -e [output_file]             example output to [output_file]\n";
        std::cout << "  container_name [output_file] container convert\n";
        std::cout << "note: Default output: ./container_convert.h\n";
      return 0;
    }
    vector<string> testCases;
    std::string output_path; // 默认输出路径
    output_path = argc == 3 ? argv[2] : "./container_convert.h";
    
    string arg = argv[1];
    if (arg == "-d") {
        processFile(output_path);
        return 0;
    } else if (arg == "-e") {
        testCases = {
            "std::vector<std::pair<int ,int > *>",
            "std::tuple<HSTVector<rt::HARTRouteNetWrapper*>, std::shared_ptr<unsigned int>, std::map<double, int>, int>",
            "std::shared_ptr<int32_t>",
            "HSTSet<unsigned int>",
            "std::vector<std::pair<unsigned int,rt::HDRTNetGuide*>>",
            "std::unordered_map<std::map<double*, int>, HSTVector<rt::nodegraph::Node>>",
            "HSTMap<HSTKeyValIter const &&,HSTKeyValIterLess>",
            "std::map<unsigned int, double>",
            "HSTVector<rt::nodegraph::Node*>",
            "HSTMap<HSTSet<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
            "std::vector<rt::nodegraph::Node*>",
            "std::vector<std::vector<rt::nodegraph::Node*>>",
            "std::set<HSTKeyValIter const *,HSTKeyValIterLess>",
            "std::set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
            "std::unordered_set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
            "std::unordered_map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
            "std::map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
            "std::tuple<int,unsigned short,float>",
            "std::tuple<std::set<std::map<short, int>>, std::string, int>",
            "std::map<std::tuple<std::set<std::map<short,int>>, std::string, int>,int,less>",
            "std::vector<HSTVector<rt::nodegraph::Node*>>",
            "std::unordered_map<std::map<double, int>, HSTVector<rt::nodegraph::Node*>>",
            "std::tuple<std::set<std::map<short, int>>, std::string, int>",
            "std::map<std::tuple<std::set<std::map<short,int>>,std::string,int>,int>",
            "std::tuple<unsigned int, int, double>",
            "std::unordered_map<std::map<double, int>, HSTVector<rt::nodegraph::Node>>",
        };
    }

    // 获取模板
    std::ifstream f("./template.json");
    if (!f.is_open()) {
        cout << "Error: template.json not found!!!" << endl;
        exit(1);
    }
    example = json::parse(f);

    // 获取需要解析的容器
    if (!testCases.size())
        testCases.push_back(argv[1]);

    // 遍历每个测试案例进行解析
    for (const auto& type : testCases) {
        json res = parseContainer(type);
        // 输出路径是debug时，打开调试模式
        if (output_path == "debug") {
            file << "##########################################################################################\n";
            file << "//Input Type: " << type << endl;
            file << res.dump(2) << endl;
            file << "------------------------------------------------------\n";
        } else {
            file << "//------------------------------------------------------\n";
            file << "// " << type << endl;
        }
        generateStruct(res);
        file << endl;
    }

    std::ofstream fd(output_path, std::ios::app);
    fd << file.str();
    fd.close();

    cout << file.str().substr(2);
    cout << "------------------------------------------------------\noutput Path: " << output_path << endl;

    return 0;
}
