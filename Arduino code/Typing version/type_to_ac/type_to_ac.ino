#include <IRSendRevInt.h>
#include <IRSendRev.h>

#define BIT_LEN         0
#define BIT_START_H     1
#define BIT_START_L     2
#define BIT_DATA_H      3
#define BIT_DATA_L      4
#define BIT_DATA_LEN    5
#define BIT_DATA        6

const int ir_freq = 38;                 // 38k, drives loop function;

unsigned char dtaSend[20];

bool goodCmd = 0;                       //checks if command is supported

String printOut;

void dtaInit(int data1,int data2)
{
    dtaSend[BIT_LEN]        = 9;           // all data that needs to be sent
    dtaSend[BIT_START_H]    = 179;          // the logic high duration of "Start"
    dtaSend[BIT_START_L]    = 90;           // the logic low duration of "Start"
    dtaSend[BIT_DATA_H]     = 11;           // the logic "long" duration in the communication
    dtaSend[BIT_DATA_L]     = 33;           // the logic "short" duration in the communication

    dtaSend[BIT_DATA_LEN]   = 4;            // Number of data which will sent. If the number is other, you should increase or reduce dtaSend[BIT_DATA+x].

    dtaSend[BIT_DATA+0]     = 8;          // data that will sent
    dtaSend[BIT_DATA+1]     = 231;
    dtaSend[BIT_DATA+2]     = data1;
    dtaSend[BIT_DATA+3]     = data2;
    //dtaSend[BIT_DATA+4]     = 192;
    //dtaSend[BIT_DATA+5]     = 63;
}

void dataChange(int newSignal1,int newSignal2) {
  dtaSend[BIT_DATA+2] = newSignal1;
  dtaSend[BIT_DATA+3] = newSignal2;
}

void sendToAC(String commandStr) {
  if (commandStr.equals("timer")) {         //timer
    dataChange(128,127);
    printOut = "Timer Activated";
    goodCmd = 1;
  }
  
  if (commandStr.equals("power")) {         //power
    dataChange(0,255);
    printOut = "Cycling A/C Power";
    goodCmd = 1;
  }

  if (commandStr.equals("up")) {       //temp up
    dataChange(168,87);
    printOut = "Raising Temperature";
    goodCmd = 1;
  }

  if (commandStr.equals("down")) {     //temp down
    dataChange(176,79);
    printOut = "Lowering Temperature";
    goodCmd = 1;
  }

  if (commandStr.equals("high")) {         //fan high
    dataChange(104,151);
    printOut = "Fan High";
    goodCmd = 1;
  }

  if (commandStr.equals("low")) {         //fan low
    dataChange(112,143);
    printOut = "Fan Low";
    goodCmd = 1;
  }

  if (commandStr.equals("cool")) {         //cool mode
    dataChange(48,207);
    printOut = "Cool Mode";
    goodCmd = 1;
  }

  if (commandStr.equals("fan")) {         //fan mode
    dataChange(16,239);
    printOut = "Fan Mode";
    goodCmd = 1;
  }

  //Serial.print("data1: " + dtaSend[BIT_DATA+2]);              //DEBUG
  //Serial.println(", data2: " + dtaSend[BIT_DATA+3]);          //DEBUG
  
  //checks if commands are good before sending anything to A/C
  if (goodCmd) {
    IR.Send(dtaSend,char(ir_freq));
    Serial.println("Command sent");
    Serial.println(printOut);
    printOut = "";
  }
  else {
    Serial.println("Command not recognized.");
    Serial.println("Please check your spelling or try another command.");
  }
  
  return;
}

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(5000);
  dtaInit(0,0);
  Serial.println("Initialized");
  Serial.println("Reference Guide of Commands: \npower\ntemp up\ntemp down\nhigh\nlow\ncool\nfan");
}

void loop()
{
  Serial.print("\nEnter an A/C function: ");
  while(!Serial.available()){
  //do nothing till input comes in
  }
  goodCmd = 0;
  sendToAC(Serial.readStringUntil(10));
}
