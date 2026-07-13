#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>

#include "Machine.h"

struct Assignment
{
    Machine* machine;
    Job* job;
    int queueSizeAfter;
};

class Scheduler
{
protected:
    std::vector<Machine*> machines;

    virtual Job* getNextJob() = 0;

public:
    explicit Scheduler(const std::vector<Machine*>& machines);
    virtual ~Scheduler() = default;

    virtual void addJob(Job* job) = 0;
    virtual void onJobAssigned(Machine* machine, Job* job, int currentTime);
    virtual void onJobWorked(Machine* machine, int currentTime);
    virtual void onTickFinished(int currentTime);
    std::vector<Assignment> schedule(int currentTime);

    virtual bool hasPendingJobs() const = 0;
    virtual int getQueueSize() const = 0;
};

#endif
