#include "Scheduler.h"

Scheduler::Scheduler(const std::vector<Machine*>& machines)
    : machines(machines)
{
}

void Scheduler::onJobWorked(Machine* machine, int currentTime)
{
    (void)machine;
    (void)currentTime;
}

void Scheduler::onJobAssigned(Machine* machine, Job* job, int currentTime)
{
    (void)machine;
    (void)job;
    (void)currentTime;
}

void Scheduler::onTickFinished(int currentTime)
{
    (void)currentTime;
}

std::vector<Assignment> Scheduler::schedule(int currentTime)
{
    std::vector<Assignment> assignments;

    for (Machine* machine : machines)
    {
        if (!hasPendingJobs())
        {
            break;
        }

        if (machine == nullptr || !machine->isFree())
        {
            continue;
        }

        Job* job = getNextJob();
        if (job == nullptr)
        {
            break;
        }

        machine->assignJob(job, currentTime);
        onJobAssigned(machine, job, currentTime);
        assignments.push_back({ machine, job, getQueueSize() });
    }

    return assignments;
}
