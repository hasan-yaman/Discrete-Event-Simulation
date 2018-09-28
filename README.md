# Project Overview
There are two different models in Starbucks. Each model has N cashiers and N/3 baristas(Assumed that N is divisible by 3). First model has one queue for cashiers and one queue for baristas([Figure 1](https://raw.githubusercontent.com/hasan-yaman/Discrete-Event-Simulation/master/fig1.png)). The other model has one queue for cashiers and N/3 queues for baristas. First 3 cashiers send the orders to the first barista and next 3 cashiers send the orders to the second barista and goes on like that([Figure 2](https://raw.githubusercontent.com/hasan-yaman/Discrete-Event-Simulation/master/fig2.png)). In both systems, cashier queue works by first come first serve design and barista queue works by most expensive coffee served first.
I simulated these two different models and collect the following statistics for each model:
- Total running time of the system
- Utilization of each cashier and barista 
- Turnaround time of each order
- Maximum length of each queue

