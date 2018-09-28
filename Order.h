//
// Created by Hasan Yaman on 16.11.2017.
//

#ifndef PROJECT2_HASAN_YAMAN_2015400207_MASTER_ORDER_H
#define PROJECT2_HASAN_YAMAN_2015400207_MASTER_ORDER_H


class Order {
    public:
        double arrival_time;
        double order_time;
        double brew_time;
        double price;
        int index;
        double done_time;

        Order(double arrival_time, double order_time, double brew_time, double price, int index);
};


#endif //PROJECT2_HASAN_YAMAN_2015400207_MASTER_ORDER_H
