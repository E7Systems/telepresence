/*
  Temperature web interface

 This example shows how to serve data from an analog input
 via the Arduino Yún's built-in webserver using the Bridge library.

 The circuit:
 * TMP36 temperature sensor on analog pin A1
 * SD card attached to SD card slot of the Arduino Yún

 Prepare your SD card with an empty folder in the SD root
 named "arduino" and a subfolder of that named "www".
 This will ensure that the Yún will create a link
 to the SD to the "/mnt/sd" path.

 In this sketch folder is a basic webpage and a copy of zepto.js, a
 minimized version of jQuery.  When you upload your sketch, these files
 will be placed in the /arduino/www/TemperatureWebPanel folder on your SD card.

 You can then go to http://arduino.local/sd/TemperatureWebPanel
 to see the output of this sketch.

 You can remove the SD card while the Linux and the
 sketch are running but be careful not to remove it while
 the system is writing to it.

 created  6 July 2013
 by Tom Igoe

 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/TemperatureWebPanel

 */

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <SoftwareSerial.h>
// Listen on default port 5555, the webserver on the Yún
// will forward there all the HTTP requests for us.
YunServer server;
String startString;
long hits = 0;
SoftwareSerial BT = SoftwareSerial(3, 2);

void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

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
    Serial.println("headers sent");
    String command = client.readString();
    command.trim();        //kill whitespace
    command.replace("key/", "");
    Serial.println(command);
    if (command.startsWith("send")) {
         int key = (command.substring(command.indexOf("/")+1, command.length()).toInt());
         Serial.println(key);
         BT.write((char)key);
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
      Serial.println(timeString);
      client.println("<script type='text/javascript'>");
      client.println("window.onkeyup = sendKey;");
      client.println("function sendKey(evt) {");
      client.println("  evt.preventDefault()");
      client.println("  xmlHttp = new XMLHttpRequest();");
      client.println("  xmlHttp.open( \"GET\", \"/arduino/key/send/\"+evt.which, false );");
      client.println("  xmlHttp.send( null );");
      client.println("}");
      client.println("</script>");
      client.println("<br>Current time on the Yun: ");
      client.println(timeString);
      client.println("<br>Press any key to transfer it to my keyboard! (use ctrl+backspace instead of backspace if it keeps going to the previous page.)");
      client.println("<br>This sketch has been running since ");
      client.println(startString);
      client.println("<br>Number of key presses so far: ");
      client.println(hits);
      client.print("</html>");
    }

    // Close connection and free resources.
    client.stop();
  delay(50); // Poll every 50ms
  }



