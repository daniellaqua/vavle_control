
//char array1[12] = {0xFF, 0xFF, 0x02, 0x80, 0x92, 0x05, 0x01, 0x42, 0x48, 0x00, 0x00, 0x1E};
//FFFF0280920501424800001E
//   char array1[12] = {0xFF,0xFF,0x02,0x80,0x92,0x05,0x01,0x42,0x48,0x48,0x48,0x1E};
//  char array1[12] = {0xFF, 0xFF, 0x02, 0x80, 0x92, 0x05, 0x01, 0x42, 0x48, 0x11, 0x11, 0x1E};
//  char array1[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* SSID = "pulsoxi_phantom";
const char* PSK = "NIFPO2018";
const char* MQTT_BROKER = "192.168.1.2";

WiFiClient espValveMaternal;
PubSubClient client(espValveMaternal);

// constants won't change. They're used here to set pin numbers:
const int ledRed = D0;      // select the pin for the LED
const int ledGreen = D2;      // select the pin for the LED
const int buttonMaternal = D6;     // the number of the pushbutton pin

// variables will change:
int buttonState = 1;
int pulsation = 0;
int open_time = 444;
int close_time = 555;

//char receivedChar;
//boolean newData = false;

//String inputString = "";         // a String to hold incoming data
//bool stringComplete = false;  // whether the string is complete

void ICACHE_RAM_ATTR ISRoutine ();

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: "); 
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // =========================================================================
  // Topic: Ventilsteuerung/Timing_Maternal
  // ========================================================================= 
  if(topic=="Ventilsteuerung/Timing_Maternal"){
    Serial.println("Timing received: ");
    String openTemp;
    String closeTemp;

    //example string {"open_time":100,"close_time":300}
    int firstParameterStart = messageTemp.indexOf(':')+1;
    int firstParameterEnd = messageTemp.indexOf(',');
    int secondParameterStart = messageTemp.indexOf(':', firstParameterStart+1)+1;
    int secondParameterEnd = messageTemp.length()-1;

    Serial.print("open time: ");
    for (int i = firstParameterStart; i < firstParameterEnd; i++) {
      Serial.print((char)message[i]);
      openTemp += (char)message[i];
    }  
    Serial.println();
    open_time=openTemp.toInt(); 

    Serial.print("close time: ");
    for (int i = secondParameterStart; i < secondParameterEnd; i++) {
      Serial.print((char)message[i]);
      closeTemp += (char)message[i];
    }
    close_time=closeTemp.toInt(); 
    Serial.println();
  }

  // =========================================================================
  // Topic: Ventilsteuerung/Remote_Pulsation_Maternal
  // =========================================================================
  if(topic=="Ventilsteuerung/Remote_Pulsation_Maternal"){
      
      Serial.print("pulsation: ");
      if(messageTemp == "on"){
        pulsation = 1;
        Serial.println("Remote: On");
        client.publish("Ventilsteuerung/Pulsation_Maternal", "on");
        client.publish("Ventilsteuerung/Hinweis_Maternal", "Remote start!");
      }
      else if(messageTemp == "off"){
        pulsation = 0;
        Serial.println("Remote: Off");
        client.publish("Ventilsteuerung/Pulsation_Maternal", "off");
        client.publish("Ventilsteuerung/Hinweis_Maternal", "Remote stop!");
      }
  }
  Serial.println();
}



void setup() {
  // initialize serial:
  Serial.begin(115200);
  Serial1.begin(9600);
  // reserve 200 bytes for the inputString:
  //inputString.reserve(200);

  setup_wifi();
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);

  // declare the LED as an OUTPUT:
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  // initialize the pushbutton pin as an input:
  //pinMode(buttonMaternal, INPUT);
  //attachInterrupt(digitalPinToInterrupt(buttonMaternal),ISRoutine,FALLING);
  attachInterrupt(buttonMaternal,ISRoutine,FALLING); 
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);
 
    WiFi.begin(SSID, PSK);
    IPAddress ip(192,168,1,110);   
    IPAddress gateway(192,168,1,1);   
    IPAddress subnet(255,255,255,0);  
    WiFi.config(ip, gateway, subnet);
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
    while (!client.connected()) {
        Serial.print("Reconnecting...");
        if (!client.connect("espValveMaternal")) {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
        client.subscribe("Ventilsteuerung/Timing_Maternal"); // Zeitverhalten
        client.subscribe("Ventilsteuerung/Remote_Pulsation_Maternal"); // Pulsation aktivieren
    }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if (pulsation == 1){  
    //open50();
    
    open100();      // öffnen zu 100%
    digitalWrite(ledRed, HIGH);
    Serial.print(open_time);
    delay(open_time);

    close_valve();   // schließen
    digitalWrite(ledRed, LOW);
    Serial.print(close_time);
    delay(close_time);

    digitalWrite(ledGreen, LOW);
  }
}


void close_valve(){
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0x02);
  Serial1.write(0x80);
  Serial1.write(0x92);  
  Serial1.write(0x05);  
  Serial1.write(0x01);
  Serial1.write(0x00);
  Serial1.write(0x00);  
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x14);
}


void open50(){
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0x02);
  Serial1.write(0x80);
  Serial1.write(0x92);  
  Serial1.write(0x05);  
  Serial1.write(0x01);
  Serial1.write(0x42);
  Serial1.write(0x48);  
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x1E);
}

void open100(){
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0x02);
  Serial1.write(0x80);
  Serial1.write(0x92);  
  Serial1.write(0x05);  
  Serial1.write(0x01);
  Serial1.write(0x42);
  Serial1.write(0xC8);  
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x9E);
}

void ISRoutine() {
    digitalWrite(ledGreen, HIGH);
    if (pulsation == 0){
      pulsation = 1;
      Serial.println("Manual On");
      client.publish("Ventilsteuerung/Pulsation_Maternal", "on");
      client.publish("Ventilsteuerung/Hinweis_Maternal", "Manual start!");
    } else{
      pulsation = 0;
      Serial.println("Manual Off");
      client.publish("Ventilsteuerung/Pulsation_Maternal", "off");
      client.publish("Ventilsteuerung/Hinweis_Maternal", "Manual stop!");
    }  
    //delay(200);
    digitalWrite(ledGreen, LOW);

}
