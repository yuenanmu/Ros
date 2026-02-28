#!/usr/bin/env python3
# coding=utf-8

import rospy
import cv2
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

#回调函数
def Cam_RGB_Callback(msg):
    #消息包转换成OpenCV格式
    bridge=CvBridge()
    try:
        cv_image=bridge.imgmsg_to_cv2(msg,"bgr8")
    except CvBridgeError as e:  #异常反馈并且退出
        rospy.logerr("格式转换错误：%s",e)
        return 
    #照片直接显示在窗口上
    cv2.imshow("RGB",cv_image)
    cv2.waitKey(1)  #等待1ms

if __name__=="__main__":
    rospy.init_node("img_node")
    #订阅视觉传感器Kinect2的图像话题
    img_sub=rospy.Subscriber("/kinect2/qhd/image_color_rect",Image,Cam_RGB_Callback,queue_size=10)
    #阻塞程序，等待回调函数被调用
    rospy.spin()