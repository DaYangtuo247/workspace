#include "container_convert.h"
#define TEMPLATE_PATH "../container_convert/template.json"

// 处理 IDA local type的.h文件
void solve_IDA_H(string inputFile, vector<string> & testCases) {
    ifstream in(inputFile);
    if (!in.is_open())
    assert(0);

    vector<string> lines;
    string line;
    while (getline(in, line)) {
        lines.push_back(line);
    }
    in.close();
    containerConvert obj(TEMPLATE_PATH);
    // 定义正则表达式（示例中采用原 Python 代码中的正则表达式）
    regex pattern(R"(^\s*([\w:]+)([&*\w ]+);\s//(<.*>))");
    
    // 遍历每一行，匹配后进行处理
    for (size_t i = 0; i < lines.size(); ++i) {
        try {
            smatch match;
            lines[i] = regex_replace(lines[i], regex(R"(^(\s*)const\s+)"), "$1");
            if (regex_search(lines[i], match, pattern)) {
                string container_name = match[1].str();
                string container_content = match[3].str();
                string container = container_name + container_content;
                if(container_name == "__int64")
                    continue;
                //cout << "[" << container << "]" <<endl;
                testCases.push_back(container);
                json res = obj.parseContainer(container);
                string container_new = obj.generateStruct(res);
                if (!regex_match(container_new, regex(R"(^[\w:]+$)")))
                    continue;
                lines[i] = string("\t") + container_new + string(" ") + match[2].str() + ";";
            }
        } catch (const exception &e) {
            testCases.pop_back();
            cerr << "Parsing error at line: " << i + 1 << ", reason: " << e.what() << endl;
        }
    }
    
    // 去除文件路径
    size_t pos = inputFile.find_last_of("/\\");
    string filename = (pos == string::npos) ? inputFile : inputFile.substr(pos + 1);
    // 写入处理后的结果到新文件（文件名在原文件后添加 _new 后缀）
    string outputFile = string("new_") + filename;
    ofstream out(outputFile);
    for (const auto &l : lines)
        out << l << "\n";
    out.close();
    
    obj.record.clear();
    cout << "generate newFile: " << outputFile << endl;
}

void replace_abstract_name_to_container(string file_path, int line_number, string & container_convert_path) {
    string line, pre;
    
    // 查询 container_covert.h 文件， 获取已解析的类型
    ifstream container_convert_h(container_convert_path, std::ios::in);
    std::map<string, string> record;
    regex pattern(R"(^struct (\w+) \{$)");
    smatch match;
    while(getline(container_convert_h, line)) {
        if (regex_match(line, match, pattern))
            record.insert({match[1], pre.substr(3)});
        pre = line;
    }
    
    // 读取src源代码
    ifstream src_file(file_path, std::ios::in | std::ios::out);
    stringstream output;
    int i;
    // 走到指定位置
    for(i = 1; i <= line_number + 1; i++) {
        getline(src_file, line);
        output << line << '\n';
    }

    int brace_count = 1;

    // 对已写的函数中所有匹配的 抽象名称全部替换为对应的容器类型
    pattern = regex(R"(\b(\w+)\b)");
    for (; brace_count > 0; i++) {
        getline(src_file, line);
        brace_count += std::count(line.begin(), line.end(), '{');
        brace_count -= std::count(line.begin(), line.end(), '}');
        smatch match;
        if (regex_search(line, match, pattern) && record.count(match[1])) {
            string key = match[1];
            line.replace(match.position(), match.length(), record[match[1]]);
            cout << "\033[0;32m+" << i << ":\033[0m " << key << " \033[0;32m->\033[0m " << record[key] << endl;
        }
        output << line << '\n';
    }
    while(getline(src_file, line)) {
        output << line << '\n';
    }
    src_file.close();
    ofstream output_file(file_path, std::ios::out);
    output_file << output.str();
    output_file.close();
}

int main(int argc, char* argv[]) {
    // 显示帮助信息
    if (argc == 1 || (argc >= 2 && string(argv[1]) == "-h")) {
        cout << "Options:\n";
        cout << "  [-h]                         Show this help message\n";
        cout << "  -d [output_file]             Generate [output_file].h.bak, clean [output_file].h\n";
        cout << "  -r input_file  [output_file] Retrieve container types from the input_file file.\n";
        cout << "  -f file.h      [output_file] The input is the file.h file of IDA local type.\n";
        cout << "  variable_name  [output_file] Container convert\n";
        cout << "  container_name [output_file] Container convert\n";
        cout << "  address        [input_file]  abstrat name to contaier name\n";
        cout << "Default [output_file | input_file]: ./container_convert.h\n";
      return 0;
    }

    containerConvert obj(TEMPLATE_PATH);
    vector<string> testCases;

    // 获取第一个参数
    string arg = argv[1];
    // 获取第二个参数
    string outputFilePath = argc == 3 ? argv[2] : "./container_convert.h";
    
    // 备份源文件，并清空 outputFilePath 文件
    if (arg == "-d") {
        obj.clearFileAndPrintf(outputFilePath);
        return 0;
    
    // 从文件中获取需要解析的容器
    } else if (arg == "-r") {
        if (argc < 3) {
            cout << "input error!!!" << endl;
            exit(0);
        }
        string inputPath = argv[2];
        outputFilePath = argc == 4 ? argv[3] : "./container_convert.h";
        fstream input(inputPath, ios::in);
        if (!input.is_open())
            assert(0);
        string line;
        while(getline(input, line))
            testCases.push_back(line);
        input.close();
    
    // 输入的是 IDA local type 的.h文件
    } else if (arg == "-f") {
        if (argc < 3) {
            cout << "input error!!!" << endl;
            exit(1);
        }
        string inputPath = argv[2];
        outputFilePath = argc == 4 ? argv[3] : "./container_convert.h";
        solve_IDA_H(inputPath, testCases);
    
    // 输入的是函数地址
    } else if (regex_match(arg, regex(R"(^0x\w+(:\w+)?$)"))) {
        string command = "/home/xiaonan/Share/scripts/toolUnified/toolUnified.py " + arg + " -cg -in_gvim";
        string result = containerConvert::execCommand(command);
        regex pattern(R"(gvim (.+) \+(\d+).*)");
        smatch match;
        if (regex_search(result, match, pattern)) {
            replace_abstract_name_to_container(match[1].str(), stoi(match[2]), outputFilePath);
            return 0;
        } else {
            cout << "input Error!" << endl;
            exit(1);
        }

    // 输入的是容器
    } else if (regex_match(arg, regex(R"(^.*<.*>.*$)"))) {
        testCases.push_back(argv[1]);
    
    // 输入的是变量
    } else if (regex_match(arg, regex("^[a-zA-Z0-9_]+$"))) {
        string command = "/home/xiaonan/Share/scripts/toolUnified/toolUnified.py " + arg + " -hg -in_gvim";
        string result = containerConvert::execCommand(command);
        // 获取容器名称
        regex pattern(R"(gvim\s.*\+\d+\s+(?:mutable|const)?\s+(.*)\s\w+;.*\n)");
        smatch match;
        if (regex_search(result, match, pattern)) {
            result = match[1];
            testCases.push_back(result);
        } else {
            cout << "input Error!" << endl;
            exit(1);
        }
    } else {
        cout << "input error!!!" << endl;
        exit(1);
    }

    stringstream ss;
    // 遍历每个测试案例进行解析
    for (const auto& type : testCases) {
        json res = obj.parseContainer(type);
        // 输出路径是debug时，打开调试模式
        if (outputFilePath == "debug") {
            ss << "##########################################################################################\n";
            ss << "\033[0;32m// Input Type: " << type << "\033[0m" << endl;
            ss << res.dump(2) << endl;
        } else {
            ss << "//##########################################################################################\n";
            ss << "\033[0;32m// Input Type: " << type << "\033[0m" << endl;
        }
        ss << "\033[1;31m//-------------------------------------------------------\033[0m\n\n"; 
        obj.generateStruct(res);
        ss << obj.result.str() << endl;
        obj.result.str("");
    }

    ofstream fd(outputFilePath, ios::app);
    regex ansiRegex(R"(\x1B\[[0-9;]*[mHfK])");
    string fd_text = regex_replace(ss.str(), ansiRegex, "");
    fd << fd_text;
    fd.close();

    auto outStr = regex_replace(ss.str(), regex("(//[^ ]|#+\n)"), "");
    if (arg != "-f") {
        cout << outStr;
        cout << "\033[1;31m------------------------------------------------------\033[0m\noutput Path: " << outputFilePath << endl;
    } else {
        cout << "output Path: " << outputFilePath << endl;
    }

    return 0;
}