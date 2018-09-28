# Project Overview
There are two different models in Starbucks. Each model has N cashiers and N/3 baristas(Assumed that N is divisible by 3). The first model has one queue for cashiers and one queue for baristas([Figure 1](https://raw.githubusercontent.com/hasan-yaman/Discrete-Event-Simulation/master/fig1.png)). The other model has one queue for cashiers and N/3 queues for baristas. First 3 cashiers send the orders to the first barista and next 3 cashiers send the orders to the second barista and goes on like that([Figure 2](https://raw.githubusercontent.com/hasan-yaman/Discrete-Event-Simulation/master/fig2.png)). In both systems, cashier queue works by first come first serve design and barista queue works by most expensive coffee served first.
I simulated these two different models and collect the following statistics for each model:
- Total running time of the system
- Utilization of each cashier and barista 
- Turnaround time of each order
- The maximum length of each queue

# Input/Output File Format
The input file has the following format:
- The first line is a number of cashiers (N) which is always divisible by 3.
- The second line is the number of orders (M).
- Following M lines contain information about orders. There are 4 variables separated with space. They are:
  - Arrival Time: The time when a customer enters the coffee shop. (in seconds)
  - Order Time: The time required to give a coffee order. (in seconds)
  - Brew Time: The time required to make coffee. (in seconds)
  - Price of Order: Price of the coffee.

The output file has the following format:
- The first line is the total running time of the coffee shop. (in seconds)
- Next line is the maximum length of the cashier queue.
- If the coffee shop has one barista queue, the next line is the maximum length of this queue.
- If the coffee shop has N/3 barista queues, next N/3 lines are maximum lengths of each barista queue.
- Next N lines are the utilization of the cashiers. 5. Next N/3 lines are the utilization of the baristas. 6. Next M lines are turnaround times of orders.
I print these values for first and second models, respectively. 

# Implementation Details
- Whenever a cashier or barista finishes his/her job, he/she immediately fetch an order waiting in the corresponding queue. If the queue is empty, he/she goes idle.
- If more than one cashier is available, the cashier with the smallest ID takes the order. For example, if cashier1 and cashier3 are available at the same time, the next order will be taken by cashier1. This principle is the same with baristas, too.
- Unit Utilization = Busy Time of the Unit / Total Running Time of The Coffee Shop
- Turnaround Time = Time when the coffee is done−Arrival time of the customer
- Total Running Time = Time when all coffee orders are done
