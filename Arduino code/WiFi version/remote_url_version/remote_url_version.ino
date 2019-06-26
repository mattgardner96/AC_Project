/*
  Matt Gardner, 7/27/18

  A/C sender sketch(main code)
  ->local version using URL-parsed commands

  Adapted from send.ino sketch, an example code for Grove IR Emitter/Receiver
  IRSendRevMaster library, created by SEEEDStudio and used under MIT License.

  Adapted from Arduino Bridge Library example code

*/
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


#define BIT_LEN         0
#define BIT_START_H     1
#define BIT_START_L     2
#define BIT_DATA_H      3
#define BIT_DATA_L      4
#define BIT_DATA_LEN    5
#define BIT_DATA        6
#define UPDATE_INTERVAL 32759              // 300 for testing purposes = 30s interval
#define CMNDNUM         7               // number of possible commands, for array implementation

const int ir_freq = 38;                 // 38k, drives loop function;

unsigned char dtaSend[20];                

String commands[CMNDNUM] = {"power", "up", "down", "high", "low", "cool", "fan"};

char serverName[] = "ipv4.icanhazip.com"; // get IP webpage server

int DDNS_update;

String IPaddress;

BridgeServer server;
HttpClient dns_updater;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dtaInit(0, 0);
  DDNS_update = 450; // initialize DDNS counter such that update occurs 45 seconds after power on
  Bridge.begin();
  Serial.println("Initialized");
  pinMode(13, OUTPUT);
  //Serial.println("List of Commands:");
  //for (int j = 0; j < CMNDNUM; j++) { Serial.println(commands[j]); }

  //server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  BridgeClient client = server.accept();
  if (client) {
    read_proc(client);
    client.stop();
  }

  delay(100);
  
  DDNS_update--; // increment every time program runs (every 100ms)
  
  if (DDNS_update == 0) {
    DDNS_update = UPDATE_INTERVAL; // reset back to top if down-counter completes
    updateDNS();
  }

}

void updateDNS() { 
  Serial.println("updating IP");
  Process p;            
  p.runShellCommand("curl -H \"Host: dynupdate.no-ip.com\" -H \"Authorization: Basic bWF0dGdhcmRuZXI5NkBnbWFpbC5jb206YXJkdWlubzEy\" -H \"User-Agent: MattsMacbook macOS/10.14 mattgardner96@gmail.com\" http://dynupdate.no-ip.com/nic/update?hostname=mattgardnerac.ddns.net");
  while(p.running());
  while(p.available()) {
    Serial.print(p.read());
  }
}

void read_proc(BridgeClient client) {
  String httpCommand = client.readStringUntil('/');
  int value;
  String ACCommand;

  if (httpCommand == "ac") {
    
    ACCommand = client.readStringUntil('/');
    
    if (ACCommand == "up" | ACCommand == "down") {
      value = client.parseInt();
    }
    
    else {
      value = 0;
    }

    client.print(sendToAC(ACCommand, value));
  }

};

// initializes the starting values for the system
void dtaInit(int data1, int data2) {
  dtaSend[BIT_LEN]        = 9;            // all data that needs to be sent
  dtaSend[BIT_START_H]    = 179;          // the logic high duration of "Start"
  dtaSend[BIT_START_L]    = 90;           // the logic low duration of "Start"
  dtaSend[BIT_DATA_H]     = 11;           // the logic "long" duration in the communication
  dtaSend[BIT_DATA_L]     = 33;           // the logic "short" duration in the communication

  dtaSend[BIT_DATA_LEN]   = 4;            // Number of data which will sent. If the number is other, you should increase or reduce dtaSend[BIT_DATA+x].

  dtaSend[BIT_DATA + 0]     = 8;          // data that will sent
  dtaSend[BIT_DATA + 1]     = 231;
  dtaSend[BIT_DATA + 2]     = data1;
  dtaSend[BIT_DATA + 3]     = data2;
  //dtaSend[BIT_DATA+4]     = 192;        // data is only 4 values
  //dtaSend[BIT_DATA+5]     = 63;         // data is only 4 values
};

// small setter method to make IR data reassignments faster
void dataChange(int data1, int data2) {
  dtaSend[BIT_DATA + 2] = data1;
  dtaSend[BIT_DATA + 3] = data2;
};

// send to A/C function
// Uses array traversal to send commands to the arduino
String sendToAC(String commandStr, int number) {
  
  int cmndData[CMNDNUM * 2] = {0, 255, 168, 87, 176, 79, 104, 151, 112, 143, 48, 207, 16, 239};

  String outStrings[CMNDNUM] = {"Cycling A/C Power", "Turning up temp", "Turning down temp", "Fan high", "Fan low", "Cool mode", "Fan mode"};

  for (int i = 0; i < CMNDNUM; i++) {
    
    if (commandStr == commands[i]) {
      dataChange(cmndData[2 * i], cmndData[2 * i + 1]);
      for (int j = 0; j <= number; j++) {
        IR.Send(dtaSend, char(ir_freq));  // send to A/C (if IR Emitter is connected...)
        delay(300);
      }
      digitalWrite(13, HIGH);
      delay(1000);
      digitalWrite(13, LOW);
      Serial.println(outStrings[i]);
      return "\n" + outStrings[i] + "\nCommand sent\n\n";
    }
  }
  
  return "Bad command";
};
