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
#define UPDATE_INTERVAL 100              // 300 for testing purposes = 30s interval
#define CMNDNUM         7               // number of possible commands, for array implementation

const int ir_freq = 38;                 // 38k, drives loop function;

unsigned char dtaSend[20];                

String commands[CMNDNUM] = {"power", "up", "down", "high", "low", "cool", "fan"};

char serverName[] = "checkip.dyndns.com"; // get IP webpage server

int DDNS_update;

String IPaddress;

BridgeServer server;
HttpClient dns_updater;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dtaInit(0, 0);
  DDNS_update = 0; // initialize DDNS counter
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
  /*Serial.print("Enter an A/C function: ");
    while (!Serial.available()) {
    //do nothing till input comes in
    }
    Serial.println(sendToAC(Serial.readStringUntil(10)));
  */
  BridgeClient client = server.accept();
  if (client) {
    read_proc(client);
    client.stop();
  }

  delay(100);
  
  DDNS_update++; // increment every time program runs (every 100ms)
  
  if (DDNS_update == UPDATE_INTERVAL) {
    DDNS_update = 0; // reset back to 0 if counter fills
    dns_updater.get("http://bWF0dGdhcmRuZXI5NkBnbWFpbC5jb206Um4hN095XmgxNjMj@dynupdate.no-ip.com/nic/update?hostname=mattgardnerac.ddns.net&myip=" + getIP());
  }
}

void read_proc(BridgeClient client) {
  String httpCommand = client.readStringUntil('/');
  int value;
  String ACCommand;

  if (httpCommand == "ac") {
    ACCommand = client.readStringUntil('/');
    //Serial.println("ACcommand = " + ACCommand); //DEBUG
    if (ACCommand == "up" | ACCommand == "down") {
      //Serial.println("got to loop"); //DEBUG
      value = client.parseInt();
    }
    else {
      //Serial.println("skipped loop"); //debug
      value = 0;
    }

      // Serial.println(value); //DEBUG
    client.print(sendToAC(ACCommand, value));
  }

};

// Gets the IP address from a server
String getIP () { /* client function to send/receive GET request data. from Arduino Forum https://forum.arduino.cc/index.php?topic=225139.0 */

  BridgeClient dns_client;
  IPaddress = "";
  
  if (dns_client.connect(serverName, 80)) {  //starts client connection, checks for connection
    Serial.println("connected");
    dns_client.println("GET / HTTP/1.0"); //download text
    dns_client.println("Host: checkip.dyndns.com");
    dns_client.println(); //end of get request
  } 
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  while(dns_client.connected() && !dns_client.available()) delay(1); //waits for data
  while (dns_client.connected() || dns_client.available()) { //connected or data available
    String c = dns_client.readString(); //gets byte from wifi buffer
    Serial.println("C: " + c);
    //Serial.println(c); //DEBUG
    IPaddress.concat(c);
  }
    
    dns_client.stop();
    IPaddress = IPaddress.substring(20);
    return IPaddress;
  }

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
  Serial.print("In the function. Command = " + commandStr);
  Serial.println(number);
  
  int cmndData[CMNDNUM * 2] = {0, 255, 168, 87, 176, 79, 104, 151, 112, 143, 48, 207, 16, 239};

  String outStrings[CMNDNUM] = {"Cycling A/C Power", "Turning up temp", "Turning down temp", "Fan high", "Fan low", "Cool mode", "Fan mode"};

  Serial.println("ENTERING FOR LOOP\n");
  for (int i = 0; i < CMNDNUM; i++) {
    //Serial.println(commandStr);
    //Serial.println(commands[i]);
    
    if (commandStr == commands[i]) {
      Serial.println("got here");
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
  Serial.println("Exiting fn");
  return "Bad command";
};

