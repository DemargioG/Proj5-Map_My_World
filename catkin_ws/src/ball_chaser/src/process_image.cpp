#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot

    ball_chaser::DriveToTarget srv;

    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv))
        ROS_ERROR("Failed to  call service.");

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;

    int sect_left = std::floor((img.width)/3);
    //int sect_middle = sect_side + (img.step%3);
    int sect_right = img.width - sect_left;

    // These Variable are used to tally up counts of white pixels in each sector
    int left = 0;
    int right = 0;
    int center = 0;

    //Variable used to keep track of where I am on each row.
    int trace = 0;


    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera

    // Loops through the image determining if the pixel is white and tallies where it is.
    // Since I am stepping through steps of the image, I chose to step in threes.
    for (int i = 2; i < (img.step*img.height)-2; i = i + 3){
        
        if (img.data[i] == 255 && img.data[i-1] == 255 && img.data[i-2] == 255 && trace < sect_left){
            left++;
        }
	if (img.data[i] == 255 && img.data[i-1] == 255 && img.data[i-2] == 255 && sect_right < trace){
            right++;
	}
	if (img.data[i] == 255 && img.data[i-1] == 255 && img.data[i-2] == 255 && trace >= sect_left && trace <= sect_right){
            center++;
	}

        trace++;  

        if (trace == img.width){
            //Reset trace at the end of a row.
            trace = 0;
        }           

    }  


    // Determines the where the highest concentration of white is and follows it.
    if(left > right && left > center){    
        drive_robot(0.0, 0.5);
    }
    if(right > left && right > center){    
        drive_robot(0.0, -0.5);
    }
    if(center > left && center > right){    
        drive_robot(0.5, 0.0);
    }
    if (left == 0 && right == 0 && center == 0){    
        drive_robot(0.0, 0.0);
    }
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
