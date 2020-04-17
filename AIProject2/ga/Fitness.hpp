#ifndef GA_FITNESS_HPP
#define GA_FITNESS_HPP

#include "Chromosome.hpp"
#include "DistanceMatrix.hpp"

namespace ga {

class Fitness
{
public:
    Fitness();

    static float
    calculateFitness(const Chromosome& chromosome, const DistanceMatrix &distanceMatrix);

    static int
    calculateTotalDistance(const Chromosome& chromosome, const DistanceMatrix &distanceMatrix);

    static Chromosome
    findMostFit(const std::vector< Chromosome >& population, const DistanceMatrix& distanceMatrix);
};

} // namespace ga

#endif // GA_FITNESS_HPP
