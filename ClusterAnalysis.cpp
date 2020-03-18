#ifndef CLUSTER_ANA
#define CLUSTER_ANA

#include <iostream>
#include "math.h"

namespace ClusterAnalysis {

    /**
     * computes distance between two vectors
     *
     * @param firstVector
     * @param secondVector
     * @return distance value
     */
    float distance(std::vector<float> *firstVector, std::vector<float> *secondVector) {

        float sum = 0;

        for (int i = 0; i < firstVector->size(); i++) {
            float diff = firstVector->at(i) - secondVector->at(i);
            sum += (diff * diff);
        }

        return sqrt(sum);
    }

    /**
     * splits training samples according
     * to their associated cluster center
     *
     * @param trainSamples
     * @param k
     * @return
     */
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

    /**
     * compute maximum value from
     * an array
     *
     * @param distances
     * @return biggest element
     */
    float max(std::vector<float> *distances) {

        float max = -100;

        for (float dist : *distances)
            if (dist > max)
                max = dist;

        return max;
    }

    /**
     * computes minimum value from an array
     *
     * @param distances
     * @return smallest element value
     */
    float min(std::vector<float> *distances) {

        float min = MAXFLOAT;

        for (float dist : *distances)
            if (dist < min)
                min = dist;

        return min;
    }

    /**
     * Dunn-Index implementation
     */
    namespace dunnIndex {

        /**
         * computes diameter, page 26 document: pr-lecture3.pdf
         * @param samples
         * @return diameter
         */
        float computeDiameter(std::vector<TrainSample> *samples) {

            std::vector<float> distances;
            int distancesSize = ((int) samples->size() * ((int) samples->size() + 1)) / 2;
            distances.reserve(distancesSize);

            for (int i = 0; i < samples->size() - 1; i++)
                for (int j = i + 1; j < samples->size(); j++)
                    distances.push_back(distance(samples->at(i).getValues(), samples->at(j).getValues()));

            return max(&distances);
        }

        /**
         * computes smallest diameter among all
         * training samples clustered in a single cluster
         * @param clusteredSamples
         * @return smallest diameter
         */
        float minDiameter(std::vector<std::vector<TrainSample> *> *clusteredSamples) {
            std::vector<float> diameters;
            diameters.reserve(clusteredSamples->size());

            for (auto list : *clusteredSamples)
                diameters.push_back(computeDiameter(list));

            return min(&diameters);
        }

        /**
         * distance between two clusters
         * definition at page 26 of pr-lecture3.pdf
         * @param firstCluster
         * @param secondCluster
         * @return
         */
        float clusterDistance(std::vector<TrainSample> *firstCluster, std::vector<TrainSample> *secondCluster) {

            std::vector<float> distances;
            distances.reserve(firstCluster->size() * secondCluster->size());

            for (auto firstSample : *firstCluster)
                for (auto secondSample : *secondCluster)
                    distances.push_back(distance(firstSample.getValues(), secondSample.getValues()));

            return min(&distances);
        }

        /**
         * computes smallest cluster distance
         * @param clusters
         * @return smallest cluster distance
         */
        float minClusterDistance(std::vector<std::vector<TrainSample> *> *clusters) {

            std::vector<float> distances;
            distances.reserve(clusters->size());

            for (int i = 0; i < clusters->size(); i++)
                for (int j = i + 1; j < clusters->size(); j++)
                    distances.push_back(clusterDistance((clusters->at(i)), (clusters->at(j))));

            return min(&distances);
        }

        /**
         * comutes Dunn-Index
         * @param training samples
         * @param k number of clusters
         * @return Dunn-Index
         */
        float analyze(std::vector<TrainSample> *trainSamples, uint8_t k) {


            std::vector<std::vector<TrainSample> *> *clusteredSamples = splitSamples(trainSamples, k);

            float maxDiameter = minDiameter(clusteredSamples),
                    minClusterDis = minClusterDistance(clusteredSamples);

            delete (clusteredSamples);
            return minClusterDis / maxDiameter;
        }
    }


    namespace davisBouldinIndex {

        /**
         * computes sum of two vectors
         * @param sum result
         * @param vector to be added
         */
        void sumVector(std::vector<float> *sum, std::vector<float> *vector) {

            for (int i = 0; i < sum->size(); i++) {
                (*sum)[i] += vector->at(i);
            }
        }

        /**
         * sums all the vectors passed in together
         *
         * @param samples to sum together
         * @return vector sum
         */
        std::vector<float> *sumVectors(std::vector<TrainSample> *samples) {

            auto *sum = new std::vector<float>(785, 0);

            for (TrainSample sample : *samples)
                sumVector(sum, sample.getValues());

            return sum;
        }

        /**
         * computes the length of a vector
         *
         * @param vector
         * @return length value
         */
        float vectorLength(std::vector<float> *vector) {

            float sum = 0;
            for (float entry : *vector)
                sum += (entry * entry);


            return sqrt(sum);
        }

        /**
         * Divides vectors with a scalar
         * @param vector to be divided
         * @param length scalar value
         */
        void vectorDivide(std::vector<float> *vector, float length) {
            for (int i = 0; i < vector->size(); i++)
                (*vector)[i] /= length;
        }

        /**
         * m_i definition at page 27 on pr-lecture3.pdf
         * @param samples
         * @param centerSize
         * @return m_i vector
         */
        std::vector<float> *m(std::vector<TrainSample> *samples, float centerSize) {

            std::vector<float> *output = sumVectors(samples);
            vectorDivide(output, centerSize);

            return output;
        }

        /**
         * d_i definiton on page 27 on pr-lecture3.pdf
         * @param samples
         * @param centerSize
         * @param m def on page 27 on pr-lecture3.pdf
         * @return d_i
         */
        float d(std::vector<TrainSample> *samples, float centerSize, std::vector<float> *m) {
            float output = 0;

            for (TrainSample sample : *samples)
                output += distance(sample.getValues(), m);

            return output / centerSize;
        }

        /**
         * definition of page 27 on pr-lecture3.pdf
         *
         * @param samples_i samples assigned to cluster center j
         * @param samples_j samples assigned to cluster center i
         * @param center_i
         * @param center_j
         * @return R_i_j
         */
        float
        R_i_j(std::vector<TrainSample> *samples_i, std::vector<TrainSample> *samples_j, ClusterCenter *center_i,
              ClusterCenter *center_j) {

            float centerSize_i = vectorLength(center_i->getValues()),
                    centerSize_j = vectorLength(center_j->getValues());


            std::vector<float> *m_i = m(samples_i, centerSize_i),
                    *m_j = m(samples_j, centerSize_j);

            float d_i = d(samples_i, centerSize_i, m_i),
                    d_j = d(samples_j, centerSize_j, m_j),
                    dist_m_i_m_j = (distance(m_i, m_j));

            return (d_i + d_j) / dist_m_i_m_j;
        }

        /**
         * R_i definiton on page 27 of pr-lecture3.pdf
         *
         * @param clusteredSamples all training samples
         * @param centers all cluster centers
         * @param i index to compute
         * @return R_i
         */
        float
        R_i(std::vector<std::vector<TrainSample> *> *clusteredSamples, std::vector<ClusterCenter> *centers, int i) {

            std::vector<float> rs;

            for (int j = 0; j < clusteredSamples->size(); j++)
                if (i != j) {
                    rs.push_back(R_i_j(
                            clusteredSamples->at(i),
                            clusteredSamples->at(j),
                            &(centers->at(i)),
                            &(centers->at(j))
                    ));
                }

            return max(&rs);
        }

        /**
         * Executes Davis-Bouldin-Index
         *
         * @param trainSamples
         * @param centers
         * @return
         */
        float analyze(std::vector<TrainSample> *trainSamples, std::vector<ClusterCenter> *centers) {

            std::vector<std::vector<TrainSample> *> *clusteredSamples = splitSamples(trainSamples, centers->size());

            float sum = 0;

            for (int i = 0; i < clusteredSamples->size(); i++)
                sum += R_i(clusteredSamples, centers, i);

            delete (clusteredSamples);
            return sum / centers->size();
        }
    }
}

#endif

