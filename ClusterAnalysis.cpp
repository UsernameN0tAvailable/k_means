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

    std::vector<std::vector<TrainSample> *> *splitSamples(std::vector<TrainSample> *trainSamples, uint8_t k) {

        auto *clusteredSamples = new std::vector<std::vector<TrainSample> *>;
        clusteredSamples->reserve(k);

        for (int i = 0; i < k; i++) {
            auto *tmp = new std::vector<TrainSample>;
            clusteredSamples->push_back(tmp);
        }


        for (int i = 0; i < trainSamples->size(); i++)
            (*clusteredSamples)[trainSamples->at(i).getClosestCenterId()]->push_back(trainSamples->at(i));

        return clusteredSamples;
    }


    float max(std::vector<float> *distances) {

        float max = -100;

        for (float dist : *distances)
            if (dist > max)
                max = dist;

        return max;
    }

    float min(std::vector<float> *distances) {

        float min = MAXFLOAT;

        for (float dist : *distances)
            if (dist < min)
                min = dist;

        return min;
    }


    namespace dunnIndex {

        float computeDiameter(std::vector<TrainSample> *samples) {

            std::vector<float> distances;

            for (int i = 0; i < samples->size() - 1; i++)
                for (int j = i + 1; j < samples->size(); j++)
                    distances.push_back(distance(samples->at(i).getValues(), samples->at(j).getValues()));

            return max(&distances);
        }

        float minDiameter(std::vector<std::vector<TrainSample> *> *clusteredSamples) {
            std::vector<float> diameters;
            diameters.reserve(clusteredSamples->size());

            for (auto list : *clusteredSamples)
                diameters.push_back(computeDiameter(list));

            return min(&diameters);
        }


        float clusterDistance(std::vector<TrainSample> *firstCluster, std::vector<TrainSample> *secondCluster) {

            std::vector<float> distances;
            distances.reserve(firstCluster->size() * secondCluster->size());

            for (auto firstSample : *firstCluster)
                for (auto secondSample : *secondCluster)
                    distances.push_back(distance(firstSample.getValues(), secondSample.getValues()));

            return min(&distances);
        }

        float minClusterDistance(std::vector<std::vector<TrainSample> *> *clusters) {

            std::vector<float> distances;
            distances.reserve(clusters->size());

            for (int i = 0; i < clusters->size(); i++)
                for (int j = i + 1; j < clusters->size(); j++)
                    distances.push_back(clusterDistance((clusters->at(i)), (clusters->at(j))));

            return min(&distances);
        }

        float analyze(std::vector<TrainSample> *trainSamples, uint8_t k) {

            std::vector<std::vector<TrainSample>*> *clusteredSamples = splitSamples(trainSamples, k);

            float maxDiameter = minDiameter(clusteredSamples),
                    minClusterDis = minClusterDistance(clusteredSamples);

            delete (clusteredSamples);
            return minClusterDis / maxDiameter;
        }
    }


    namespace davisBouldinIndex{

        void sumVector(std::vector<float>* sum, std::vector<float>* vector){

            for(int i = 0; i < sum->size(); i++){
                (*sum)[i] += vector->at(i);
            }
        }


        std::vector<float>* sumVectors(std::vector<TrainSample*>* samples){

            auto* sum = new std::vector<float>(samples->at(0)->getValues()->size(), 10);

            for(TrainSample* sample : *samples)
                sumVector(sum, sample->getValues());

            return sum;
        }


        float vectorLength(std::vector<float>* vector){

            float sum = 0;
            for(float entry : *vector )
                sum += entry;

            return sqrt(sum);
        }

        void vectorDivide(std::vector<float>* vector, float length){
            for(int i = 0; i < vector->size(); i++)
                (*vector)[i] /= length;

        }


        std::vector<float>* m_i(std::vector<TrainSample*>* samples, ClusterCenter* center){

            if(samples->at(0)->getClosestCenterId() == center->getId())
                std::cout << "correct!!" << std::endl;

            float length = vectorLength(center->getValues());
            std::vector<float>* output = sumVectors(samples);
            vectorDivide(output, length);

            return output;
        }

        float d_i(){
            // TODO: compute vector length here!!! not in m_i

        }

        float analyze(std::vector<TrainSample>* trainSamples, std::vector<ClusterCenter>* centers){

            std::vector<std::vector<TrainSample>*> *clusteredSamples = splitSamples(trainSamples, centers->size());




            delete(clusteredSamples);
            return 0.0;
        }

    }
}

#endif

