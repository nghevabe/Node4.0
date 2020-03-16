#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include<string>
// Update these with values suitable for your network.

#define mqtt_server "tailor.cloudmqtt.com" //is.banhvatra.vn
#define mqtt_topic_pub "SMART_PROJECT/ESP_L1" //broker.mqttdashboard.com
#define mqtt_topic_sub "SMART_PROJECT/ESP_L1"
#define mqtt_user "dhvuddfk"
#define mqtt_pwd "-2Y-VhjTn8lE"

void writeString(char add,String data);
String read_String(char add);

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

int time_counter = 0;

// Led01
const int led01_Pin_Red =  2; 
const int led01_Pin_Green =  0; 
const int led01_Pin_Blue =  4; 


WiFiClient espClient;// Tạo đối tượng wificlient
PubSubClient client(espClient);// Khai báo là client có thuộc tính của PubSubClient
long lastMsg = 0;
char msg[50] = "Linh Tran oi";
int value = 0;

const char* ssid = "ESP_L1";
const char* password = "12345678";

    int id_length = 0;
    int pass_length = 0;

    int idLoad_length = 0;

    int passLoad_length = 0;

    String id_load;
    String pass_load;


ESP8266WebServer server(80); //Server on port 80


void setup() {
  Serial.begin(115200);

  pinMode(led01_Pin_Red, OUTPUT);
  pinMode(led01_Pin_Green, OUTPUT);
  pinMode(led01_Pin_Blue, OUTPUT);

  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP(); //Get IP address

  delay(2000);

  LoadEeprom();

  Get_PassAnsId();

  server.on("/", handleRoot);        

  server.on("/wifi", handleMesseger);  


  server.begin();  
  
  
}

void setup_wifi(char* idw, char* passw) {

  time_counter = 0; 
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to Wifi");

  
  WiFi.begin(idw, passw);
  while (WiFi.status() != WL_CONNECTED && time_counter < 25) {
    delay(500);
    Serial.print(".");
    time_counter ++ ;
  }

  if(time_counter > 20){
    Serial.println("WiFi connect Fail");
    Serial.println("Check Id and PassWord");
  }

  if(time_counter < 20)
  {
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //
   if(idLoad_length == 0 && passLoad_length == 0){
        SaveEeprom(idw, passw);
   }

  }

  // 

 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {// Nếu chưa kết nối
    Serial.print("Attempting MQTT connection..."); // thì in ra dòng này
    // Attempt to connect
    if (client.connect("ESP_L01",mqtt_user, mqtt_pwd)) { //nếu kết nối đúng 
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
 
  String strSignal = String(Signal);

  strLength = strSignal.length();

  if(strLength == 10)
  {
  strRedValue = strSignal.substring(0, 3);
  strGreenValue = strSignal.substring(3, 6);
  strBlueValue = strSignal.substring(6, 9);
  strLed = strSignal.substring(9,10);
  }

 

  

      
}

void handleRoot() {
 
  server.send(200, "text/plain", "ESP Service!");
}

void handleMesseger() {                         //Handle POST Request
    
    String readId = server.arg("id");
    String readPass = server.arg("pass");
    Serial.println("ID: " + readId);
    Serial.println("PASS: " + readPass);
    id_length = readId.length();
    pass_length = readPass.length();

    char idWifi[50]; 
    char passWifi[50];
    readId.toCharArray(idWifi, 50);
    readPass.toCharArray(passWifi, 50);

    if(id_length > 0 && pass_length > 0){

      server.send(200, "text/plain", "ESP_01");

      Serial.println("=====");
      Serial.println(idWifi);
      Serial.println(passWifi);
      Serial.println("=====");
      
      setup_wifi(idWifi,passWifi);
      
      client.setServer(mqtt_server, mqtt_port);// Hàm kết nối vào mqtt server
      client.setCallback(callback);
    }
    

}

void SaveEeprom(char* idw, char* passw){

    EEPROM.begin(512);
    Serial.print("Writing Data...");
  
    delay(1000);
    writeString(10, idw);  //Address 10 and String type data
    delay(1000);
    writeString(30, passw);  //Address 30 and String type data
    delay(1000);

    
    Serial.println("Done");
  
}

void Get_PassAnsId(){

  idLoad_length = id_load.length();
  passLoad_length = pass_load.length();
  
   if(idLoad_length > 0 && passLoad_length > 0)
  {
  Serial.println("Load Data: ");
  Serial.println(id_load);
  Serial.println(pass_load);

    char idWifi[50]; 
    char passWifi[50];
    id_load.toCharArray(idWifi, 50);
    pass_load.toCharArray(passWifi, 50);

  setup_wifi(idWifi,passWifi);
  delay(2000);
  client.setServer(mqtt_server, mqtt_port);// Hàm kết nối vào mqtt server
  client.setCallback(callback);
  
  delay(10);
  } else {
    Serial.println("Don't have any Data to Load");
    Serial.println("Please Connect With ESP To Share Wifi");

    
    
  }
  
}

void LoadEeprom(){

  EEPROM.begin(512);
  id_load = read_String(10);
  delay(1000);
  pass_load = read_String(30);
  delay(1000);

  

 
  
}

void writeString(char add,String data)
{
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   //Add termination null character for String Data
  EEPROM.commit();
}


String read_String(char add)
{
  int i;
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}
  

void loop() {
  
  server.handleClient(); 

  if(idLoad_length > 0 && passLoad_length > 0){
  if (!client.connected()) {
    reconnect();

    
    
  }

  } else {
    Serial.println("Chưa kết nối với wifi");
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

         analogWrite(led01_Pin_Red, RedValue * 3.8); 
         analogWrite(led01_Pin_Green, GreenValue * 3.8);
         analogWrite(led01_Pin_Blue, BlueValue * 3.8);
          
        }

 

       }
      
         delay(500);
                 

           
}
