#pragma once

class TrainSample {

private:
    std::vector<float> values; // because max value is 254
    uint8_t distancesChecked;
    uint8_t closestClusterIndex;
    float smallestDistance;

    void splitAndLoad(std::string &valueString);

public:
    // constructor
    TrainSample(std::string &valueString);

    void checkDistanceForMin(float distance);

    std::vector<float>* getValues();

    int getSampleSize();
    uint8_t getClosestCenterId();

    void resetAttributes();
};
