#include <iostream>
#include <vector>
#include <fstream>
#include "TrainSample.cpp"
#include "ClusterCenter.cpp"

float biggestDistanceDelta(std::vector<ClusterCenter>& clusterCenters){


    float max = 0;

    for(ClusterCenter clusterCenter: clusterCenters) {

        if (clusterCenter.getLastRepDelta() > max)
            max = clusterCenter.getLastRepDelta();

    }
    return max;
}


void repositionCenters(std::vector<ClusterCenter> *clusterCenters, std::vector<TrainSample> *trainSamples) {

    uint8_t clusterIndex = 0;
    int sampleSize = trainSamples->at(0).getSampleSize(); // assume everything has the same size

    for (auto clusterIterator = clusterCenters->begin(); clusterIterator != clusterCenters->end(); ++clusterIterator) {

        float nearestSamplesCount = 0;

        std::vector<float> meanPosition(sampleSize, 0);

        for(auto trainSampleIterator = trainSamples->begin(); trainSampleIterator != trainSamples->end(); ++ trainSampleIterator){
            if(trainSampleIterator->getClosestCenterId() == clusterIndex) {
                nearestSamplesCount++;
                std::vector<uint8_t> *sampleValues = trainSampleIterator->getValues();
                // sum
                for(int valuesIndex = 0; valuesIndex < sampleValues->size(); valuesIndex++){
                    meanPosition[valuesIndex] += (float)sampleValues->at(0);
                }
            }

            // compute mean values
            for(int index = 0; index < meanPosition.size(); index++)
                meanPosition[index] = meanPosition[index]/nearestSamplesCount;

        }
        clusterIterator->reposition(meanPosition);
        clusterIndex++;
    }
}


std::vector<TrainSample> *readAndLoadSamples(std::string &filePath) {

    std::string line;
    std::ifstream myFile(filePath);

    auto *samples = new std::vector<TrainSample>;

    if (myFile.is_open())
        while (getline(myFile, line))
            samples->emplace_back(line);
    else
        std::cout << "failed to open" << std::endl;

    return samples;
}

int main() {

    const uint8_t k = 3;

    // read and load train samples
    std::string filePath = "/home/myself/CLionProjects/k_means/train.csv";
    std::vector<TrainSample> *samples = readAndLoadSamples(filePath);
    const int sampleSize = samples->at(0).getSampleSize(); // assuming all samples have same size

    // create cluster centers
    // initialize cluster centers
    std::vector<ClusterCenter> clusterCenters;
    clusterCenters.reserve(k);
    for (uint8_t i = 0; i < k; i++)
        clusterCenters.emplace_back(sampleSize, 254, samples, i);

    std::cout << "start" << std::endl;

    while(biggestDistanceDelta(clusterCenters) > 1) {

        for (ClusterCenter clusterCenter : clusterCenters)
            clusterCenter.computeDistances(); // compute distances

        repositionCenters(&clusterCenters, samples);
    }

    std::cout << "done" << std::endl;
    delete (samples);
    return 0;
}



