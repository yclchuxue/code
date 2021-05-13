#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COLOR_BAGIN  "\001\033[1m\002"//强调，加粗，高亮
#define COLOR_END "\001\033[0m\002"
#define COLOR_BKG_SEF(x,y) "\001\033["#x";"#y"m\002"    //x为外边框颜色，y为里面字的颜色
#define COLOR_BKG(x) "\001\033["#x"m\002"//x<40-49>
#define COLOR_SEF(x) "\001\033["#x"m\002"//x<30-39>

int main()
{
    printf(COLOR_BAGIN COLOR_BKG_SEF(42,37) "ycl"COLOR_END);
    //printf(COLOR_BAGIN "ycl"COLOR_END "ycl");
    //printf(COLOR_BAGIN COLOR_BKG_SEF(42,33) "ycl");



    return 0;

}