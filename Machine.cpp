#include "Machine.h"

Machine::Machine(int id)
    : id(id), currentJob(nullptr)
{
}

bool Machine::isFree() const
{
    return currentJob == nullptr;
}

void Machine::assignJob(Job* job, int currentTime)
{
    if (job == nullptr || !isFree())
    {
        return;
    }

    currentJob = job;
    currentJob->start(currentTime);
}

Job* Machine::workOneUnit(int completionTime)
{
    if (isFree())
    {
        return nullptr;
    }

    currentJob->workOneUnit();

    if (currentJob->getRemainingTime() == 0)
    {
        currentJob->finish(completionTime);
        Job* completedJob = currentJob;
        currentJob = nullptr;
        return completedJob;
    }

    return nullptr;
}

Job* Machine::releaseCurrentJob()
{
    Job* releasedJob = currentJob;
    currentJob = nullptr;
    if (releasedJob != nullptr)
    {
        releasedJob->pause();
    }
    return releasedJob;
}

Job* Machine::getCurrentJob() const
{
    return currentJob;
}

int Machine::getId() const
{
    return id;
}
