#include "../utility.h"
#include "container_convert.cc"

int main(int argc, char* argv[]) {
    // 显示帮助信息
    if (argc == 1 || (argc >= 2 && string(argv[1]) == "-h")) {
        std::cout << "Options:\n";
        std::cout << "  -h                           Show this help message\n";
        std::cout << "  -d [output_file]             Generate [output_file].h.bak, clean [output_file].h\n";
        std::cout << "  -r input_file  [output_file] Retrieve container types from the input_file file.\n";
        std::cout << "  variable_name  [output_file] Container convert\n";
        std::cout << "  container_name [output_file] Container convert\n";
        std::cout << "Default output: ./container_convert.h\n";
      return 0;
    }

    containerConvert obj("../template.json");
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
            ss << "//Input Type: " << type << endl;
            ss << res.dump(2) << endl;
            ss << "------------------------------------------------------\n";
        } else {
            ss << "//------------------------------------------------------\n";
            ss << "// " << type << endl;
        }
        obj.generateStruct(res);
        ss << obj.result.str() << endl;
        obj.result.str("");
    }

    std::ofstream fd(outputFilePath, std::ios::app);
    fd << ss.str();
    fd.close();

    cout << ss.str().substr(2);
    cout << "------------------------------------------------------\noutput Path: " << outputFilePath << endl;

    return 0;
}