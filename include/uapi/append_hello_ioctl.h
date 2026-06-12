/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_APPEND_HELLO_IOCTL_H
#define _UAPI_APPEND_HELLO_IOCTL_H

#include <linux/ioctl.h>

#define APPEND_HELLO_MAX_PATH 256
#define APPEND_HELLO_MAX_DATA 256

struct append_hello_req {
	char path[APPEND_HELLO_MAX_PATH];
	char data[APPEND_HELLO_MAX_DATA];
};

struct append_hello_path_req {
	char path[APPEND_HELLO_MAX_PATH];
};

#define APPEND_HELLO_IOC_MAGIC	0xE6
#define APPEND_HELLO_IOC_APPEND _IOW(APPEND_HELLO_IOC_MAGIC, 0x80, struct append_hello_req)
#define APPEND_HELLO_IOC_APPEND_FIXED _IOW(APPEND_HELLO_IOC_MAGIC, 0x81, struct append_hello_path_req)

#endif /* _UAPI_APPEND_HELLO_IOCTL_H */
