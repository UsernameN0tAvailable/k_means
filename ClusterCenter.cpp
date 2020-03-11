#include <iostream>
#include <vector>
#include <cmath>
#include "ClusterCenter.h"

/*
 * Public Methods
 */

ClusterCenter::ClusterCenter(int size, int maxValue, std::vector<TrainSample>* trainSamples, uint8_t id)
:trainSamples(trainSamples), id(id), repositionDelta(10000000000.0) // huge value
{
    values.reserve(size);
    for (int i = 0; i < size; i++)
        values.push_back(std::rand() % (maxValue + 1));
}

void ClusterCenter::computeDistances() {

    for(auto sampleIterator = trainSamples->begin(); sampleIterator != trainSamples->end(); ++sampleIterator) {
        std::vector<uint8_t>* trainSampleValues = sampleIterator->getValues();
        int entryIndex = 0;
        float sum = 0;
        for(uint8_t value : values){
            float diff = (float)trainSampleValues->at(entryIndex) - (float)value;
            sum += (diff * diff); // sqrt
            entryIndex++;
        }
        sampleIterator->checkDistanceForMin(sqrt(sum));
    }
}


void ClusterCenter::reposition(std::vector<float> newValues) {

    distance(&values, &newValues);

    values = newValues;

}


void ClusterCenter::distance(std::vector<float>* firstVector, std::vector<float>* secondVector){

    float sum = 0;
    for(int index = 0; index < firstVector->size(); index++) {
        float diff =  (firstVector->at(index) - secondVector->at(index));
        sum = (diff * diff);
    }

    std::cout << "sum : " <<  sum << std::endl;

    repositionDelta = sqrt(sum);
}


float ClusterCenter::getLastRepDelta() { return repositionDelta; }





