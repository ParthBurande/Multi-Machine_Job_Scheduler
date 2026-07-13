#ifndef FCFSSCHEDULER_H
#define FCFSSCHEDULER_H

#include <queue>
#include <vector>

#include "Scheduler.h"

class FCFSScheduler : public Scheduler
{
private:
    std::queue<Job*> waitingJobs;

protected:
    Job* getNextJob() override;

public:
    explicit FCFSScheduler(const std::vector<Machine*>& machines);

    void addJob(Job* job) override;
    bool hasPendingJobs() const override;
    int getQueueSize() const override;
};

#endif
