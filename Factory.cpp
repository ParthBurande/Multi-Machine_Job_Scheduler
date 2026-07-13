#include "Factory.h"

#include <stdexcept>

Factory::Factory(int numberOfMachines)
{
    if (numberOfMachines <= 0)
    {
        throw std::invalid_argument("Factory must contain at least one machine");
    }

    machines.reserve(numberOfMachines);
    for (int i = 1; i <= numberOfMachines; ++i)
    {
        machines.emplace_back(i);
    }
}

std::vector<Machine>& Factory::getMachines()
{
    return machines;
}

const std::vector<Machine>& Factory::getMachines() const
{
    return machines;
}
