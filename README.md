# Project1


> 小组成员：
>
- 吴豪奇
- 谢思豪
- 麻俊特
- 程世磊
- 王力

---

## 嵌入式GPU – TX1作为主控芯片的智能小车。
计算设备：NVIDIA Jetson TX1控制设备：Arduino Mega 2560

> NVIDIA 嵌入式开发平台 + 目标检测算法介绍 + ROS 以及 Arduino集成介绍

### Arduino控制小车的运动
- 无线传输，键盘输入，控制前后左右移动
- 获取Camera的输入，通过TX1进行计算，输出识别结果，并传入arduino

### 利用NVIDIA进行图像识别
- 使用框架进行训练，可以是动物，也可以是数字

### ROS控制Arduino和Jetson TX1的交互
- 通过ROS整合物件识别与电机控制

## 任务分工
吴豪奇+麻俊特：

- 使用`物件识别Caffe_SSD`的库
- 结合 `OpenCV 抓取 Cemera 影像`，对视频流中的图像进行识别- 讯息传递系统：`集成 ROS 与Caffe`，将Jetson TX1 中的动作指令发送到电机控制：Arduino上去。
- wifi remote control TX1系统，发送keyboard指令

程世磊+王力：

- 撰写 `Arduino 控制码`
- 对不同的输入执行不同的动作
- 接受信息的来源：
  - Camera接受的图像识别之后由给Ariduino
  - `Camera（OpenCV）--> Jetson TX1 --> Ariduino`
  - 无线传输：Jetson TX1 接受来自 KeyBoard的输入，再传到Ariduino
  - `KeyBoard --> Jetson TX1 --> Ariduino`

谢思豪：

- 研究如何使用ROS
- Topic中的发布/订阅（同一workspace），在通过USB连接的Arduibo和TX1之间的通信实现

## 第一天问题
- caffe-SSD无法执行，make时报错
- Arduibo和TX1之间的通信
 不同port控制Arduibo的程序
测试一下VIM
