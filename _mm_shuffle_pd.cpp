#include <emmintrin.h>
#include <bits/stdc++.h>
using namespace std;

class test
{
public:
    double *a_0x38;
    double *a_0x40;
    double *a_0x48;
    double *a_0x50;
};

class test1
{
public:
    double *a_0x0;
    double *a_0x8;
};

int main()
{
    double a, b, c, d;

    test A;
    A.a_0x38 = &a;
    A.a_0x40 = &b;
    A.a_0x48 = &c;
    A.a_0x50 = &d;

    __m128d a1 = (__m128d)_mm_loadu_si128((const __m128i *)&A.a_0x38);
    __m128d b1 = (__m128d)_mm_loadu_si128((const __m128i *)&A.a_0x48);

    test1 B;
    B.a_0x0 = nullptr;
    B.a_0x8 = nullptr;
    *(__m128d *)&B.a_0x0 = _mm_shuffle_pd(a1, b1, 2);

    cout << &A.a_0x38 << "\t" << A.a_0x38 << endl;
    cout << &A.a_0x40 << "\t" << A.a_0x40 << endl;
    cout << &A.a_0x48 << "\t" << A.a_0x48 << endl;
    cout << &A.a_0x50 << "\t" << A.a_0x50 << endl;

    cout << endl;

    cout << B.a_0x0 << endl;
    cout << B.a_0x8 << endl;

    return 0;
}
