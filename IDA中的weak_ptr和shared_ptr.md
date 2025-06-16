### 本文要点

**1.std::make_shared() 接收返回值 不能是 weakptr**,必须为 std::shared_ptr,unique_ptr 或 auto,否则会直接清空

**2.区分weak_ptr和shared_ptr的在IDA中析构函数**

**3.创建指针尽量使用std::make_shared()**



在 C++ 中，**智能指针**能够自动释放内存，避免内存泄漏和悬空指针问题定义在 `＜memory＞` 头文件中，主要包括以下三种类型：

### 1. 三种智能指针

- **`std::shared_ptr`**：

  - 共享所有权的智能指针，多个 `shared_ptr` 可以管理同一资源。

  - 使用引用计数（reference counting）跟踪资源的使用情况，**当最后一个 `shared_ptr` 被销毁或重置时，资源被释放。**

    ```cpp
    #include <memory>
    std::shared_ptr<int> ptr1 = std::make_shared<int>(20); //引用计数初始化为1
    std::shared_ptr<int> ptr2 = ptr1; // 引用计数增至 2
    // 当 ptr1 和 ptr2 都销毁时，智能指针指向的内存释放
    ```

- **`std::weak_ptr`**：

  - 非拥有型的智能指针，通常与 `shared_ptr` 一起使用，不能直接访问资源。

  - 用于解决 `shared_ptr` 可能导致的循环引用问题（会导致内存泄漏）。

  - **不增加引用计数**，可以检查资源是否仍然存在（通过 `lock()` 或 `expired()` 方法）。

    ```cpp
    #include <memory>
    std::shared_ptr<int> ptr1 = std::make_shared<int>(20); //引用计数初始化为一
    std::weak_ptr<int> ptr2 = ptr1; // 引用计数不增加
    // 当 ptr1 ，智能指针指向的内存释放
    ```

    

- **`std::unique_ptr`**：

  - 独占所有权的智能指针，一个资源只能由一个 `unique_ptr` 管理。

  - 不能复制，只能移动（通过 `std::move`），确保资源所有权的唯一性。

  - 当 `unique_ptr` 超出作用域或被销毁时，自动释放所管理的资源。

    ```cpp
    #include <memory>
    std::unique_ptr<int> ptr = std::make_unique<int>(10); // 创建并管理动态分配的整数
    // ptr 离开作用域时，内存自动释放
    ```

### 2. `std::shared_ptr` 和 `std::weak_ptr` 详解

`std::weak_ptr` 是一种特殊的智能指针，设计目的是为了辅助 `shared_ptr`，主要解决以下问题：

- **循环引用**：当两个对象通过 `shared_ptr` 相互引用时，引用计数永远不会降为 0，导致内存泄漏。
- **访问已释放资源**：`weak_ptr` 允许检查资源是否仍然存在，避免访问悬空指针。



二者结构如下

```cpp
struct std::shared_ptr
{
  void *_M_ptr_0x0;
  std::shared_count *_M_refcount_0x8;
};

struct std::shared_ptr
{
  void *_M_ptr_0x0;
  std::weak_count *_M_refcount_0x8;
};

//std::__shared_count 和  std::weak_count 是 _Sp_counted_base 类型的不同名称
struct std::_Sp_counted_base
{
  uint64_t *vtable_sp_counted_0x0;
  _Atomic_word(int)  _M_use_count_0x8;     // #shared 该值为 0 时触发指向空间的析构
  _Atomic_word(int)  _M_weak_count_0xc;    // #weak + (#shared != 0) 该值 + 上值 = 1 时触发控制块析构
  void *_M_ptr_0x10;
}
//在shared_ptr构造时触发 
_Sp_counted_base() : _M_use_count_0x8(1), _M_weak_count_0xc(1) { } //_M_weak_count_0xc最小值为 1
```

两者内容是一致的，不一样的是处理策略

`std::shared_ptr` 使用的是 shared_block 类型控制块，

`weak_ptr` 使用的是 weak_block 类型控制块，

#### `weak_ptr` 的关键特性 即二者区别：

- **不控制资源生命周期**：`weak_ptr` 不增加 `shared_ptr` 的**_M_use_count**引用计数，也不负责释放资源，增加**_M_weak_count**数量。
- **通过 `lock()` 访问资源**：调用 `weak_ptr::lock()` 返回一个 `shared_ptr`，如果资源已释放，返回指向**nullptr**空的 `shared_ptr`。
- **需要检查有效性**：通过 `expired()` 方法检查资源是否已被销毁。

#### `shared_ptr` IDA析构示例

```cpp
void __fastcall std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_release(std::_Sp_counted_base *this)
{
  int M_use_count_0x8; // eax
  int M_weak_count_0xc; // eax

  if ( &_pthread_key_create ) //线程处理
  {
    if ( _InterlockedExchangeAdd(&this->_M_use_count_0x8, 0xFFFFFFFF) != 1 ) // _M_use_count_0x8 - 1
      return;
  }
 // else
 // {
 //   M_use_count_0x8 = this->_M_use_count_0x8;
 //   this->_M_use_count_0x8 = M_use_count_0x8 - 1;
 //   if ( M_use_count_0x8 != 1 )
 //     return;
 // }
  (*((void (__fastcall **)(std::_Sp_counted_base *))this->vtable_sp_counted_0x0 + 2))(this); //析构指针管理的空间
  if ( &_pthread_key_create )
  {
    M_weak_count_0xc = _InterlockedExchangeAdd(&this->_M_weak_count_0xc, 0xFFFFFFFF); // _M_weak_count_0xc - 1
  }
 // else {
 //   M_weak_count_0xc = this->_M_weak_count_0xc;
 //   this->_M_weak_count_0xc = M_weak_count_0xc - 1;
 // }
  if ( M_weak_count_0xc == 1 )
    (*((void (__fastcall **)(std::_Sp_counted_base *))this->vtable_sp_counted_0x0 + 3))(this); //析构指针控制块空间
}
```

#### `weak_ptr` IDA析构示例

```cpp
int64 __fastcall std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_weak_release(std::_Sp_counted_base *this)
{
  __int64 result; // rax

  if ( &_pthread_key_create ) //线程处理
  {
    result = (unsigned int)_InterlockedExchangeAdd(&this->_M_weak_count_0xc, 0xFFFFFFFF); // // _M_weak_count_0xc - 1
    if( (_DWORD)result != 1 )
      return result;
    return (*((__int64 (__fastcall **)(std::_Sp_counted_base *))this->vtable_sp_counted_0x0 + 3))(this); //析构指针控制块空间
  }
 // result = (unsigned int)this->_M_weak_count_0xc;
 // this->_M_weak_count_0xc = result - 1;
 // if ( (_DWORD)result == 1 )
 //   return (*((__int64 (__fastcall **)(std::_Sp_counted_base *))this->vtable_sp_counted_0x0 + 3))(this);
  return result;
}
```

#### 示例：`weak_ptr` 导致的空指针错误

```cpp
std::vector<std::weak_ptr<Object>> vec;

//代码A  错误写法
std::shared_ptr<Object> getSharedPtr() {
  vec.push_back(std::make_shared<Object>());
  return vec.back().lock();
}

//代码B   正确写法
std::shared_ptr<Object> getSharedPtr() {
  auto ptr = std::make_shared<Object>();
  vec.push_back(ptr);
  return vec.back().lock(); 或 return ptr；
}          

```

#### 解析

```cpp
std::vector<std::weak_ptr<Object>> vec;
std::shared_ptr<Object> getSharedPtr() {
  vec.push_back(std::make_shared<Object>()); 
  //三个操作  
  //1、 tmp = std::make_shared<Object>()；创建智能指针 完成初始化操作 M_use_count(1), _M_weak_count(1)
  //2、 vec.push_back(tmp);  将 tmp 赋值给一个 weakptr ，_M_weak_count++; M_use_count不变；
  //3、 ~tmp();  tmp析构 M_use_count--； usecount为0,触发指向指针的析构，完成后智能指针已经为 nullptr
  return vec.back().lock(); 
  //此时再通过 weak_ptr 的 lock() ，创建一个shared_ptr，但智能指针管理的空间已经为空，获取的也是一个 空智能指针 
}
```



#### ptr的赋值

```cpp

int main() {
  
  std::shared_ptr<Object> sh_ptr = std::make_shared<Object>();
  std::weak_ptr<Object> wk_ptr = sh_ptr; // shared_ptr 可以直接赋值给 wk_ptr
  
  if(wk_ptr.use_count() || !wk_ptr.expired) //两式子等价
    sh_ptr = wk_ptr.lock(); // wk_ptr 需要先验证是否存在再进行赋值 IDA中有明显判断
}
```



#### weakptr的一些标志



```cpp

// 0x626036：device2
if ( !v18 )
    std::__throw_bad_weak_ptr(); 
 

```

#### 示例：`shaerd_ptr` 导致的空指针错误

```cpp

//代码A  错误写法
std::shared_ptr<Object> XXXX::getSharedPtr() {
  Object* p = new Object();
  auto ret = std::shared_ptr(p);
 	this->dev_xxx_0x0 = std::shared_ptr(p); // 此时对于指针 p 已经有两个不同智能指针进行管理，假如 ret 析构后 dev_xxx_0x0 就变成了悬挂指针
  return ret;// 
}

//代码B   正确写法
std::shared_ptr<Object> XXXX::getSharedPtr() {
  auto ret = std::make_shared<Object>();
 	this->dev_xxx_0x0 = ret;
  return ret;
}
    

```

#### 

#### 总结

**1.std::make_shared() 接收返回值 不能是 weakptr**,必须为 std::shared_ptr 或者 auto,否则会直接清空

**2.区分weak_ptr和shared_ptr在IDA中的析构函数**

**3.创建指针尽量使用std::make_shared()**

