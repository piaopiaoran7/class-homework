#include <stdio.h>
int main(void)
{
    char buf[128];
    printf("请输入一段文字：");
    scanf("%s", buf);
    printf("你输入的内容：%s\n", buf);
    return 0;
}
