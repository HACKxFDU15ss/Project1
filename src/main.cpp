
#include "ros/ros.h"
#include "std_msgs/String.h"

#include "object_detection.h"
#include "time_helper.h"

char code(int label){
	switch (label){
		case 2: return 'b';
		case 7:return 'f';
		case 8:return 'a';
		case 12: return 'd';
		case 15: return 'p';
		case 18:return 'c';
		case 19: return 'e';
		default: return 'g';
	}
}

// test
int main(int argc, char* argv[])
{
	std::string configfile("/home/ubuntu/15ss_ws/src/ros_caffe/src/object_detection-v2/config/config.xml");
	SSD ssd(configfile);

    cv::VideoCapture vcap(0);
    if (false == vcap.isOpened())
    {
        fprintf(stderr, "camera cannot open!\n");
        return -1;
    }

	//-----ros
	
     ros::init(argc, argv, "img_talker");
     ros::NodeHandle n;
     ros::Publisher chatter_pub = n.advertise<std_msgs::String>("photo", 1);
	
	//------

    cv::Mat frame;
    while (true)
    {
		{
			//__TIC__();
			vcap >> frame;
			//printf("frame w = %d\th = %d\n", frame.cols, frame.rows);
			//printf("sg.xu: camera capture elapsed time: ");
			//__TOC__();
		}

		__TIC__();
        std_msgs::String msg;
        int label = ssd.detect(frame);
        char c = code(label);
        printf("label: %d\tmsg: %c\t", label, c);
    	msg.data = "";
    	msg.data += c;
    	
    	//if(msg >=0 && msg < 20){
    		
			//ROS_INFO("%s", msg.data.c_str());
		chatter_pub.publish(msg);
    	//}
    	
		printf("sg.xu: total elapsed time: ");
		__TOC__();
        
        imshow("camcap", frame);
		int key = cvWaitKey(1) & 255;
        if (key  == 27)
        {
        	//printf("\nkey: %d\n",key);
			vcap.release();
            break;
        }
    }
}

