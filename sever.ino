#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>
#include <HTTPClient.h>

const char *SSID = "Ooredoo3A46FC";
const char *PWD = "QQKDNK2A97#GC";


const byte INFRA_PIN = 23; // Broche TRIGGER

char*  message;


// Web server running on port 80 (http port)
WebServer server(80);

 

// JSON data buffer
StaticJsonDocument<200> jsonDocument;
char buffer[200];

// connection to the wifi mba3id ndeha fil setup
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PWD);
  // wifi for connection 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }
 
  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());
}

// setup API resources
void setup_routing() {     
  server.on("/message", getMessage);
  // start server    
  server.begin(); 
}

void create_json(char *tag, char *message) {  
  jsonDocument.clear();  
  jsonDocument["type"] = tag;
  jsonDocument["message_envoye"] = message;
  
  serializeJson(jsonDocument, buffer);
}
 
void add_json_object(char *tag, char *message) {
  JsonObject obj = jsonDocument.createNestedObject();
  jsonDocument["type"] = tag;
  jsonDocument["message_envoye"] = message;
   
}

//void read_sensor_data(void * parameter) {
   //for (;;) {
      //digitalWrite(TRIGGER_PIN, HIGH);
      //delayMicroseconds(1000);
      //digitalWrite(TRIGGER_PIN, LOW);

      //duration = pulseIn(ECHO_PIN, HIGH); // receive reflected waves
      //distance = duration / 58.2;   // convert to distance
      //delay(1000); 
      //Serial.println("Read sensor data");
 
     // delay the task
     //vTaskDelay(60000 / portTICK_PERIOD_MS);
   
  // }
//}

// implement the Rest API
void getMessage() {
  int obstacle = digitalRead(23);
  if (obstacle== HIGH){
            Serial.print("la boite est vide"   );
            message="la boite est vide";
            
             delay (1000);}
  else {
            Serial.print("vous avez recu un message  ");
            message ="vous avez recu un message";
            
            delay(1000); }
  Serial.println("Get Message");
 create_json("message_envoye",message);
  server.send(200, "application/json", buffer);
  
}

void handlePost() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  
  // Respond to the client
  server.send(200, "application/json", "");
}

//void setup_task() {    
  //xTaskCreate(     
  //read_sensor_data,      
  //"Read sensor data",       
  //1000,      
  //NULL,      
  //1,     
  //NULL     
  //);     
//}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);    
     
  // Sensor setup    
  connectToWiFi();     
     
  setup_routing(); 
  /* Initialise les broches */
  pinMode(INFRA_PIN, INPUT);
 
}

void loop() {
 getMessage();
  server.handleClient();
}
