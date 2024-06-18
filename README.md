# CPU Scheduling Algorithms in C++
This project implements four different CPU scheduling algorithms in C++: First-Come, First-Served (FCFS), Shortest Job First (SJF), Round Robin (RR), and Priority Scheduling. The code calculates and compares the average waiting time for each algorithm and determines the most efficient algorithm based on the minimum average waiting time.

## Description
The program takes the number of processes and their respective arrival times, burst times, and priorities as input from the user. It then applies the four scheduling algorithms to the processes and computes the waiting time and turnaround time for each process under each algorithm. Finally, it compares the average waiting times of the algorithms and outputs the most efficient algorithm.

## Scheduling Algorithms
1.)First-Come, First-Served (FCFS):

  Processes are scheduled in the order they arrive in the ready queue.
  Non-preemptive scheduling.
2.)Shortest Job First (SJF):

Processes with the shortest burst time are scheduled first.
Non-preemptive scheduling.
3.)Round Robin (RR):

Each process gets executed for a fixed time quantum in a cyclic order.
Preemptive scheduling.
4.)Priority Scheduling:

Processes are scheduled based on their priority.
Non-preemptive scheduling.
