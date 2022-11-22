#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(2, 3);

bool DEBUG = true;   
int responseTime = 10; 
int LED_RED = 13;
int LED_BLUE = 11;

void setup()
{
  pinMode(LED_RED,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(9,OUTPUT);
  
  Serial.begin(115200);
  while (!Serial) {}
  
  wifiSerial.begin(115200);
  while (!wifiSerial) {}
  
  sendToWifi("AT+CWMODE=2",responseTime,DEBUG); 
  sendToWifi("AT+CIFSR",responseTime,DEBUG); 
  sendToWifi("AT+CIPMUX=1",responseTime,DEBUG); 
  sendToWifi("AT+CIPSERVER=1,80",responseTime,DEBUG); 
}


void loop()
{
  if(Serial.available()>0){
     String message = readSerialMessage();
    if(find(message,"debugEsp8266:")){
      String result = sendToWifi(message.substring(13,message.length()),responseTime,DEBUG);
      if(find(result,"OK"))
        sendData("\nOK");
      else
        sendData("\nEr");
    }
  }
  if(wifiSerial.available()>0){
    
    String message = readWifiSerialMessage();
    Serial.print(message);

    if(find(message,"REDON")){
      digitalWrite(LED_RED,HIGH);
    }else if(find(message,"REDOFF")){
      digitalWrite(LED_RED,LOW);
    }
    else if(find(message,"BLUEON")){
      digitalWrite(11,HIGH);
    }
    else if(find(message,"BLUEOFF")){
      digitalWrite(11,LOW);
    }

    else if(find(message,"YON")){
      digitalWrite(9,HIGH);
    }
    else if(find(message,"YEOFF")){
      digitalWrite(9,LOW);
    }
    


  }
  delay(responseTime);
}

void sendData(String str){
  String len="";
  len+=str.length();
  sendToWifi("AT+CIPSEND=0,"+len,responseTime,DEBUG);
  delay(100);
  sendToWifi(str,responseTime,DEBUG);
 
}

boolean find(String string, String value){
  return string.indexOf(value)>=0;
}


String  readSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(Serial.available()>0){
    value[index_count]=Serial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}


String  readWifiSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(wifiSerial.available()>0){
    value[index_count]=wifiSerial.read();
    index_count++;
    value[index_count] = '\0'; 
  }
  String str(value);
  str.trim();
  return str;
}

String sendToWifi(String command, const int timeout, boolean debug){
  String response = "";
  wifiSerial.println(command);
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(wifiSerial.available())
    { 
    char c = wifiSerial.read(); 
    response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}
