# linux-ioctl-test

一个最小化的 Linux ioctl 示例项目，包含：

- 内核模块（misc 设备）：`/dev/append_hello`
- 用户态测试程序（通过 ioctl 调用模块）
- 共享 UAPI 头文件（内核态和用户态共用）

## 目录结构

- `include/uapi/append_hello_ioctl.h`：ioctl 命令和数据结构定义
- `module/append_hello_mod.c`：内核模块实现
- `test_c/append_hello_mod_test.c`：用户态测试程序

## 依赖

- Linux 内核头（与当前运行内核匹配）
- `make`
- `gcc`
- `sudo`（加载/卸载模块时需要）

## 快速开始

在仓库根目录执行：

```bash
make
sudo make load
make test-build
sudo make test
```

默认会向 `/tmp/ioctl_append_test.txt` 追加内容。

## 常用命令

```bash
# 构建内核模块 + 用户态程序
make

# 仅构建内核模块
make module

# 仅构建用户态程序
make test-build

# 加载/卸载模块
sudo make load
sudo make unload

# 运行用户态测试（需要模块已加载）
sudo make test

# 清理构建产物
make clean
```

## 手动运行测试程序

```bash
./test_c/append_hello_mod_test [target_file] [text] [--fixed]
```

示例：

```bash
sudo ./test_c/append_hello_mod_test /tmp/ioctl_append_test.txt "hello from ioctl\n"
sudo ./test_c/append_hello_mod_test /tmp/ioctl_append_test.txt --fixed
```
