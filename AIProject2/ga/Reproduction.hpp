#ifndef GA_REPRODUCTION_HPP
#define GA_REPRODUCTION_HPP

#include "Chromosome.hpp"
#include "DistanceMatrix.hpp"

namespace ga {

class Reproduction
{
public:
    Reproduction();

    // Returns an updated population with new children
    void reproduce(std::vector< Chromosome >& population, const DistanceMatrix &distanceMatrix, const bool &useRoulette);

private:
    std::vector< Chromosome > crossover(const Chromosome& firstParent, const Chromosome& secondParent);

    Chromosome mutate(const Chromosome& initialChromosome);
};

} // namespace ga

#endif // GA_REPRODUCTION_HPP
