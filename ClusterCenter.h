#include "TrainSample.h"

class ClusterCenter {

private:
    uint8_t id;
    std::vector<float>* values;
    std::vector<TrainSample>* trainSamples;
    float repositionDelta;

    void distance(std::vector<float>* firstVector, std::vector<float>* secondVector);

public:
/**
 * Constructs random vector
 *
 * @param size number of entries of the vector
 * @param maxValue  maximum value of the vector entries
 */
    ClusterCenter(int size, int maxValue, std::vector<TrainSample>* trainSamples, uint8_t id);

    void computeDistances();

    void reposition(std::vector<float>* newValues);

    float getLastRepDelta();
    uint8_t getId();

};