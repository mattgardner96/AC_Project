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
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  Serial.print("Bridge Started");

  server.listenOnLocalHost();
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

void process(client) {
  
}





