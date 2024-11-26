#include "utility.h"

class SafeIntPointer {
public:
    explicit SafeIntPointer(int v) : m_value(new int(v)), m_used(new int(1)) {}
    ~SafeIntPointer() {
        cout << "~SafeIntPointer" << endl;
        (*m_used)--;  // 引用计数减1
        if (*m_used <= 0) {
            delete m_used;
            delete m_value;
            cout << "real delete resources" << endl;
        }
    }

    SafeIntPointer(const SafeIntPointer& other) {
        m_used = other.m_used;
        m_value = other.m_value;
        (*m_used)++;  // 引用计数加1
    }
    SafeIntPointer& operator=(const SafeIntPointer& other) {
        if (this == &other)  // 避免自我赋值!!
            return *this;

        m_used = other.m_used;
        m_value = other.m_value;
        (*m_used)++;  // 引用计数加1
        return *this;
    }

    int get() { return *m_value; }
    int getRefCount() {
        return *m_used;
    }

private:
    int* m_used;  // 引用计数
    int* m_value;
};

int main() {
    SafeIntPointer a(5);
    cout << "ref count = " << a.getRefCount() << endl;
    SafeIntPointer b = a;
    cout << "ref count = " << a.getRefCount() << endl;
    SafeIntPointer c = b;
    cout << "ref count = " << a.getRefCount() << endl;
}

/*
ref count = 1
ref count = 2
ref count = 3
~SafeIntPointer
~SafeIntPointer
~SafeIntPointer
real delete resources
*/
