/**
 * @brief 内联汇编
 * @details
 * ./first.md
 */

#include <stdio.h>

// for example 4
int gcd(int a, int b);
// for example 7
float sinx(float degree);
float cosx(float degree);
float square_root(float val);

int main() {
    // example 0

    /* Add 10 and 20 and store result into register %eax */
    __asm__("movl $10, %eax;"
            "movl $20, %ebx;"
            "addl %ebx, %eax;");

    /* Subtract 20 from 10 and store result into register %eax */
    __asm__("movl $10, %eax;"
            "movl $20, %ebx;"
            "subl %ebx, %eax;");

    /* Multiply 10 and 20 and store result into register %eax */
    __asm__("movl $10, %eax;"
            "movl $20, %ebx;"
            "imull %ebx, %eax;");

    // example 1
    int val;
    asm("movl %%eax, %0;" : "=r"(val));
    printf("example 1: %d\n", val);
    /***
     * 在本例中，变量“val”保存在寄存器中，
     * 寄存器 eax 中的值被复制到该寄存器中，
     * “val”的值从该寄存器更新到内存中。
     * 当指定“r”约束时，
     * gcc 可以将变量保存在任何可用的通用寄存器中。
     * 我们还可以通过使用特定的寄存器约束直接指定寄存器名称。
     * 寄存器约束如下：
     * +---+--------------------+
     * | r |    Register(s)     |
     * +---+--------------------+
     * | a |   %eax, %ax, %al   |
     * | b |   %ebx, %bx, %bl   |
     * | c |   %ecx, %cx, %cl   |
     * | d |   %edx, %dx, %dl   |
     * | S |   %esi, %si        |
     * | D |   %edi, %di        |
     * +---+--------------------+
     */

    // example 2
    int no = 100;
    asm("movl %1, %%ebx;"
        "movl %%ebx, %0;"
        : "=r"(val) /* output */
        : "r"(no)   /* input */
        : "%ebx"    /* clobbered register */
    );
    printf("example 2: %d %d\n", no, val);
    /***
     * 在上面的示例中，“val”是输出操作数，由 %0 引用，
     * “no”是输入操作数，由 %1 引用。
     * “r”是对操作数的约束，它告诉 GCC 使用任何寄存器来存储操作数。
     * 输出操作数约束应该有一个约束修饰符“=”来指定只写模式下的输出操作数。
     * 寄存器名称前有两个 % 前缀，这有助于 GCC 区分操作数和寄存器。
     * 操作数有一个 % 作为前缀。
     * 第三个冒号后被破坏的寄存器 %ebx 通知 GCC %ebx 的值将在 "asm" 中修改，
     * 因此 GCC 不会使用该寄存器来存储任何其他值。
     */

    // example 3
    int arg1 = 1, arg2 = 2, add;
    __asm__("addl %%ebx, %%eax;" : "=a"(add) : "a"(arg1), "b"(arg2));
    printf("example 3: %d %d %d\n", arg1, arg2, add);
    /***
     * 这里的“add”是寄存器 eax 引用的输出操作数。
     * arg1 和 arg2 分别是寄存器 eax 和 ebx 引用的输入操作数。
     * 让我们看一个使用扩展内联汇编语句的完整示例。
     * 它对整数操作数执行简单的算术运算并显示结果（在下载中作为算术.c 提供）。
     */

    // example 4
    {
        int arg1, arg2, add, sub, mul, quo, rem;

        arg1 = 4;
        arg2 = 2;

        /* Perform Addition, Subtraction, Multiplication & Division */
        __asm__("addl %%ebx, %%eax;" : "=a"(add) : "a"(arg1), "b"(arg2));
        __asm__("subl %%ebx, %%eax;" : "=a"(sub) : "a"(arg1), "b"(arg2));
        __asm__("imull %%ebx, %%eax;" : "=a"(mul) : "a"(arg1), "b"(arg2));

        __asm__("movl $0x0, %%edx;"
                "movl %2, %%eax;"
                "movl %3, %%ebx;"
                "idivl %%ebx;"
                : "=a"(quo), // 整除结果
                  "=d"(rem)  // 取余结果
                : "g"(arg1), "g"(arg2));

        printf("example 4:\n");
        printf("%d + %d = %d\n", arg1, arg2, add);
        printf("%d - %d = %d\n", arg1, arg2, sub);
        printf("%d * %d = %d\n", arg1, arg2, mul);
        printf("%d / %d = %d\n", arg1, arg2, quo);
        printf("%d %% %d = %d\n", arg1, arg2, rem);
    }

    // exapmle 5
    /***
     * 如果我们的汇编语句必须在我们放置的地方执行，
     * （即不能作为优化移出循环），
     * 请将关键字“volatile”或
     * “__volatile__”放在
     * “asm”或“__asm__”之后和（）之前.
     */
    // 请参考以下示例，
    // 该示例使用众所周知的欧几里得算法（被誉为第一算法）计算最大公约数。
    int first = 6, second = 9;
    printf("example 5:\n");
    printf("GCD of %d & %d is %d\n", first, second, gcd(first, second));

    // exapmle 6
    // 下面是更多使用 FPU（浮点单元）指令集的示例。
    // 执行简单浮点运算的示例程序：
    {
        float arg1, arg2, add, sub, mul, div;

        printf("Enter two numbers : ");
        scanf("%f%f", &arg1, &arg2);

        /* Perform floating point Addition, Subtraction, Multiplication &
         * Division */
        __asm__("fld %1;"
                "fld %2;"
                "fadd;"
                "fstp %0;"
                : "=g"(add)
                : "g"(arg1), "g"(arg2));

        __asm__("fld %2;"
                "fld %1;"
                "fsub;"
                "fstp %0;"
                : "=g"(sub)
                : "g"(arg1), "g"(arg2));

        __asm__("fld %1;"
                "fld %2;"
                "fmul;"
                "fstp %0;"
                : "=g"(mul)
                : "g"(arg1), "g"(arg2));

        __asm__("fld %2;"
                "fld %1;"
                "fdiv;"
                "fstp %0;"
                : "=g"(div)
                : "g"(arg1), "g"(arg2));

        printf("%f + %f = %f\n", arg1, arg2, add);
        printf("%f - %f = %f\n", arg1, arg2, sub);
        printf("%f * %f = %f\n", arg1, arg2, mul);
        printf("%f / %f = %f\n", arg1, arg2, div);
    }

    // exapmle 7
    // 计算三角函数（如 sin 和 cos）的示例程序：
    float theta;
    printf("Enter theta in degrees : ");
    scanf("%f", &theta);

    printf("sinx(%f) = %f\n", theta, sinx(theta));
    printf("cosx(%f) = %f\n", theta, cosx(theta));
    printf("square_root(%f) = %f\n", theta, square_root(theta));

    return 0;
}

int gcd(int a, int b) {
    int result;
    /* Compute Greatest Common Divisor using Euclid's Algorithm */
    __asm__ __volatile__("movl %1, %%eax;"
                         "movl %2, %%ebx;"
                         "CONTD: cmpl $0, %%ebx;"
                         "je DONE;"
                         "xorl %%edx, %%edx;"
                         "idivl %%ebx;"
                         "movl %%ebx, %%eax;"
                         "movl %%edx, %%ebx;"
                         "jmp CONTD;"
                         "DONE: movl %%eax, %0;"
                         : "=g"(result)
                         : "g"(a), "g"(b));

    return result;
}

float sinx(float degree) {
    float result, two_right_angles = 180.0f;
    /* Convert angle from degrees to radians and then calculate sin value */
    __asm__ __volatile__("fld %1;"
                         "fld %2;"
                         "fldpi;"
                         "fmul;"
                         "fdiv;"
                         "fsin;"
                         "fstp %0;"
                         : "=g"(result)
                         : "g"(two_right_angles), "g"(degree));
    return result;
}

float cosx(float degree) {
    float result, two_right_angles = 180.0f, radians;
    /* Convert angle from degrees to radians and then calculate cos value */
    __asm__ __volatile__("fld %1;"
                         "fld %2;"
                         "fldpi;"
                         "fmul;"
                         "fdiv;"
                         "fstp %0;"
                         : "=g"(radians)
                         : "g"(two_right_angles), "g"(degree));
    __asm__ __volatile__("fld %1;"
                         "fcos;"
                         "fstp %0;"
                         : "=g"(result)
                         : "g"(radians));
    return result;
}

float square_root(float val) {
    float result;
    __asm__ __volatile__("fld %1;"
                         "fsqrt;"
                         "fstp %0;"
                         : "=g"(result)
                         : "g"(val));
    return result;
}