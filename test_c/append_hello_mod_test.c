// SPDX-License-Identifier: GPL-2.0
/*
 * Userspace test for append_hello_mod kernel module.
 *
 * Build:
 *   gcc -O2 -Wall -Wextra -I../include/uapi -o append_hello_mod_test append_hello_mod_test.c
 *
 * Run:
 *   sudo ./append_hello_mod_test /tmp/ioctl_append_test.txt "hello world\\n"
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "../include/uapi/append_hello_ioctl.h"

int main(int argc, char **argv)
{
	const char *dev = "/dev/append_hello";
	const char *path = "~/tmp/ioctl_append_test.txt";
	const char *msg = "hello world\\n";
	struct append_hello_req req = {0};
	struct append_hello_path_req path_req = {0};
	int use_fixed = 0;
	int fd;
	int ret;

	if (argc > 4) {
		fprintf(stderr, "Usage: %s [target_file] [text] [--fixed]\\n", argv[0]);
		return 2;
	}

	if (argc >= 2)
		path = argv[1];
	if (argc == 3)
		msg = argv[2];
	if (argc == 4 && strcmp(argv[3], "--fixed") == 0)
		use_fixed = 1;
	if (argc == 2 && strcmp(argv[1], "--fixed") == 0) {
		use_fixed = 1;
		path = "/home/happy_grass/tmp/ioctl_append_test.txt";
	}

	if (snprintf(req.path, sizeof(req.path), "%s", path) >= (int)sizeof(req.path)) {
		fprintf(stderr, "path too long\\n");
		return 2;
	}

	if (!use_fixed && snprintf(req.data, sizeof(req.data), "%s", msg) >= (int)sizeof(req.data)) {
		fprintf(stderr, "text too long\\n");
		return 2;
	}

	if (snprintf(path_req.path, sizeof(path_req.path), "%s", path) >= (int)sizeof(path_req.path)) {
		fprintf(stderr, "path too long\\n");
		return 2;
	}

	fd = open(dev, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open(%s) failed: %s\\n", dev, strerror(errno));
		return 1;
	}

	if (use_fixed) {
		ret = ioctl(fd, APPEND_HELLO_IOC_APPEND_FIXED, &path_req);
		if (ret < 0) {
			fprintf(stderr, "ioctl(APPEND_HELLO_IOC_APPEND_FIXED) failed: %s\\n", strerror(errno));
			close(fd);
			return 1;
		}
	} else {
		ret = ioctl(fd, APPEND_HELLO_IOC_APPEND, &req);
		if (ret < 0) {
			fprintf(stderr, "ioctl(APPEND_HELLO_IOC_APPEND) failed: %s\\n", strerror(errno));
			close(fd);
			return 1;
		}
	}

	close(fd);
	if (use_fixed)
		printf("OK: appended fixed hello text to %s via %s\\n", path, dev);
	else
		printf("OK: appended custom text to %s via %s\\n", path, dev);
	return 0;
}
