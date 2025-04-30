import re
from collections import defaultdict


'''
--------代码格式化--------
输入：
namespace galaxcore { namespace Neptune { namespace Timing { namespace TmPtr { class abc; } } } }
namespace galaxcore { namespace Neptune { namespace Timing { class TmEdge; } } }
namespace galaxcore { class HVMCondition; }
namespace galaxcore { class HVMPerThreadCondCacher; }
namespace galaxcore { template <class T> class HSTPtrHashFcn; }

输出：
namespace galaxcore {
    class HVMCondition;
    class HVMPerThreadCondCacher;
    template <class T> class HSTPtrHashFcn;
} // namespace galaxcore

namespace galaxcore::Neptune::Timing {
    class TmEdge;
} // namespace galaxcore::Neptune::Timing

namespace galaxcore::Neptune::Timing::TmPtr {
    class abc;
} // namespace galaxcore::Neptune::Timing::TmPtr

'''


def parse_namespaces_from_code(code: str) -> str:
    # 压缩所有空白字符，方便正则解析
    code = re.sub(r'\s+', ' ', code.strip())

    def parse_namespace_block(code, current_ns=None):
        result = defaultdict(list)
        i = 0
        while i < len(code):
            # 尝试匹配 namespace 块
            ns_match = re.match(r'\s*namespace\s+(\w+)\s*\{', code[i:])
            if ns_match:
                ns_name = ns_match.group(1)
                brace_start = i + ns_match.end() - 1
                brace_count = 1
                j = brace_start + 1
                while j < len(code) and brace_count > 0:
                    if code[j] == '{':
                        brace_count += 1
                    elif code[j] == '}':
                        brace_count -= 1
                    j += 1
                inner_block = code[brace_start+1:j-1].strip()
                full_ns = f"{current_ns}::{ns_name}" if current_ns else ns_name
                inner_result = parse_namespace_block(inner_block, full_ns)
                for k, v in inner_result.items():
                    result[k].extend(v)
                i = j
                continue  # 避免误处理为语句
            # 跳过独立的 '}'
            elif code[i] == '}':
                i += 1
                continue
            # 匹配普通语句（类声明等）
            stmt_match = re.match(r'\s*([^;{}]+?)\s*;', code[i:])
            if stmt_match:
                stmt = stmt_match.group(1).strip()
                ns_key = current_ns if current_ns else 'global'
                result[ns_key].append(f'{stmt};')
                i += stmt_match.end()
            else:
                i += 1
        return result

    # 调用解析器
    final_result = parse_namespace_block(code)

    # 拼接输出字符串
    output = []
    for ns in sorted(final_result.keys(), key=lambda x: x or 'global'):
        output.append(f'namespace {ns} {{')
        for decl in final_result[ns]:
            output.append(f'    {decl}')
        output.append(f'}} // namespace {ns}\n')

    return '\n'.join(output)


if __name__ == '__main__':
    with open('input.txt', 'r', encoding='utf-8') as f:
        raw_input = f.read()

    formatted_output = parse_namespaces_from_code(raw_input)
    print(formatted_output)

    # 如果要写回文件：
    # with open('output.cpp', 'w', encoding='utf-8') as f:
    #     f.write(formatted_output)
