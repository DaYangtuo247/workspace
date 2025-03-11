#include "container_convert.cc"
#define TEMPLATE_PATH "../container_convert/template.json"

// 处理 IDA local type的.h文件
void solve_IDA_H(std::string inputFile, vector<string> & testCases) {
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
    regex pattern(R"(^\s*(?:const|matable)?\s*([\w:]+)\s([&*\w ]+);\s//(<.*>))");
    
    // 遍历每一行，匹配后进行处理
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].find("CSL::XML") != std::string::npos)
            continue;
        try {
            smatch match;
            if (regex_search(lines[i], match, pattern)) {
                std::string container_name = match[1].str() + match[3].str();
                //cout << "[" << container_name << "]" <<endl;
                testCases.push_back(container_name);
                json res = obj.parseContainer(container_name);
                std::string container_name_new = obj.generateStruct(res);
                lines[i] = string("\t") + container_name_new + string(" ") + match[2].str() + ";";
            }
        } catch (const exception &e) {
            cerr << "solve line " << i + 1 << " error" << e.what() << endl;
        }
    }
    
    // 写入处理后的结果到新文件（文件名在原文件后添加 _new 后缀）
    string outputFile = std::string("new_") + inputFile;
    ofstream out(outputFile);
    for (const auto &l : lines)
        out << l << "\n";
    out.close();

    cout << "generate newFile: " << outputFile << endl;
}

int main(int argc, char* argv[]) {
    // 显示帮助信息
    if (argc == 1 || (argc >= 2 && string(argv[1]) == "-h")) {
        std::cout << "Options:\n";
        std::cout << "  [-h]                         Show this help message\n";
        std::cout << "  -d [output_file]             Generate [output_file].h.bak, clean [output_file].h\n";
        std::cout << "  -r input_file  [output_file] Retrieve container types from the input_file file.\n";
        std::cout << "  -f file.h      [output_file] The input is the file.h file of IDA local type.\n";
        std::cout << "  variable_name  [output_file] Container convert\n";
        std::cout << "  container_name [output_file] Container convert\n";
        std::cout << "Default output: ./container_convert.h\n";
      return 0;
    }

    containerConvert obj(TEMPLATE_PATH);
    vector<string> testCases;

    // 获取第一个参数
    string arg = argv[1];
    // 获取第二个参数
    std::string outputFilePath = argc == 3 ? argv[2] : "./container_convert.h";
    
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
        std::fstream input(inputPath, std::ios::in);
        if (!input.is_open())
            assert(0);
        std::string line;
        while(getline(input, line))
            testCases.push_back(line);
        input.close();
    
    // 输入的是 IDA local type 的.h文件
    } else if (arg == "-f") {
        if (argc < 3) {
            cout << "input error!!!" << endl;
            exit(0);
        }
        string inputPath = argv[2];
        outputFilePath = argc == 4 ? argv[3] : "./container_convert.h";
        solve_IDA_H(inputPath, testCases);
    
    // 输入的是变量
    } else if (regex_match(arg, regex("^[a-zA-Z0-9_]+$"))) {
        string command = "/home/xiaonan/Share/scripts/toolUnified/toolUnified.py " + arg + " -hg -in_gvim";
        string temp = obj.getVariableType(command);
        testCases.push_back(temp);

    // 输入的是容器
    } else if (regex_match(arg, regex("^.*<.*>.*$"))) {
        testCases.push_back(argv[1]);
    } else {
        cout << "input error!!!" << endl;
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

    ofstream fd(outputFilePath, std::ios::app);
    regex ansiRegex(R"(\x1B\[[0-9;]*[mHfK])");
    string fd_text = regex_replace(ss.str(), ansiRegex, "");
    fd << fd_text;
    fd.close();

    auto outStr = regex_replace(ss.str(), std::regex("(//[^ ]|#+\n)"), "");
    if (arg != "-f") {
        cout << outStr;
        cout << "\033[1;31m------------------------------------------------------\033[0m\noutput Path: " << outputFilePath << endl;
    } else {
        cout << "output Path: " << outputFilePath << endl;
    }

    return 0;
}