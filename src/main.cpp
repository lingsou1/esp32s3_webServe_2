/*
接线说明:无

程序说明:通过esp32s3建立一个简单的网页服务器,通过html(超文本标记语言)来编写网页程序来控制开发板上的LED灯的亮灭

注意事项:无

函数示例:无

作者:灵首

时间:2023_3_9

*/
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>

//对开发板上的灯进行宏定义
#define LED_A 10
#define LED_B 11


WebServer esp32s3_webServe(80);//实例化一个网页服务的对象
WiFiMulti wifi_multi;  //建立WiFiMulti 的对象,对象名称是 wifi_multi


/*
# brief 连接WiFi的函数
# param 无
# retval 无
*/
void wifi_multi_con(void){
  int i=0;
  while(wifi_multi.run() != WL_CONNECTED){
    delay(1000);
    i++;
    Serial.print(i);
  }
}


/*
# brief 写入自己要连接的WiFi名称及密码,之后会自动连接信号最强的WiFi
# param 无
# retval  无
*/
void wifi_multi_init(void){
  wifi_multi.addAP("LINGSOU123","12345678");
  wifi_multi.addAP("LINGSOU12","12345678");
  wifi_multi.addAP("LINGSOU1","12345678");
  wifi_multi.addAP("LINGSOU234","12345678");   //通过 wifi_multi.addAP() 添加了多个WiFi的信息,当连接时会在这些WiFi中自动搜索最强信号的WiFi连接
}



/*
# brief 处理网站根目录"/"的访问请求
# param 无
# retval  无
*/
void handleRoot(){
    esp32s3_webServe.send(200,"text/html","<form action=\"/LED\" method=\"POST\"> <input type=\"submit\" value=\"controlLED\"> </form>");
    //200是状态码;"text/html是描述将以HTML来展示一段文本;最后的是一段HTML(超文本标记语言)代码,
    //大概就是创建了一个名为"controlLED"的按钮,按下该按钮浏览器会以POST请求的方式向开发板发出HTTP请求
    //当开发板接收到该请求后会执行 handleLED 函数
}


/*
# brief 处理LED控制请求的函数
# param 无
# retval 无
*/
void handleLED(){
  digitalWrite(LED_A,!digitalRead(LED_A));
  delay(500);
  digitalWrite(LED_B,!digitalRead(LED_B));
  delay(500);//开发板上的具体操作内容,控制灯的亮灭

  esp32s3_webServe.sendHeader("Location","/");//"Location":跳转的位置;"/":根目录
  esp32s3_webServe.send(303);//这两句一起是发送了303状态码,同时回到页面根目录
}



/*
# brief 错误处理,当访问出错时会出现的页面情况
# param 无
# retval  无
*/
void handleNotFound(){
  esp32s3_webServe.send(404,"text/plain","404 NotFound");//404:状态码(404是不正常的),"text/plain":发送的内容格式,"404 NotFound":发送的内容
}



/*
# brief esp32s3建立网页服务初始化函数,提供一个网页根目录处理函数以及一个处理LED控制请求的函数和错误处理函数
# param 无
# retval  无
*/
void esp32s3_webServe_init(void){
  esp32s3_webServe.begin();
  esp32s3_webServe.on("/",HTTP_GET,handleRoot);
  esp32s3_webServe.on("/LED",HTTP_POST,handleLED);
  esp32s3_webServe.onNotFound(handleNotFound);
  Serial.print("\n HTTp esp32s3_webServe started");
}



void setup() {
  Serial.begin(9600);

  pinMode(LED_A,OUTPUT);
  pinMode(LED_B,OUTPUT);//设置灯的引脚的状态

  wifi_multi_init();//储存多个WiFi
  wifi_multi_con();//自动连接WiFi

  Serial.print("\nconnect wifi:");
  Serial.print(WiFi.SSID());
  Serial.print("\nIP address:");
  Serial.print(WiFi.localIP());//输出连接信息(连接的WIFI名称及开发板的IP地址)

  esp32s3_webServe_init();//初始化网页服务,开启服务器工作

}


void loop() {
  esp32s3_webServe.handleClient();//循环运行,使能运行处理HTTP访问请求,尽量不要在其中加上延时函数,避免服务器不工作
}
