#include "SJFScheduler.h"

#include <algorithm>

SJFScheduler::SJFScheduler(const std::vector<Machine*>& machines)
    : Scheduler(machines)
{
}

void SJFScheduler::addJob(Job* job)
{
    if (job != nullptr)
    {
        waitingJobs.push_back(job);
    }
}

Job* SJFScheduler::getNextJob()
{
    if (waitingJobs.empty())
    {
        return nullptr;
    }

    auto next = std::min_element(waitingJobs.begin(), waitingJobs.end(), [](const Job* left, const Job* right) {
        if (left->getProcessingTime() == right->getProcessingTime())
        {
            return left->getArrivalTime() == right->getArrivalTime()
                ? left->getId() < right->getId()
                : left->getArrivalTime() < right->getArrivalTime();
        }
        return left->getProcessingTime() < right->getProcessingTime();
    });

    Job* job = *next;
    waitingJobs.erase(next);
    return job;
}

bool SJFScheduler::hasPendingJobs() const
{
    return !waitingJobs.empty();
}

int SJFScheduler::getQueueSize() const
{
    return static_cast<int>(waitingJobs.size());
}
