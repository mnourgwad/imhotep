/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>

// WiFi connection credentials
const char* ssid = "ssid";
const char* password = "password";

// Dummy data for testing
String noOfBatteries = "4";
String maxReading    = "350";
float readings[]  = {240, 273, 145, 168};

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\n');
  Serial.println(req);
  client.flush();
  
  // Match the request
  String val;
  if (req.equals("ConfigData")) {

    // convert the float array to string
    String readingsQuery = "";
    for (int i = 0; i < noOfBatteries.toInt(); i++) {
      if (i != noOfBatteries.toInt() - 1) {
        readingsQuery += readings[i];
        readingsQuery += ",";
      } else {
        readingsQuery += readings[i];
      }
    }

    // join all the data in one string
    val = noOfBatteries + "-" + maxReading + "-" + readingsQuery;
    client.print(val);
    delay(1);
  } else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  
  client.flush();

  // Send the response to the client
  // Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

