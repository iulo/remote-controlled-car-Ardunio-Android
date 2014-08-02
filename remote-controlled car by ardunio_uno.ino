/*
 *  Author:        liulong(刘龙)
 *  Date:          2013.06.04
 *  Description:   Design of Telecontrol Car Based on Arduino 
                   and Android Phone’s Bluetooth.
    MCU:           Arduino uno
 *  About:      http://www.cnblogs.com/liu-l/category/566618.html
*/


#include <MeetAndroid.h>
#include <FlexiTimer2.h>
//
int get;
float  data;
static int choice=5;
static int speed =150;
char accelBuff[100]; //定义接收数组用来存放手机发送的加速度传感器参数
int accelDot[2]; //存放数据中的小数点位于accelBuff中的位置，小数点的十六进制为2e
int accelSemi; //存放数据中的分号位于accelBuff中的位置，分号的十六进制为3b
int x,y;
int i;
int  pwm1,pwm2;
int  left,right;
const int LED1=8;
const int LED2=9;
static boolean LED_SIGNAL = HIGH;

// declare MeetAndroid so that you can call functions with it
MeetAndroid meetAndroid;

void flash()
{
	if(left==1)	digitalWrite(LED1, LED_SIGNAL);
	else 		digitalWrite(LED1, LOW);
	if(right==1)	digitalWrite(LED2, LED_SIGNAL);
	else 		digitalWrite(LED2, LOW);
 	 LED_SIGNAL = !LED_SIGNAL;
 	 //Serial.println("led");
}

void setup()
{
  
	pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    FlexiTimer2::set(300, flash);
	FlexiTimer2::start();
  Serial.begin(9600);
  meetAndroid.registerFunction(choice_control,'R');

  meetAndroid.registerFunction(cha_switch, 'c');
  meetAndroid.registerFunction(change_speed, 'p');


  pinMode(4, OUTPUT); //M1
    pinMode(5, OUTPUT); //E1
    pinMode(6, OUTPUT); //E2
    pinMode(7, OUTPUT); //M2
    pinMode(20, OUTPUT);
    pinMode(21, OUTPUT);

    pinMode(19, INPUT);
    pinMode(13, OUTPUT);
}



void loop()
{
 if(digitalRead(19)==HIGH)
 {
   meetAndroid.receive();

 }
 else
 {
  acc_control();
}



}

/***************************************************
         change_speed 速度调节
***************************************************/

void change_speed(byte flag, byte numOfValues)
{
  speed=meetAndroid.getInt();
}
/***************************************************
// change_speed 结束
***************************************************/

void choice_control(byte flag, byte numOfValues)
{
 choice=meetAndroid.getInt();

}

/***************************************************
         cha_switch  方向控制
***************************************************/
void cha_switch(byte flag, byte numOfValues)
{
  get=meetAndroid.getInt();
  if(choice==5)
  {   digitalWrite(20, HIGH);
    digitalWrite(21, LOW);
    if( get==1||get==2||get==3||get==4 )
    {
      if(get==1)
      {
        Serial.println("forward");
        analogWrite(4, speed);
        analogWrite(5, 0);
        analogWrite(6, speed);
        analogWrite(7, 0);
      }

      if(get==2)
      {
        Serial.println("back");
        analogWrite(4, 0);
        analogWrite(5, speed);
        analogWrite(6, 0);
        analogWrite(7, speed);

      }

      if(get==3)
      {
        Serial.println("left");
        analogWrite(4, speed);
        analogWrite(5, 0);
        analogWrite(6, speed);
        analogWrite(7, speed);
      }

      if(get==4)
      {
        Serial.println("right");
        analogWrite(4, speed);
        analogWrite(5, speed);
        analogWrite(6, speed);
        analogWrite(7, 0);
      }
    }
    else if(get==0)
    {
      Serial.println("stop");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
    }
  }
}
/***************************************************
         cha_switch 结束
****************************************************/





/***************************************************
        加速度计调节
****************************************************/

        void acc_control()
        {
//接收数据
if(Serial.available())
{digitalWrite(21, HIGH);
  digitalWrite(20, LOW);
    accelBuff[0] = Serial.read(); //read
    //amarino发送的数据包含一个标志A
    if(accelBuff[0] == 'A')
    {
      //开始接收数据，数据以0x13结束
      i=1;
      while(1)
      {
        if(Serial.available())
        {
          accelBuff[i] = Serial.read(); //read
          if(accelBuff[i] == 0x13)
          break;
          else
          i++;
        }
      }
      //一直接收数据，直到接收数据结束
    }

    //获取接收数据中分号的位置
    for(i = 0 ; i < 100 ; i++)
    {
      if(accelBuff[i] == 0x3b)
      {
        accelSemi= i;
        break;
      }
    }

    //获取接收数据中小数点的位置
    for(i = 0;i < 100;i++)
    {
      if(accelBuff[i] == 0x2e)  //小数点
      {
        accelDot[0]= i;       //小数点位置
        break;
      }
    }

    for(i = accelSemi;i < 100;i++)  //从第一个分号位置开始，去找第二个小数点位置
    {
      if(accelBuff[i] == 0x2e)
      {
        accelDot[1]= i;
        break;
      }
    }
    ////////////////////////////////
    //根据接收到的数据控制小车的运动，4 5右轮  6 7左轮
    ////////////////////////////////

    //首先让小车停止
    analogWrite(4, 255);
    analogWrite(5, 255);
    analogWrite(6, 255);
    analogWrite(7, 255);
    //Serial.println("STOP");
    left=0;right=0;

    if(accelBuff[1] == '-')
    {
      if(accelDot[0] == 4)  x=(int)(accelBuff[2]-0x30)*10+(accelBuff[3]-0x30);  //A-12.3333
      else          x=(int)(accelBuff[2]-0X30);
    }

    else
    {
      if(accelDot[0] == 3)  x=(int)(accelBuff[1]-0x30)*10+(accelBuff[2]-0X30);  //A12.3
      else          x=(int)(accelBuff[1]-0X30);
    }

    if(accelBuff[accelSemi+1] == '-')
    {
      //如果 第二个小数点的位置等于=第一个分号+4,如-23.5;-23.5说明有十位数
      if(accelDot[1] == accelSemi+4)  y=(int)(accelBuff[accelSemi+2]-0X30)*10+(accelBuff[accelSemi+3]-0X30);
      else              y=(int)(accelBuff[accelSemi+2]-0X30);
    }

   else//无负号
   {
    //如果 第二个小数点的位置等于=第一个分号+3,如-23.5;23.5说明有十位数
    if(accelDot[1] == accelSemi+3)  y=(int)(accelBuff[accelSemi+1]-0X30)*10+(accelBuff[accelSemi+2]-0X30);
    else              y=(int)(accelBuff[accelSemi+1]-0X30);
  }

  pwm1=x*30;
  if(pwm1>255) pwm1=255;
  pwm2=y*30;
  if(pwm2>255) pwm2=255;
  //Serial.println(pwm1);     Serial.println(pwm2);


    //通过X轴数据的正负判端前进还是后退
    if(accelBuff[1] == '-') //手机前倾 小车向前走  A-0.31603462;0.49799395;12.421119
    {
      if(accelDot[0] == 4 || (accelBuff[2]-0X30) > 1)    //A-12.3 小数点在第四位（说明X参数为两位整数）  或者   数据第一位本身大于1（0的acsii码为0x30）
      {
       // Serial.println("forward");
       analogWrite(4, pwm1);
       analogWrite(5, 0);
       analogWrite(6, pwm1);
       analogWrite(7, 0);
     }
   }
  else                      //后退
  {
    if(accelDot[0] == 3 || (accelBuff[1]-0X30) > 1)
    {
     // Serial.println("back");
     analogWrite(4, 0);
     analogWrite(5, pwm1);
     analogWrite(6, 0);
     analogWrite(7, pwm1);
   }
 }

    //通过Y轴数据的正负判断左转还是右转
    /////////////////////////////////////////////////////    A-0.31603462;0.49799395;12.421119
  if(accelBuff[accelSemi+1] == '-')       //左转                                 //                                     ;-23.5
  {
    right=0;
      if(accelDot[1] == accelSemi+4 || (accelBuff[accelSemi+2]-0X30) > 1)    //如果 第二个小数点的位置等于=第一个分号+4,如上面假设的-23.5 或者是Y加速度值的第一位本身就大于1
      {
        Serial.println("left");
        analogWrite(4, pwm2);
        analogWrite(5, 0);
        analogWrite(6, 255);
        analogWrite(7, 255);
        left=1;

      }
    }
  else                  //右转
  {
    left=0;
    if(accelDot[1] == accelSemi+3 || (accelBuff[accelSemi+1]-0X30) > 1)
    {
  //    Serial.println("right");
  analogWrite(4, 255);
  analogWrite(5, 255);
  analogWrite(6, pwm2);
  analogWrite(7, 0);
  right=1;
}
}
}
}

/***************************************************
        加速度计调节  结束
****************************************************/


