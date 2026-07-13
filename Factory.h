#ifndef FACTORY_H
#define FACTORY_H

#include <vector>

#include "Machine.h"

class Factory
{
private:
    std::vector<Machine> machines;

public:
    explicit Factory(int numberOfMachines);

    std::vector<Machine>& getMachines();
    const std::vector<Machine>& getMachines() const;
};

#endif
