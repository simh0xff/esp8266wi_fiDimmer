#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#define R  16
#define G  5
#define B  4
#define S 14 //сигнал о начале работы
int regim = 0;
int incColour = 1;
int decColour = 0;     
int i = 0;

const char * HTML_code = "<!DOCTYPE html>"
"<html>"
 "<head>"
   "<h1 style=color:#000000>LedStripDriver</h1>"
  "</head>"
  "<body>"
     "<p style=color:#FF0000>Press button to change the mode</p><br>"
    "<style>"
   " html {"
   " text-align: center;"
"}"
".buttonRight {"
 " position: relative;"
  "display: inline-block;"
 " font-size: 4 10%;"
  "font-weight: 900;"
  "color: rgb(209,209,217);"
 " text-decoration: none;"
  "text-shadow: 0 -1px 2px rgba(0,0,0,.2);"
 " padding: 10px;"
 " outline: none;"
 " border-radius: 4px;"
  "background: linear-gradient(rgb(110,112,120), rgb(81,81,86)) rgb(110,112,120);"
 " box-shadow:"
   "0 1px rgba(255,255,255,.2) inset,"
  " 0 3px 5px rgba(0,1,6,.5),"
  " 0 0 1px 1px rgba(0,1,6,.2);"
 " transition: .2s ease-in-out;"
"}"
".buttonRight:hover:not(:active) {"
 " background: linear-gradient(rgb(126,126,134), rgb(70,71,76)) rgb(126,126,134);"
"}"
".buttonRight:active {"
"  top: 1px;"
"  background: linear-gradient(rgb(76,77,82), rgb(56,57,62)) rgb(76,77,82);"
 " box-shadow:"
"   0 0 1px rgba(0,0,0,.5) inset,"
   "0 2px 3px rgba(0,0,0,.5) inset,"
  " 0 1px 1px rgba(255,255,255,.1);"
"}"
".button {"
 " position: relative;"
  "display: inline-block;"
 " font-size: 4 10%;"
  "font-weight: 900;"
  "color: rgb(209,209,217);"
 " text-decoration: none;"
  "text-shadow: 0 -1px 2px rgba(0,0,0,.2);"
 " padding: 10px;"
 " outline: none;"
 " border-radius: 4px;"
  "background: linear-gradient(rgb(110,112,120), rgb(81,81,86)) rgb(110,112,120);"
 " box-shadow:"
   "0 1px rgba(255,255,255,.2) inset,"
  " 0 3px 5px rgba(0,1,6,.5),"
  " 0 0 1px 1px rgba(0,1,6,.2);"
 " transition: .2s ease-in-out;"
"}"
".button:hover:not(:active) {"
 " background: linear-gradient(rgb(126,126,134), rgb(70,71,76)) rgb(126,126,134);"
"}"
".button:active {"
"  top: 1px;"
"  background: linear-gradient(rgb(76,77,82), rgb(56,57,62)) rgb(76,77,82);"
 " box-shadow:"
"   0 0 1px rgba(0,0,0,.5) inset,"
   "0 2px 3px rgba(0,0,0,.5) inset,"
  " 0 1px 1px rgba(255,255,255,.1);"
"}"
 ".buttonLeft {"
 " position: relative;"
  "display: inline-block;"
 " font-size: 4 10%;"
  "font-weight: 900;"
  "color: rgb(209,209,217);"
 " text-decoration: none;"
  "text-shadow: 0 -1px 2px rgba(0,0,0,.2);"
 "  padding: 10px;"
 " outline: none;"
 " border-radius: 5px;"
  "background: linear-gradient(rgb(110,112,120), rgb(81,81,86)) rgb(110,112,120);"
 " box-shadow:"
   "0 1px rgba(255,255,255,.2) inset,"
  " 0 3px 5px rgba(0,1,6,.5),"
  " 0 0 1px 1px rgba(0,1,6,.2);"
 " transition: .2s ease-in-out;"
"}"
".buttonLeft:hover:not(:active) {"
 " background: linear-gradient(rgb(126,126,134), rgb(70,71,76)) rgb(126,126,134);"
"}"
".buttonLeft:active {"
"  top: 1px;"
"  background: linear-gradient(rgb(76,77,82), rgb(56,57,62)) rgb(76,77,82);"
 " box-shadow:"
"   0 0 1px rgba(0,0,0,.5) inset,"
   "0 2px 3px rgba(0,0,0,.5) inset,"
  " 0 1px 1px rgba(255,255,255,.1);"
"}"
 
"</style>"
   "<input type=\"button\" class=\"buttonLeft\" value=\"Decrease\"onclick=""javascript:window.location=\"decrease\">"
    "<input type=\"button\" class=\"button\" value=\"RainbowLoop\"onclick=""javascript:window.location=\"rainbowLoop\">"
    "<input type=\"button\" class=\"buttonRight\" value=\"Increase\"onclick=""javascript:window.location=\"increase\">"
 "</body>"
"</html>" ;
MDNSResponder mdns;
const char* ssid = "Esp8266LedPower";
const char* password = "ledstripdriver";
ESP8266WebServer server(80);
void setup() {
 Serial.begin(115200);
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
server.on("/", handleRoot);

  server.on("/increase", [](){
    server.send(200, "text/html",HTML_code );
              regim++;
            (regim>=3) ? regim=2 : regim = regim ;
            (regim<=0) ? regim=0 : regim = regim ;
 
if(regim==0){
  setColourRgb(1023,0,0);
   }
if(regim==1){
  setColourRgb(0,1023,0);
 }
if(regim==2){
  setColourRgb(0,0,1023);
}
  });
  server.on("/decrease", [](){
    server.send(200, "text/html",HTML_code );
              regim--;
            (regim>=3) ? regim=2 : regim = regim ;
            (regim<=0) ? regim=0 : regim = regim ;
 
if(regim==0){
  setColourRgb(1023,0,0);
   }
if(regim==1){
  setColourRgb(0,1023,0);
 }
if(regim==2){
  setColourRgb(0,0,1023);
}
  
  });
  server.on("/rainbowLoop", [](){
     server.send(200, "text/html",HTML_code );
   while(i<10){
      i++;
  unsigned int rgbColour[3];
 
  rgbColour[0] = 1023;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  

  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int c = 0; c < 1023; c += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      
      setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
      delay(1); 
       
      }
     }
    }
  i=0; 
  setColourRgb(0,0,0);
  });
  server.onNotFound(Error404);
   pinMode(R,OUTPUT);
    pinMode(G,OUTPUT);
     pinMode(B,OUTPUT);
      pinMode(S,OUTPUT);     
  server.begin();
  
} 

void loop(void) {
  server.handleClient(); 
   }


void setColourRgb( int red,  int green, int blue) {
  analogWrite(R, red);
  analogWrite(G, green);
  analogWrite(B, blue);
  }




void handleRoot(void){
    server.send(200, "text/html", HTML_code);
    digitalWrite(S,HIGH);
  }


  
  void Error404(void){
   server.send(200, "text/html","<title>ERROR404</title>"); 
  }

