#include "container.cpp"

stringstream file; // 输出到文件

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

string generate_struct(const json& input) {
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

    //生成的结构体名称
    string struct_name = example[type]["abbreviation"].get<std::string>() + "_";

    int i = 1;
    json output = example[type]["struct"];
    for (const auto & input_el : input.begin().value().items()) {
        json value_new = input_el.value();

        // 存在继承结构, 只执行一次，无需递归
        if (example[type].contains("public")) {
            json new_j = {{example[type]["public"], value}};
            sub_struct_name = generate_struct(new_j);
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
            sub_struct_name = generate_struct(value_new);
            struct_name += sub_struct_name + "_";
        }

        // 修改输出类型
        for (const auto & el : output.items()) {
            // 如果是 T, T1, T2, T3...., 修改为实际存储的类型
            if (isTN(el.value(), i))
                el.value() = sub_struct_name;
        }
        i++;
    }

    // 去掉末尾的 '_' 字符
    struct_name = struct_name.substr(0, struct_name.size() - 1);

    // 修改输出类型
    for (const auto & el : output.items()) {
        // 修改指向自身的指针
        if (el.value() == type)
            el.value() = struct_name;
    }

    // 输出结构
    cout << "struct " << struct_name << " {\n";
    file << "struct " << struct_name << " {\n";
    for (const auto & el : output.items()) {
        cout << "  " << el.value().get<std::string>() << " " << el.key() << ";" << endl;
        file << "  " << el.value().get<std::string>() << " " << el.key() << ";" << endl;
    }
    cout << "};\n";
    file << "};\n";
    return struct_name;
}


int main(int argc, char *argv[]) {
    // 测试案例
    vector<string> testCases = {
        "HASTMap<HSTKeyValIter const &&,HSTKeyValIterLess>",
        "std::map<unsigned int, double>",
        "HSTVector<rt::nodegraph::Node*>",
        "HASTMap<HASTSet<HSTKeyValIter const &&>*,HSTKeyValIterLess>",
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

    std::ifstream f("./template.json");
    example = json::parse(f);

    // 获取文件名和字符串
    std::string output_file = "output.txt";
    std::string content;

    // ./exe "输出文件名" "需要解析的结构"
    if (argc == 3) { 
        output_file = argv[1];
        testCases.clear();
        testCases.push_back(argv[2]);
    }
    // ./exe "需要解析的结构"
    else if (argc == 2) {
        testCases.clear();
        testCases.push_back(argv[1]);
        cout << "输出至文件: output.txt" << endl;
    }

    std::ofstream fd("output.txt", std::ios::trunc);
    if (!fd.is_open()) {
        cout << "file open error !" << endl;
        exit(1);
    }

    // 遍历每个测试案例进行解析
    for (const auto& type : testCases) {
        cout << "##########################################################################################\n";
        cout << "正在解析类型: " << type << endl;
        file << "##########################################################################################\n";
        file << "正在解析类型: " << type << endl;
        // 获取解析的json对象
        size_t pos = 0;
        json res = parseContainer(type); // 用于将输入解析为json对象，保证没有错误
        
        cout << res.dump(2) << endl;
        cout << "------------------------------------------------------\n";
        file << res.dump(2) << endl;
        file << "------------------------------------------------------\n";
        
        // 将json对象解析为结构体
        generate_struct(res);
        cout << endl << endl;
        file << endl << endl;
    }

    fd << file.str();

    fd.close();

    return 0;
}
