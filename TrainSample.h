#pragma once

class TrainSample {

private:
    std::vector<uint8_t> values; // because max value is 254
    std::vector<float> distancesToCenters;
    uint8_t distancesChecked;
    uint8_t closestClusterIndex;
    float smallestDistance;

    void splitAndSave(std::string &valueString);

public:
    // constructor
    TrainSample(std::string &valueString);

    void checkDistanceForMin(float distance);

    std::vector<uint8_t>* getValues();

    int getSampleSize();
    uint8_t getClosestCenterId();

    void resetAttributes();
};
