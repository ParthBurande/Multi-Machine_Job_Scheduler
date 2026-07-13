#include "Job.h"

#include <stdexcept>

Job::Job(int id, int processingTime, int arrivalTime, int priority)
    : id(id),
      processingTime(processingTime),
      remainingTime(processingTime),
      arrivalTime(arrivalTime),
      priority(priority),
      startTime(-1),
      endTime(-1),
      status(JobStatus::WAITING)
{
    if (processingTime <= 0)
    {
        throw std::invalid_argument("processingTime must be positive");
    }

    if (arrivalTime < 0)
    {
        throw std::invalid_argument("arrivalTime cannot be negative");
    }
}

void Job::start(int currentTime)
{
    if (status == JobStatus::WAITING)
    {
        if (startTime < 0)
        {
            startTime = currentTime;
        }
        status = JobStatus::RUNNING;
    }
}

void Job::workOneUnit()
{
    if (status == JobStatus::RUNNING && remainingTime > 0)
    {
        --remainingTime;
    }
}

void Job::pause()
{
    if (status == JobStatus::RUNNING && remainingTime > 0)
    {
        status = JobStatus::WAITING;
    }
}

void Job::finish(int completionTime)
{
    if (remainingTime == 0)
    {
        endTime = completionTime;
        status = JobStatus::COMPLETED;
    }
}

bool Job::isCompleted() const
{
    return status == JobStatus::COMPLETED;
}

bool Job::hasStarted() const
{
    return startTime >= 0;
}

int Job::getId() const
{
    return id;
}

int Job::getProcessingTime() const
{
    return processingTime;
}

int Job::getRemainingTime() const
{
    return remainingTime;
}

int Job::getArrivalTime() const
{
    return arrivalTime;
}

int Job::getPriority() const
{
    return priority;
}

int Job::getStartTime() const
{
    return startTime;
}

int Job::getEndTime() const
{
    return endTime;
}

JobStatus Job::getStatus() const
{
    return status;
}
