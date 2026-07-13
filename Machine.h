#ifndef MACHINE_H
#define MACHINE_H

#include "Job.h"

class Machine
{
private:
    int id;
    Job* currentJob;

public:
    explicit Machine(int id);

    bool isFree() const;
    void assignJob(Job* job, int currentTime);
    Job* workOneUnit(int completionTime);
    Job* releaseCurrentJob();

    Job* getCurrentJob() const;
    int getId() const;
};

#endif
