#ifndef CLUSTER_ANA
#define CLUSTER_ANA

#include <iostream>
#include "math.h"




namespace ClusterAnalysis {

    float distance(std::vector<float> *firstVector, std::vector<float> *secondVector) {

        float sum = 0;

        for (int i = 0; i < firstVector->size(); i++) {
            float diff = firstVector->at(i) - secondVector->at(i);
            sum += (diff * diff);
        }

        return sqrt(sum);
    }

    std::vector<float> *gammaAndAlpha(std::vector<TrainSample> *trainSamples, std::vector<float>* distances) {

        float gamma = 0,
                alpha = 0;

        for (int i = 0; i < trainSamples->size() - 1; i++) {
            for (int j = i + 1; j < trainSamples->size(); j++) {

                TrainSample sampleI = trainSamples->at(i),
                        sampleJ = trainSamples->at(j);

                if (sampleI.getClosestCenterId() == sampleJ.getClosestCenterId()) {
                    alpha++;
                    float dist = distance(sampleI.getValues(), sampleJ.getValues());
                    distances->push_back(dist);
                    gamma += dist;
                }
            }
        }

        auto *output = new std::vector<float>{alpha, gamma};
        return output;
    }



    float computeMin(std::vector<float>* distances, float alpha){

        std::vector<float> distancesTemp = (*distances);

        float output = 0;
        int alphaI = (int)alpha;

        for(int j = 0; j < alphaI && j < distancesTemp.size(); j++) {
            int minIndex = 0;
            float min = MAXFLOAT;


            for (int i = 0; i < distancesTemp.size(); i++) {
                if (distancesTemp[i] < min) {
                    min = distancesTemp[i];
                    minIndex = i;
                }
            }
            output += min;
            distancesTemp[minIndex] = MAXFLOAT;
        }

        return output;
    }


    float computeMax(std::vector<float>* distances, float alpha){

        std::vector<float> distancesTemp = (*distances);

        float output = 0;
        int alphaI = (int)alpha;

        for(int j = 0; j < alphaI && j < distancesTemp.size(); j++) {
            int maxIndex = 0;
            float max = -100;


            for (int i = 0; i < distancesTemp.size(); i++) {
                if (distancesTemp[i] > max) {
                    max = distancesTemp[i];
                    maxIndex = i;
                }
            }
            output += max;
            distancesTemp[maxIndex] = -100;
        }

        return output;
    }



    void cIndex( std::vector<TrainSample> *trainSamples) {

        auto* distances = new std::vector<float>;

        std::vector<float> *gammaAlpha = gammaAndAlpha(trainSamples, distances);

        float alpha = gammaAlpha->at(0),
                gamma = gammaAlpha->at(1),
                min = computeMin(distances, alpha),
                max = computeMax(distances, alpha),
                c_index = (gamma - min)/(max - min);

        std::cout << "c-Index : " << c_index << std::endl;

        delete(distances);
        delete(gammaAlpha);
    }
}

#endif

