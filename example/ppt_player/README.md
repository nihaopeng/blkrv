# ppt-player

该软件用于轮播ppt图片，依赖图形版本

## 实现方式

该软件通过socket从宿主机获取ppt的图片文件，并存储到flash硬盘中，再通过系统提供的图形api直接绘制png图像。

环境需要存在flkt，并且编译内核时采用`make ENABLE_GPU=1`