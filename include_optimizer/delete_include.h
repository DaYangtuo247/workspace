#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <utility>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;

using Removal = pair<string, string>;  // {source_file, target_header}

// 工具函数：获取不带路径的文件名
string get_filename_only(const string& path) {
    size_t pos = path.find_last_of("/\\");
    return (pos == string::npos) ? path : path.substr(pos + 1);
}

// 解析 .dot 文件中 color=red 的边
vector<Removal> parse_dot_file_edge(const string& dot_path) {
    vector<Removal> removals;
    ifstream infile(dot_path);
    if (!infile) {
        cerr << "无法打开 dot 文件: " << dot_path << endl;
        return removals;
    }

    regex pattern(R"xx("([^"]+)"\s*->\s*"([^"]+)"\s*\[.*color\s*=\s*red.*\])xx");
    string line;

    while (getline(infile, line)) {
        smatch match;
        if (regex_search(line, match, pattern)) {
            string source = match[1];
            string target = get_filename_only(match[2]);
            removals.emplace_back(source, target);
        }
    }

    return removals;
}

// 递归查找文件：返回第一个匹配的完整路径
bool find_file_recursive(const string& base_dir, const string& filename, string& result_path) {
    DIR* dir = opendir(base_dir.c_str());
    if (!dir) return false;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string name = entry->d_name;
        if (name == "." || name == "..") continue;

        string full_path = base_dir + "/" + name;

        struct stat info;
        stat(full_path.c_str(), &info);

        if (S_ISDIR(info.st_mode)) {
            if (find_file_recursive(full_path, filename, result_path)) {
                closedir(dir);
                return true;
            }
        } else if (S_ISREG(info.st_mode)) {
            if (name == filename) {
                result_path = full_path;
                closedir(dir);
                return true;
            }
        }
    }

    closedir(dir);
    return false;
}

// 从文件中移除 #include 某个头文件（只匹配文件名）
void remove_include_line(const string& filepath, const string& target_filename) {
    ifstream infile(filepath);
    if (!infile) {
        cerr << "open file error: " << filepath << endl;
        return;
    }

    vector<string> new_lines;
    string line;
    bool removed = false;

    regex include_pattern(R"(^\s*#\s*include\s*[<"]([^">]+)[">])");

    static unordered_set<string> include_set; // 用于记录已经打印过的文件
    vector<string> removed_lines;  // 存储被移除的内容

    while (getline(infile, line)) {
        smatch match;
        if (regex_search(line, match, include_pattern)) {
            string included = match[1];
            if (get_filename_only(included) == target_filename) {
                removed_lines.emplace_back(line); // 记录行号和内容
                removed = true;
                continue; // 跳过该行，不写入新文件
            }
        }
        new_lines.push_back(line); // 保留未被移除的行
    }
    infile.close();

    // 如果有移除的内容，打印文件路径和被移除的行
    if (removed) {
        if (include_set.find(filepath) == include_set.end()) {
            include_set.insert(filepath);
            cout << "remove include line: " << filepath << endl;
        }
        for (const auto& content : removed_lines) {
            cout << "    - " << content << endl;
        }

        // 写回修改后的文件内容
        ofstream outfile(filepath);
        for (const auto& l : new_lines) {
            outfile << l << "\n";
        }
    }
}

// int main(int argc, char* argv[]) {
//     if (argc != 3) {
//         cerr << "用法: " << argv[0] << " <dot文件路径> <头文件目录>" << endl;
//         return 1;
//     }

//     string dot_file = argv[1];
//     string header_root = argv[2];
//     vector<Removal> removals = parse_dot_file_edge(dot_file);

//     for (const auto& item : removals) {
//         string source_file = item.first;
//         string target_header = item.second;

//         string full_source_path;
//         if (find_file_recursive(header_root, source_file, full_source_path)) {
//             remove_include_line(full_source_path, target_header);
//         } else {
//             cerr << "?? 未找到头文件: " << source_file << " in " << header_root << endl;
//         }
//     }

//     return 0;
// }
