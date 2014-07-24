#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <SoftwareSerial.h>
// Listen on default port 5555, the webserver on the Yún
// will forward there all the HTTP requests for us.
YunServer server;
String startString;
long hits = 0;
int pinForward = 2;
int pinBack = 3;
int pinLeft = 4;
int pinRight = 5;

void setup() {
  Serial.begin(9600);

  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  pinMode(pinForward, OUTPUT);
  pinMode(pinBack, OUTPUT);
  pinMode(pinRight, OUTPUT);
  pinMode(pinLeft, OUTPUT);
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();

  // get the time that this sketch started:
  Process startTime;
  startTime.runShellCommand("date");
  while (startTime.available()) {
    char c = startTime.read();
    startString += c;
  }
}

void loop() {
  // Get clients coming from server
  YunClient client = server.accept();

  // There is a new client?
  if (client) {
    
    client.println("Status: 200");
    client.println("Content-type: text/html");
    client.println(); //mandatory blank line
    client.println("<!DOCTYPE html>");
    client.println("<html>");
    // read the command
    String command = client.readString();
    command.trim();        //kill whitespace
    command.replace("key/", "");
    Serial.println(command);
    if (command.startsWith("send") && command.endsWith("start")) {
         command.replace("/start", "");
         int key = (command.substring(command.indexOf("/")+1, command.length()).toInt());
         if(key == 37) {
            digitalWrite(pinLeft, HIGH); 
         } else if(key == 38) {
            digitalWrite(pinForward, HIGH);
         } else if(key == 39) {
            digitalWrite(pinRight, HIGH); 
         } else if(key == 40) {
            digitalWrite(pinBack, HIGH); 
           
         }
         hits++;
    } else if(command.startsWith("send") && command.endsWith("stop")) {
         command.replace("/stop", "");
         int key = (command.substring(command.indexOf("/")+1, command.length()).toInt());
         if(key == 37) {
            digitalWrite(pinLeft, LOW); 
         } else if(key == 38) {
            digitalWrite(pinForward, LOW);
         } else if(key == 39) {
            digitalWrite(pinRight, LOW); 
         } else if(key == 40) {
            digitalWrite(pinBack, LOW); 
           
         }
         hits++;
      
    }

      // get the time from the server:
      Process time;
      time.runShellCommand("date");
      String timeString = "";
      while (time.available()) {
        char c = time.read();
        timeString += c;
      }
      client.println("<script type='text/javascript'>");
      client.println("var pressingKey = false;");
      client.println("window.onkeyup = sendKeyUp;");
      client.println("function sendKeyUp(evt) {");
      client.println("  evt.preventDefault()");
      client.println("  xmlHttp = new XMLHttpRequest();");
      client.println("  xmlHttp.open( \"GET\", \"/arduino/key/send/\"+evt.which+\"/stop\", false );");
      client.println("  xmlHttp.send( null );");
      client.println("  pressingKey = false;");
      client.println("}");
      client.println("window.onkeydown = sendKeyDown;");
      client.println("function sendKeyDown(evt) {");
      client.println("  if(!pressingKey) {");
      client.println("    evt.preventDefault()");
      client.println("    xmlHttp = new XMLHttpRequest();");
      client.println("    xmlHttp.open( \"GET\", \"/arduino/key/send/\"+evt.which+\"/start\", false );");
      client.println("    xmlHttp.send( null );");
      client.println("    pressingKey = true;");
      client.println("  }");
      client.println("}");
      client.println("</script>");
      client.println("<br>Current time on the Yun: ");
      client.println(timeString);
      client.println("<br>Use the arrow keys to control the tank!");
      client.println("<br>This sketch has been running since ");
      client.println(startString);
      client.println("<br>Number of commands so far: ");
      client.println(hits);
      client.print("</html>");
    }

    // Close connection and free resources.
    client.stop();
  delay(50); // Poll every 50ms
  }



