//
//  main.cpp
//  send_to_AC
//
//  Created by Matt Gardner on 7/8/18.
//  Copyright © 2018 Matt Gardner. All rights reserved.
//

#include </Users/mattgardneradmin/Desktop/AC Project/IRSendRev-master/IRSendRev.h>
#include <string>
#include <iostream>

#define BIT_LEN         0
#define BIT_START_H     1
#define BIT_START_L     2
#define BIT_DATA_H      3
#define BIT_DATA_L      4
#define BIT_DATA_LEN    5
#define BIT_DATA        6

const int ir_freq = 38;                 // 38k, drives loop function;

unsigned char dtaSend[20];

std::string command;

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
};

void dataChange(int newSignal1,int newSignal2) {
    dtaSend[BIT_DATA+2] = newSignal1;
    dtaSend[BIT_DATA+3] = newSignal2;
};

void sendToAC(std::string commandStr) {
    if (commandStr.compare("power") == 0) {         //power
        dataChange(0,255);
        printf("\nCycling A/C Power");
        //IR.Send(dtaSend,char(ir_freq));
    }

    if (commandStr.compare("up") == 0) {       //temp up
        dataChange(168,87);
    }

    if (commandStr.compare("down") == 0) {     //temp down
        dataChange(176,79);
    }

    if (commandStr.compare("high") == 0) {         //fan high
        dataChange(104,151);
    }

    if (commandStr.compare("low") == 0) {         //fan low
        dataChange(112,143);
    }

    if (commandStr.compare("cool") == 0) {         //cool mode
        dataChange(48,207);
    }

    if (commandStr.compare("fan") == 0) {         //fan mode
        dataChange(16,239);
    }

    printf("\ndata1: %d",dtaSend[BIT_DATA+2]);
    printf(", data2: %d",dtaSend[BIT_DATA+3]);
    printf("\nCommand sent\n\n");
    return;

    //IR.Send(dtaSend,char(ir_freq));
};

int main() {
    while (command.compare("exit") != 0) {
        printf("Enter an A/C function: ");
        std::cin >> command;
        sendToAC(command);
    };
    printf("Exiting program\n");
    return 0;
};
