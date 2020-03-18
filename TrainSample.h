#ifndef TRAIN_SAMPLE
#define TRAIN_SAMPLE

class TrainSample {

private:
    std::vector<float> values; // because max value is 254
    uint8_t distancesChecked; // number of checked cluster centers
    uint8_t closestClusterIndex; // index of the closest cluster center
    float smallestDistance;      // value of the smallest distance to cluster center

    // splits string and loads values into this.values
    void splitAndLoad(std::string &valueString);

public:
    /**
     * Constructs training sample
     * from csv string
     * @param valueString
     */
    TrainSample(std::string &valueString);

    /**
     * checks if this distance is
     * smaller than this.smallestDistance
     * @param distance
     */
    void checkDistanceForMin(float distance);

    std::vector<float>* getValues();

    /**
     * returns size of of this.values
     */
    int getSampleSize();

    /**
     * returns the id of closest cluster center
     * @return
     */
    uint8_t getClosestCenterId();

    /**
     * resets this.smallestDistance and
     * this.closestClusterIndex
     */
    void resetAttributes();
};

#endif