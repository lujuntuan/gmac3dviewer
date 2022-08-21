# gmac3dviewer

这是一个关于自动驾驶智能座舱的3D仿真场景预览程序

## 依赖：

- C++14

- CMake大于3.10

- Qt5.15或Qt6 (Core Gui Quick Widgets)

- Windows平台（带sdk）：64位msvc2015/msvcvc2017/msvcvc2019

- Linux平台（无sdk）：osg3.6.5


## 如何编译：

```cmake
cmake -B build
cmake --build build --target install
```

## 如何使用：

```cmake
F1:打开动画列表
F2:打开测试列表
F3:全屏或取消全屏
F4:退出
F5:打印HomePoint和FlyPoint到控制台
```

## 版权所有:

Juntuan.Lu, 2020-2030, All rights reserved.
