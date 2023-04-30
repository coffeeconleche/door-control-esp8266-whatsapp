#include <ESP8266WiFi.h>

const char* ssid = "YOUR_WIFI_SSID"; //
const char* password = "YOUR_WIFI_PASSWORD";

int RELAY_pin = 4; // RELAY pin. D2 is GIPO4
int port = 200; // Port we will use for the webserver

const long duration_pulse = 1000; //Milliseconds that the RELAY will be on.
WiFiServer server(port);

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY_pin, OUTPUT);

  digitalWrite(RELAY_pin, LOW); // Start with the RELAY OFF

  Serial.print("Connecting to the WiFi newtork");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");  
  server.begin();  // Starts the server
  Serial.println("Server started");

  Serial.print("IP Address of WiFI network: "); 
  Serial.println(WiFi.localIP()); // Prints IP address on Serial Monitor
  Serial.print("Copy and paste the following URL: http://");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.print(port);
  Serial.println("/");

}

void loop()
{
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  Serial.println("Waiting for new client");
  while(!client.available())
  {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  

  int relayState = LOW;
  
  if(request.indexOf("/RELAY=ON") != -1)
  {
    digitalWrite(RELAY_pin, HIGH); // Switch ON RELAY
    relayState = HIGH;
    delay(duration_pulse); // Keep it ON for 1000 milliseconds
    digitalWrite(RELAY_pin, LOW); // Switch OFF RELAY
    relayState = HIGH;
  }

/*------------------HTML UI Page---------------------*/

  client.println("HTTP/1.1 200 OK"); // Standalone web server with an ESP8266
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Door server.");
 
  client.println("<br><br>");
  client.println("<a href=\"/RELAY=ON\"\"><button>Switch ON RELAY for 1 second</button></a>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}