#include <IRSendRev.h>
#include <IRSendRevInt.h>
#include <BridgeUdp.h>
#include <Console.h>
#include <Bridge.h>
#include <BridgeServer.h>
#include <Mailbox.h>
#include <BridgeSSLClient.h>
#include <HttpClient.h>
#include <FileIO.h>
#include <Process.h>
#include <BridgeClient.h>

BridgeServer server;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  Bridge.begin();
  digitalWrite(13,HIGH);
  Serial.println("Bridge started");

  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  BridgeClient client = server.accept();
  if (client) {
    process(client);
    client.stop();
  }

  delay(50); 
}

void process(BridgeClient client) {
  String command = client.readStringUntil('/');

  if (command == "digital") {
    digitalCommand(client);
  }
  if (command == "analog") {
    analogCommand(client);
  }
  if (command == "mode") {
    modeCommand(client);
  }
}

void digitalCommand (BridgeClient client) {
  int pin, value;

  pin = client.parseInt();
  
  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  }
  else {
    value = digitalRead(pin);
  }

  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.print(value);

  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

void analogCommand(BridgeClient client) {
  int pin, value;

  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    analogWrite(pin, value);

    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  }
  else {
    value = analogRead(pin);

    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);

    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}

void modeCommand (BridgeClient client) {
  int pin;
  pin = client.parseInt();

  if (client.read() != '/') {
    client.println(F("error"));
    return;
  }

  String mode = client.readStringUntil('\r');
  if (mode == "input") {
    pinMode(pin,INPUT);
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as input"));
    return;
  }
  if (mode == "output") {
    pinMode(pin,OUTPUT);
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as output"));
    return;
  }
  client.print(F("error: invalid mode "));
  client.print(mode);
}

