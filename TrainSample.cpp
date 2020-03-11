#include <iostream>
#include <vector>
#include <sstream>
#include "TrainSample.h"


/**
 * splits the string according to delimiter
 * and saves it into values
 * @param valueString
 */
void TrainSample::splitAndSave(std::string &valueString) {
    char delimiter = ','; // because csv

    // split it convert it to string and save it
    std::stringstream ss(valueString);
    std::string item;

    while (std::getline(ss, item, delimiter))
        values.emplace_back((uint8_t) std::stoi(item));
}


TrainSample::TrainSample(std::string &valuesString)
: distancesChecked(0), smallestDistance(10000000000000.0) // huge impossible value
{
    splitAndSave(valuesString);
}



void TrainSample::checkDistanceForMin(float distance) {
    if(smallestDistance > distance){
        smallestDistance = distance;
        closestClusterIndex = distancesChecked;
    }
    distancesChecked++;
}

std::vector<uint8_t> *TrainSample::getValues() { return &values; }

int TrainSample::getSampleSize() { return values.size(); }

uint8_t TrainSample::getClosestCenterId(){return closestClusterIndex;}

