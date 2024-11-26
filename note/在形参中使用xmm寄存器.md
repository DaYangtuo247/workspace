## 在形参中xmm寄存器的使用

### 普通double/float

对于普通double/float形参，都存储在xmm寄存器中，当xmm寄存器使用完毕，才会存储在栈中，不同cpu的xmm个数不同，但普遍为8个

同时：一个xmm寄存器可以容纳两个double、4个float类型

### 类对象

对于形参是类对象，而类对象中只存储double/float

* 只存储double，小于等于2个double时使用xmm寄存器存储类对象，大于时使用栈
* 只存储float，小于等于4个float时使用xmm，大于时使用栈

一个xmm宽度为128位，当同时存储double/float，需要考虑字节对齐的影响

### sta::DelayVsta\<T\>

```cpp
template <typename T>
struct sta::DelayVsta {
  T sta_vsta_float_0x0;
};
```

对于这种模板类作结构的，如果T为float，需要注意将使用xmm寄存器。即8占用8字节，但实际上float只占用4字节，这会导致之后的变量错位。
