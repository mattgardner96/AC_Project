/*
Matt Gardner, 7/25/18

A/C sender sketch(main code)
->Type-to-send version (no web connectivity)

Adapted from send.ino sketch, an example code for Grove IR Emitter/Receiver
IRSendRevMaster library, created by SEEEDStudio and used under MIT License.

*/
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
#include <IRSendRev.h>
#include <IRSendRevInt.h>

#define BIT_LEN         0
#define BIT_START_H     1
#define BIT_START_L     2
#define BIT_DATA_H      3
#define BIT_DATA_L      4
#define BIT_DATA_LEN    5
#define BIT_DATA        6

const int ir_freq = 38;                 // 38k, drives loop function;

unsigned char dtaSend[20];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(5000);
  dtaInit(0, 0);
  Serial.println("Initialized");
  Serial.println("Reference Guide of Commands: \npower\ntemp up\ntemp down\nhigh\nlow\ncool\nfan");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("\nEnter an A/C function: ");
  while (!Serial.available()) {
    //do nothing till input comes in
  }
  Serial.print(sendToAC(Serial.readStringUntil(10)));
}

// initials the starting values for the system
void dtaInit(int data1,int data2) {
    dtaSend[BIT_LEN]        = 9;            // all data that needs to be sent
    dtaSend[BIT_START_H]    = 179;          // the logic high duration of "Start"
    dtaSend[BIT_START_L]    = 90;           // the logic low duration of "Start"
    dtaSend[BIT_DATA_H]     = 11;           // the logic "long" duration in the communication
    dtaSend[BIT_DATA_L]     = 33;           // the logic "short" duration in the communication

    dtaSend[BIT_DATA_LEN]   = 4;            // Number of data which will sent. If the number is other, you should increase or reduce dtaSend[BIT_DATA+x].

    dtaSend[BIT_DATA+0]     = 8;            // data that will sent
    dtaSend[BIT_DATA+1]     = 231;
    dtaSend[BIT_DATA+2]     = data1;
    dtaSend[BIT_DATA+3]     = data2;
    //dtaSend[BIT_DATA+4]     = 192;        // data is only 4 values
    //dtaSend[BIT_DATA+5]     = 63;         // data is only 4 values
};

// small setter method to make IR data reassignments faster
void dataChange(int data1, int data2) {
    dtaSend[BIT_DATA+2] = data1;
    dtaSend[BIT_DATA+3] = data2;
};

// send to A/C method
// works with a series of if statements that compare the argument to the
// possible options and assign the data to be sent out. Then sends the correct
// data
String sendToAC(String commandStr) {
    String outString = String("Command not recognized");

    bool goodCmd = false;

    if (commandStr.compareTo("power") == 0) {        //power
        dataChange(0,255);
        outString = "Cycling A/C Power";
        goodCmd = true;
    }
    if (commandStr.compareTo("up") == 0) {           //temp up
        dataChange(168,87);
        outString = "Turning up temp";
        goodCmd = true;
    };
    if (commandStr.compareTo("down") == 0) {         //temp down
        dataChange(176,79);
        outString = "Turning down temp";
        goodCmd = true;
    };
    if (commandStr.compareTo("high") == 0) {         //fan high
        dataChange(104,151);
        outString = "Fan high";
        goodCmd = true;
    };
    if (commandStr.compareTo("low") == 0) {          //fan low
        dataChange(112,143);
        outString = "Fan low";
        goodCmd = true;
    };
    if (commandStr.compareTo("cool") == 0) {         //cool mode
        dataChange(48,207);
        outString = "Cool mode";
        goodCmd = true;
    };
    if (commandStr.compareTo("fan") == 0) {          //fan mode
        dataChange(16,239);
        outString = "Fan mode";
        goodCmd = true;
    };

    Serial.print("\ndata1: ");                  //DEBUG
    Serial.println(dtaSend[BIT_DATA+2]);        //DEBUG
    Serial.print("\ndata2: ");                  //DEBUG
    Serial.println(dtaSend[BIT_DATA+3]);        //DEBUG

    if(goodCmd) {
        IR.Send(dtaSend,char(ir_freq));   // would send to A/C (if A/C were connected...)
        return "\n" + outString + "\nCommand sent\n\n";
    }
    else return outString;
};
