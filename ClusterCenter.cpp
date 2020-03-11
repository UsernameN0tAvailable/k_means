#include <iostream>
#include <vector>
#include <cmath>
#include "ClusterCenter.h"


/*
 * Private Methods
 */

void ClusterCenter::computeDistances() {

    distances.reserve(trainSamples->size());

    for(TrainSample trainSample : *trainSamples){
        std::vector<uint8_t>* values = trainSample.getValues();
        int entryIndex = 0;
        float sum = 0;
        for(uint8_t value : *values){
            sum += std::pow(values->at(entryIndex) - value, 2);
            entryIndex++;
        }

        float distance = sqrt(sum);

        distances.push_back(distance);
        trainSample.pushDistance(distance);
    }
}




/*
 * Public Methods
 */

ClusterCenter::ClusterCenter(int size, int maxValue, std::vector<TrainSample>* trainSamples, uint8_t id)
:trainSamples(trainSamples), id(id)
{
    values.reserve(size);
    for (int i = 0; i < size; i++)
        values.push_back(std::rand() % (maxValue + 1));
}


void ClusterCenter::reposition(){
    computeDistances();





};


std::vector<uint8_t>* ClusterCenter::getValues() {
    return &values;
}




