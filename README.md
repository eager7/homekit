HomeKit 协议栈
---------------------
[TOC]

#### 介绍
此工程实现了苹果的homekit协议，可以通过苹果手机的家庭APP以及Siri控制符合HAP协议的智能硬件。

#### 依赖
在编译前需要安装部分软件包依赖:
```
sudo apt-get install libssl-dev libjson-c-dev libavahi-compat-libdnssd-dev
```
在某些系统下，如Ubuntu16.04，上面的包会自动安装Bonjour服务，
如果还不支持Bonjour服务，需要安装mDNSResponser
编译方式如下。

##### mDNSResponser编译
下载软件包解压，然后编译并安装：
```
tar -xvf mDNSResponder-98.tar.gz
cd mDNSResponder-98/mDNSPosix/
make os=linux
sudo make os=linux install
```

#### 编译
使用CMakeList.txt进行编译，在homekit目录下创建一个编译目录，然后运行cmake：
```
mkdir build
cd build
cmake ..
make
```

#### 运行
编译出应用后，在build目录下执行编译出的可执行文件即可。
