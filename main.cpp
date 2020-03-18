#include <iostream>
#include <vector>
#include <fstream>
#include "TrainSample.cpp"
#include "ClusterCenter.cpp"
#include "ClusterAnalysis.cpp"

/**
 * resets the training samples attributes
 * is executed after every cluster center
 * reposition
 * @param samples
 */
void resetSamplesAttributes(std::vector<TrainSample> *samples) {

    for (auto sampleIterator = samples->begin(); sampleIterator != samples->end(); ++sampleIterator)
        sampleIterator->resetAttributes();

}

/**
 * computes the biggest reposition
 * among all cluster centers
 * @param clusterCenters
 * @return
 */
float biggestRepDelta(std::vector<ClusterCenter> &clusterCenters) {

    float max = 0;

    for (ClusterCenter clusterCenter: clusterCenters)
        if (clusterCenter.getLastRepDelta() > max)
            max = clusterCenter.getLastRepDelta();

    return max;
}


/**
 * 1)computes the mean value of all training
 * samples assigned to a cluster center
 * 2) repositions the cluster center
 * using the mean value
 *
 * @param clusterCenters
 * @param trainSamples
 */
void repositionCenters(std::vector<ClusterCenter> *clusterCenters, std::vector<TrainSample> *trainSamples) {

    int sampleSize = trainSamples->at(0).getSampleSize(); // assume everything has the same size

    for (auto clusterIterator = clusterCenters->begin(); clusterIterator != clusterCenters->end(); ++clusterIterator) {

        float nearestSamplesCount = 0;

        auto *meanPosition = new std::vector<float>(sampleSize, 0);

        for (auto trainSampleIterator = trainSamples->begin();
             trainSampleIterator != trainSamples->end(); ++trainSampleIterator) {
            if (trainSampleIterator->getClosestCenterId() == clusterIterator->getId()) {
                nearestSamplesCount++;
                std::vector<float> *sampleValues = trainSampleIterator->getValues();
                // sum
                for (int valuesIndex = 0; valuesIndex < sampleValues->size(); valuesIndex++)
                    (*meanPosition)[valuesIndex] += sampleValues->at(valuesIndex);
            }
        }

        // compute mean values
        if (nearestSamplesCount > 0) {
            for (int index = 0; index < meanPosition->size(); index++)
                (*meanPosition)[index] = (*meanPosition)[index] / nearestSamplesCount;

            clusterIterator->reposition(meanPosition);
        }
    }
}

/**
 * load the train set from file and
 * create TrainSample instances
 * @param filePath
 * @return training samples
 */
std::vector<TrainSample> *readAndLoadSamples(std::string &filePath) {

    std::string line;
    std::ifstream myFile(filePath);

    auto *samples = new std::vector<TrainSample>;

    if (myFile.is_open())
        while (getline(myFile, line))
            samples->emplace_back(line);
    else {
        std::cout << "failed to open" << std::endl;
        exit(0);
    }

    return samples;
}

int main(int argc, char* argv[]) {

    if(argc < 2){
        std::cout << "not enough arguments" << std::endl;
        exit(0);
    }

    const int k = std::stoi(argv[2]),
    repositionDeltaLimit = 1;
    // read and load train samples
    std::string filePath(argv[1]);
    std::vector<TrainSample> *samples = readAndLoadSamples(filePath);
    const int sampleSize = samples->at(0).getSampleSize(); // assuming all samples have same size
    // create cluster centers
    // initialize cluster centers
    std::vector<ClusterCenter> clusterCenters;
    clusterCenters.reserve(k);
    for (uint8_t i = 0; i < k; i++)
        clusterCenters.emplace_back(sampleSize, 254, samples, i);

    /*
     * Clustering
     */
    int replacements = 0;
    std::cout << "clustering ..." << std::endl;

    do {
        for (ClusterCenter clusterCenter : clusterCenters)
            clusterCenter.computeDistances(); // compute distances

        repositionCenters(&clusterCenters, samples);
        resetSamplesAttributes(samples);
        replacements++;

    } while (biggestRepDelta(clusterCenters) > repositionDeltaLimit);

    std::cout << "needed iterations: " << replacements << "\ncluster quality analysis ..." << std::endl;

    using namespace ClusterAnalysis;
    float dunnIndex = dunnIndex::analyze(samples, k);
    float DB = davisBouldinIndex::analyze(samples, &clusterCenters);

    std::cout << "k : " << k << std::endl;
    std::cout << "stop criterion, minimum reposition delta of : " << (unsigned)repositionDeltaLimit << std::endl;
    std::cout << "total cluster center replacements : " << replacements << std::endl;
    std::cout << "Dunn Index : " << dunnIndex << std::endl;
    std::cout <<  "Davis-Boulden Index : " << DB << std::endl;

    delete (samples);
    return 0;
}



