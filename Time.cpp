//
// Created by Hasan Yaman on 18.11.2017.
//

#include "Time.h"

Time::Time(double time) {
    this->time = time;
    this->isArrival = false;
    this->isOrderDone = false;
    this->isBrewDone = false;
    this->cashier_index = -1;
    this->barista_index = -1;
}
