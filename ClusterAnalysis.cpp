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

            std::vector<std::vector<TrainSample> *> *clusteredSamples = splitSamples(trainSamples, k);

            float maxDiameter = minDiameter(clusteredSamples),
                    minClusterDis = minClusterDistance(clusteredSamples);

            delete (clusteredSamples);
            return minClusterDis / maxDiameter;
        }
    }


    namespace davisBouldinIndex {

        void sumVector(std::vector<float> *sum, std::vector<float> *vector) {

            for (int i = 0; i < sum->size(); i++) {
                (*sum)[i] += vector->at(i);
            }
        }


        std::vector<float> *sumVectors(std::vector<TrainSample> *samples) {

            auto *sum = new std::vector<float>(785, 0);

            for (TrainSample sample : *samples)
                sumVector(sum, sample.getValues());

            return sum;
        }


        float vectorLength(std::vector<float> *vector) {

            float sum = 0;
            for (float entry : *vector)
                sum += (entry * entry);




            return sqrt(sum);
        }

        void vectorDivide(std::vector<float> *vector, float length) {
            for (int i = 0; i < vector->size(); i++)
                (*vector)[i] /= length;
        }


        std::vector<float> *m(std::vector<TrainSample> *samples, float centerSize) {

            std::vector<float> *output = sumVectors(samples);
            vectorDivide(output, centerSize);

            return output;
        }

        float d(std::vector<TrainSample> *samples, float centerSize, std::vector<float> *m) {
                    float output = 0;

            for (TrainSample sample : *samples)
                output += distance(sample.getValues(), m);


            return output / centerSize;
        }

        float
        R_i_j(std::vector<TrainSample> *samples_i, std::vector<TrainSample> *samples_j, ClusterCenter *center_i,
              ClusterCenter *center_j) {

            float centerSize_i = vectorLength(center_i->getValues()),
             centerSize_j = vectorLength(center_j->getValues());


            std::vector<float>* m_i = m(samples_i, centerSize_i),
                    *m_j = m(samples_j, centerSize_j);

            float d_i = d(samples_i, centerSize_i, m_i),
                    d_j = d(samples_j, centerSize_j, m_j),
                    dist_m_i_m_j = (distance(m_i, m_j));

            return (d_i + d_j)/dist_m_i_m_j;
        }

        float R_i(std::vector<std::vector<TrainSample>*> *clusteredSamples, std::vector<ClusterCenter> *centers, int i){

            std::vector<float> rs;

                for(int j = 0; j < clusteredSamples->size(); j++)
                    if(i != j){
                        rs.push_back(R_i_j(
                                clusteredSamples->at(i),
                                clusteredSamples->at(j),
                                &(centers->at(i)),
                                &(centers->at(j))
                                ));
                    }

            return max(&rs);
        }

        float analyze(std::vector<TrainSample> *trainSamples, std::vector<ClusterCenter> *centers) {

            std::vector<std::vector<TrainSample>*> *clusteredSamples = splitSamples(trainSamples, centers->size());

            float sum = 0;

            for(int i = 0; i < clusteredSamples->size(); i++)
                sum += R_i(clusteredSamples, centers, i);


            delete (clusteredSamples);
            return sum/centers->size();
        }
    }
}

#endif

