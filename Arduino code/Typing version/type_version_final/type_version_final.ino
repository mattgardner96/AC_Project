/*
Matt Gardner, 7/25/18
A/C sender sketch(main code)
->Type-to-send version (no web connectivity)
Adapted from send.ino sketch, an example code for Grove IR Emitter/Receiver
IRSendRevMaster library, created by SEEEDStudio and used under MIT License.
*/
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

const int CMNDNUM = 7;                // number of possible commands, for array implementation

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(5000);
  while(!Serial);
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
  Serial.println(sendToAC(Serial.readStringUntil(10)));
  Serial.println(dtaSend[BIT_DATA+2]);
  Serial.println(dtaSend[BIT_DATA+3]);
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
// Uses array traversal to send commands to the arduino
String sendToAC(String commandStr) {
  
    String commands[CMNDNUM] = {"power","up","down","high","low","cool","fan"};

    int cmndData[CMNDNUM*2] = {0,255,168,87,176,79,104,151,112,143,48,207,16,239};

    String outStrings[CMNDNUM] = {"Cycling A/C Power","Turning up temp","Turning down temp","Fan high","Fan low","Cool mode","Fan mode"};

    for (int i = 0; i < CMNDNUM; i++) { 
        if(commandStr == commands[i]) {
            dataChange(cmndData[2*i],cmndData[2*i+1]);
            IR.Send(dtaSend,char(ir_freq));   // send to A/C (if IR Emitter is connected...)
            return "\n" + outStrings[i] + "\nCommand sent\n\n";
        }
    }
    
    return "Bad command";
};
