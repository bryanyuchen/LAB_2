#include <ESP8266WiFi.h>
#include <Servo.h>

//------------------VARIABLES------------------//
WiFiServer server(80); //Initialize the server on Port 80
Servo myservo1;
Servo myservo2;
const short int LED_PIN = 16;//GPIO16
short int old_angle = 0;
bool reset = -1;
//------------------VARIABLES------------------//

//--------------------SETUP--------------------//
void setup() {
WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
WiFi.softAP("Hello_IoT", "12345678"); // Provide the (SSID, password); .
server.begin(); // Start the HTTP Server
//Looking under the hood
Serial.begin(115200);
IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server
Serial.print("Server IP is: "); // Print the IP to the monitor window
Serial.println(HTTPS_ServerIP);

//LED INIT
pinMode(LED_PIN, OUTPUT); //GPIO16 is an OUTPUT pin;
digitalWrite(LED_PIN, LOW); //Initial state is ON

//SERVO INIT
myservo1.attach(2);
myservo2.attach(14);
myservo1.write(0);
myservo2.write(90);
}
//--------------------SETUP--------------------//

//----------------PLAY FUNCTIONS---------------//
void playRest(){
  myservo2.write(90);
  delay(800);
  return;
}
void playSequence(char* Notes, int sizeofNotes){

  for (int i = 0; i < sizeofNotes; i++)
  {
   if (reset == 1) {
    return;
   }
    switch(Notes[i]){

   case 'c':
      playNote('c');
      Serial.println(" Playing note c");
      break;
  
   case 'd':
      playNote('d');
      Serial.println(" Playing note d");
      break; 

   case 'e':
      playNote('e');
      Serial.println(" Playing note e");
      break; 

   case 'f':
      playNote('f');
      Serial.println(" Playing note f");
      break; 

   case 'g':
      playNote('g');
      Serial.println(" Playing note g");
      break; 

   case 'a':
      playNote('a');
      Serial.println(" Playing note a");
      break; 

   case 'r': //r = rest
      playRest();
      Serial.println(" Playing rest");
      break; 
      
   default : /* Optional */
   break;
  }
  }
}
void playNote(char note){
  double delayTime; //'delay' is a function recognized by arduino so we add 'time' to the end to distinguish
  double travelTime; //time takes for servo to travel
  int new_angle, angle_delta;
  int pos;
//assign angle to new note
  switch (note) {
   case 'c':
      new_angle = 45;
      break;
  
   case 'd':
      new_angle = 40;
      break; 

   case 'e':
      new_angle = 32;
      break; 

   case 'f':
      new_angle = 25;
      break; 

   case 'g':
      new_angle = 15;
      break; 

   case 'a':
      new_angle = 0;
      break; 
      
   default : /* Optional */
   break;
  }
  angle_delta = new_angle - old_angle;
  if(angle_delta < 0) {
    pos = new_angle-5;
  }
  if(angle_delta > 0) {
    pos = new_angle+5;
  }
  
  //Serial.print("angle is at ");
  //Serial.print(pos);
  //travelTime = abs(angle_delta)*0.0025;
  //delayTime = 1.05 - travelTime;
  
  myservo1.write(pos);
  myservo2.write(0);
  delay(900);
  
  old_angle = new_angle;
  return;
}

void playDefault() {
  //char Notes[16] = {'c','d','e','f','g','f','e','d','c','d','e','f','g','f','e','d'};
char Notes[] = {'c','d','e','f','g','f','e','d'};
 playSequence(Notes,8);
}
//----------------PLAY FUNCTIONS---------------//

//------------------MAIN LOOP------------------//
void loop() {
myservo1.write(0);
myservo2.write(90);
WiFiClient client = server.available();
if (!client) {
return;
}
//Looking under the hood
Serial.println("Somebody has connected :)");
//Read what the browser has sent into a String class

//and print the request to the monitor
String request = client.readStringUntil('\r');
//Looking under the hood
Serial.println(request);
// Handle the Request
if (request.indexOf("/c") != -1){
  Serial.println("playing c");
  playNote('c');}
else if (request.indexOf("/d") != -1){
  Serial.println("playing d");
  playNote('d');}
  else if (request.indexOf("/e") != -1){
    Serial.println("playing e");
  playNote('e');}
  else if (request.indexOf("/f") != -1){
    Serial.println("playing f");
  playNote('f');}
  else if (request.indexOf("/g") != -1){
    Serial.println("playing g");
  playNote('g');}
  else if (request.indexOf("/a") != -1){
    Serial.println("playing a");
  playNote('a');}
  else if (request.indexOf("/default") != -1){
  playDefault();}
  else if (request.indexOf("/reset") != -1){
  reset = 1;}
// Prepare the HTML document to respond and add buttons:
String s = "HTTP/1.1 200 OK\r\n";
s += "Content-Type: text/html\r\n\r\n";
s += "<!DOCTYPE HTML>\r\n<html>\r\n";
s += "<br><input type=\"button\" name=\"b1\" value=\"Play C\"";
s += " onclick=\"location.href='/c'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"b1\" value=\"Play D\"";
s += " onclick=\"location.href='/d'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"b1\" value=\"Play E\"";
s += " onclick=\"location.href='/e'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"b1\" value=\"Play F\"";
s += " onclick=\"location.href='/f'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"b1\" value=\"Play G\"";
s += " onclick=\"location.href='/g'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"b1\" value=\"Play A\"";
s += " onclick=\"location.href='/a'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"b1\" value=\"Play Default\"";
s += " onclick=\"location.href='/default'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"b1\" value=\"Reset\"";
s += " onclick=\"location.href='/reset'\">";
s += "</html>\n";
//Serve the HTML document to the browser.
client.flush(); //clear previous info in the stream
client.print(s); // Send the response to the client
delay(1);
Serial.println("Client disonnected"); //Looking under the hood

}
//------------------MAIN LOOP------------------//
