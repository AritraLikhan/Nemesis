#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

int main() {
    std::ifstream infile("input.txt");
    std::ofstream outfile("output.txt");

    if (!infile) {
        std::cerr << "Error: Cannot open input file." << std::endl;
        return 1;
    }

    if (!outfile) {
        std::cerr << "Error: Cannot open output file." << std::endl;
        return 1;
    }

    std::vector<int> process_id, arrival_time, burst_time;
    std::vector<int> start_time, completion_time, turnaround_time, waiting_time, response_time;

    int id, arrival, burst;

    // Read input from file
    while (infile >> id >> arrival >> burst) {
        process_id.push_back(id);
        arrival_time.push_back(arrival);
        burst_time.push_back(burst);
    }

    int n = process_id.size();
    start_time.resize(n);
    completion_time.resize(n);
    turnaround_time.resize(n);
    waiting_time.resize(n);
    response_time.resize(n);

    // Sort processes by arrival time using bubble sort
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arrival_time[j] > arrival_time[j + 1]) {
                std::swap(process_id[j], process_id[j + 1]);
                std::swap(arrival_time[j], arrival_time[j + 1]);
                std::swap(burst_time[j], burst_time[j + 1]);
            }
        }
    }

    int current_time = 0;
    int total_turnaround_time = 0, total_waiting_time = 0, total_response_time = 0;
    int total_idle_time = 0, completed = 0;

    for (int i = 0; i < n; ++i) {
        if (current_time < arrival_time[i]) {
            total_idle_time += (arrival_time[i] - current_time);
            current_time = arrival_time[i];
        }

        start_time[i] = current_time;
        completion_time[i] = start_time[i] + burst_time[i];
        turnaround_time[i] = completion_time[i] - arrival_time[i];
        waiting_time[i] = turnaround_time[i] - burst_time[i];
        response_time[i] = start_time[i] - arrival_time[i];

        current_time = completion_time[i];

        total_turnaround_time += turnaround_time[i];
        total_waiting_time += waiting_time[i];
        total_response_time += response_time[i];
        completed++;
    }

    // Calculate performance metrics
    float cpu_utilization = ((current_time - total_idle_time) / (float)current_time) * 100;
    float throughput = completed / (float)current_time;
    float avg_turnaround_time = total_turnaround_time / (float)n;
    float avg_waiting_time = total_waiting_time / (float)n;
    float avg_response_time = total_response_time / (float)n;

    // Write results to the output file
    outfile << "ProcessID\tArrivalTime\tBurstTime\tStartTime\tCompletionTime\tTurnaroundTime\tWaitingTime\tResponseTime\n";
    for (int i = 0; i < n; ++i) {
        outfile << process_id[i] << "\t\t" << arrival_time[i] << "\t\t" << burst_time[i]
                << "\t\t" << start_time[i] << "\t\t" << completion_time[i]
                << "\t\t" << turnaround_time[i] << "\t\t" << waiting_time[i]
                << "\t\t" << response_time[i] << "\n";
    }

    outfile << "\nCPU Utilization: " << std::fixed << std::setprecision(2) << cpu_utilization << "%\n";
    outfile << "Throughput: " << throughput << " processes/unit time\n";
    outfile << "Average Turnaround Time: " << avg_turnaround_time << "\n";
    outfile << "Average Waiting Time: " << avg_waiting_time << "\n";
    outfile << "Average Response Time: " << avg_response_time << "\n";

    infile.close();
    outfile.close();

    std::cout << "Results have been written to output.txt" << std::endl;
    return 0;
}
