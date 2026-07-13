#include "PriorityScheduler.h"

#include <algorithm>

PriorityScheduler::PriorityScheduler(const std::vector<Machine*>& machines)
    : Scheduler(machines)
{
}

void PriorityScheduler::addJob(Job* job)
{
    if (job != nullptr)
    {
        waitingJobs.push_back(job);
    }
}

Job* PriorityScheduler::getNextJob()
{
    if (waitingJobs.empty())
    {
        return nullptr;
    }

    auto next = std::min_element(waitingJobs.begin(), waitingJobs.end(), [](const Job* left, const Job* right) {
        if (left->getPriority() == right->getPriority())
        {
            return left->getArrivalTime() == right->getArrivalTime()
                ? left->getId() < right->getId()
                : left->getArrivalTime() < right->getArrivalTime();
        }
        return left->getPriority() < right->getPriority();
    });

    Job* job = *next;
    waitingJobs.erase(next);
    return job;
}

bool PriorityScheduler::hasPendingJobs() const
{
    return !waitingJobs.empty();
}

int PriorityScheduler::getQueueSize() const
{
    return static_cast<int>(waitingJobs.size());
}
