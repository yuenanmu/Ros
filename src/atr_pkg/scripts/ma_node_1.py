#!/usr/bin/env python3
#coding=utf-8

import rospy
from std_msgs.msg import String
from qq_msgs.msg import Carry
#define callback function
def chao_callback(msg):
    rospy.logwarn(msg.grade)
    rospy.logwarn(msg.star)
    rospy.loginfo(msg.data)

def yao_callback(msg):
    rospy.loginfo(msg.data)

if __name__=="__main__":
    try:
        #init node
        rospy.init_node("ma_node")
        rospy.logwarn("ma_node is started  ")

        #subscribe topic
        sub=rospy.Subscriber("chao_node_toptic",Carry,chao_callback,queue_size=10)
        sub_1=rospy.Subscriber("yao_node_topic",String,yao_callback,queue_size=10)

        rospy.spin()#查看消息是否到达
    except rospy.ROSInterruptException:
        rospy.logerr("节点运行中断")
