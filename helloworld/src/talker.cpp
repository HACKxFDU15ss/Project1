 #include "ros/ros.h"
   #include "std_msgs/String.h"
   #include "std_msgs/Empty.h"
   #include <sstream>
   #include <iostream>
   #include <string.h>
   #include <termio.h>  
   using namespace std;
   

int scanKeyboard()  
{  
int in;  
struct termios new_settings;  
struct termios stored_settings;  
tcgetattr(0,&stored_settings);  
new_settings = stored_settings;  
new_settings.c_lflag &= (~ICANON);  
new_settings.c_cc[VTIME] = 0;  
tcgetattr(0,&stored_settings);  
new_settings.c_cc[VMIN] = 1;  
tcsetattr(0,TCSANOW,&new_settings);  
  
in = getchar();  
  
tcsetattr(0,TCSANOW,&stored_settings);  
return in;  
}  

   /**
  33  * This tutorial demonstrates simple sending of messages over the ROS system.
  34  */
   int main(int argc, char **argv)
   {
     /**
  38    * The ros::init() function needs to see argc and argv so that it can perform
  39    * any ROS arguments and name remapping that were provided at the command line.
  40    * For programmatic remappings you can use a different version of init() which takes
  41    * remappings directly, but for most command-line programs, passing argc and argv is
  42    * the easiest way to do it.  The third argument to init() is the name of the node.
  43    *
  44    * You must call one of the versions of ros::init() before using any other
  45    * part of the ROS system.
  46    */
     ros::init(argc, argv, "talker");
   
     /**
  50    * NodeHandle is the main access point to communications with the ROS system.
  51    * The first NodeHandle constructed will fully initialize this node, and the last
  52    * NodeHandle destructed will close down the node.
  53    */
     ros::NodeHandle n;
   
     /**
  57    * The advertise() function is how you tell ROS that you want to
  58    * publish on a given topic name. This invokes a call to the ROS
  59    * master node, which keeps a registry of who is publishing and who
  60    * is subscribing. After this advertise() call is made, the master
  61    * node will notify anyone who is trying to subscribe to this topic name,
  62    * and they will in turn negotiate a peer-to-peer connection with this
  63    * node.  advertise() returns a Publisher object which allows you to
  64    * publish messages on that topic through a call to publish().  Once
  65    * all copies of the returned Publisher object are destroyed, the topic
  66    * will be automatically unadvertised.
  67    *
  68    * The second parameter to advertise() is the size of the message queue
  69    * used for publishing messages.  If messages are published more quickly
      * than we can send them, the number here specifies how many messages to
      * buffer up before throwing some away.
      */
     ros::Publisher chatter_pub = n.advertise<std_msgs::String>("toggle_led", 1);
   
     //ros::Rate loop_rate(10);
   
     /**
      * A count of how many messages we have sent. This is used to create
      * a unique string for each message.
      */
     int count = 0;
     int input=0;
     int end=101;
     while (1)
     {
	input=scanKeyboard();
	if(input==end){
		break;		
	}
       /**
        * This is a message object. You stuff it with data, and then publish it.
        */
       std_msgs::String msg;
   
       std::stringstream ss;
       ss << char(input) <<" "<< count;
       msg.data = ss.str();
   
       ROS_INFO("%s", msg.data.c_str());
   
       /**
        * The publish() function is how you send messages. The parameter
        * is the message object. The type of this object must agree with the type
        * given as a template parameter to the advertise<>() call, as was done
        * in the constructor above.
       */
      chatter_pub.publish(msg);
  
      //ros::spinOnce();
  
      //loop_rate.sleep();
      ++count;
    }
  
 
    return 0;
  }
