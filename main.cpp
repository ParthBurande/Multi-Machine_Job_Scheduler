#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Factory.h"
#include "FCFSScheduler.h"
#include "Job.h"
#include "PriorityScheduler.h"
#include "RoundRobinScheduler.h"
#include "Simulation.h"
#include "SJFScheduler.h"

namespace
{
struct SimulationInput
{
    int machines;
    int timeQuantum;
    std::string scheduler;
    std::vector<Job> jobs;
};

std::string readFile(const std::string& filename)
{
    std::ifstream input(filename);
    if (!input)
    {
        throw std::runtime_error("Unable to open input file: " + filename);
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

int readRequiredInt(const std::string& objectText, const std::string& key)
{
    const std::regex pattern("\\\"" + key + "\\\"\\s*:\\s*(-?\\d+)");
    std::smatch match;

    if (!std::regex_search(objectText, match, pattern))
    {
        throw std::runtime_error("Missing numeric field: " + key);
    }

    return std::stoi(match[1].str());
}

int readOptionalInt(const std::string& objectText, const std::string& key, int defaultValue)
{
    const std::regex pattern("\\\"" + key + "\\\"\\s*:\\s*(-?\\d+)");
    std::smatch match;

    if (!std::regex_search(objectText, match, pattern))
    {
        return defaultValue;
    }

    return std::stoi(match[1].str());
}

std::string readOptionalString(const std::string& objectText, const std::string& key, const std::string& defaultValue)
{
    const std::regex pattern("\\\"" + key + "\\\"\\s*:\\s*\\\"([^\\\"]+)\\\"");
    std::smatch match;

    if (!std::regex_search(objectText, match, pattern))
    {
        return defaultValue;
    }

    return match[1].str();
}

SimulationInput defaultInput()
{
    SimulationInput input{ 3, 2, "FCFS", {} };
    input.jobs.emplace_back(1, 5, 0, 3);
    input.jobs.emplace_back(2, 3, 0, 1);
    input.jobs.emplace_back(3, 7, 2, 2);
    input.jobs.emplace_back(4, 2, 4, 5);
    input.jobs.emplace_back(5, 4, 5, 4);
    return input;
}

SimulationInput parseInputFile(const std::string& filename)
{
    const std::string text = readFile(filename);
    SimulationInput input{
        readRequiredInt(text, "machines"),
        readOptionalInt(text, "timeQuantum", 2),
        readOptionalString(text, "scheduler", "FCFS"),
        {}
    };

    const std::regex jobPattern("\\{[^{}]*\\\"id\\\"[^{}]*\\}");
    auto begin = std::sregex_iterator(text.begin(), text.end(), jobPattern);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it)
    {
        const std::string jobText = it->str();
        input.jobs.emplace_back(
            readRequiredInt(jobText, "id"),
            readRequiredInt(jobText, "processingTime"),
            readRequiredInt(jobText, "arrivalTime"),
            readRequiredInt(jobText, "priority"));
    }

    if (input.jobs.empty())
    {
        throw std::runtime_error("Input must contain at least one job");
    }

    return input;
}
}

int main(int argc, char* argv[])
{
    try
    {
        SimulationInput input = argc > 1 ? parseInputFile(argv[1]) : defaultInput();

        Factory factory(input.machines);

        std::vector<Machine*> machinePointers;
        for (Machine& machine : factory.getMachines())
        {
            machinePointers.push_back(&machine);
        }

        FCFSScheduler fcfsScheduler(machinePointers);
        SJFScheduler sjfScheduler(machinePointers);
        PriorityScheduler priorityScheduler(machinePointers);
        RoundRobinScheduler roundRobinScheduler(machinePointers, input.timeQuantum);

        Scheduler* scheduler = &fcfsScheduler;
        if (input.scheduler == "SJF")
        {
            scheduler = &sjfScheduler;
        }
        else if (input.scheduler == "PRIORITY")
        {
            scheduler = &priorityScheduler;
        }
        else if (input.scheduler == "RR")
        {
            scheduler = &roundRobinScheduler;
        }

        Simulation simulation(factory, scheduler, input.jobs);
        simulation.run();
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Error: " << exception.what() << '\n';
        return 1;
    }

    return 0;
}
