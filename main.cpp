#include <iostream>
#include <vector>
#include <fstream>
#include "TrainSample.cpp"
#include "ClusterCenter.cpp"

std::vector<TrainSample>* readAndLoadSamples (std::string& filePath) {

    std::string line;
    std::ifstream myFile (filePath);

    auto* samples = new std::vector<TrainSample>;

    if(myFile.is_open())
        while(getline(myFile, line))
            samples->emplace_back(line);
    else
        std::cout << "failed to open" << std::endl;


    return samples;
}

int main() {

    const uint8_t k = 3;



    // read and load train samples
    std::string filePath = "/home/myself/CLionProjects/k_means/train.csv";
    std::vector<TrainSample>* samples = readAndLoadSamples(filePath);
    const int sampleSize = samples->at(0).getSampleSize(); // assuming all samples have same size

    // create cluster centers
    // initialize cluster centers
    std::vector<ClusterCenter> clusterCent;
    clusterCent.reserve(k);
    for(uint8_t i = 0; i < k; i++)
        clusterCent.emplace_back(sampleSize, 254, samples);

    for(auto it = samples->begin(); it != samples->end(); ++it){

    }

    std::cout << "done" << std::endl;
    delete(samples);
    return 0;
}



