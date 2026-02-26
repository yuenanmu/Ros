#!/usr/bin/env python3
#coding=utf-8

import rospy
from std_msgs.msg import String

def PublisherWaypoint():
    navi_waypoint=String()
    navi_waypoint.data="2"

    rospy.sleep(1)
    wp_pub.publish(navi_waypoint)
    rospy.loginfo("已发布导航目标点：%s", navi_waypoint.data)
    
def NavResultCallback(msg):
    rospy.loginfo("导航结果：%s", msg.data)
if __name__ == '__main__':
    rospy.init_node("wp_node")

    wp_pub=rospy.Publisher("/waterplus/navi_waypoint",String,queue_size=10)
    wp_sub=rospy.Subscriber("/waterplus/navi_result", String, NavResultCallback,queue_size=10)

    PublisherWaypoint()#发布导航目标点
    rospy.spin()#保持节点运行，等待回调函数被调用
