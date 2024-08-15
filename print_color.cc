#include <stdio.h>
#include <string.h>

typedef unsigned char UCHAR;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
#pragma pack(1)
typedef struct protocol_msg_align {
    UINT8 msgType;
    UINT8 data1;
    UINT8 data2;
    UINT16 len;
    char data[100];
} PRO_MSG_ALIGN;
#pragma

#define HL_TWK_RED_YEL "\033[1m\033[5;31;43m"  // 闪烁高亮红字黄底
#define HL_RED_WRT "\033[1;31;47m"             // 高亮红色白底

#define HL_RED "\033[1;31m"   // 高亮红色
#define HL_GRN "\033[1;32m"   // 高亮绿色
#define HL_YEL "\033[1;33m"   // 高亮黄色
#define HL_DGRN "\033[1;36m"  // 高亮深绿

#define PF_CLR "\033[0m"  // 清除

#define myprintf(color, format, args...) \
    do {                                 \
        \  
        printf(color);                   \
        printf(format, ##args);          \
        printf(PF_CLR);                  \
    } while (0)

void dump_frm(char *title, UINT8 *data, int len) {
    int i = 0;

    myprintf(HL_YEL, "%s\n", title);
    for (i = 0; i < len; i++) {
        if (i == 0) {
            myprintf(HL_RED, "%02x ", data[i]);
        } else if (i == 3 || i == 4) {
            myprintf(HL_DGRN, "%02x ", data[i]);
        } else {
            myprintf(HL_GRN, "%02x ", data[i]);
        }
    }
    putchar('\n');
}

int main(int args, char *argv[]) {
    UCHAR frm[] = {0x12, 0x34, 0x56, 0x00, 0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    dump_frm("frm<<<", frm, sizeof(frm));

    printf("\033[1;31mThis text is in red and bold.\033[0m\n");
    printf("\033[0;31mThis text is in red and not bold.\033[0m\n");

    printf("\033[0;36m****一口Linux*****【0;36m】\033[0m\r\n");
    printf("\033[1;36m****一口Linux*****【1;36m】\033[0m\r\n");
    printf("\033[4;36m****一口Linux*****【4;36m】\033[0m\r\n");
    printf("\033[5;36m****一口Linux*****【5;36m】\033[0m\r\n");
    printf("\033[7;36m****一口Linux*****【7;36m】\033[0m\r\n");
    printf("\033[8;36m****一口Linux*****【8;36m】\033[0m\r\n");
    printf("\033[22;36m****一口Linux*****【22;36m】\033[0m\r\n");
    printf("\033[24;36m****一口Linux*****【24;36m】\033[0m\r\n");
    printf("\033[25;36m****一口Linux*****【25;36m】\033[0m\r\n");
    printf("\033[27;36m****一口Linux*****【27;36m】\033[0m\r\n");

    printf("\033[30m****一口Linux*****【30】\033[0m\r\n");
    printf("\033[31m****一口Linux*****【31】\033[0m\r\n");
    printf("\033[32m****一口Linux*****【32】\033[0m\r\n");
    printf("\033[33m****一口Linux*****【33】\033[0m\r\n");
    printf("\033[34m****一口Linux*****【34】\033[0m\r\n");
    printf("\033[35m****一口Linux*****【35】\033[0m\r\n");
    printf("\033[36m****一口Linux*****【36】\033[0m\r\n");
    printf("\033[37m****一口Linux*****【37】\033[0m\r\n");

    printf("\033[40m****一口Linux*****【40】\033[0m\r\n");
    printf("\033[41m****一口Linux*****【41】\033[0m\r\n");
    printf("\033[42m****一口Linux*****【42】\033[0m\r\n");
    printf("\033[43m****一口Linux*****【43】\033[0m\r\n");
    printf("\033[44m****一口Linux*****【44】\033[0m\r\n");
    printf("\033[45m****一口Linux*****【45】\033[0m\r\n");
    printf("\033[46m****一口Linux*****【46】\033[0m\r\n");
    printf("\033[47m****一口Linux*****【47】\033[0m\r\n");

    return 0;
}