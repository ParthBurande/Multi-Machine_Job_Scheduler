#ifndef STATISTICS_H
#define STATISTICS_H

#include <string>
#include <vector>

#include "Job.h"
#include "Machine.h"

enum class EventType
{
    TIME,
    JOB_ARRIVED,
    JOB_ASSIGNED,
    MACHINE_WORKING,
    JOB_COMPLETED
};

struct Event
{
    int time;
    EventType type;
    int jobId;
    int machineId;
    int remainingTime;
    int queueSize;
};

struct JobMetric
{
    int jobId;
    int arrivalTime;
    int burstTime;
    int startTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

class Statistics
{
private:
    std::vector<Event> events;
    std::vector<JobMetric> jobMetrics;
    int completedJobs;
    int totalWaitingTime;
    int totalTurnaroundTime;
    std::vector<int> machineBusyTime;

    double averageWaitingTime() const;
    double averageTurnaroundTime() const;
    double throughput(int simulationTime) const;
    double machineUtilization(int machineIndex, int simulationTime) const;

public:
    explicit Statistics(int numberOfMachines);

    void recordTime(int currentTime, int queueSize);
    void recordArrival(const Job* job, int currentTime, int queueSize);
    void recordAssignment(const Job* job, const Machine* machine, int currentTime, int queueSize);
    void recordMachineWorking(const Job* job, const Machine* machine, int currentTime, int queueSize);
    void recordCompletion(const Job* job, const Machine* machine, int currentTime, int queueSize);

    void printTimeline() const;
    void printStatistics(int simulationTime) const;
    void exportToJson(const std::string& filename, int simulationTime) const;
};

#endif
