//
//  Sender.hpp
//  send_to_AC
//
//  Created by Matt Gardner on 7/24/18.
//  Copyright © 2018 Matt Gardner. All rights reserved.
//

#ifndef Sender_hpp
#define Sender_hpp

#include <stdio.h>
#include <string>

using namespace std;

class Sender {
public:
    int ir_freq;
    void dtaInit(int data1,int data2);
    void dataChange(int data1,int data2);
    string sendToAC(string commandStr);
};

#endif /* sender_hpp */
