#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<string>
// Update these with values suitable for your network.

#define mqtt_server "tailor.cloudmqtt.com" //is.banhvatra.vn
#define mqtt_topic_pub "SMART_PROJECT/ESP_01" //broker.mqttdashboard.com
#define mqtt_topic_sub "SMART_PROJECT/ESP_01"
#define mqtt_user "dhvuddfk"
#define mqtt_pwd "-2Y-VhjTn8lE"

const uint16_t mqtt_port = 11359;
String inString = "";

String strValue = "";
String strLed = "";

String strRedValue = "";
String strGreenValue = "";
String strBlueValue = "";

int strLength;

char Signal[20] = "";
char c_id[] = "x";
char c_pass[] = "x";
char mes ;

int data;

// Led01
const int led01_Pin_Red =  15; 
const int led01_Pin_Green =  13; 
const int led01_Pin_Blue =  12; 

// Led02
const int led02_Pin_Red =  2; 
const int led02_Pin_Green =  0; 
const int led02_Pin_Blue =  4;

// Led03
const int led03_Pin_Red =  5; 
const int led03_Pin_Green =  14; 
const int led03_Pin_Blue =  16;

WiFiClient espClient;// Tạo đối tượng wificlient
PubSubClient client(espClient);// Khai báo là client có thuộc tính của PubSubClient
long lastMsg = 0;
char msg[50] = "Linh Tran oi";
int value = 0;

const char* ssid = "ESP_01";
const char* password = "12345678";

    int id_length = 0;
    int pass_length = 0;


ESP8266WebServer server(80); //Server on port 80


void setup() {
  Serial.begin(115200);

  pinMode(led03_Pin_Red, OUTPUT);
  pinMode(led03_Pin_Green, OUTPUT);
  pinMode(led03_Pin_Blue, OUTPUT);
  
  pinMode(led02_Pin_Red, OUTPUT);
  pinMode(led02_Pin_Green, OUTPUT);
  pinMode(led02_Pin_Blue, OUTPUT);
  
  pinMode(led01_Pin_Red, OUTPUT);
  pinMode(led01_Pin_Green, OUTPUT);
  pinMode(led01_Pin_Blue, OUTPUT);

 


  setup_wifi("K2", "2910311770"); // Hàm tự viết ở dưới để kết nối wifi

   
  client.setServer(mqtt_server, mqtt_port);// Hàm kết nối vào mqtt server
  client.setCallback(callback);
}

void setup_wifi(char* idw, char* passw) {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to Wifi");

  
  WiFi.begin("K2", "2910311770");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {// Nếu chưa kết nối
    Serial.print("Attempting MQTT connection..."); // thì in ra dòng này
    // Attempt to connect
    if (client.connect("ESP8266Client",mqtt_user, mqtt_pwd)) { //nếu kết nối đúng 
      Serial.println("connected");// in ra là đã kết nối
  
       //client.publish(mqtt_topic_pub, "Linh Tran");
  
      client.subscribe(mqtt_topic_sub);// và lại hóng tin từ broker
    } else {
      Serial.print("failed, rc="); // còn nếu không thì in ra cái thất bại
      Serial.print(client.state());// in ra trạng thái của client
      Serial.println(" try again in 5 seconds"); 
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for(int i=0;i<20;i++)
  {
    Signal[i] = Signal[10];
  }
  
  for (int i = 0; i < length; i++) {
   
    Signal[i] = payload[i];
    
  }
  Serial.println(Signal);
     //Serial
     // USB\VID_1A86&PID_7523
    // 
    // 2551500112

  String strSignal = String(Signal);
  
  strRedValue = strSignal.substring(0, 3);
  strGreenValue = strSignal.substring(3, 6);
  strBlueValue = strSignal.substring(6, 9);
  strLed = strSignal.substring(9,10);

  strLength = strSignal.length();

      
}


void loop() {
  


 
  if (!client.connected()) {
    reconnect();

    
    
  }

  
    client.loop();
    long now = millis();// gán thời gian bây giờ là millis

        int RedValue = strRedValue.toInt();
        int GreenValue = strGreenValue.toInt();
        int BlueValue = strBlueValue.toInt();

        //Serial.println("do dai: ");
        //Serial.println(strLength);
     
         if(strLength == 10)
         {

         if((strLed == "1"))
        {

         analogWrite(led01_Pin_Red, RedValue); 
         analogWrite(led01_Pin_Green, GreenValue);
         analogWrite(led01_Pin_Blue, BlueValue);
          
        }

         if((strLed == "2"))
        {

         analogWrite(led02_Pin_Red, RedValue); 
         analogWrite(led02_Pin_Green, GreenValue);
         analogWrite(led02_Pin_Blue, BlueValue);
             
        }

          if((strLed == "3"))
        {
         analogWrite(led03_Pin_Red, RedValue); 
         analogWrite(led03_Pin_Green, GreenValue);
         analogWrite(led03_Pin_Blue, BlueValue);
        
  
        }

       }
      
         delay(500);
                 

           
}
