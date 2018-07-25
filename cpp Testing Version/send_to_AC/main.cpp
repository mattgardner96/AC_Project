//
//  main.cpp
//  send_to_AC
//
//  Created by Matt Gardner on 7/8/18.
//  Copyright © 2018 Matt Gardner. All rights reserved.
//

#include </Users/mattgardneradmin/Desktop/AC_Project/IRSendRev-master/IRSendRev.h>
#include <string>
#include <iostream>
#include "Sender.hpp"
#include "Sender.cpp"

using namespace std;

string command,output;

Sender send;

int main() {
    while (command.compare("exit") != 0) {
        printf("Enter an A/C function: ");
        std::cin >> command;
        std::cout << send.sendToAC(command) << "\n";
    };
    printf("Exiting program\n");
    return 0;
}
