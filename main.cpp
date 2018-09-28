//
// Created by Hasan Yaman on 17.11.2017.
//

#include <iostream>
#include <vector>
#include <queue>
#include "Order.h"
#include "Cashier.h"
#include "Barista.h"
#include <complex>
#include <iomanip>
#include <ctime>
#include "Time.h"
#include <set>

using namespace std;

struct CompareOrders {
    bool operator()(Order const &o1, Order const &o2) {
        return o1.price < o2.price;
    }
};

struct CompareTimes {
    bool operator()(const Time &time1, const Time &time2) const {
        return time1.time > time2.time;
    }
};


int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }


    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");

    //process first line

    int number_of_cashiers;
    fscanf(input_file, "%i", &number_of_cashiers);

    int number_of_baristas = number_of_cashiers / 3;

    //process second line

    int number_of_orders;
    fscanf(input_file, "%i", &number_of_orders);

    //process orders

    vector<Order> orders;

    Cashier cashiers[number_of_cashiers];
    Barista baristas[number_of_baristas];

    for (int i = 0; i < number_of_orders; ++i) {
        double arrival_time;
        double order_time;
        double brew_time;
        double price;
        fscanf(input_file, "%lf", &arrival_time);
        fscanf(input_file, "%lf", &order_time);
        fscanf(input_file, "%lf", &brew_time);
        fscanf(input_file, "%lf", &price);

        Order o(arrival_time, order_time, brew_time, price, i);
        orders.push_back(o);
    }

    queue<Order> cashier_queue;

    priority_queue<Order, vector<Order>, CompareOrders> barista_queue;

    priority_queue<Time, vector<Time>, CompareTimes> next_check_time;
    priority_queue<Time, vector<Time>, CompareTimes> second_model_next_check_time;

    // set next check times

    for (int i = 0; i < number_of_orders; ++i) {
        Time t(orders.at(i).arrival_time);
        t.isArrival = true;
        next_check_time.push(t);
        second_model_next_check_time.push(t);
    }


    int order_index = 0;

    int max_length_cashier_queue = 0;
    int max_length_barista_queue = 0;

    priority_queue<int, vector<int>, greater<int> > free_cashier_indexes;
    priority_queue<int, vector<int>, greater<int> > free_barista_indexes;

    // set free cashier and barista indexes

    for (int i = 0; i < number_of_cashiers; ++i) {
        free_cashier_indexes.push(i);
    }
    for (int i = 0; i < number_of_baristas; ++i) {
        free_barista_indexes.push(i);
    }

    double total_running_time = 0; // to hold total time for first model

    // first model

    while (!next_check_time.empty()) {
        Time time = next_check_time.top();
        next_check_time.pop();
        total_running_time = time.time;

        double current_time = time.time;
        bool isArrival = time.isArrival;
        bool isOrderDone = time.isOrderDone;
        bool isBrewDone = time.isBrewDone;


        if (isArrival) {
            // if time is arrival time, add order which has arrival time equals current time to cashier queue

            Order o = orders.at(order_index);
            cashier_queue.push(o);
            ++order_index;

            if (!free_cashier_indexes.empty()) {
                // if there is free cashier

                // find free cashier index
                int index_cashier = free_cashier_indexes.top();
                free_cashier_indexes.pop();

                Order o2 = cashier_queue.front();
                cashier_queue.pop();

                Cashier free_cashier = cashiers[index_cashier];
                free_cashier.current_order_index = o2.index;
                free_cashier.busy_time += o2.order_time;
                cashiers[index_cashier] = free_cashier;

                // add next check time
                Time nextTime(current_time + o2.order_time);
                nextTime.isOrderDone = true;
                nextTime.cashier_index = index_cashier;
                next_check_time.push(nextTime);
            }

        } else if (isOrderDone) {
            // if time is order done time

            int index_cashier = time.cashier_index;
            free_cashier_indexes.push(index_cashier); // cashier is available
            int current_order_index = cashiers[index_cashier].current_order_index;
            Order o = orders.at(current_order_index);
            barista_queue.push(o);  // and order goes to barista queue...

            // check for  free barista
            if (!free_barista_indexes.empty()) {
                int index_barista = free_barista_indexes.top();
                free_barista_indexes.pop();

                Order o2 = barista_queue.top();
                barista_queue.pop();

                // and order goes to barista...

                Barista free_barista = baristas[index_barista];
                free_barista.busy_time += o2.brew_time;
                free_barista.current_order_index = current_order_index;
                baristas[index_barista] = free_barista;

                Time nextTime(current_time + o2.brew_time);
                nextTime.isBrewDone = true;
                nextTime.barista_index = index_barista;
                next_check_time.push(nextTime);
            }

            if (!cashier_queue.empty()) {
                // we have a free cashier, why we do not process a next order?
                Order o2 = cashier_queue.front();
                cashier_queue.pop();

                int index_cashier2 = free_cashier_indexes.top();
                free_cashier_indexes.pop();

                Cashier free_cashier = cashiers[index_cashier2];
                free_cashier.current_order_index = o2.index;
                free_cashier.busy_time += o2.order_time;
                cashiers[index_cashier2] = free_cashier;

                Time t1(current_time + o2.order_time);
                t1.isOrderDone = true;
                t1.cashier_index = index_cashier2;
                next_check_time.push(t1);
            }
        } else if (isBrewDone) {
            // if time is brew done time
            // so we have free barista
            int index_barista = time.barista_index;
            free_barista_indexes.push(index_barista);
            int current_order_index = baristas[index_barista].current_order_index;
            Order o = orders.at(current_order_index);
            o.done_time = current_time;
            orders.at(current_order_index) = o;

            if (!barista_queue.empty()) {

                // we know we have a free barista, so process a next order!
                Order o2 = barista_queue.top();
                barista_queue.pop();

                int index_barista2 = free_barista_indexes.top();
                free_barista_indexes.pop();

                Barista free_barista = baristas[index_barista2];
                free_barista.busy_time += o2.brew_time;
                free_barista.current_order_index = o2.index;
                baristas[index_barista2] = free_barista;

                Time t1(current_time + o2.brew_time);
                t1.isBrewDone = true;
                t1.barista_index = index_barista2;
                next_check_time.push(t1);
            }
        }

        // check cashier and barista queues size to find their values when thet have max. values
        if (cashier_queue.size() > max_length_cashier_queue) {
            max_length_cashier_queue = cashier_queue.size();
        }

        if (barista_queue.size() > max_length_barista_queue) {
            max_length_barista_queue = barista_queue.size();
        }

    }


    // print results to output file

    fprintf(output_file, "%.2lf", total_running_time);
    fprintf(output_file, "\n");
    fprintf(output_file, "%i", max_length_cashier_queue);
    fprintf(output_file, "\n");
    fprintf(output_file, "%i", max_length_barista_queue);
    fprintf(output_file, "\n");


    for (int i = 0; i < number_of_cashiers; ++i) {
        double utilization = cashiers[i].busy_time / total_running_time;
        cashiers[i].busy_time = 0; // set zero for second model
        fprintf(output_file, "%.2lf", utilization);
        fprintf(output_file, "\n");
    }
    for (int i = 0; i < number_of_baristas; ++i) {
        double utilization = baristas[i].busy_time / total_running_time;
        baristas[i].busy_time = 0; // set zero for second model
        fprintf(output_file, "%.2lf", utilization);
        fprintf(output_file, "\n");
    }
    for (int i = 0; i < number_of_orders; ++i) {
        double turnaround_time = orders.at(i).done_time - orders.at(i).arrival_time;
        fprintf(output_file, "%.2lf", turnaround_time);
        fprintf(output_file, "\n");
    }


    queue<Order> second_model_cashier_queue;

    priority_queue<Order, vector<Order>, CompareOrders> second_model_barista_queues[number_of_baristas];

    priority_queue<int, vector<int>, greater<int> > second_model_free_cashier_indexes;

    set<int> second_model_free_barista_indexes;

    // set free cashier and barista indexes for second model

    for (int i = 0; i < number_of_cashiers; ++i) {
        second_model_free_cashier_indexes.push(i);
    }
    for (int i = 0; i < number_of_baristas; ++i) {
        second_model_free_barista_indexes.insert(i);
    }


    int second_model_max_length_cashier_queue = 0;

    int second_model_order_index = 0;

    int second_model_max_lengths_barista_queues[number_of_baristas];

    // set array with full of zeros
    fill(second_model_max_lengths_barista_queues, second_model_max_lengths_barista_queues + number_of_baristas, 0);

    double second_model_total_running_time = 0;

    // second model

    while (!second_model_next_check_time.empty()) {
        Time time = second_model_next_check_time.top();
        second_model_next_check_time.pop();
        second_model_total_running_time = time.time;

        double current_time = time.time;
        bool isArrival = time.isArrival;
        bool isOrderDone = time.isOrderDone;
        bool isBrewDone = time.isBrewDone;

        if(isArrival) {

            // when time is arrival time, add order to cashier queue and if there is free
            // cashier, then order goes to this free cashier

            Order o = orders.at(second_model_order_index);
            second_model_cashier_queue.push(o);
            ++second_model_order_index;

            if (!second_model_free_cashier_indexes.empty()) {
                int index_cashier = second_model_free_cashier_indexes.top();
                second_model_free_cashier_indexes.pop();

                Order o2 = second_model_cashier_queue.front();
                second_model_cashier_queue.pop();

                Cashier free_cashier = cashiers[index_cashier];
                free_cashier.current_order_index = o2.index;
                free_cashier.busy_time += o2.order_time;
                cashiers[index_cashier] = free_cashier;


                Time nextTime(current_time + o2.order_time);
                nextTime.isOrderDone = true;
                nextTime.cashier_index = index_cashier;
                second_model_next_check_time.push(nextTime);

            }
        } else if(isOrderDone) {

            // when time is order done time, add order to barista queue

            int index_cashier = time.cashier_index;
            second_model_free_cashier_indexes.push(index_cashier);
            int current_order_index = cashiers[index_cashier].current_order_index;
            Order o = orders.at(current_order_index);  // burada işi biten order
            int current_barista_index = (index_cashier / 3);
            priority_queue<Order, vector<Order>, CompareOrders> current_barista_queue = second_model_barista_queues[current_barista_index];
            current_barista_queue.push(o);


            if(second_model_free_barista_indexes.count(current_barista_index)) {
               // if there is free barista for this order, then go!

                Order o2 = current_barista_queue.top();
                current_barista_queue.pop();

                Barista free_barista = baristas[current_barista_index];
                free_barista.busy_time += o2.brew_time;
                free_barista.current_order_index = o2.index;
                baristas[current_barista_index] = free_barista;
                second_model_free_barista_indexes.erase(current_barista_index);

                Time nextTime(current_time + o2.brew_time);
                nextTime.isBrewDone = true;
                nextTime.barista_index = current_barista_index;
                second_model_next_check_time.push(nextTime);
            }

            // check for max size!
            if(current_barista_queue.size() > second_model_max_lengths_barista_queues[current_barista_index]) {
                second_model_max_lengths_barista_queues[current_barista_index] = current_barista_queue.size();
            }

            second_model_barista_queues[current_barista_index] = current_barista_queue;


            if (!second_model_cashier_queue.empty()) {
                // if we have order in cashier queue , then process it because now we have free cashier

                Order o2 = second_model_cashier_queue.front();
                second_model_cashier_queue.pop();

                int index_cashier2 = second_model_free_cashier_indexes.top();
                second_model_free_cashier_indexes.pop();

                Cashier free_cashier = cashiers[index_cashier2];
                free_cashier.current_order_index = o2.index;
                free_cashier.busy_time += o2.order_time;
                cashiers[index_cashier2] = free_cashier;

                Time t1(current_time + o2.order_time);
                t1.isOrderDone = true;
                t1.cashier_index = index_cashier2;
                second_model_next_check_time.push(t1);
            }
        } else if(isBrewDone) {
            // if time is brew done time, then check if there is order in barista queue
            // then you can process order

            int index_barista = time.barista_index;
            second_model_free_barista_indexes.insert(index_barista);

            int current_order_index = baristas[index_barista].current_order_index;

            Order o = orders.at(current_order_index);
            o.done_time = current_time;
            orders.at(current_order_index) = o;

            priority_queue<Order, vector<Order>, CompareOrders> current_barista_queue = second_model_barista_queues[index_barista];
            if(!current_barista_queue.empty()) {
                Order o2 = current_barista_queue.top();
                current_barista_queue.pop();

                second_model_barista_queues[index_barista] = current_barista_queue;

                Barista free_barista = baristas[index_barista];
                free_barista.busy_time += o2.brew_time;
                free_barista.current_order_index = o2.index;
                baristas[index_barista] = free_barista;
                second_model_free_barista_indexes.erase(index_barista);


                Time t1(current_time + o2.brew_time);
                t1.isBrewDone = true;
                t1.barista_index = index_barista;
                second_model_next_check_time.push(t1);
            }

            // check for max. size
            if(current_barista_queue.size() > second_model_max_lengths_barista_queues[index_barista]) {
                second_model_max_lengths_barista_queues[index_barista] = current_barista_queue.size();
            }
        }

        // check for max.size
        if(second_model_cashier_queue.size() > second_model_max_length_cashier_queue) {
            second_model_max_length_cashier_queue = second_model_cashier_queue.size();
        }

    }

    // print results to second file!

    fprintf(output_file, "\n");
    fprintf(output_file, "%.2lf", second_model_total_running_time);
    fprintf(output_file, "\n");
    fprintf(output_file, "%i", second_model_max_length_cashier_queue);
    fprintf(output_file, "\n");
    for (int i = 0; i < number_of_baristas; ++i) {
        fprintf(output_file, "%i", second_model_max_lengths_barista_queues[i]);
        fprintf(output_file, "\n");
    }

    for (int i = 0; i < number_of_cashiers; ++i) {
        double utilization = cashiers[i].busy_time / second_model_total_running_time;
        fprintf(output_file, "%.2lf", utilization);
        fprintf(output_file, "\n");
    }
    for (int i = 0; i < number_of_baristas; ++i) {
        double utilization = baristas[i].busy_time / second_model_total_running_time;
        fprintf(output_file, "%.2lf", utilization);
        fprintf(output_file, "\n");
    }
    for (int i = 0; i < number_of_orders; ++i) {
        double turnaround_time = orders.at(i).done_time - orders.at(i).arrival_time;
        fprintf(output_file, "%.2lf", turnaround_time);
        fprintf(output_file, "\n");
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}