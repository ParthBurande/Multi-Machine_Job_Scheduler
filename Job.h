#ifndef JOB_H
#define JOB_H

enum class JobStatus
{
    WAITING,
    RUNNING,
    COMPLETED
};

class Job
{
private:
    int id;
    int processingTime;
    int remainingTime;
    int arrivalTime;
    int priority;
    int startTime;
    int endTime;
    JobStatus status;

public:
    Job(int id, int processingTime, int arrivalTime, int priority);

    void start(int currentTime);
    void workOneUnit();
    void pause();
    void finish(int completionTime);

    bool isCompleted() const;
    bool hasStarted() const;

    int getId() const;
    int getProcessingTime() const;
    int getRemainingTime() const;
    int getArrivalTime() const;
    int getPriority() const;
    int getStartTime() const;
    int getEndTime() const;
    JobStatus getStatus() const;
};

#endif
