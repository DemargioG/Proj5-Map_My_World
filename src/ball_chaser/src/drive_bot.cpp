#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
//#include <std_msgs/Float64.h>

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

// TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
    //ROS_INFO("DriveToTargetRequest received - Linear x:")


    // Publish Velocity

    geometry_msgs::Twist motor_vel;
    //std_msgs::Float64 linearX_vel, angularZ_vel;

    //linearX_vel.data = (double)req.linear_x;
    //angularZ_vel.data = (double)req.angular_z;

    motor_vel.linear.x = (double)req.linear_x;
    motor_vel.angular.z = (double)req.angular_z;

    motor_command_publisher.publish(motor_vel);


    // Response message
    res.msg_feedback = "Velocities set - Linear: " + std::to_string((float)req.linear_x) + " , Angular: " + std::to_string((float)req.angular_z);
    ROS_INFO_STREAM(res.msg_feedback);

}


int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // TODO: Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
    
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);
    ROS_INFO("Drive Robo");

    // TODO: Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the requested velocities instead of constant values
   /* while (ros::ok()) {
        // Create a motor_command object of type geometry_msgs::Twist
        geometry_msgs::Twist motor_command;
        // Set wheel velocities, forward [0.5, 0.0]
        motor_command.linear.x = 0.5;
        motor_command.angular.z = 0.0;
        // Publish angles to drive the robot
        motor_command_publisher.publish(motor_command);
    }*/

    // TODO: Handle ROS communication events
    ros::spin();

    return 0;
}
