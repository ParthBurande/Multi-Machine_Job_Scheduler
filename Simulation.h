#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include "Factory.h"
#include "Job.h"
#include "Scheduler.h"
#include "Statistics.h"

class Simulation
{
private:
    int currentTime;
    int completedJobs;

    Factory& factory;
    Scheduler* scheduler;
    std::vector<Job>& jobs;
    Statistics statistics;

    void addArrivingJobs();
    void executeMachines();
    void printFactoryState() const;

public:
    Simulation(Factory& factory, Scheduler* scheduler, std::vector<Job>& jobs);

    void run();
};

#endif
