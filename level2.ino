#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


MDNSResponder mdns;


const char* ssid = "kavin"; // your connection name
const char* password = "12345612"; // your connection password

ESP8266WebServer server(80);

int gpio1_pin = D1;  // D4 of nodemcu
int gpio2_pin = D2; // D7 of nodemcu
int gpio3_pin =14; //  D5 of nodemcu




//Check if header is present and correct
bool is_authentified(){
  Serial.println("Enter is authentified");
  if (server.hasHeader("Cookie")){
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

//login page, also called for disconnect
void handleLogin(){
  String msg;
  if (server.hasHeader("Cookie")){
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")){
    Serial.println("Disconnection");
    server.sendHeader("Location","/login");
    server.sendHeader("Cache-Control","no-cache");
    server.sendHeader("Set-Cookie","ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")){
    if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "root" ) // enter ur username and password you want
    {
      server.sendHeader("Location","/");
      server.sendHeader("Cache-Control","no-cache");
      server.sendHeader("Set-Cookie","ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;

      }

  msg = "Wrong username/password! try again.";
  Serial.println("Log in Failed");
  }
  String content = "<html><body style='background-color:MediumAquaMarine'><form action='/login' method='POST'><p  align ='center' style='font-size:300%;'><u><b><i>  Log In  </i></b></u></p><br>";
  content += " <p   align ='center' style='font-size:160%'><b> UserName:<input type='text' name='USERNAME' placeholder='user name' required></b></p><br>";
  content += "<p  align ='center' style='font-size:160%'><b>Password:<input type='password' name='PASSWORD' placeholder='password' required></b></p><br>";
  content += "<p  align ='center' style='font-size:160%'><input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "</p><br> </body></html>";
  server.send(200, "text/html", content);
}

//root page can be accessed only if authentification is ok
void handleRoot(){
  Serial.println("Enter handleRoot");
  String header;
  if (!is_authentified()){
    server.sendHeader("Location","/login");
    server.sendHeader("Cache-Control","no-cache");
    server.send(301);
    return;
  }
  String content =  "<body style='background: #80c6f7'><h1 align ='center'><b><u><i><strong>HOME AUTOMATION</strong></i></u></b></h1><br><p align ='center'>Switch #1 <a href=\"switch1On\"><button>ON</button></a>&nbsp;<a href=\"switch1Off\"><button>OFF</button></a></p>";
  content += "<br><p  align ='center'>Switch #2 <a href=\"switch2On\"><button>ON</button></a>&nbsp;<a href=\"switch2Off\"><button>OFF</button></a></p>";
  content += "<br><p  align ='center'>Switch #3 <a href=\"switch3On\"><button>ON</button></a>&nbsp;<a href=\"switch3Off\"><button>OFF</button></a></p>";
  content += "<br><p><marquee direction='right'>Developed by Cyber Access </marquee></p>";
  content += "<br><br><br><br></body>"; 
  
   if (server.hasHeader("User-Agent")){
    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
    
    
  }
  content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
  server.send(200, "text/html", content);
}

//no need authentification
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}



void setup(void){


   // preparing GPIOs
  pinMode(gpio1_pin, OUTPUT);
  digitalWrite(gpio1_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);
  pinMode(gpio3_pin, OUTPUT);
  digitalWrite(gpio3_pin, LOW);
  
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.begin(115200);
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

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }


  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works without need of authentification");
  });

  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );


  server.on("/",[](){
   //
  });
  server.on("/switch1On", [](){
   //
    if (is_authentified()){ 
	    digitalWrite(gpio1_pin, HIGH);
	    delay(1000);}
  });
  server.on("/switch1Off", [](){
 // 
    if (is_authentified()){ 
	    digitalWrite(gpio1_pin, LOW);
	    delay(1000); }
  });
  server.on("/switch2On", [](){
  //
    digitalWrite(gpio2_pin, HIGH);
    delay(1000);
  });
  server.on("/switch2Off", [](){
    //
    digitalWrite(gpio2_pin, LOW);
    delay(1000); 
  });
  server.on("/switch3On", [](){
    digitalWrite(gpio3_pin, HIGH);
    delay(1000);
  });
  server.on("/switch3Off", [](){
    digitalWrite(gpio3_pin, LOW);
    delay(1000); 
  });


  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
