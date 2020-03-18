#include <iostream>
#include <vector>
#include <sstream>
#include "math.h"
#include "TrainSample.h"


/**
 * splits the string according to delimiter
 * and saves it into values
 * @param valueString
 */
void TrainSample::splitAndLoad(std::string &valueString) {
    char delimiter = ','; // because csv

    // split it convert it to string and save it
    std::stringstream ss(valueString);
    std::string item;

    while (std::getline(ss, item, delimiter))
        values.emplace_back((uint8_t) std::stof(item));
}


TrainSample::TrainSample(std::string &valuesString)
: distancesChecked(0), smallestDistance(MAXFLOAT), closestClusterIndex(0) // huge impossible value
{
    splitAndLoad(valuesString);
}



void TrainSample::checkDistanceForMin(float distance) {
    if(smallestDistance > distance){
        smallestDistance = distance;
        closestClusterIndex = distancesChecked;
    }
    distancesChecked++;
}

void TrainSample::resetAttributes(){
    smallestDistance = 10000000000000.0;
    distancesChecked = 0;
}

std::vector<float> *TrainSample::getValues() { return &values; }

int TrainSample::getSampleSize() { return values.size(); }

uint8_t TrainSample::getClosestCenterId(){return closestClusterIndex;}

