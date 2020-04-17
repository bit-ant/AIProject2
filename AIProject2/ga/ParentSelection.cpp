#include "ParentSelection.hpp"
#include "Fitness.hpp"
#include <math.h>
#include <algorithm>

namespace ga {

ParentSelection::ParentSelection()
{

}

Chromosome
ParentSelection::rouletteWheelParentSelection(const std::vector< Chromosome >& population, const DistanceMatrix& distanceMatrix)
{
    Chromosome selectedParent;

    float totalFitness = 0.0;
    for (const Chromosome& c : population)
    {
        totalFitness += Fitness::calculateFitness(c, distanceMatrix);
    }

    float randNum = 0.0;
    if (totalFitness > 0.0f)
    {
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // Values from 0 to 1
        randNum = fmod(r,totalFitness);
    }

    float partialFitness = 0.0;
    for (const Chromosome& c : population)
    {
        partialFitness += Fitness::calculateFitness(c, distanceMatrix);
        if (partialFitness > randNum)
        {
            selectedParent = c;
            break;
        }
    }

    return selectedParent;
}

Chromosome
ParentSelection::rankParentSelection(const std::vector< Chromosome >& population, const DistanceMatrix& distanceMatrix)
{
    Chromosome selectedParent;

    std::vector < std::pair < Chromosome, float > > populationWithTotalFitness;

    // Calculate fitness for all chromosomes
    for (const auto& c : population)
    {
        populationWithTotalFitness.emplace_back(c, Fitness::calculateFitness(c, distanceMatrix));
    }

    // Sort based on fitness
    std::sort(populationWithTotalFitness.begin(), populationWithTotalFitness.end(),
              [](const std::pair < Chromosome, float >& left, const std::pair < Chromosome, float >& right)
                {
                    return left.second > right.second;
                }
              );

    std::vector< Chromosome > rankedPopulation;
    for (const auto& p : populationWithTotalFitness)
    {
        rankedPopulation.push_back(p.first);
    }

    selectedParent = rouletteWheelParentSelection(rankedPopulation, distanceMatrix);

    return selectedParent;
}

} // namespace ga
