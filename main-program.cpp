#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
};

// Function to compare processes by arrival time
bool compareByArrival(Process a, Process b) {
    return a.arrivalTime < b.arrivalTime;
}

// Function to calculate waiting time and turnaround time for FCFS
void calculateFCFS(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0;
    for (int i = 0; i < n; ++i) {
        processes[i].waitingTime = max(0, currentTime - processes[i].arrivalTime);
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
        currentTime += processes[i].burstTime;
    }
}

// Function to calculate waiting time and turnaround time for SJF
void calculateSJF(vector<Process>& processes) {
    int n = processes.size();
    vector<bool> completed(n, false);
    int completedCount = 0, currentTime = 0;

    while (completedCount < n) {
        int minBurstTime = INT_MAX, shortestProcess = -1;
        for (int i = 0; i < n; ++i) {
            if (!completed[i] && processes[i].arrivalTime <= currentTime && processes[i].burstTime < minBurstTime) {
                minBurstTime = processes[i].burstTime;
                shortestProcess = i;
            }
        }
        if (shortestProcess == -1) {
            ++currentTime;
            continue;
        }
        processes[shortestProcess].waitingTime = currentTime - processes[shortestProcess].arrivalTime;
        processes[shortestProcess].turnaroundTime = processes[shortestProcess].waitingTime + processes[shortestProcess].burstTime;
        currentTime += processes[shortestProcess].burstTime;
        completed[shortestProcess] = true;
        ++completedCount;
    }
}

// Function to calculate waiting time and turnaround time for Round Robin
void calculateRR(vector<Process>& processes, int quantum) {
    int n = processes.size();
    queue<int> q;
    vector<int> remainingBurstTime(n);
    for (int i = 0; i < n; ++i) {
        remainingBurstTime[i] = processes[i].burstTime;
    }
    int currentTime = 0, index = 0;

    while (index < n || !q.empty()) {
        while (index < n && processes[index].arrivalTime <= currentTime) {
            q.push(index);
            ++index;
        }
        if (q.empty()) {
            ++currentTime;
            continue;
        }
        int currentProcess = q.front();
        q.pop();
        int timeSlice = min(quantum, remainingBurstTime[currentProcess]);
        remainingBurstTime[currentProcess] -= timeSlice;
        currentTime += timeSlice;

        if (remainingBurstTime[currentProcess] == 0) {
            processes[currentProcess].waitingTime = currentTime - processes[currentProcess].arrivalTime - processes[currentProcess].burstTime;
            processes[currentProcess].turnaroundTime = processes[currentProcess].waitingTime + processes[currentProcess].burstTime;
        } else {
            while (index < n && processes[index].arrivalTime <= currentTime) {
                q.push(index);
                ++index;
            }
            q.push(currentProcess);
        }
    }
}

// Function to calculate waiting time and turnaround time for Priority Scheduling
void calculatePriority(vector<Process>& processes) {
    int n = processes.size();
    vector<bool> completed(n, false);
    int completedCount = 0, currentTime = 0;

    while (completedCount < n) {
        int highestPriority = INT_MAX, priorityProcess = -1;
        for (int i = 0; i < n; ++i) {
            if (!completed[i] && processes[i].arrivalTime <= currentTime && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                priorityProcess = i;
            }
        }
        if (priorityProcess == -1) {
            ++currentTime;
            continue;
        }
        processes[priorityProcess].waitingTime = currentTime - processes[priorityProcess].arrivalTime;
        processes[priorityProcess].turnaroundTime = processes[priorityProcess].waitingTime + processes[priorityProcess].burstTime;
        currentTime += processes[priorityProcess].burstTime;
        completed[priorityProcess] = true;
        ++completedCount;
    }
}

// Function to calculate and return average waiting time
double calculateAverageWaitingTime(const vector<Process>& processes) {
    int totalWaitingTime = 0;
    for (auto& p : processes) {
        totalWaitingTime += p.waitingTime;
    }
    return (double)totalWaitingTime / processes.size();
}

// Function to print process info
void printProcessInfo(const vector<Process>& processes, const string& algorithm, double avgWaitingTime) {
    cout << "\nProcess info for " << algorithm << ":\n";
    for (const auto& p : processes) {
        cout << "Process " << p.id << ": Waiting Time = " << p.waitingTime << ", Turnaround Time = " << p.turnaroundTime << endl;
    }
    cout << "Average Waiting Time for " << algorithm << ": " << avgWaitingTime << endl;
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i].id = i + 1;
        cout << "Enter arrival time, burst time, and priority for process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
    }

    vector<Process> fcfsProcesses = processes;
    vector<Process> sjfProcesses = processes;
    vector<Process> rrProcesses = processes;
    vector<Process> priorityProcesses = processes;

    // Calculate average waiting time for each algorithm
    sort(fcfsProcesses.begin(), fcfsProcesses.end(), compareByArrival);
    calculateFCFS(fcfsProcesses);
    double avgWaitingTimeFCFS = calculateAverageWaitingTime(fcfsProcesses);
    printProcessInfo(fcfsProcesses, "First-Come, First-Served (FCFS)", avgWaitingTimeFCFS);

    sort(sjfProcesses.begin(), sjfProcesses.end(), compareByArrival);
    calculateSJF(sjfProcesses);
    double avgWaitingTimeSJF = calculateAverageWaitingTime(sjfProcesses);
    printProcessInfo(sjfProcesses, "Shortest Job First (SJF)", avgWaitingTimeSJF);

    int quantum;
    cout << "\nEnter the time quantum for Round Robin: ";
    cin >> quantum;
    sort(rrProcesses.begin(), rrProcesses.end(), compareByArrival);
    calculateRR(rrProcesses, quantum);
    double avgWaitingTimeRR = calculateAverageWaitingTime(rrProcesses);
    printProcessInfo(rrProcesses, "Round Robin (RR)", avgWaitingTimeRR);

    sort(priorityProcesses.begin(), priorityProcesses.end(), compareByArrival);
    calculatePriority(priorityProcesses);
    double avgWaitingTimePriority = calculateAverageWaitingTime(priorityProcesses);
    printProcessInfo(priorityProcesses, "Priority Scheduling", avgWaitingTimePriority);

    // Determine the algorithm with minimum average waiting time
    string minAlgorithm;
    double minAvgWaitingTime = min({avgWaitingTimeFCFS, avgWaitingTimeSJF, avgWaitingTimeRR, avgWaitingTimePriority});
    if (minAvgWaitingTime == avgWaitingTimeFCFS) {
        minAlgorithm = "First-Come, First-Served (FCFS)";
    } else if (minAvgWaitingTime == avgWaitingTimeSJF) {
        minAlgorithm = "Shortest Job First (SJF)";
    } else if (minAvgWaitingTime == avgWaitingTimeRR) {
        minAlgorithm = "Round Robin (RR)";
    } else if (minAvgWaitingTime == avgWaitingTimePriority) {
        minAlgorithm = "Priority Scheduling";
    }

    // Output results
    cout << "\nSummary of Average Waiting Times:" << endl;
    cout << "FCFS: " << avgWaitingTimeFCFS << endl;
    cout << "SJF: " << avgWaitingTimeSJF << endl;
    cout << "Round Robin: " << avgWaitingTimeRR << endl;
    cout << "Priority Scheduling: " << avgWaitingTimePriority << endl;
    cout << "\nMost Efficient Algorithm: " << minAlgorithm << " with Average Waiting Time: " << minAvgWaitingTime << endl;

    return 0;
}
