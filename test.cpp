#include "utility.h"

int main() {
    for (int v4 = 0;v4 < 200; v4 += 40) {
        unsigned int t = 0xCCCCCCCDLL * ((v4 * 40) >> 3);
        cout << t << "   " << v4 << endl;
    }

    return 0;
}
