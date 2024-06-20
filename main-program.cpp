#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int priority;
    int response_time;
    int start_time;
};

struct GanttChartEntry {
    int pid;
    int start_time;
    int end_time;
};

// First Come First Serve
void calculateTimesFCFS(vector<Process>& processes, vector<GanttChartEntry>& gantt_chart) {
    int current_time = 0;
    bool is_first_process = true;
    for (auto& process : processes) {
        if (is_first_process) {
            current_time = process.arrival_time;
            is_first_process = false;
        } else if (current_time < process.arrival_time) {
            current_time = process.arrival_time;
        }
        GanttChartEntry entry = {process.pid, current_time, current_time + process.burst_time};
        gantt_chart.push_back(entry);

        process.waiting_time = current_time - process.arrival_time;
        process.turnaround_time = process.waiting_time + process.burst_time;
        process.completion_time = current_time + process.burst_time;
        current_time += process.burst_time;
        process.response_time = process.waiting_time;
    }
}

// Shortest Remaining Time First
void calculateTimesSRTF(vector<Process>& processes, vector<GanttChartEntry>& gantt_chart) {
    int n = processes.size();
    int current_time = 0;
    int completed = 0;
    int prev = -1;

    for (auto& process : processes) {
        process.remaining_time = process.burst_time;
        process.response_time = -1;
    }

    while (completed != n) {
        int min_remaining_time = INT_MAX;
        int shortest = -1;
        bool check = false;

        for (int i = 0; i < n; i++) {
            if ((processes[i].arrival_time <= current_time) && (processes[i].remaining_time < min_remaining_time) && 
                (processes[i].remaining_time > 0)) {
                min_remaining_time = processes[i].remaining_time;
                shortest = i;
                check = true;
            }
        }

        if (!check) {
            current_time++;
            continue;
        }

        if (processes[shortest].response_time == -1) {
            processes[shortest].response_time = current_time - processes[shortest].arrival_time;
        }

        if (prev != shortest) {
            if (prev != -1 && processes[prev].remaining_time > 0) {
                gantt_chart.push_back({processes[prev].pid, current_time - 1, current_time});
            }
            prev = shortest;
        }

        processes[shortest].remaining_time--;

        current_time++;

        if (processes[shortest].remaining_time == 0) {
            completed++;
            processes[shortest].completion_time = current_time;
            processes[shortest].waiting_time = processes[shortest].completion_time - processes[shortest].arrival_time - processes[shortest].burst_time;
            if (processes[shortest].waiting_time < 0) {
                processes[shortest].waiting_time = 0;
            }
            processes[shortest].turnaround_time = processes[shortest].burst_time + processes[shortest].waiting_time;
            gantt_chart.push_back({processes[shortest].pid, current_time - processes[shortest].burst_time, current_time});
        }
    }
}

// Priority Scheduling (Lower Number Higher Priority)
void calculateTimesPriority(vector<Process>& processes, vector<GanttChartEntry>& gantt_chart) {
    int n = processes.size();
    int current_time = 0;
    int aging_interval = 60000; // 1 minute
    int aging_factor = 1; // Priority decrement factor per aging interval
    vector<bool> completed(n, false);
    int completed_count = 0;

    while (completed_count < n) {
        int highest_priority = numeric_limits<int>::max();
        int idx = -1;

        // Find the process with the highest priority (lowest priority number) that is ready to execute
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrival_time <= current_time && processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {
            // If the highest priority process has not started yet or is preempting another process
            if (current_time < processes[idx].arrival_time) {
                current_time = processes[idx].arrival_time;
            }
            if (processes[idx].response_time == -1) {
                processes[idx].response_time = current_time - processes[idx].arrival_time;
            }
            // Execute the process for one time unit
            gantt_chart.push_back({processes[idx].pid, current_time, current_time + 1});
            processes[idx].remaining_time--;

            // Update waiting time for other processes
            for (int i = 0; i < n; i++) {
                if (i != idx && !completed[i] && processes[i].arrival_time <= current_time) {
                    processes[i].waiting_time++;
                }
            }

            current_time++;
            // Check if the process has completed execution
            if (processes[idx].remaining_time == 0) {
                processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
                processes[idx].completion_time = current_time;
                completed[idx] = true;
                completed_count++;
            }

            // Apply aging to all non-completed processes
            for (int i = 0; i < n; i++) {
                if (!completed[i] && processes[i].arrival_time <= current_time) {
                    if (((current_time - processes[i].arrival_time) % aging_interval) == 0) {
                        processes[i].priority -= aging_factor;
                        // Ensure priority does not drop below a certain minimum (optional)
                        if (processes[i].priority < 0) {
                            processes[i].priority = 0;
                        }
                    }
                }
            }
        } else {
            current_time++; // No process is ready to execute, move to next time unit
        }
    }
}

// Round Robin Algorithm
void calculateTimesRR(vector<Process>& processes, vector<GanttChartEntry>& gantt_chart, int time_quantum) {
    int current_time = 0;
    queue<int> readyQueue;
    vector<int> remaining_time(processes.size());
    vector<int> arrival_time(processes.size());

    for (int i = 0; i < (int)processes.size(); ++i) {
        remaining_time[i] = processes[i].burst_time;
        arrival_time[i] = processes[i].arrival_time;
    }

    int completed = 0;
    int n = processes.size();
    int currentProcess = 0;
    vector<int> lastExecutionTime(processes.size(), 0);

    while (completed != n) {
        bool done = true;

        for (int i = 0; i < (int)processes.size(); i++) {
            if (arrival_time[i] <= current_time && remaining_time[i] > 0) {
                readyQueue.push(i);
                arrival_time[i] = INT_MAX; // Ensure it isn't added again
            }
        }

        if (!readyQueue.empty()) {
            currentProcess = readyQueue.front();
            readyQueue.pop();

            if (remaining_time[currentProcess] > time_quantum) {
                if (lastExecutionTime[currentProcess] == 0) {
                    processes[currentProcess].start_time = current_time;
                }

                gantt_chart.push_back({processes[currentProcess].pid, current_time, current_time + time_quantum});

                current_time += time_quantum;
                remaining_time[currentProcess] -= time_quantum;
                lastExecutionTime[currentProcess] = current_time;

                for (int i = 0; i < (int)processes.size(); i++) {
                    if (arrival_time[i] <= current_time && remaining_time[i] > 0 && i != currentProcess) {
                        readyQueue.push(i);
                        arrival_time[i] = INT_MAX; // Ensure it isn't added again
                    }
                }

                readyQueue.push(currentProcess);
            } else {
                if (lastExecutionTime[currentProcess] == 0) {
                    processes[currentProcess].start_time = current_time;
                }

                gantt_chart.push_back({currentProcess, current_time, current_time + remaining_time[currentProcess]});

                current_time += remaining_time[currentProcess];
                processes[currentProcess].completion_time = current_time;
                processes[currentProcess].turnaround_time = current_time - processes[currentProcess].arrival_time;
                processes[currentProcess].waiting_time = processes[currentProcess].turnaround_time - processes[currentProcess].burst_time;
                remaining_time[currentProcess] = 0;
                completed++;
            }

            done = false;
        }

        if (done) {
            current_time++;
        }
    }
}

// Auto schedule the processes
void auto_schedule(vector<Process>& processes, vector<GanttChartEntry>& gantt_chart, int time_quantum) {
    double sum = 0, mean = 0, STDDEV = 0, N = processes.size();
    for (int i = 0; i < (int)processes.size(); i++) {
        sum += processes[i].burst_time;
    }
    mean = (sum / (N * 1.0)) * 1.0;
    for (int i = 0; i < (int)processes.size(); i++) {
        STDDEV += ((processes[i].burst_time - mean) * (processes[i].burst_time - mean));
    }
    STDDEV = sqrt(STDDEV / N);
    if (STDDEV < mean / 2.0) {
        calculateTimesFCFS(processes, gantt_chart);
        return;
    }
    if (N > 7) {
        calculateTimesRR(processes, gantt_chart, time_quantum);
        return;
    }

    calculateTimesSRTF(processes, gantt_chart);
}

// For display in terminal
void displayProcesses(const vector<Process>& processes) {
    int n = processes.size();
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    cout << "PID\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\tResponse Time\tCompletion Time\n";
    for (const auto& process : processes) {
        cout << process.pid << "\t"
             << process.arrival_time << "\t\t"
             << process.burst_time << "\t\t"
             << process.priority << "\t\t"
             << process.waiting_time << "\t\t"
             << process.turnaround_time << "\t\t"
             << process.response_time << "\t\t"
             << process.completion_time << "\n";
        total_waiting_time += process.waiting_time;
        total_turnaround_time += process.turnaround_time;
    }

    cout << "\nAverage Waiting Time: " << (float)total_waiting_time / n << "\n";
    cout << "Average Turnaround Time: " << (float)total_turnaround_time / n << "\n";
}

void displayGanttChart(const vector<GanttChartEntry>& gantt_chart) {
    cout << "\nGantt Chart:\n";

    // Print the top bar
    cout << " ";
    for (const auto& entry : gantt_chart) {
        cout << "------";
    }
    cout << "\n|";
    for (const auto& entry : gantt_chart) {
        cout << "  P" << entry.pid << "  |";
    }
    cout << "\n ";

    // Print the bottom bar
    for (const auto& entry : gantt_chart) {
        cout << "------";
    }
    cout << "\n";

    // Print the time labels
    cout << "0";
    for (const auto& entry : gantt_chart) {
        cout << setw(6) << entry.end_time;
    }
    cout << "\n";
}

void sorted(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.arrival_time == b.arrival_time && a.priority == b.priority) return a.pid < b.pid;
        else if (a.arrival_time == b.arrival_time) return a.priority < b.priority;
        return a.arrival_time < b.arrival_time;
    });
}

// Scheduler
void scheduler(const string& algorithm, vector<Process>& processes, int time_quantum = 1) {
    sorted(processes);
    vector<GanttChartEntry> gantt_chart;

    if (algorithm == "FCFS") {
        calculateTimesFCFS(processes, gantt_chart);
    } else if (algorithm == "SRTF") {
        calculateTimesSRTF(processes, gantt_chart);
    } else if (algorithm == "Priority") {
        calculateTimesPriority(processes, gantt_chart);
    } else if (algorithm == "RR") {
        calculateTimesRR(processes, gantt_chart, time_quantum);
    } else if (algorithm == "auto") {
        auto_schedule(processes, gantt_chart, time_quantum);
    } else {
        cout << "Unsupported scheduling algorithm.\n";
        return;
    }

    displayProcesses(processes);
    displayGanttChart(gantt_chart);
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time, burst time, and priority for process " << i + 1 << ": ";
        cin >> processes[i].arrival_time >> processes[i].burst_time >> processes[i].priority;
    }
    cout << '\n';
    string algorithm;

    cout << " \tSelect the Algorithm " << '\n';
    cout << " Type 'FCFS' for First Come First Serve " << '\n';
    cout << " Type 'SRTF' for Shortest Remaining Time First " << '\n';
    cout << " Type 'Priority' for Priority Scheduling " << '\n';
    cout << " Type 'RR' for Round Robin " << '\n';
    cout << " Type 'auto' for Auto Schedule " << '\n';
    cout << '\n';
    cout << " Type your Algorithm : ";
    cin >> algorithm;

    int timeQuantum;
    if (algorithm == "RR" || algorithm == "auto") {
        cout << "\nEnter the time quantum: ";
        cin >> timeQuantum;
    } else {
        timeQuantum = 1; // Default value if not required
    }

    scheduler(algorithm, processes, timeQuantum);

    return 0;
}
