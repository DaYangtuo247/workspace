#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <cstdlib> // 用于调用系统命令
#include "include_graph.h"
#include "mark_edge.h"
#include "delete_include.h"


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << " <project Root Dir> <model: true/false>\n";
        // example: ./main.exe /home/user/project true
        return 1;
    }

    std::string root_dir = argv[1];
    bool test = (std::string(argv[2]) == "true");
    std::string input_filename = "./output.dot";

    IncludeGraph graph;
    graph.build(root_dir); // 构建包含关系图
    graph.generate_dependency_graph(input_filename); // 生成依赖关系图
    
    
    // ------------------------标记output.dot，生成mark_output.dot------------------------
    // 解析输入文件（不改变 input.dot 的边记录）
    parse_dot_file(input_filename);
    
    string directory = get_directory(input_filename);
    string filename = get_filename(input_filename);
    string output_filename = directory + "/mark_" + filename;
    // 根据原始边记录进行冗余检测，并写入输出文件（标记冗余边）
    write_dot_file(output_filename);
    
    // 输出标记后的 dot 文件
    std::cout << "Dependency graph generated: " << input_filename << ", " << output_filename << "\n";
    

    // -------------------------------调用 Graphviz 生成图片-------------------------------
    std::string command1 = "dot -Tpng " + input_filename + " -o " + input_filename + ".png";
    std::string command2 = "dot -Tpng " + output_filename + " -o " + output_filename + ".png";
    int result1 = std::system(command1.c_str());
    int result2 = std::system(command2.c_str());
    if (result1 == 0 && result2 == 0) {
        std::cout << "Graph images generated: " << input_filename << ".png and " << output_filename << ".png" << "\n";
    } else {
        std::cerr << "Failed to generate graph images. Please ensure Graphviz is installed and accessible.\n";
    }


    // ------------------------根据结果，去文件路径中删除冗余的头文件引用------------------------
    if (test) {
        return 0;
    }
    vector<Removal> removals = parse_dot_file_edge(output_filename);

    for (const auto& item : removals) {
        string source_file = item.first;
        string target_header = item.second;

        string full_source_path;
        if (find_file_recursive(root_dir, source_file, full_source_path)) {
            remove_include_line(full_source_path, target_header);
        } else {
            cerr << "no found header file: " << source_file << " in " << root_dir << endl;
        }
    }

    return 0;
}