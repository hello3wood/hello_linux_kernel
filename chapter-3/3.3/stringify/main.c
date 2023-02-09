#include <stdio.h>

#define FOO	bar

#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

int main(void)
{
	printf("__stringify(FOO)=%s\n", __stringify(FOO));
	printf("__stringify_1(FOO)=%s\n", __stringify_1(FOO));
	return 0;
}
