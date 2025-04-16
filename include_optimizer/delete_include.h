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

// ���ߺ�������ȡ����·�����ļ���
string get_filename_only(const string& path) {
    size_t pos = path.find_last_of("/\\");
    return (pos == string::npos) ? path : path.substr(pos + 1);
}

// ���� .dot �ļ��� color=red �ı�
vector<Removal> parse_dot_file_edge(const string& dot_path) {
    vector<Removal> removals;
    ifstream infile(dot_path);
    if (!infile) {
        cerr << "�޷��� dot �ļ�: " << dot_path << endl;
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

// �ݹ�����ļ������ص�һ��ƥ�������·��
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

// ���ļ����Ƴ� #include ĳ��ͷ�ļ���ֻƥ���ļ�����
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

    static unordered_set<string> include_set; // ���ڼ�¼�Ѿ���ӡ�����ļ�
    vector<string> removed_lines;  // �洢���Ƴ�������

    while (getline(infile, line)) {
        smatch match;
        if (regex_search(line, match, include_pattern)) {
            string included = match[1];
            if (get_filename_only(included) == target_filename) {
                removed_lines.emplace_back(line); // ��¼�кź�����
                removed = true;
                continue; // �������У���д�����ļ�
            }
        }
        new_lines.push_back(line); // ����δ���Ƴ�����
    }
    infile.close();

    // ������Ƴ������ݣ���ӡ�ļ�·���ͱ��Ƴ�����
    if (removed) {
        if (include_set.find(filepath) == include_set.end()) {
            include_set.insert(filepath);
            cout << "remove include line: " << filepath << endl;
        }
        for (const auto& content : removed_lines) {
            cout << "    - " << content << endl;
        }

        // д���޸ĺ���ļ�����
        ofstream outfile(filepath);
        for (const auto& l : new_lines) {
            outfile << l << "\n";
        }
    }
}

// int main(int argc, char* argv[]) {
//     if (argc != 3) {
//         cerr << "�÷�: " << argv[0] << " <dot�ļ�·��> <ͷ�ļ�Ŀ¼>" << endl;
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
//             cerr << "?? δ�ҵ�ͷ�ļ�: " << source_file << " in " << header_root << endl;
//         }
//     }

//     return 0;
// }
