#ifndef ROUNDROBINSCHEDULER_H
#define ROUNDROBINSCHEDULER_H

#include <map>
#include <queue>
#include <vector>

#include "Scheduler.h"

class RoundRobinScheduler : public Scheduler
{
private:
    std::queue<Job*> waitingJobs;
    std::map<int, int> machineQuantumUsed;
    std::vector<Job*> jobsToRequeueAfterTick;
    int timeQuantum;

protected:
    Job* getNextJob() override;

public:
    RoundRobinScheduler(const std::vector<Machine*>& machines, int timeQuantum);

    void addJob(Job* job) override;
    void onJobAssigned(Machine* machine, Job* job, int currentTime) override;
    void onJobWorked(Machine* machine, int currentTime) override;
    void onTickFinished(int currentTime) override;
    bool hasPendingJobs() const override;
    int getQueueSize() const override;
};

#endif
