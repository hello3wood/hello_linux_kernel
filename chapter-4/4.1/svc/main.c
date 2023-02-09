#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern int syscall_write(int fd, char *data, size_t len);

int main(void)
{
	int ret;
	int count;
	char *buf = "Hello syscall!\n";

	count = strlen(buf)+1;
	ret = syscall_write(STDOUT_FILENO, buf, count);
	if (ret != count) {
		printf("write stdout ret=%d\n", ret);
	}
	return 0;
}
