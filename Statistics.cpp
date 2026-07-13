#include "Statistics.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace
{
std::string eventTypeToString(EventType type)
{
    switch (type)
    {
        case EventType::TIME:
            return "TIME";
        case EventType::JOB_ARRIVED:
            return "JOB_ARRIVED";
        case EventType::JOB_ASSIGNED:
            return "JOB_ASSIGNED";
        case EventType::MACHINE_WORKING:
            return "MACHINE_WORKING";
        case EventType::JOB_COMPLETED:
            return "JOB_COMPLETED";
    }

    return "UNKNOWN";
}
}

Statistics::Statistics(int numberOfMachines)
    : completedJobs(0),
      totalWaitingTime(0),
      totalTurnaroundTime(0),
      machineBusyTime(numberOfMachines, 0)
{
    if (numberOfMachines <= 0)
    {
        throw std::invalid_argument("Statistics requires at least one machine");
    }
}

double Statistics::averageWaitingTime() const
{
    if (completedJobs == 0)
    {
        return 0.0;
    }

    return static_cast<double>(totalWaitingTime) / completedJobs;
}

double Statistics::averageTurnaroundTime() const
{
    if (completedJobs == 0)
    {
        return 0.0;
    }

    return static_cast<double>(totalTurnaroundTime) / completedJobs;
}

double Statistics::throughput(int simulationTime) const
{
    if (simulationTime <= 0)
    {
        return 0.0;
    }

    return static_cast<double>(completedJobs) / simulationTime;
}

double Statistics::machineUtilization(int machineIndex, int simulationTime) const
{
    if (simulationTime <= 0 || machineIndex < 0 || machineIndex >= static_cast<int>(machineBusyTime.size()))
    {
        return 0.0;
    }

    return 100.0 * machineBusyTime[machineIndex] / simulationTime;
}

void Statistics::recordTime(int currentTime, int queueSize)
{
    events.push_back({ currentTime, EventType::TIME, -1, -1, -1, queueSize });
}

void Statistics::recordArrival(const Job* job, int currentTime, int queueSize)
{
    if (job != nullptr)
    {
        events.push_back({ currentTime, EventType::JOB_ARRIVED, job->getId(), -1, job->getRemainingTime(), queueSize });
    }
}

void Statistics::recordAssignment(const Job* job, const Machine* machine, int currentTime, int queueSize)
{
    if (job != nullptr && machine != nullptr)
    {
        events.push_back({ currentTime, EventType::JOB_ASSIGNED, job->getId(), machine->getId(), job->getRemainingTime(), queueSize });
    }
}

void Statistics::recordMachineWorking(const Job* job, const Machine* machine, int currentTime, int queueSize)
{
    if (job == nullptr || machine == nullptr)
    {
        return;
    }

    const int machineIndex = machine->getId() - 1;
    if (machineIndex >= 0 && machineIndex < static_cast<int>(machineBusyTime.size()))
    {
        ++machineBusyTime[machineIndex];
    }

    events.push_back({ currentTime, EventType::MACHINE_WORKING, job->getId(), machine->getId(), job->getRemainingTime(), queueSize });
}

void Statistics::recordCompletion(const Job* job, const Machine* machine, int currentTime, int queueSize)
{
    if (job == nullptr || machine == nullptr)
    {
        return;
    }

    ++completedJobs;
    const int turnaroundTime = job->getEndTime() - job->getArrivalTime();
    const int waitingTime = turnaroundTime - job->getProcessingTime();
    totalWaitingTime += waitingTime;
    totalTurnaroundTime += turnaroundTime;
    jobMetrics.push_back({
        job->getId(),
        job->getArrivalTime(),
        job->getProcessingTime(),
        job->getStartTime(),
        job->getEndTime(),
        turnaroundTime,
        waitingTime
    });

    events.push_back({ currentTime, EventType::JOB_COMPLETED, job->getId(), machine->getId(), 0, queueSize });
}

void Statistics::printTimeline() const
{
    std::cout << "\n========== Timeline ==========\n";

    for (const Event& event : events)
    {
        switch (event.type)
        {
            case EventType::TIME:
                std::cout << "\nTime: " << event.time << " | Queue: " << event.queueSize << '\n';
                break;
            case EventType::JOB_ARRIVED:
                std::cout << "Job " << event.jobId << " arrived. Queue: " << event.queueSize << '\n';
                break;
            case EventType::JOB_ASSIGNED:
                std::cout << "Machine " << event.machineId << " assigned Job " << event.jobId << ". Queue: " << event.queueSize << '\n';
                break;
            case EventType::MACHINE_WORKING:
                std::cout << "Machine " << event.machineId << " worked on Job " << event.jobId
                          << " (remaining " << event.remainingTime << ").\n";
                break;
            case EventType::JOB_COMPLETED:
                std::cout << "Job " << event.jobId << " completed on Machine " << event.machineId << ".\n";
                break;
        }
    }

    std::cout << "==============================\n";
}

void Statistics::printStatistics(int simulationTime) const
{
    std::cout << "\n========== Statistics ==========" << '\n';
    std::cout << "Completed Jobs: " << completedJobs << '\n';
    std::cout << "Average Waiting Time: " << averageWaitingTime() << '\n';
    std::cout << "Average Turnaround Time: " << averageTurnaroundTime() << '\n';
    std::cout << "Throughput: " << throughput(simulationTime) << " jobs/unit" << "\n\n";

    for (int i = 0; i < static_cast<int>(machineBusyTime.size()); ++i)
    {
        std::cout << "Machine " << (i + 1) << " Utilization: "
                  << machineUtilization(i, simulationTime) << "%\n";
    }

    std::cout << "===============================" << '\n';
}

void Statistics::exportToJson(const std::string& filename, int simulationTime) const
{
    std::ofstream out(filename);
    if (!out)
    {
        throw std::runtime_error("Unable to open JSON output file: " + filename);
    }

    out << std::setprecision(6);
    out << "{\n";
    out << "  \"completedJobs\": " << completedJobs << ",\n";
    out << "  \"simulationTime\": " << simulationTime << ",\n";
    out << "  \"averageWaitingTime\": " << averageWaitingTime() << ",\n";
    out << "  \"averageTurnaroundTime\": " << averageTurnaroundTime() << ",\n";
    out << "  \"throughput\": " << throughput(simulationTime) << ",\n";
    out << "  \"jobMetrics\": [\n";

    for (std::size_t i = 0; i < jobMetrics.size(); ++i)
    {
        const JobMetric& metric = jobMetrics[i];
        out << "    { \"jobId\": " << metric.jobId
            << ", \"arrivalTime\": " << metric.arrivalTime
            << ", \"burstTime\": " << metric.burstTime
            << ", \"startTime\": " << metric.startTime
            << ", \"completionTime\": " << metric.completionTime
            << ", \"turnaroundTime\": " << metric.turnaroundTime
            << ", \"waitingTime\": " << metric.waitingTime << " }";
        if (i + 1 < jobMetrics.size())
        {
            out << ',';
        }
        out << '\n';
    }

    out << "  ],\n";
    out << "  \"ganttChart\": [\n";

    struct GanttSegment
    {
        int machineId;
        int jobId;
        int startTime;
        int endTime;
        int remainingTime;
    };

    std::vector<GanttSegment> ganttSegments;
    for (int machineId = 1; machineId <= static_cast<int>(machineBusyTime.size()); ++machineId)
    {
        bool hasOpenSegment = false;
        GanttSegment currentSegment{ machineId, -1, 0, 0, 0 };

        for (const Event& event : events)
        {
            if (event.type != EventType::MACHINE_WORKING || event.machineId != machineId)
            {
                continue;
            }

            const bool canMerge = hasOpenSegment
                && currentSegment.jobId == event.jobId
                && currentSegment.endTime == event.time;

            if (canMerge)
            {
                currentSegment.endTime = event.time + 1;
                currentSegment.remainingTime = event.remainingTime;
                continue;
            }

            if (hasOpenSegment)
            {
                ganttSegments.push_back(currentSegment);
            }

            currentSegment = { event.machineId, event.jobId, event.time, event.time + 1, event.remainingTime };
            hasOpenSegment = true;
        }

        if (hasOpenSegment)
        {
            ganttSegments.push_back(currentSegment);
        }
    }

    for (std::size_t i = 0; i < ganttSegments.size(); ++i)
    {
        const GanttSegment& segment = ganttSegments[i];
        out << "    { \"machineId\": " << segment.machineId
            << ", \"jobId\": " << segment.jobId
            << ", \"startTime\": " << segment.startTime
            << ", \"endTime\": " << segment.endTime
            << ", \"remainingTime\": " << segment.remainingTime << " }";
        if (i + 1 < ganttSegments.size())
        {
            out << ',';
        }
        out << '\n';
    }

    out << "  ],\n";
    out << "  \"machineUtilization\": [\n";

    for (int i = 0; i < static_cast<int>(machineBusyTime.size()); ++i)
    {
        out << "    { \"machineId\": " << (i + 1)
            << ", \"busyTime\": " << machineBusyTime[i]
            << ", \"utilizationPercent\": " << machineUtilization(i, simulationTime) << " }";
        if (i + 1 < static_cast<int>(machineBusyTime.size()))
        {
            out << ',';
        }
        out << '\n';
    }

    out << "  ],\n";
    out << "  \"events\": [\n";

    for (std::size_t i = 0; i < events.size(); ++i)
    {
        const Event& event = events[i];
        out << "    { \"time\": " << event.time
            << ", \"type\": \"" << eventTypeToString(event.type) << "\""
            << ", \"jobId\": " << event.jobId
            << ", \"machineId\": " << event.machineId
            << ", \"remainingTime\": " << event.remainingTime
            << ", \"queueSize\": " << event.queueSize << " }";
        if (i + 1 < events.size())
        {
            out << ',';
        }
        out << '\n';
    }

    out << "  ]\n";
    out << "}\n";
}
