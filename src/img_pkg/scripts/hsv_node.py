#!/usr/bin/env python3
# coding=utf-8

#导入模块
import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

hue_min=10 #色调最小值
hue_max=40
satu_min=90 #饱和度最小值
satu_max=255
val_min=1 #亮度最小值
val_max=255

def img_callback(msg):
    #全局变量
    global hue_min,hue_min,hue_max,satu_min,satu_max,val_min,val_max
    #接收Ros图像消息包，转换成OpenCV图像格式
    bridge=CvBridge()
    try:
        cv_image=bridge.imgmsg_to_cv2(msg,"bgr8")
    except CvBridgeError as e:
        rospy.logerr("格式转换错误: %s",e)
        return
    #OenCV图像处理
    hsv_image=cv2.cvtColor(cv_image,cv2.COLOR_BGR2HSV) #将RGB图片转换成HSV

    ##直方图均衡化
    h,s,v=cv2.split(hsv_image)#分离HSV通道
    v=cv2.equalizeHist(v) #对亮度通道做直方图均衡化
    hsv_image=cv2.merge([h,s,v]) #合并HSV通道

    ##HSV空间二值化
    th_image=cv2.inRange(hsv_image,(hue_min,satu_min,val_min),(hue_max,satu_max,val_max))
    ##开操作，去除噪点
    element=cv2.getStructuringElement(cv2.MORPH_RECT,(5,5)) #定义结构元素
    th_image=cv2.morphologyEx(th_image,cv2.MORPH_OPEN,element)
    ##闭操作，连接连通域
    th_image=cv2.morphologyEx(th_image,cv2.MORPH_CLOSE,element)
    #画十字线
    target_x,target_y,pix_count=0,0,0
    image_height,image_width=th_image.shape[:2]
    for y in range(image_height):
        for x in range(image_width):
            if th_image[y,x]==255:
                target_x+=x
                target_y+=y
                pix_count+=1
    if pix_count>0:
        target_x//=pix_count
        target_y//=pix_count
        print(f"颜色质心坐标( {target_x} , {target_y} )  点数 = {pix_count}")
        #在cv_image上画坐标
        cv2.line(cv_image,(target_x-10,target_y),(target_x+10,target_y),(0,255,0),2)
        cv2.line(cv_image,(target_x,target_y-10),(target_x,target_y+10),(0,255,0),2)
    else:
        print("未检测到目标颜色")
    #显示图像
    cv2.imshow("HSV Image",hsv_image)
    cv2.imshow("Threshold Image",th_image)
    cv2.imshow("Original Image",cv_image)
    cv2.waitKey(4)

def nothing(x):
    pass

if __name__=="__main__":
    #初始化ROS节点,订阅图像话题
    rospy.init_node("hsv_node",anonymous=True)#anonymous=True表示节点名称自动生成，防止节点名称冲突
    img_sub=rospy.Subscriber("/kinect2/qhd/image_color_rect",Image,img_callback,queue_size=10)
    #创建窗口
    cv2.namedWindow("Threshold Control")
    cv2.createTrackbar("hue_min","Threshold Control",hue_min,179,nothing)
    cv2.createTrackbar("hue_max","Threshold Control",hue_max,179,nothing)
    cv2.createTrackbar("satu_min","Threshold Control",satu_min,255,nothing)
    cv2.createTrackbar("satu_max","Threshold Control",satu_max,255,nothing)
    cv2.createTrackbar("val_min","Threshold Control",val_min,255,nothing)    
    cv2.createTrackbar("val_max","Threshold Control",val_max,255,nothing)
    cv2.namedWindow("HSV Image")
    cv2.namedWindow("Threshold Image")
    cv2.namedWindow("Original Image")

    rate=rospy.Rate(10)
    while not rospy.is_shutdown():
        #更新阈值参数
        hue_min=cv2.getTrackbarPos("hue_min","Threshold Control")
        hue_max=cv2.getTrackbarPos("hue_max","Threshold Control")
        satu_min=cv2.getTrackbarPos("satu_min","Threshold Control")
        satu_max=cv2.getTrackbarPos("satu_max","Threshold Control")
        val_min=cv2.getTrackbarPos("val_min","Threshold Control")
        val_max=cv2.getTrackbarPos("val_max","Threshold Control")
        rate.sleep()
    cv2.destroyAllWindows()