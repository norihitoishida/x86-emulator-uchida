#include <stdio.h>
void main(void){
    int val;
    int *ptr = &val;
    printf("val=%d\n", val);
    printf("&val=%p\n", &val);
    printf("ptr=%p\n",ptr);
    *ptr = 41;
    printf("val=%d\n", val);
    printf("&val=%p\n", &val);
    printf("ptr=%p\n",ptr);
}