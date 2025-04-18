#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <cstdlib> // 用于调用系统命令
#include <regex>
#include <string>
#include "include_graph.h"
#include "mark_edge.h"
#include "delete_include.h"

using namespace std;

// 将 .dot 文件中的箭头方向翻转
void reverse_dot_file(const string& filepath) {
    ifstream infile(filepath);
    vector<string> reversed_lines;
    string line;

    // 正则表达式匹配箭头方向
    regex edge_pattern(R"(\s*\"([^\"]+)\"\s*->\s*\"([^\"]+)\"(.*);)");

    while (getline(infile, line)) {
        smatch match;
        if (regex_match(line, match, edge_pattern)) {
            // 提取源节点和目标节点，并翻转箭头方向
            string source = match[1];
            string target = match[2];
            string attributes = match[3]; // 保留边的属性（如 [color = red]）

            // 构造翻转后的边
            string reversed_line = "    \"" + target + "\" -> \"" + source + "\"" + attributes + ";";
            reversed_lines.push_back(reversed_line);
        } else {
            // 非边定义的行直接保留
            reversed_lines.push_back(line);
        }
    }
    infile.close();

    // 写回翻转后的内容
    ofstream outfile(filepath);

    for (const auto& reversed_line : reversed_lines) {
        outfile << reversed_line << "\n";
    }

    outfile.close();
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << " <project Root Dir> outputName [rp]\n";
        std::cerr << "rp: true to remove redundant includes, false to keep them \n";
        // example: ./main.exe /home/user/project [replace]
        return 1;
    }

    std::string root_dir = argv[1];

    bool test = false;
    if (argc == 4 && std::string(argv[3]) == "rp") {
        test = true;
    }

    std::string output_file = string("./") + argv[2] + ".dot";
    cout << "output file: " << output_file << endl;

    IncludeGraph graph;
    graph.build(root_dir); // 构建包含关系图
    graph.generate_dependency_graph(output_file); // 生成依赖关系图
    
    
    // ------------------------标记output.dot，生成mark_output.dot------------------------
    // 解析输入文件（不改变 input.dot 的边记录）
    parse_dot_file(output_file);
    
    string directory = get_directory(output_file);
    string filename = get_filename(output_file);
    string mark_output_file = directory + "/mark_" + filename;
    // 根据原始边记录进行冗余检测，并写入输出文件（标记冗余边）
    write_dot_file(mark_output_file);
    
    // 输出标记后的 dot 文件
    std::cout << "Dependency graph generated: " << output_file << ", " << mark_output_file << "\n";
    
    reverse_dot_file(output_file);
    reverse_dot_file(mark_output_file);

    // -------------------------------调用 Graphviz 生成图片-------------------------------
    std::string command1 = "dot -Tsvg " + output_file + " -o " + output_file + ".svg";
    std::string command2 = "dot -Tsvg " + mark_output_file + " -o " + mark_output_file + ".svg";
    int result1 = std::system(command1.c_str());
    int result2 = std::system(command2.c_str());
    if (result1 == 0 && result2 == 0) {
        std::cout << "Graph images generated: " << output_file << ".svg and " << mark_output_file << ".svg" << "\n";
    } else {
        std::cerr << "Failed to generate graph images. Please ensure Graphviz is installed and accessible.\n";
    }
    

    // ------------------------根据结果，去文件路径中删除冗余的头文件引用------------------------
    if (!test) {
        return 0;
    }
    reverse_dot_file(mark_output_file);
    vector<Removal> removals = parse_dot_file_edge(mark_output_file);

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