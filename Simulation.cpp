#include "Simulation.h"

#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

Simulation::Simulation(Factory& factory, Scheduler* scheduler, std::vector<Job>& jobs)
    : currentTime(0),
      completedJobs(0),
      factory(factory),
      scheduler(scheduler),
      jobs(jobs),
      statistics(static_cast<int>(factory.getMachines().size()))
{
    if (scheduler == nullptr)
    {
        throw std::invalid_argument("Simulation requires a scheduler");
    }
}

void Simulation::addArrivingJobs()
{
    for (Job& job : jobs)
    {
        if (job.getArrivalTime() == currentTime && job.getStatus() == JobStatus::WAITING)
        {
            scheduler->addJob(&job);
            statistics.recordArrival(&job, currentTime, scheduler->getQueueSize());
        }
    }
}

void Simulation::executeMachines()
{
    const int completionTime = currentTime + 1;
    std::vector<std::pair<Job*, Machine*>> completedThisTick;

    for (Machine& machine : factory.getMachines())
    {
        Job* activeJob = machine.getCurrentJob();
        if (activeJob == nullptr)
        {
            continue;
        }

        Job* completedJob = machine.workOneUnit(completionTime);
        statistics.recordMachineWorking(activeJob, &machine, currentTime, scheduler->getQueueSize());

        if (completedJob != nullptr)
        {
            completedThisTick.push_back({ completedJob, &machine });
        }
        else
        {
            scheduler->onJobWorked(&machine, currentTime);
        }
    }

    for (const auto& completion : completedThisTick)
    {
        ++completedJobs;
        statistics.recordCompletion(completion.first, completion.second, completionTime, scheduler->getQueueSize());
    }

    scheduler->onTickFinished(currentTime);
}

void Simulation::printFactoryState() const
{
    std::cout << "\n---------------------------------\n";
    std::cout << "Current Time: " << currentTime << "\n\n";

    for (const Machine& machine : factory.getMachines())
    {
        std::cout << "Machine " << machine.getId() << ": ";

        if (machine.isFree())
        {
            std::cout << "Idle";
        }
        else
        {
            const Job* job = machine.getCurrentJob();
            std::cout << "Running Job " << job->getId()
                      << " (Remaining Time: " << job->getRemainingTime() << ")";
        }

        std::cout << '\n';
    }

    std::cout << "Waiting Queue Size: " << scheduler->getQueueSize() << '\n';
    std::cout << "---------------------------------\n";
}

void Simulation::run()
{
    while (completedJobs < static_cast<int>(jobs.size()))
    {
        statistics.recordTime(currentTime, scheduler->getQueueSize());

        addArrivingJobs();

        const std::vector<Assignment> assignments = scheduler->schedule(currentTime);
        for (const Assignment& assignment : assignments)
        {
            statistics.recordAssignment(assignment.job, assignment.machine, currentTime, assignment.queueSizeAfter);
        }

        executeMachines();
        printFactoryState();

        ++currentTime;
    }

    statistics.printTimeline();
    statistics.printStatistics(currentTime);
    statistics.exportToJson("simulation.json", currentTime);

    std::cout << "\nSimulation finished. JSON exported to simulation.json\n";
}
