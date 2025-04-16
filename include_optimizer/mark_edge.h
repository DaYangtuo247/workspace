#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <queue>

using namespace std;

// 定义图节点结构
struct GraphNode {
    string name;                     // 节点名称
    vector<GraphNode*> neighbors;    // 邻居节点
};

// 全局图映射（文件名到节点）
unordered_map<string, GraphNode*> GraphMap;
// 存储每一条边（保留 input.dot 中的顺序和重复），反向存储
// 例如 "file1.h" -> "file3.h" 表示 file1.h 包含 file3.h，解析后存储为 ("file3.h", "file1.h")
vector<pair<GraphNode*, GraphNode*>> Edges;

// 辅助函数：从包含双引号的 token 中提取出实际字符串
string extractQuoted(const string& token) {
    size_t start = token.find('\"');
    size_t end = token.rfind('\"');
    if (start != string::npos && end != string::npos && start < end) {
        return token.substr(start + 1, end - start - 1);
    }
    return token; // 没有找到双引号则返回原串
}

// 读取 .dot 文件并解析图
// 注意：这里不对边进行去重，完全按照 input.dot 中的记录处理
void parse_dot_file(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        // 只处理包含 "->" 的行
        if (line.find("->") != string::npos) {
            stringstream ss(line);
            string token_u, arrow, token_v;
            ss >> token_u >> arrow >> token_v; // 例如解析 "file1.h" "->" "file3.h;" 
            
            // 提取文件名；注意 token_v 可能带有尾部的 ';'
            string u = extractQuoted(token_u);
            if (!token_v.empty() && token_v.back() == ';')
                token_v.pop_back();
            string v = extractQuoted(token_v);
            
            // 按照预期进行反向存储，即将 "u -> v" 转为 "v -> u"
            if (GraphMap.find(u) == GraphMap.end()) {
                GraphMap[u] = new GraphNode{u, {}};
            }
            if (GraphMap.find(v) == GraphMap.end()) {
                GraphMap[v] = new GraphNode{v, {}};
            }
            // 添加反向边：从 v 指向 u
            GraphMap[v]->neighbors.push_back(GraphMap[u]);
            // 保存边记录，保留每一条（包括重复）
            Edges.push_back({GraphMap[v], GraphMap[u]});
        }
    }
}

// 广度优先搜索：判断从节点 a 出发是否可以到达节点 b
bool bfs(GraphNode* a, GraphNode* b) {
    unordered_map<GraphNode*, bool> visited;
    queue<GraphNode*> q;
    q.push(a);
    visited[a] = true;
    while (!q.empty()) {
        GraphNode* current = q.front();
        q.pop();
        if (current == b) return true;
        for (GraphNode* neighbor : current->neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    return false;
}

// 判断边 u -> v（即实际表示 v include u）是否为冗余
// 修正思路：删除所有相同的边实例，再进行判断，最后恢复所有实例
bool is_redundant_edge(GraphNode* u, GraphNode* v) {
    auto& neighbors = u->neighbors;
    // 找到所有指向 v 的位置（可能有多个重复）
    vector<size_t> positions;
    for (size_t i = 0; i < neighbors.size(); i++) {
        if (neighbors[i] == v)
            positions.push_back(i);
    }
    if (positions.empty())
        return false; // 不存在此边

    // 删除所有找到的边实例（从后向前删除，避免影响前面的索引）
    vector<GraphNode*> removedEdges;
    for (int i = positions.size() - 1; i >= 0; i--) {
        size_t pos = positions[i];
        removedEdges.push_back(neighbors[pos]);
        neighbors.erase(neighbors.begin() + pos);
    }
    
    // 检查删除所有这些边后是否仍能从 u 到达 v
    bool redundant = bfs(u, v);
    
    // 恢复所有被删除的边（恢复顺序无关紧要，只要保证数量正确）
    // 这里我们将每个边恢复到末尾即可，因为后续的 BFS 检测不会受影响
    for (size_t i = 0; i < removedEdges.size(); i++) {
        neighbors.push_back(removedEdges[i]);
    }
    
    return redundant;
}

// 将结果写入新的 .dot 文件，标记冗余边（添加 [color = red] 属性）
void write_dot_file(const string& filename) {
    ofstream file(filename);
    file << "digraph G {\n";
    // 按照 input.dot 中的边顺序处理（包括重复记录）
    for (auto &edge : Edges) {
        // 注意这里仍然是反向输出，即输出 "v -> u"
        GraphNode* u = edge.first;
        GraphNode* v = edge.second;
        if (is_redundant_edge(u, v))
            file << "    \"" << v->name << "\" -> \"" << u->name << "\" [color = red];\n";
        else
            file << "    \"" << v->name << "\" -> \"" << u->name << "\";\n";
    }
    file << "}\n";
}

void print_graph() {
    for (const auto& p : GraphMap) {
        const string& name = p.first;
        GraphNode* node = p.second;
        cout << "node: [" << name << "], neighbor: ";
        for (GraphNode* neighbor : node->neighbors) {
            cout << neighbor->name << " ";
        }
        cout << endl;
    }
}

string get_directory(const string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == string::npos) return "."; // 当前目录
    return filepath.substr(0, pos);
}

string get_filename(const string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == string::npos) return filepath;
    return filepath.substr(pos + 1);
}

// int main(int argc, char* argv[]) {
//     if (argc < 2) {
//         cerr << "用法: " << argv[0] << " <input_filename.dot>" << endl;
//         return 1;
//     }

//     string input_filename = argv[1];
//     string directory = get_directory(input_filename);
//     string filename = get_filename(input_filename);
//     string output_filename = directory + "/mark_" + filename;

//     // 解析输入文件（不改变 input.dot 的边记录）
//     parse_dot_file(input_filename);
    
//     // 可打印图结构用于调试
//     print_graph();
    
//     // 根据原始边记录进行冗余检测，并写入输出文件（标记冗余边）
//     write_dot_file(output_filename);
    
//     cout << "Done! Output written to " << output_filename << endl;

//     return 0;
// }