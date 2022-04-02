# 内联汇编

- [参考资料：https://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C](https://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C)

> 汇编语言以两种形式出现：英特尔风格和 AT&T 风格。 GNU C 编译器，即 GCC 使用 AT&T 语法，这就是我们将使用的。

1. 寄存器命名：寄存器名称以 % 为前缀，因此寄存器为 %eax、%cl 等，而不仅仅是 eax、cl。
2. 操作数的顺序：与 Intel 约定（第一个操作数是目标）不同，操作数的顺序是源在前，目标在后。 例如，英特尔语法“mov eax, edx”在 AT&T 汇编中看起来像“mov %edx, %eax”。
3. 操作数大小：在 AT&T 语法中，内存操作数的大小由操作码名称的最后一个字符确定。 后缀是 b 表示（8 位）字节，w 表示（16 位）字，l 表示（32 位）长。 例如，上述指令的正确语法应该是“movl %edx, %eax”。
4. 立即数操作数：立即数操作数用 $ 前缀标记，如“addl $5, %eax”，表示将立即数长值 5 添加到寄存器 %eax）。
5. 内存操作数：缺少操作数前缀表示它是内存地址； 因此 "movl $bar, %ebx" 将变量 bar 的地址放入寄存器 %ebx，而 "movl bar, %ebx" 将变量 bar 的内容放入寄存器 %ebx。
6. 索引：索引或间接通过将索引寄存器或间接存储单元地址括在括号中来完成。 例如，“movl 8(%ebp), %eax”（将 %ebp 指向的单元格偏移 8 处的内容移动到寄存器 %eax 中）。
