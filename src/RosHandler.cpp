//
// Created by socrob on 09-04-2019.
//

#include "RosHandler.h"
#include <iostream>

RosHandler::RosHandler( int argc, char** argv): nh_("~"){

    sub_imu_ = nh_.subscribe( "/mavros/imu/data",
                                1,
                                &RosHandler::imuCallBack,
                                this);
    sub_image_ = nh_.subscribe( "/camera/image_rect_color",
                                1,
                                &RosHandler::imageCallBack,
                                this);

    sub_odometry_ = nh_.subscribe( "/svo/pose_cam/0",
                                1,
                                &RosHandler::odometryCallBack,
                                this);

    rviz_marker_corner_pub_ = nh_.advertise<visualization_msgs::Marker>
            ("visualization_marker", 10);




}


//TODO(ALG): Define all the callBack functions
void RosHandler::imuCallBack(const sensor_msgs::Imu::ConstPtr &msg) {
    ros_imu_ = *msg;
    new_imu_  = true;
    std::cout<<"Recieved IMU"<<std::endl;

}

void RosHandler::imageCallBack(const sensor_msgs::ImageConstPtr &msg) {
    ros_image_ = *msg;
    this->current_image_ = cv_bridge::toCvCopy(msg, "mono8")->image;
    std::cout<<"Recieved Image: "<<std::endl;
    new_image_ = true;



}

void RosHandler::odometryCallBack(const geometry_msgs::PoseStampedConstPtr &msg) {
    std::cout<<"Recieved SVO Odometry: "<<std::endl;
    ros_odometry_  = *msg;
    new_odometry_ = true;

}

sensor_msgs::Imu RosHandler::getRosImu(){
    return ros_imu_;
}

sensor_msgs::Image RosHandler::getRosImage(){
    return ros_image_;
}

geometry_msgs::PoseStamped RosHandler::getRosOdometry(){
    return ros_odometry_;
}

void RosHandler::resetBools() {
    new_imu_  = false;
    new_image_  = false;
    new_odometry_  = false;
}

void RosHandler::publishMarkerCorners(visualization_msgs::Marker points) {
    rviz_marker_corner_pub_.publish(points);

}

void RosHandler::publishCurrentTFPose(tf::Transform current_pose_transform) {

    broadcast_pose_.sendTransform(tf::StampedTransform(current_pose_transform, ros::Time::now(), "world", "SpaceCoBot"));



}