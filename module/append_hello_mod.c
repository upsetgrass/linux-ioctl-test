// SPDX-License-Identifier: GPL-2.0
/*
 * append_hello_mod.c - misc device ioctl example that appends text to a file.
 *
 * Device node: /dev/append_hello
 */

#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/namei.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "../include/uapi/append_hello_ioctl.h"

static int append_hello_do_write(const struct append_hello_req *req)
{
	struct file *filp;
	loff_t pos;
	size_t len;
	ssize_t written;

	if (req->path[0] == '\0')
		return -EINVAL;

	len = strnlen(req->data, APPEND_HELLO_MAX_DATA);
	if (len == APPEND_HELLO_MAX_DATA)
		return -EINVAL;

	filp = filp_open(req->path, O_WRONLY | O_CREAT, 0644);
	if (IS_ERR(filp))
		return PTR_ERR(filp);

	if (!S_ISREG(file_inode(filp)->i_mode)) {
		filp_close(filp, NULL);
		return -EINVAL;
	}

	pos = i_size_read(file_inode(filp));
	written = kernel_write(filp, req->data, len, &pos);
	filp_close(filp, NULL);

	if (written < 0)
		return written;
	if (written != len)
		return -EIO;

	return 0;
}

static int append_hello_do_write_fixed(const struct append_hello_path_req *req)
{
	struct append_hello_req write_req = {0};

	if (strscpy(write_req.path, req->path, sizeof(write_req.path)) < 0)
		return -EINVAL;

	if (strscpy(write_req.data, "hello world\n", sizeof(write_req.data)) < 0)
		return -EINVAL;

	return append_hello_do_write(&write_req);
}

static long append_hello_ioctl(struct file *file, unsigned int cmd,
			       unsigned long arg)
{
	struct append_hello_req req;
	struct append_hello_path_req path_req;

	if (_IOC_TYPE(cmd) != APPEND_HELLO_IOC_MAGIC)
		return -ENOTTY;

	switch (cmd) {
	case APPEND_HELLO_IOC_APPEND:
		if (copy_from_user(&req, (void __user *)arg, sizeof(req)))
			return -EFAULT;

		req.path[APPEND_HELLO_MAX_PATH - 1] = '\0';
		req.data[APPEND_HELLO_MAX_DATA - 1] = '\0';
		return append_hello_do_write(&req);
	case APPEND_HELLO_IOC_APPEND_FIXED:
		if (copy_from_user(&path_req, (void __user *)arg, sizeof(path_req)))
			return -EFAULT;

		path_req.path[APPEND_HELLO_MAX_PATH - 1] = '\0';
		return append_hello_do_write_fixed(&path_req);
	default:
		return -ENOTTY;
	}
}

static const struct file_operations append_hello_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = append_hello_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = append_hello_ioctl,
#endif
};

static struct miscdevice append_hello_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "append_hello",
	.fops = &append_hello_fops,
	.mode = 0600,
};

static int __init append_hello_init(void)
{
	int ret;

	ret = misc_register(&append_hello_dev);
	if (ret)
		return ret;

	pr_info("append_hello: loaded, device /dev/%s\n", append_hello_dev.name);
	return 0;
}

static void __exit append_hello_exit(void)
{
	misc_deregister(&append_hello_dev);
	pr_info("append_hello: unloaded\n");
}

module_init(append_hello_init);
module_exit(append_hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sample");
MODULE_DESCRIPTION("Append text to file via module ioctl");
