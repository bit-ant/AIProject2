#include "Fitness.hpp"

namespace ga {

Fitness::Fitness()
{

}

float
Fitness::calculateFitness(const Chromosome& chromosome, const DistanceMatrix& distanceMatrix)
{
    float ret = 0;
    const std::vector< unsigned int > chromosomeGenes = chromosome.getChromosomeGenes();
    if (chromosomeGenes.size() > 1) // Error check
    {
        for (unsigned int i = 0; i < chromosomeGenes.size() - 1; i++)
        {
            ret += distanceMatrix.getDistanceBetweenCities(chromosomeGenes[i], chromosomeGenes[i + 1]);
        }
        // Separately calculate distance from end to start, because travelling is supposed to be cyclic
        ret += distanceMatrix.getDistanceBetweenCities(chromosomeGenes.back(), chromosomeGenes.front());

        // Reverse the value, because fitness is supposed to be better when the value is greater
        // This way, bigger overall distance means less fitness and vice versa
        if (ret > 0)
        {
            ret = 1/ret;
        }
    }
    return ret;
}

int
Fitness::calculateTotalDistance(const Chromosome& chromosome, const DistanceMatrix &distanceMatrix)
{
    int ret = 0;
    const std::vector< unsigned int > chromosomeGenes = chromosome.getChromosomeGenes();
    if (chromosomeGenes.size() > 1) // Error check
    {
        for (unsigned int i = 0; i < chromosomeGenes.size() - 1; i++)
        {
            ret += distanceMatrix.getDistanceBetweenCities(chromosomeGenes[i], chromosomeGenes[i + 1]);
        }
        // Separately calculate distance from end to start, because travelling is supposed to be cyclic
        ret += distanceMatrix.getDistanceBetweenCities(chromosomeGenes.back(), chromosomeGenes.front());
    }
    return ret;
}

Chromosome
Fitness::findMostFit(const std::vector< Chromosome >& population, const DistanceMatrix& distanceMatrix)
{
    Chromosome mostFit;
    float maxFitness = 0.0f;

    // Calculate fitness for all chromosomes
    for (const auto& c : population)
    {
        const float fitness = Fitness::calculateFitness(c, distanceMatrix);
        if (fitness > maxFitness)
        {
            maxFitness = fitness;
            mostFit = c;
        }
    }
    return mostFit;
}

} // namespace ga
