#include "FCFSScheduler.h"

FCFSScheduler::FCFSScheduler(const std::vector<Machine*>& machines)
    : Scheduler(machines)
{
}

void FCFSScheduler::addJob(Job* job)
{
    if (job != nullptr)
    {
        waitingJobs.push(job);
    }
}

Job* FCFSScheduler::getNextJob()
{
    if (waitingJobs.empty())
    {
        return nullptr;
    }

    Job* job = waitingJobs.front();
    waitingJobs.pop();
    return job;
}

bool FCFSScheduler::hasPendingJobs() const
{
    return !waitingJobs.empty();
}

int FCFSScheduler::getQueueSize() const
{
    return static_cast<int>(waitingJobs.size());
}
