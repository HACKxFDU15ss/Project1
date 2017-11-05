#include <ros.h>
#include "DualMC33926MotorShield.h"
#include "SR04.h"
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>


DualMC33926MotorShield md;
ros::NodeHandle  nh;
String str;
int i;


//unsigned long time;
unsigned long stopTime;
//can forward
boolean canForward;
int currentAction;
int nextAction;
/* 1: forward  
 * 2. left     
 * 3. right
 * 4. rotate
 * 5. 
 * 6. 
 */


//const int TRIG_PIN1=43;
//const int ECHO_PIN1=45;
const int TRIG_PIN2=30;
const int ECHO_PIN2=31;
const int TRIG_PIN3=32;
const int ECHO_PIN3=33;

//SR04 sr04 = SR04(ECHO_PIN1,TRIG_PIN1);
SR04 sr05 = SR04(ECHO_PIN2,TRIG_PIN2);
SR04 sr06 = SR04(ECHO_PIN3,TRIG_PIN3);
long a;



void standup(){
  a = sr05.Distance()+sr06.Distance();
  if(a < 40){
    canForward = false;
    if(currentAction == 1){
     md.setM1Speed(0);
     md.setM2Speed(0); 
    }
    
    Serial.println("hit");
  }else{
    canForward = true; 
  }
}

void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("fault");
    while(1);
  }
}


void forward(int v, int t){
  if((!canForward) && v > 0){return;}
  
  md.setM1Speed(v);
  md.setM2Speed(-v);
  
  currentAction = 1;
  //time = time+t*1000;
  stopTime = millis() + t;
}

void s(){
  md.setM1Speed(0);
  md.setM2Speed(0);
}

void rotate(int degree,int sign){
  currentAction = 4;
  md.setM1Speed(100*sign);
  md.setM2Speed(100*sign);

  switch(degree){
    case 90:{
      stopTime = millis()+2300;
      break;
    }
    case 360:{
      stopTime = millis()+8000;
      break;
    }
    default:{
      break;
    }
  }
}

void left(){
    if(!canForward){return;}

  currentAction = 2;
  md.setM1Speed(100);
  md.setM2Speed(-50); 
  stopTime = millis() + 4000;
}


void right(){
    if(!canForward){return;}

  currentAction = 3;
  md.setM1Speed(50);
  md.setM2Speed(-100); 
  stopTime = millis() + 4000;
}

/*void rotate90(int a){
  switch(a){
    case 1:{
    md.setM1Speed(100);
    break;
    }
    case 2:{
      md.setM2Speed(-100);
      break;
    }
    default:{
         break; 
    }
  }
  delay(1000);
  s();
}*/

/*void left(){
  md.setM1Speed(400);
  delay(1000);
  s();
}
*/
/*void rotate360(int flag){
  md.setM1Speed(100 * flag);
  md.setM2Speed(100 * flag);
  delay(1000);
  s();
}*/

void snake(){
  left();
  nextAction = 2;
  
}



void cat(){
  forward(100,2000);
}

void bike(){
  forward(-100,2000);
}

void sofa(){
  rotate(360,-1);
}

void dog(){
  rotate(360,1);
}

void train(){
  rotate(90,-1);
  
  nextAction = 1;
  //forward(400,1000);
}

void car(){
  rotate(90,1);
  
  nextAction = 1;
  //forward(400,1000);
}

void messageCb(const std_msgs::String& toggle_msg){
//  String a  = "  ";
//  a.charAt(0);

str = toggle_msg.data;
//  str="w";
  switch(str.charAt(0)){
    case 'w':{
      forward(100,2000);
  break;
    }
    case 's':{
      forward(-100,2000);
      break;
    }
    case 'a':{
      rotate(90,1);
      break;
    }
    case 'd':{
      rotate(90,-1);
      break;
    }
    
    case 'x':{
      s();
      break;
    }
    case 'g':{
     snake(); 
      break;
    }

    default:{
      break;
    }
  }
  
}

void photoCb(const std_msgs::String& toggle_msg){
  //restrict other instruct when an action is executing
  if(millis() <= stopTime){
    return;
  }
  
  
  
  String i = toggle_msg.data;

  switch(i.charAt(0)){
    case 'a':{
      cat();
      break;
    }
    case 'b':{
      bike();
      break;
    }
    case 'c':{
      sofa();
      break;
    }
    case 'd':{
      dog();
      break;
    }
    case 'e':{
      train();
      break;
    }
    case 'f':{
      car();
      break;
    }
    case 'p':{
      snake();
      break;
    }
    
    default:{
      break;
    }
  }
}



ros::Subscriber<std_msgs::String> sub("toggle_led", &messageCb );
ros::Subscriber<std_msgs::String> photo("photo", &photoCb);

void setup() {
  // put your setup code here, to run once:
  
  nh.initNode();
  md.init();
  nh.subscribe(sub);
  nh.subscribe(photo);
  stopTime = millis();
  currentAction = 0;
  nextAction = 0;
  canForward = true;
  Serial.begin(57600);
  
}


void loop() {
  if(millis() > stopTime){
    currentAction = 0;
    if(nextAction == 1){
      nextAction = 0;
      forward(100,2000);
    }
    else if(nextAction == 2){
      nextAction = 0;
      right();
    }
    else{
      s();
    }
  }
  

  nh.spinOnce();
  standup();
  delay(100);
  
  //standup();
} 
