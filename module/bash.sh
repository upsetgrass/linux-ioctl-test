# 执行insmod指令加载模块到内核中
sudo insmod append_hello_mod.ko

# 由于当前模块是miscdevice类型的append_hello设备，此时会在`/dev`下生成一个`append_hello`文件
