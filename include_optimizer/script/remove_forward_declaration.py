import re
from collections import OrderedDict

def remove_forward_declarations_from_file(filepath):
    # 1. 读取所有行
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    # 2. 定位 NAMESPACE_BEGIN 与 //FWD_DECL_END 索引
    begin_idx = end_idx = None
    for i, ln in enumerate(lines):
        if 'NAMESPACE_BEGIN' in ln and begin_idx is None:
            begin_idx = i
        elif '//FWD_DECL_END' in ln and begin_idx is not None:
            end_idx = i
            break
    if begin_idx is None or end_idx is None:
        raise RuntimeError('未找到 NAMESPACE_BEGIN 或 //FWD_DECL_END 标记')

    # 3. 提取并格式化原有声明
    orig_block = [ln.strip() for ln in lines[begin_idx+1:end_idx] if ln.strip()]

    # 4. 准备上下文
    before = lines[:begin_idx+1]    # 包含 NAMESPACE_BEGIN
    end_line = lines[end_idx]       # //FWD_DECL_END
    after = lines[end_idx+1:]       # 标记后所有行

    # 5. 正则定义
    ns_open_re = re.compile(r'^\s*namespace\s+(\w+)\s*{')
    fwd_decl_re = re.compile(r'^\s*(class|struct)\s+(\w+)\s*;')

    # 6. 扫描全文，提取前向声明并记录其行号
    brace_count = 0
    namespace_stack = []
    new_decls = []
    new_set = set()
    to_remove_lines = set()

    for i, ln in enumerate(lines):
        stripped = ln.strip()
        if begin_idx < i < end_idx:
            continue  # 忽略 NAMESPACE_BEGIN 与 //FWD_DECL_END 之间的内容

        # namespace 入栈
        m_ns = ns_open_re.match(ln)
        if m_ns:
            namespace_stack.append((m_ns.group(1), brace_count))

        # 前向声明
        m_fd = fwd_decl_re.match(stripped)
        if m_fd:
            kind, name = m_fd.groups()
            decl = f"{kind} {name};"
            ns_path = [ns for ns, _ in namespace_stack]
            if ns_path:
                prefix = 'namespace ' + ' { namespace '.join(ns_path)
                suffix = ' }' * len(ns_path)
                line = f"{prefix} {{ {decl}{suffix}"
            else:
                line = decl

            if decl not in orig_block and decl not in new_set:
                new_decls.append(line)
                new_set.add(decl)

            # 标记原声明行用于删除
            to_remove_lines.add(i)

        # 更新 brace_count
        brace_count += ln.count('{') - ln.count('}')

        # namespace 出栈
        while namespace_stack and brace_count <= namespace_stack[-1][1]:
            namespace_stack.pop()

    # 7. 合并原有和新增声明，去重并排序
    merged = sorted(set(orig_block + new_decls), key=lambda s: s)
    decl_lines = [s + '\n' for s in merged]

    # 8. 删除原文件中所有前向声明
    filtered_lines = []
    for i, ln in enumerate(lines):
        if i in to_remove_lines:
            continue
        if begin_idx < i < end_idx:
            continue  # 删除原有声明区域的内容
        filtered_lines.append(ln)

    # 9. 写回文件
    with open(filepath, 'w', encoding='utf-8') as f:
        f.writelines(before)
        f.writelines(decl_lines)
        f.write(end_line)
        f.writelines(after if end_idx + 1 >= len(lines) else filtered_lines[end_idx + 1:])

    print(f"Processed forward declarations into {filepath}")

# 示例调用: remove_forward_declarations_from_file('your_file.cpp')


# 使用示例
file_path = r'c:\Users\24736\Desktop\workspace\test.cpp'
remove_forward_declarations_from_file(file_path)