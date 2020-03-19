/**
 * Author: Mattia Pedrazzi 14-727-622
 *
 *
 * How to use:
 *
 * 1) compile:
 * g++ main.cpp -o output -O3
 *
 * 2) execute
 * ./output <absolute file path of training set csv> <number of clusters>
 *
 * 3) example
 * ./output /home/myself/train.csv 3
 *
 *
 * ==================================================================
 *
 * The documentation is in the header files.
 * The documentation for the cluster analysis algorithms
 * is located inside of ClusterAnalysis.cpp
 *
 * ==================================================================
 *
 * NOTE:
 *
 * I honestly did not quite understand the definition of cluster distance on page 26 of pr-lecture3.pdf,
 * so I implemented two version of it, namely ClusterAnalysis::dunnIndex::minClusterDistanceLecture (which is what I
 * interpreted from the lecture) and ClusterAnalysis:dunnIndex::minClusterDistance (which is what I find more
 * reasonable).
 *
 *
 * p.s: I apologize for writing this in C++, I regret this and I will never do it again.
 *
 *
 */


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
        if (nearestSamplesCount > 0)
            for (int index = 0; index < meanPosition->size(); index++)
                (*meanPosition)[index] = (*meanPosition)[index] / nearestSamplesCount;


        // puts is to zero if no samples are assigned to cluster center
        clusterIterator->reposition(meanPosition);

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

    do {
        for (ClusterCenter clusterCenter : clusterCenters)
            clusterCenter.computeDistances(); // compute distances

        repositionCenters(&clusterCenters, samples);
        resetSamplesAttributes(samples);
        replacements++;

    } while (  biggestRepDelta(clusterCenters) > repositionDeltaLimit);

    using namespace ClusterAnalysis;
    float dunnIndex = dunnIndex::analyze(samples, &clusterCenters);
    float DB = davisBouldinIndex::analyze(samples, &clusterCenters);

    std::cout << "k : " << k << std::endl;
    std::cout << "stop criterion, minimum reposition delta of : " << repositionDeltaLimit << std::endl;
    std::cout << "total cluster center replacements : " << replacements << std::endl;
    std::cout << "Dunn Index : " << dunnIndex << std::endl;
    std::cout <<  "Davis-Boulden Index : " << DB << std::endl;

    delete (samples);
    return 0;
}



