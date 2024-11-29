#include <iostream>

#ifdef __has_include
    #if __has_include(<bits/stdc++.h>)
        #include <bits/stdc++.h>
    #else
        #include <vector>
        #include <map>
        #include <set>
        #include <unordered_map>
        #include <unordered_set>
        #include <string>
        #include <algorithm>
        #include <iostream>
        #include <queue>
        #include <stack>
        #include <cmath>
        #include <numeric>
    #endif
#else
    // 如果编译器不支持 __has_include，可以选择直接包含常见的头文件
    #include <vector>
    #include <map>
    #include <set>
    #include <unordered_map>
    #include <unordered_set>
    #include <string>
    #include <algorithm>
    #include <iostream>
    #include <queue>
    #include <stack>
    #include <cmath>
    #include <numeric>
#endif

using namespace std;
