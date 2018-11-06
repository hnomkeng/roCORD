//
//  showmsg_testing.cpp
//  roCORD
//
//  Created by Norman Ziebal on 22.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#include "showmsg_testing.hpp"
#include <iostream>

void ShowMessage(const char *msg) {
    std::cout << "[Message]: " << msg << std::endl;
}
void ShowStatus(const char *msg) {
    std::cout << "[Status]: " << msg << std::endl;
}
void ShowInfo(const char *msg) {
    std::cout << "[Info]: " << msg << std::endl;
}
void ShowNotice(const char *msg) {
    std::cout << "[Notice]: " << msg << std::endl;
}
void ShowWarning(const char *msg) {
    std::cout << "[Warning]: " << msg << std::endl;
}
void ShowDebug(const char *msg) {
    std::cout << "[Debug]: " << msg << std::endl;
}
void ShowError(const char *msg) {
    std::cout << "[Error]: " << msg << std::endl;
}
void ShowFatalError(const char *msg) {
    std::cout << "[Message]: " << msg << std::endl;
}
