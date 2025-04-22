#include <string>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

class IncludeGraph {
public:
    void build(const std::string& root_dir);
    void generate_dependency_graph(const std::string& output_dot_file);

private:
    void scan_directory(const std::string& path);
    void parse_file(const std::string& filepath);

    std::unordered_map<std::string, std::unordered_set<std::string>> include_map;
};


void IncludeGraph::build(const std::string& root_dir) {
    scan_directory(root_dir);
}

void IncludeGraph::scan_directory(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        std::cerr << "Invalid directory: " << path << "\n";
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string entry_name = entry->d_name;

        // 跳过当前目录和父目录
        if (entry_name == "." || entry_name == "..") {
            continue;
        }

        std::string full_path = path + "/" + entry_name;

        struct stat entry_stat;
        if (stat(full_path.c_str(), &entry_stat) == -1) {
            std::cerr << "Failed to stat file: " << full_path << "\n";
            continue;
        }

        if (S_ISDIR(entry_stat.st_mode)) {
            // 如果是目录，递归扫描子目录
            scan_directory(full_path);
        } else if (S_ISREG(entry_stat.st_mode) && full_path.substr(full_path.find_last_of('.') + 1) == "h") {
            // 如果是头文件，解析文件内容
            parse_file(full_path);
        }
    }

    closedir(dir);
}

void IncludeGraph::parse_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << "\n";
        return;
    }

    std::string line;
    // 使用正则表达式匹配 #include "filename"
    std::regex include_regex("#include\\s*\"([^\"]+)\"");
    std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);

    while (std::getline(file, line)) {
        std::smatch match;
        try {
            if (std::regex_search(line, match, include_regex)) {
                // 提取包含的文件名并存入 include_map
                std::string include_path = match[1];
                std::string include_file = include_path.substr(include_path.find_last_of("/\\") + 1);
                include_map[filename].insert(include_file);
            }
        } catch (const std::regex_error& e) {
            std::cerr << "Regex error in file: " << filepath << " - " << e.what() << "\n";
            return;
        }
    }
}

void IncludeGraph::generate_dependency_graph(const std::string& output_dot_file) {
    std::ofstream dot_file(output_dot_file);
    if (!dot_file.is_open()) {
        std::cerr << "Failed to open file: " << output_dot_file << "\n";
        return;
    }

    dot_file << "digraph IncludeGraph {\n";
    for (const auto& pair : include_map) {
        const std::string& file = pair.first;
        const auto& includes = pair.second;

        for (const auto& include : includes) {
            // 生成 DOT 格式的依赖关系图
            dot_file << "    \"" << file << "\" -> \"" << include << "\";\n";
        }
    }
    dot_file << "}\n";

    dot_file.close();
}
