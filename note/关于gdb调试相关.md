## 关于`gdb`调试相关

### examine

`gdb` 中的 `examine` 命令（缩写为 `x`）用于查看内存中的数据。它可以根据指定的格式显示内存内容，常用于调试时查看变量、指针、堆栈以及程序的运行状态。

`x` 命令的基本格式为：

```
x/[n][f][u] address
```

- **n**: 表示要查看的单元数（count），即要读取的字节、半字、字等的数量。例如，`x/10x` 表示查看 10 个单元。
- **f**: 表示显示的格式，常见的格式包括：
  - `x` 或 `xg`：十六进制格式
  - `d`：十进制格式
  - `u`：无符号十进制格式
  - `o`：八进制格式
  - `t`：二进制格式
  - `f`：浮点数格式
  - `i`：反汇编
  - `c`：字符格式
  - `s`：字符串格式
- **u**: 单位大小，表示每个单元的字节数。常用的单位大小：
  - `b`：字节（1 字节）
  - `h`：半字（2 字节）
  - `w`：字（4 字节）
  - `g`：双字（8 字节）
  

例如：

1. **查看某个地址的 4 个字节的十六进制表示**:
   ```
   x/4x 0x601040
   ```

2. **查看内存中的字符串**:
   ```
   x/s 0x601040
   ```

3. **查看内存的十六进制和十进制表示**:
   ```
   x/2x 0x601040  # 查看两个单元，以十六进制显示
   x/2d 0x601040  # 查看两个单元，以十进制显示
   ```

4. **反汇编内存中的指令**:
   ```
   x/10i 0x400560
   ```

#### 详细说明：

- `x/10x`：显示从指定地址开始的 10 个单位，单位以十六进制格式显示。
- `x/5i`：将从指定地址开始的 5 条指令进行反汇编。
- `x/s`：显示从指定地址开始的字符串。
- `x/bx`：以字节为单位，并用十六进制显示。

这些命令可以帮助开发者在调试过程中快速查看和分析内存内容。