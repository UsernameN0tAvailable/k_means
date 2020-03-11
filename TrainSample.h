#pragma once

class TrainSample {

private:
    std::vector<uint8_t> values; // because max value is 254

    void splitAndSave(std::string &valueString);

public:
    // constructor
    TrainSample(std::string &valueString);

    std::vector<uint8_t>* getValues();

    int getSampleSize();
};
