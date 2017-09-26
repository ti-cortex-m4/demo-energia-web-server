/****************************************************************
 * Designer: Erhan YILMAZ										*
 * Application: Tiva Connected Launchpad Web Server Application	*
 * Date: 19-08-2015												*
 * Version: 1.0													*					
 * Description:	Web server application with Tiva connected		*
 * launchpad. Codes writen on energia							*
 * *************************************************************/

#define LED1 PN_1
#define LED2 PN_0
#define LED3 PF_4
#define LED4 PF_0

#include "Ethernet.h"
// Prototypes
void printEthernetData();
void GetSwitchState(EthernetClient);
void SetLEDStates(EthernetClient, String);

EthernetServer server(80);
int statusConfig = 0;


void setup() {
  Serial.begin(115200);    

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);

  Serial.println("Connecting to Ethernet....");  

  Ethernet.begin(0);
  // For static ip use below code
  /*
  IPAddress ip = IPAddress(10,2,7,158);
  IPAddress dns = IPAddress(10,2,7,200);
  IPAddress gw = IPAddress(10,2,7,254);
  IPAddress mask = IPAddress(255,255,0,0);
  Ethernet.begin(0, ip, dns, gw);
  */
  server.begin();
  printEthernetData();
}

EthernetClient client;
String HTTP_req;            // stores the HTTP request
void loop() 
{
  client = server.available();

  if (client)
  {                             // if you get a client,
    Serial.print("new client on port ");           // print a message out the serial port
    Serial.println(client.port());
    String currentLine = "";                // make a String to hold incoming data from the client
    boolean newConnection = true;     // flag for new connections
    unsigned long connectionActiveTimer;  // will hold the connection start time

    boolean currentLineIsBlank = true;
    while (client.connected())
    {       // loop while the client's connected
      if (newConnection)
      {                 // it's a new connection, so
        connectionActiveTimer = millis(); // log when the connection started
        newConnection = false;          // not a new connection anymore
      }
      if (!newConnection && connectionActiveTimer + 1000 < millis())
      { 
        // if this while loop is still active 1000ms after a web client connected, something is wrong
        break;  // leave the while loop, something bad happened
      }


      if (client.available())
      {             // if there's bytes to read from the client,    
        char c = client.read();             // read a byte, then
        // This lockup is because the recv function is blocking.
        Serial.print(c);
        HTTP_req += c;  // save the HTTP request 1 char at a time
        // last line of client request is blank and ends with \n
        // respond to client only after last line received
        if (c == '\n' && currentLineIsBlank)
        {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: keep-alive");
          client.println();
          // AJAX request for switch state
          if (HTTP_req.indexOf("ajax_switch") > -1)
          {
            // read switch state and send appropriate paragraph text
            GetSwitchState(client);
            SetLEDStates(client, HTTP_req);

          }
          else 
          {  // HTTP request for web page
            // send web page - contains JavaScript with AJAX calls
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<title>Erhan Web Page</title>");
            client.println("<script>");
            client.println("var led1, x1, led2, x2, led3, x3, led4, x4;");
            client.println("function GetSwitchState() {");
            client.println("nocache = led1 + led2 + led3 + led4 + \"&nocache=\"\+ Math.random() * 1000000;");
            client.println("var request = new XMLHttpRequest();");
            client.println("request.onreadystatechange = function() {");
            client.println("if (this.readyState == 4) {");
            client.println("if (this.status == 200) {");
            client.println("if (this.responseText != null) {");            
            client.println("if (this.responseText.indexOf(\"LED1ON\") > -1){document.getElementById(\"LED1\").style.fill = \"yellow\";}");
            client.println("else {document.getElementById(\"LED1\").style.fill = \"black\";}");
            client.println("if (this.responseText.indexOf(\"LED2ON\") > -1){document.getElementById(\"LED2\").style.fill = \"yellow\";}");
            client.println("else {document.getElementById(\"LED2\").style.fill = \"black\";}");
            client.println("if (this.responseText.indexOf(\"LED3ON\") > -1){document.getElementById(\"LED3\").style.fill = \"yellow\";}");
            client.println("else {document.getElementById(\"LED3\").style.fill = \"black\";}");
            client.println("if (this.responseText.indexOf(\"LED4ON\") > -1){document.getElementById(\"LED4\").style.fill = \"yellow\";}");
            client.println("else {document.getElementById(\"LED4\").style.fill = \"black\";}");
            client.println("if (this.responseText.indexOf(\"S1:ON\") > -1){document.getElementById(\"SW1\").style.fill = \"red\";document.getElementById(\"text1\")\.innerHTML =\"SW1:ON\";}");
            client.println("else {document.getElementById(\"SW1\").style.fill = \"white\"; document.getElementById(\"text1\")\.innerHTML =\"SW1:OFF\";}");
            client.println("if (this.responseText.indexOf(\"S2:ON\") > -1){document.getElementById(\"SW2\").style.fill = \"red\"; document.getElementById(\"text2\")\.innerHTML =\"SW2:ON\";}");
            client.println("else {document.getElementById(\"SW2\").style.fill = \"white\"; document.getElementById(\"text2\")\.innerHTML =\"SW2:OFF\";}");
            client.println("}}}}");
            client.println("request.open(\"GET\", \"ajax_switch\" + nocache, true);");
            client.println("request.send(null);");
            client.println("setTimeout('GetSwitchState()', 500);");
            client.println("}");
            client.println("function SetLEDStates(num){");
            client.println("switch(num){");
            client.println("case 1: if(x1==1){led1=\"&LED1ON\";x1=0;}else{led1=\"&LED1OFF\";x1=1;}break;");
            client.println("case 2: if(x2==1){led2=\"&LED2ON\";x2=0;}else{led2=\"&LED2OFF\";x2=1;}break;");
            client.println("case 3: if(x3==1){led3=\"&LED3ON\";x3=0;}else{led3=\"&LED3OFF\";x3=1;}break;");
            client.println("case 4: if(x4==1){led4=\"&LED4ON\";x4=0;}else{led4=\"&LED4OFF\";x4=1;}break;");
            client.println("case 0: led1=\"&LED1OFF\";x1=1; led2=\"&LED2OFF\";x2=1; led3=\"&LED3OFF\";x3=1; led4=\"&LED4OFF\";x4=1;break;");
            client.println("}}");
            client.println("</script>");
            client.println("</head>");
            client.println("<body onload=\"GetSwitchState();SetLEDStates(0);\"><center>");
            client.println("<h1>Erhan YILMAZ AJAX Test Application</h1>");
            client.println("<div id=\"text1\"></div>");
            client.println("<svg width=\"40\" height=\"40\"><circle id=\"SW1\" cx=\"20\" cy=\"20\" r=\"10\"stroke=\"black\" stroke-width=\"4\" fill=\"white\"/></svg>");
            client.println("<div id=\"text2\"></div>");
            client.println("<svg width=\"40\" height=\"40\"><circle id=\"SW2\" cx=\"20\" cy=\"20\" r=\"10\"stroke=\"black\" stroke-width=\"4\" fill=\"white\"/></svg>");
            client.println("<p><button type=\"L1\" onclick=\"SetLEDStates(1);\">LED1</button>&nbsp;");
            client.println("<button type=\"L2\" onclick=\"SetLEDStates(2);\">LED2</button>&nbsp;");
            client.println("<button type=\"L3\" onclick=\"SetLEDStates(3);\">LED3</button>&nbsp;");
            client.println("<button type=\"L4\" onclick=\"SetLEDStates(4);\">LED4</button>&nbsp;</p>");
            client.println("<p><center>");
            client.println("<svg width=\"50\" height=\"50\"><circle id=\"LED1\" cx=\"20\" cy=\"20\" r=\"10\"stroke=\"green\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("<svg width=\"50\" height=\"50\"><circle id=\"LED2\" cx=\"20\" cy=\"20\" r=\"10\"stroke=\"green\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("<svg width=\"50\" height=\"50\"><circle id=\"LED3\" cx=\"20\" cy=\"20\" r=\"10\"stroke=\"green\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("<svg width=\"50\" height=\"50\"><circle id=\"LED4\" cx=\"20\" cy=\"20\" r=\"10\"stroke=\"green\" stroke-width=\"4\" fill=\"black\"/></svg>");
            client.println("</center></p>");
            client.println("</center></body>");
            client.println("</html>");
          }
          // display received HTTP request on serial port
          Serial.print(HTTP_req);
          HTTP_req = "";            // finished with request, empty string
          break;
        }
        // every line of text received from the client ends with \r\n
        if (c == '\n')
        {
          // last character on line of received text
          // starting new line with next character read
          currentLineIsBlank = true;
        } 
        else if (c != '\r')
        {
          // a text character was received from client
          currentLineIsBlank = false;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

// send the state of the switch to the web browser
void GetSwitchState(EthernetClient cl)
{ 
  char x=0;

  if (!digitalRead(PUSH1)) x+=1;
  if (!digitalRead(PUSH2)) x+=2;	
  
  switch(x)
  {
  case 0:
    cl.println("S1:OFF S2:OFF");
  break;

  case 1:
    cl.println("S1:ON S2:OFF");
  break;

  case 2:
    cl.println("S1:OFF S2:ON");
  break;

  default:
    cl.println("S1:ON S2:ON");
  break;
  }
}

void SetLEDStates(EthernetClient cl, String HTTP_req)
{

  if (HTTP_req.indexOf("LED1ON") > -1)
  {
    digitalWrite(LED1, HIGH); 
    cl.println("LED1ON");
  }
  else
  {
    digitalWrite(LED1, LOW); 
    cl.println("LED1OFF");
  }

  if (HTTP_req.indexOf("LED2ON") > -1)
  {
    digitalWrite(LED2, HIGH); 
    cl.println("LED2ON");
  }
  else
  {
    digitalWrite(LED2, LOW); 
    cl.println("LED2OFF");
  }

  if (HTTP_req.indexOf("LED3ON") > -1)
  {
    digitalWrite(LED3, HIGH); 
    cl.println("LED3ON");
  }
  else
  {
    digitalWrite(LED3, LOW); 
    cl.println("LED3OFF");
  }

  if (HTTP_req.indexOf("LED4ON") > -1)
  {
    digitalWrite(LED4, HIGH); 
    cl.println("LED4ON");
  }
  else
  {
    digitalWrite(LED4, LOW); 
    cl.println("LED4OFF");
  }
}


void serialEvent() {
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read(); 
  } 
}

void printEthernetData()
{
  // print your IP address:
  Serial.println();
  Serial.println("IP Address Information:");  
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address:\t");
  Serial.println(ip);

  // print your MAC address:

  IPAddress subnet = Ethernet.subnetMask();
  Serial.print("NetMask:\t");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = Ethernet.gatewayIP();
  Serial.print("Gateway:\t");
  Serial.println(gateway);

  // print your gateway address:
  IPAddress dns = Ethernet.dnsServerIP();
  Serial.print("DNS:\t\t");
  Serial.println(dns);

}

