//
// Created by Hasan Yaman on 16.11.2017.
//

#include "Order.h"

Order::Order(double arrival_time, double order_time, double brew_time, double price, int index) {
    this->arrival_time = arrival_time;
    this->order_time = order_time;
    this->brew_time = brew_time;
    this->price = price;
    this->index = index;
    this->done_time = 0;
}

