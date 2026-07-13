#include "RoundRobinScheduler.h"

#include <algorithm>

RoundRobinScheduler::RoundRobinScheduler(const std::vector<Machine*>& machines, int timeQuantum)
    : Scheduler(machines),
      timeQuantum(std::max(1, timeQuantum))
{
}

void RoundRobinScheduler::addJob(Job* job)
{
    if (job != nullptr)
    {
        waitingJobs.push(job);
    }
}

Job* RoundRobinScheduler::getNextJob()
{
    if (waitingJobs.empty())
    {
        return nullptr;
    }

    Job* job = waitingJobs.front();
    waitingJobs.pop();
    return job;
}

void RoundRobinScheduler::onJobAssigned(Machine* machine, Job* job, int currentTime)
{
    (void)job;
    (void)currentTime;
    if (machine != nullptr)
    {
        machineQuantumUsed[machine->getId()] = 0;
    }
}

void RoundRobinScheduler::onJobWorked(Machine* machine, int currentTime)
{
    (void)currentTime;
    if (machine == nullptr || machine->isFree())
    {
        return;
    }

    Job* job = machine->getCurrentJob();
    if (job == nullptr || job->isCompleted())
    {
        return;
    }

    const int machineId = machine->getId();
    int& used = machineQuantumUsed[machineId];
    ++used;

    if (used >= timeQuantum && job->getRemainingTime() > 0)
    {
        Job* releasedJob = machine->releaseCurrentJob();
        if (releasedJob != nullptr)
        {
            jobsToRequeueAfterTick.push_back(releasedJob);
        }
        used = 0;
    }
}

void RoundRobinScheduler::onTickFinished(int currentTime)
{
    (void)currentTime;
    for (Job* job : jobsToRequeueAfterTick)
    {
        waitingJobs.push(job);
    }
    jobsToRequeueAfterTick.clear();
}

bool RoundRobinScheduler::hasPendingJobs() const
{
    return !waitingJobs.empty() || !jobsToRequeueAfterTick.empty();
}

int RoundRobinScheduler::getQueueSize() const
{
    return static_cast<int>(waitingJobs.size() + jobsToRequeueAfterTick.size());
}
