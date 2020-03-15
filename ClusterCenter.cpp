#include <iostream>
#include <vector>
#include <math.h>
#include "ClusterCenter.h"

/*
 * Public Methods
 */

ClusterCenter::ClusterCenter(int size, int maxValue, std::vector<TrainSample>* trainSamples, uint8_t id)
:trainSamples(trainSamples), id(id), repositionDelta(10000000000.0) // huge value
{
    values = new std::vector<float>;
    values->reserve(size);
    for (int i = 0; i < size; i++)
        values->push_back(std::rand() % (maxValue + 1));
}

void ClusterCenter::computeDistances() {

    for(auto sampleIterator = trainSamples->begin(); sampleIterator != trainSamples->end(); ++sampleIterator) {
        std::vector<float>* trainSampleValues = sampleIterator->getValues();
        int entryIndex = 0;
        float sum = 0;
        for(float value : *values){
            float diff = trainSampleValues->at(entryIndex) - value;
            sum += (diff * diff); // pow(diff, 2)
            entryIndex++;
        }
        sampleIterator->checkDistanceForMin(sqrt(sum));
    }
}


void ClusterCenter::reposition(std::vector<float>* newValues) {
    distance(values, newValues);
    delete(values);
    values = newValues;
}


void ClusterCenter::distance(std::vector<float>* firstVector, std::vector<float>* secondVector){
    float sum = 0;
    for(int index = 0; index < firstVector->size(); index++) {
        float diff =  (firstVector->at(index) - secondVector->at(index));
        sum += (diff * diff);
    }
    repositionDelta = sqrt(sum);
}


float ClusterCenter::getLastRepDelta() { return repositionDelta; }
uint8_t ClusterCenter::getId() { return id; }





