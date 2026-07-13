#ifndef SJFSCHEDULER_H
#define SJFSCHEDULER_H

#include <vector>

#include "Scheduler.h"

class SJFScheduler : public Scheduler
{
private:
    std::vector<Job*> waitingJobs;

protected:
    Job* getNextJob() override;

public:
    explicit SJFScheduler(const std::vector<Machine*>& machines);

    void addJob(Job* job) override;
    bool hasPendingJobs() const override;
    int getQueueSize() const override;
};

#endif
