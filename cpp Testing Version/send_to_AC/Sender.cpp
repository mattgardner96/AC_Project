//
//  Sender.cpp
//  send_to_AC
//
//  Created by Matt Gardner on 7/24/18.
//  Copyright © 2018 Matt Gardner. All rights reserved.
//

#include "Sender.hpp"
#include "/Users/mattgardneradmin/Desktop/AC_Project/IRSendRev-master/IRSendRev.h"
#include <string>
#include <iostream>

using namespace std;

#define BIT_LEN         0
#define BIT_START_H     1
#define BIT_START_L     2
#define BIT_DATA_H      3
#define BIT_DATA_L      4
#define BIT_DATA_LEN    5
#define BIT_DATA        6

const int ir_freq = 38;                 // 38k, drives loop function;

unsigned char dtaSend[20];

void Sender::dtaInit(int data1,int data2)
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
    //dtaSend[BIT_DATA+4]     = 192;      // data is only 4 values
    //dtaSend[BIT_DATA+5]     = 63;       // data is only 4 values
};

void Sender::dataChange(int data1,int data2) {
    dtaSend[BIT_DATA+2] = data1;
    dtaSend[BIT_DATA+3] = data2;
};

string Sender::sendToAC(string commandStr) {

    string outString = "Command not recognized";

    if (commandStr.compare("power") == 0) {         //power
        dataChange(0,255);
        outString = "Cycling A/C Power";
    }

    if (commandStr.compare("up") == 0) {       //temp up
        dataChange(168,87);
        outString = "Turning up temp";
    }

    if (commandStr.compare("down") == 0) {     //temp down
        dataChange(176,79);
        outString = "Turning down temp";
    }

    if (commandStr.compare("high") == 0) {         //fan high
        dataChange(104,151);
        outString = "Fan high";
    }

    if (commandStr.compare("low") == 0) {         //fan low
        dataChange(112,143);
        outString = "Fan low";
    }

    if (commandStr.compare("cool") == 0) {         //cool mode
        dataChange(48,207);
        outString = "Cool mode";
    }

    if (commandStr.compare("fan") == 0) {         //fan mode
        dataChange(16,239);
        outString = "Fan mode";
    }

    printf("\ndata1: %d",dtaSend[BIT_DATA+2]);      //DEBUG
    printf(", data2: %d",dtaSend[BIT_DATA+3]);      //DEBUG

    //IR.Send(dtaSend,char(ir_freq));   // would send to A/C (if A/C were connected...)
    return "\n" + outString + "\nCommand sent\n\n";
};
