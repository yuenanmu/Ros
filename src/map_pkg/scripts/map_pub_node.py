#!/usr/bin/env python3
#coding=utf-8
import rospy
from nav_msgs.msg import OccupancyGrid

if __name__=="__main__":
    # Initialize the ROS node
    rospy.init_node("map_pub_node")

    #Create a publisher for the OccupancyGrid message
    map_pub=rospy.Publisher("/map",OccupancyGrid,queue_size=10)

    rate=rospy.Rate(3)
    while not rospy.is_shutdown():
        #创建一个OccupancyGrid消息实例
        map_msg=OccupancyGrid()

        #定义消息头信息
        map_msg.header.frame_id="map"
        map_msg.header.stamp=rospy.Time.now()
        #定义地图basic信息
        map_msg.info.origin.position.x=0.0
        map_msg.info.origin.position.y=0.0
        map_msg.info.origin.position.z=0.0
        map_msg.info.resolution=1.0
        map_msg.info.width=4
        map_msg.info.height=2

        #定义地图数据(值<=>有障碍物的概率)
        map_msg.data=[0]*map_msg.info.width*map_msg.info.height
        map_msg.data[0]=100  #设置左上角栅格为障碍物
        map_msg.data[1]=100   #设置右下角栅格为未知区域
        map_msg.data[2]=0
        map_msg.data[3]=-1
        map_msg.data[4]=70
        map_msg.data[5]=70
        map_msg.data[7]=1
        #发布地图消息
        map_pub.publish(map_msg)
        rate.sleep()

