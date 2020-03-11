#include "TrainSample.h"

class ClusterCenter {

private:
    std::vector<uint8_t> values; // because max value is 254
    std::vector<TrainSample>* trainSamples;

public:
    /**
 * Constructs random vector
 *
 * @param size number of entries of the vector
 * @param maxValue  maximum value of the vector entries
 */
    ClusterCenter(int size, int maxValue, std::vector<TrainSample>* trainSamples);

    std::vector<uint8_t>* getValues();

};