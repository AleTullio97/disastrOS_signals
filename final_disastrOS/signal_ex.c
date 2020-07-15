#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>

int main(){
	uint8_t a = 0x02; // 0000 0010
	uint8_t b= 0x0f;  // 0000 1111
	uint8_t c= 0x07;  // 0000 0111
	printf("a: %d,  b: %d,  a & b :%d\n",a,b,a&b);
	printf("a: %d,  c: %d,  a & c :%d\n",a,c,a&c);	
	printf("c: %d,  b: %d,  c & b :%d\n",c,b,c&b);
	return 0;
}


























