#include <WiFi.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid= "Mahesh";
const char* password= "Aryan@14";

Servo myServo; //msServo is object of class Servo
const int servoPin=5;
String output5state="reverse";
String outputangle45="0";
String outputangle90="0";
String outputangle180="0";

// Decode HTTP GET value
String valueString = String(5);
int pos1 = 0;
int pos2 = 0;

String header; //To store HTTP request
WiFiServer server(80); //Serevr Port(80)

//Millis() function

unsigned long presentTime=millis();
unsigned long pastTime = 0;
const long delayTime= 2000; //2s=2000ms

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  myServo.attach(servoPin); //Attach to GPIO 5

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("WiFi connected To");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Connected To;");
  display.setCursor(0,20);
  display.println(WiFi.localIP());
  display.display();
  server.begin();

}

void loop() {

  //WiFiClient client creates a new client that connects to specific IP address
  //server.available() = incoming clients
    WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    presentTime = millis();
    pastTime = presentTime;
    Serial.println("New Client Connected");        
    String currentLine = "";                //String to hold incoming data from the client

    while (client.connected() && presentTime - pastTime <= delayTime) { 
      // loop while the client is connected
      presentTime = millis();

      if (client.available()) {             
        char c = client.read();             // read a byte, then print on serial monitor and add to header variable
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row then that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK"); //Response code to send
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);

              //indexOf("GET /?value="): This checks if the HTTP request is a GET request with the URL path "/?value="
              // ">=0" is condition checks if the substring "GET /?value=" is found in the header string.
              //pos1: This variable stores the index position of the first occurrence of the character '=' in the header string.
              //pos2: This variable stores the index position of the first occurrence of the character '&' in the header string.
              //valueString: This variable is assigned a substring of the header string, starting from the character after '=' (pos1 + 1) and ending just before '&' (pos2). 
              
              //Rotate the servo
              myServo.write(valueString.toInt());
              Serial.println(valueString);

              display.clearDisplay();
              display.setTextSize(1);
              display.setTextColor(WHITE);
              display.setCursor(0,0);
              display.println("MOTOR POSITION");
              display.setCursor(0,20);
              display.println(valueString);
              display.display(); 
            } 

            else if(header.indexOf("GET /5/forward")>=0){
              Serial.println("Motor Start");
              output5state="forward";
              
              for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
                myServo.write(posDegrees);
                Serial.println(posDegrees);
                delay(20);
                }
              
              display.clearDisplay();
              display.setTextSize(1);
              display.setTextColor(WHITE);
              display.setCursor(0,0);
              display.println("MOTOR STATUS");
              display.setCursor(0,20);
              display.println(output5state);
              display.display();
            }

            else if (header.indexOf("GET /5/reverse")>=0){
              output5state="reverse";
              for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
                myServo.write(posDegrees);
                Serial.println(posDegrees);
                delay(20);
                }
              
              display.clearDisplay();
              display.setTextSize(1);
              display.setTextColor(WHITE);
              display.setCursor(0,0);
              display.println("MOTOR STATUS");
              display.setCursor(0,20);
              display.println(output5state);
              display.display();
            }
            else if (header.indexOf("GET /5/45")>=0){
              outputangle45="45";
              myServo.write(45);

              display.clearDisplay();
              display.setTextSize(2);
              display.setTextColor(WHITE);
              display.setCursor(0,0);
              display.println("ANGLE");
              display.setCursor(0,40);
              display.println(outputangle45);
              display.display();
            }

            else if (header.indexOf("GET /5/0-45")>=0){
              outputangle45="0";
              myServo.write(0);

              display.clearDisplay();
              display.setTextSize(2);
              display.setTextColor(WHITE);
              display.setCursor(0,0);
              display.println("ANGLE");
              display.setCursor(0,40);
              display.println(outputangle45);
              display.display();
            }

            else if (header.indexOf("GET /5/90")>=0){
              outputangle90="90";
              myServo.write(90);

              display.clearDisplay();
              display.setTextSize(2);
              display.setTextColor(WHITE);
              display.setCursor(0,0);
              display.println("ANGLE");
              display.setCursor(0,40);
              display.println(outputangle90);
              display.display();
            }

            else if (header.indexOf("GET /5/0-90")>=0){
              outputangle90="0";
              myServo.write(0);

              display.clearDisplay();
              display.setTextSize(2);
              display.setTextColor(WHITE);
              display.setCursor(0,0);
              display.println("ANGLE");
              display.setCursor(0,40);
              display.println(outputangle90);
              display.display();
            }

            else if (header.indexOf("GET /5/180")>=0){
              outputangle180="180";
              myServo.write(180);

              display.clearDisplay();
              display.setTextSize(2);
              display.setTextColor(WHITE);
              display.setCursor(0,0);
              display.println("ANGLE");
              display.setCursor(0,40);
              display.println(outputangle180);
              display.display();
            }

            else if (header.indexOf("GET /5/0-180")>=0){
              outputangle180="0";
              myServo.write(0);

              display.clearDisplay();
              display.setTextSize(2);
              display.setTextColor(WHITE);
              display.setCursor(0,0);
              display.println("ANGLE");
              display.setCursor(0,40);
              display.println(outputangle180);
              display.display();
            }


            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            client.println("<title >\"ESP WEB SERVER FOR SERVO MOTOR\" </title>");
            
            client.println("<style>");
            client.println("body { text-align: center; font-family: \"Helvitica\"; margin-left:auto;}");
            client.println("body {background-image: url(\"https://t3.ftcdn.net/jpg/02/69/76/36/360_F_269763698_m8yMmBCJ8c7N51ohpyd1Um6Akn7WttNW.jpg\"); background-repeat: no-repeat; background-size: cover;}");


            client.println(".div {display:inline-block;}");
            client.println(".button {background-color:#4CAF50; border:none; display: inline-block; color:white; padding:16px 40px; text-decoration:none; font-size:15px; margin:2px; cursor:pointer; border-radius:50px}");
            //button padding refers to the space between the content (text or icon) of a button and the edges of the button itself and The cursor is a pointer and indicates a link
            client.println(".button2 {background-color:#555555;}");

            client.println(".slider { width: 300px; }");
            client.println("</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>"); //XML Http Request send and receive
                     
            // Web Page
            client.println("</head>");
            client.println("<body><h1>Servo Control Slider</h1>");
            client.println("<style>body { background-color: white; }</style>");
            client.println("<p>SERVO ANGLE: <span id=\"servoPos\"></span></p>");
            client.println("<div>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            client.println("</div>");
            
            //Javascript
            client.println("<script>");
            client.println("var slider = document.getElementById(\"servoSlider\");"); //Retrives the value of slider input with id "servoSLider" and assign to variable
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;"); //Update the content of element with id "servoPos"
            //inner.HTML is property of element you are replacing its content with the value store in varible slider
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }"); //oninput event is triggered whenever the value of the input element changes,
            client.println("function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
            //$.get() method to make an AJAX request. The $.get() method is used to send an HTTP GET request
            ///?value=": This is the URL to which the GET request is sent.
            client.println("<div>");
            client.println("<h2>SERVO FORWARD/REVERSE</h2>");
            if (output5state=="reverse"){
              client.println("<p><a href =\"/5/forward\"> <button class= \"button\">Forward</button></a></p>");
            }
            else {
              client.println("<p><a href =\"/5/reverse\"> <button class= \"button button2\">Reverse</button></a></p>");
            }
            client.println("</div>");

            client.println("<br>");
            client.println("<div>");
            client.println("<h2>ANGLE 45:</h2>");
          
            if (outputangle45=="0"){
              client.println("<p><a href =\"/5/45\"> <button class= \"button\">Angle 45</button></a></p>");

            }
            else{
              client.println("<p><a href =\"/5/0-45\"> <button class= \"button button2\">REST</button></a></p>");

            }
            client.println("</div>");

            client.println("<br>");
            client.println("<div>");
            client.println("<h2>ANGLE 90:</h2>");
            
            if (outputangle90=="0"){
              client.println("<p><a href =\"/5/90\"> <button class= \"button\">Angle 90</button></a></p>");

            }
            else{
              client.println("<p><a href =\"/5/0-90\"> <button class= \"button button2\">REST</button></a></p>");

            }
            client.println("</div>");

            client.println("<br>");
            client.println("<div>");
            client.println("<h3>ANGLE 180:</h3>");
            
            if (outputangle180=="0"){
              client.println("<p><a href =\"/5/180\"> <button class= \"button\">Angle 180</button></a></p>");

            }
            else{
              client.println("<p><a href =\"/5/0-180\"> <button class= \"button button2\">REST</button></a></p>");

            }
            client.println("</div>");

           
            client.println("</body></html>");     
            
        
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
