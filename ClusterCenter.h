#ifndef CLUSTER_CENTER
#define CLUSTER_CENTER

class ClusterCenter {

private:
    uint8_t id;                     // cluster id
    std::vector<float>* values;     // cluster id
    std::vector<TrainSample>* trainSamples; // all training samples
    float repositionDelta;                  // the computeRepositionDelta between the last position and the new position

    // compute delta between last and new position
    void computeRepositionDelta(std::vector<float>* firstVector, std::vector<float>* secondVector);

public:
/**
 * Constructs random vector
 *
 * @param size number of entries of the vector
 * @param maxValue  maximum value of the vector entries
 */
    ClusterCenter(int size, int maxValue, std::vector<TrainSample>* trainSamples, uint8_t id);

    /**
     * Computes computeRepositionDelta between
     * this cluster center and all
     * and the TrainSample computes
     * if this cluster center is the closest one
     */
    void computeDistances();

    /**
     * repositions the cluster center
     * @param newValues the new position of this cluster
     * center
     */
    void reposition(std::vector<float>* newValues);

    /**
     * @return last reposition delta
     */
    float getLastRepDelta();

    uint8_t getId();
    std::vector<float>* getValues();

};

#endif
