# CPU Scheduling
- CPU scheduling is a fundamental concept in operating systems that determines which process in the ready queue is to be allocated the CPU for execution. The main goal of CPU scheduling is to optimize CPU utilization and system responsiveness. Efficient CPU scheduling can significantly improve the performance of the system.

![2024-06-20 (10)](https://github.com/GarvJuneja977/CPU-Schedular/assets/172182458/afae7bd7-27f4-4258-9dd7-90024ccf4b45)


# Process Scheduling Simulator
This project is a Process Scheduling Simulator written in C++. It implements various CPU scheduling algorithms, including First-Come, First-Serve (FCFS), Shortest Remaining Time First (SRTF), Priority Scheduling, and Round Robin (RR). The simulator can automatically choose the most suitable algorithm based on the burst times of the processes.

## Features
- First-Come, First-Serve (FCFS): Processes are executed in the order they arrive.
- Shortest Remaining Time First (SRTF): The process with the shortest remaining burst time is executed next.
- Priority Scheduling: Processes with higher priority are executed first.
- Round Robin (RR): Each process is assigned a fixed time slot in a cyclic order.
- Auto Schedule: Automatically chooses the most suitable algorithm based on the standard deviation of burst times.

![2024-06-20 (9)](https://github.com/GarvJuneja977/CPU-Schedular/assets/172182458/cb112cbe-4fe3-423c-acd4-c54fc79a7a51)



## Getting Started
### Prerequisites
To compile and run this program, you need a C++ compiler. If you don't have one installed, you can use GCC, which is commonly available on Unix-like systems.
- 1.) Clone the Repository:
  ```
  git clone https://github.com/GarvJuneja977/CPU-Schedular.git
  cd CPU-Schedular 
- 2.) Compile the Program:
  ```
  g++ main-program.cpp -o main-program
- 3.) Run the Program:
  ```
  ./main-program
- 4.) Input:
  - The program prompts the user to input the number of processes and details for each process:
    - Arrival Time: The time at which the process arrives.
    - Burst Time: The total time required by the process.
    - The priority of the process (lower number indicates higher priority).
  - After entering the process details, the user is prompted to select the scheduling algorithm:
    - FCFS: First Come First Serve
    - SRTF: Shortest Remaining Time First
    - Priority: Priority Scheduling
    - RR: Round Robin
    - auto: Automatically selects the scheduling algorithm
   

# Detailed Explanation
## Data Structures
- Process: Represents a process with its properties.
- GanttChartEntry: Represents an entry in the Gantt chart.

## Functions
- calculateTimesFCFS: Implements the FCFS scheduling algorithm.
- calculateTimesSRTF: Implements the SRTF scheduling algorithm.
- calculateTimesPriority: Implements the Priority Scheduling algorithm.
- calculateTimesRR: Implements the Round Robin scheduling algorithm.
- auto_schedule: Automatically selects the scheduling algorithm.
- displayProcesses: Displays the details of each process after scheduling.
- displayGanttChart: Displays the Gantt chart.
- sorted: Sorts processes based on arrival time and priority.
- scheduler: Main function to select and execute the scheduling algorithm

# Example Usage
## Input
```
Enter the number of processes: 3
Enter arrival time, burst time, and priority for process 1: 0 5 2
Enter arrival time, burst time, and priority for process 2: 1 3 1
Enter arrival time, burst time, and priority for process 3: 2 8 3

Select the Algorithm
Type 'FCFS' for First Come First Serve
Type 'SRTF' for Shortest Remaining Time First
Type 'Priority' for Priority Scheduling
Type 'RR' for Round Robin
Type 'auto' for Auto Schedule

Type your Algorithm : FCFS
```

## Output
```
PID Arrival Time Burst Time Priority Waiting Time Turnaround Time Response Time Completion Time
1   0            5          2        0             5             0             5
2   1            3          1        4             7             4             8
3   2            8          3        6             14            6             16

Average Waiting Time: 3.33333
Average Turnaround Time: 8.66667

Gantt Chart:
 -------------------
|  P1  |  P2  |  P3  |
 -------------------
0      5      8     16
```
## License
- This project is licensed under the MIT License.
