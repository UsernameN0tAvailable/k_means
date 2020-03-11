#pragma once

class TrainSample {

private:
    std::vector<uint8_t> values; // because max value is 254
    std::vector<float> distancesToCenters;

    void splitAndSave(std::string &valueString);

public:
    // constructor
    TrainSample(std::string &valueString);

    void pushDistance(float distance);


    std::vector<uint8_t>* getValues();
    int getSampleSize();

};
