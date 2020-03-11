#include <iostream>
#include <vector>
#include "ClusterCenter.h"

ClusterCenter::ClusterCenter(int size, int maxValue, std::vector<TrainSample>* trainSamples)
:trainSamples(trainSamples)
{
    values.reserve(size);
    for (int i = 0; i < size; i++)
        values.push_back(std::rand() % (maxValue + 1));
}

std::vector<uint8_t>* ClusterCenter::getValues() {
    return &values;
}


