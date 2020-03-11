#include "TrainSample.h"

class ClusterCenter {

private:
    uint8_t id;
    std::vector<uint8_t> values; // because max value is 254
    std::vector<TrainSample>* trainSamples;
    std::vector<double> distances;


    void computeDistances();

public:

    /**
 * Constructs random vector
 *
 * @param size number of entries of the vector
 * @param maxValue  maximum value of the vector entries
 */
    ClusterCenter(int size, int maxValue, std::vector<TrainSample>* trainSamples, uint8_t id);

    void reposition();

    std::vector<uint8_t>* getValues();


};