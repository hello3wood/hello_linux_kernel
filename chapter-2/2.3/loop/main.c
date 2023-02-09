#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define LOOP_CTL_DEV		"/dev/loop-control"
#define LOOP_CTL_ADD		0x4C80
#define LOOP_CTL_REMOVE		0x4C81
#define LOOP_ID			1000

int main(void)
{
	int dev;
	int ret;
	unsigned long id;

	dev = open(LOOP_CTL_DEV, O_RDONLY);
	if (dev == -1) {
		printf("open %s failed: %s\n",
		       LOOP_CTL_DEV, strerror(errno));
		return 0;
	}

	id = LOOP_ID;
	printf("Add loop%d...", id);
	ret = ioctl(dev, LOOP_CTL_ADD, id);
	if (ret == -1) {
		printf("failed: %s\n", strerror(errno));
		goto out_dev;
	}
	printf("Success.\n");

	system("ls -l /dev/loop*");

	printf("Remove loop%d...", id);
	ret = ioctl(dev, LOOP_CTL_REMOVE, id);
	if (ret == -1) {
		printf("Failed: %s\n", strerror(errno));
		goto out_dev;
	}
	printf("Success.\n");

	system("ls -l /dev/loop*");
out_dev:
	close(dev);
	return 0;
}
