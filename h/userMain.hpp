//
// Created by os on 9/3/23.
//

#ifndef PROJECT_BASE_V1_1_USERMAIN_HPP
#define PROJECT_BASE_V1_1_USERMAIN_HPP

void userMain();

void wrapperUserMain(void*) {
    userMain();
}

#endif //PROJECT_BASE_V1_1_USERMAIN_HPP
