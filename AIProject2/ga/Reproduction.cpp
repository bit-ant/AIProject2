#include "Reproduction.hpp"
#include <algorithm>
#include "ParentSelection.hpp"
#include "Fitness.hpp"

namespace ga {

Reproduction::Reproduction()
{

}

void Reproduction::reproduce(std::vector< Chromosome >& population, const DistanceMatrix& distanceMatrix, const bool& useRoulette)
{
    // Select parents and crossover
    Chromosome firstParent;
    Chromosome secondParent;
    if (useRoulette)
    {
        firstParent = ParentSelection::rouletteWheelParentSelection(population, distanceMatrix);
    }
    else
    {
        firstParent = ParentSelection::rankParentSelection(population, distanceMatrix);
    }
    std::vector< Chromosome > populationWithoutSelected;
    for (const auto& c : population)
    {
        if (c.getChromosomeGenes() != firstParent.getChromosomeGenes())
        {
            populationWithoutSelected.push_back(c);
        }
    }
    if (useRoulette)
    {
        secondParent = ParentSelection::rouletteWheelParentSelection(populationWithoutSelected, distanceMatrix);
    }
    else
    {
        secondParent = ParentSelection::rankParentSelection(populationWithoutSelected, distanceMatrix);
    }

    std::vector< Chromosome > children = crossover(firstParent, secondParent);

    for (const auto& c : children)
    {
        population.push_back(c);
    }

    // Mutate some chromosomes. The number of mutations is hardcoded to 1/10th of the population
    for (unsigned int i = 0; i < population.size()/10; i ++)
    {
        const size_t randomPos = static_cast< size_t >(rand()) % population.size();
        population[randomPos] = mutate(population[randomPos]);
    }

    // Eliminate less fit. Hardcored to 1/5th of the population
    // The chromosomes to be removed are the less fit
    std::vector < std::pair < Chromosome, float > > populationWithTotalDistance;
    // Calculate fitness for all chromosomes
    for (const auto& c : population)
    {
        populationWithTotalDistance.emplace_back(c, Fitness::calculateFitness(c, distanceMatrix));
    }
    // Sort based on fitness
    std::sort(populationWithTotalDistance.begin(), populationWithTotalDistance.end(),
              [](const std::pair < Chromosome, float >& left, const std::pair < Chromosome, float >& right)
                {
                    return left.second > right.second;
                }
              );
    // Delete less fit chromosomes
    populationWithTotalDistance.resize(populationWithTotalDistance.size() - populationWithTotalDistance.size() / 5);

    // Update the population with the new elements
    population.clear();
    for (const auto& c : populationWithTotalDistance)
    {
        population.push_back(c.first);
    }
}

std::vector< Chromosome > Reproduction::crossover(const Chromosome& firstParent, const Chromosome& secondParent)
{
    std::vector< Chromosome > children;

    // Generate random positions to determine where crossover shall occur
    std::vector < unsigned int > genes = firstParent.getChromosomeGenes();
    const size_t randomPos1 = static_cast< size_t >(rand()) % genes.size();
    const size_t randomPos2 = static_cast< size_t >(rand()) % genes.size();

    std::vector < unsigned int > selectedPartFirstParent;
    std::vector < unsigned int > selectedPartSecondParent;

    size_t startGene;
    size_t endGene;

    if (randomPos1 < randomPos2)
    {
        startGene = randomPos1;
        endGene = randomPos2;
    }
    else
    {
        startGene = randomPos2;
        endGene = randomPos1;
    }
    for (size_t i = startGene; i < endGene; i++)
    {
        selectedPartFirstParent.push_back(firstParent.getChromosomeGenes()[i]);
        selectedPartSecondParent.push_back(secondParent.getChromosomeGenes()[i]);
    }

    Chromosome firstChild;
    Chromosome secondChild;

    std::vector < unsigned int > firstChildGenes;
    std::vector < unsigned int > secondChildGenes;

    // Add genes from parent to child up to before the selected gene part
    for (size_t i = 0; i < startGene; i++)
    {
        if (std::find(selectedPartFirstParent.begin(), selectedPartFirstParent.end(), secondParent.getChromosomeGenes()[i])
                == selectedPartFirstParent.end() &&
                std::find(firstChildGenes.begin(), firstChildGenes.end(), secondParent.getChromosomeGenes()[i])
                == firstChildGenes.end())
        {
            // No duplicate, we can add the gene in the new child
            firstChildGenes.push_back(secondParent.getChromosomeGenes()[i]);
        }
        else
        {
            // We don't want duplication in the gene, look for the next available unique gene
            for (size_t j = i + 1; j < secondParent.getChromosomeGenes().size(); j++)
            {
                if (std::find(selectedPartFirstParent.begin(), selectedPartFirstParent.end(), secondParent.getChromosomeGenes()[j])
                        == selectedPartFirstParent.end() &&
                        std::find(firstChildGenes.begin(), firstChildGenes.end(), secondParent.getChromosomeGenes()[j])
                        == firstChildGenes.end()) // Make sure no duplicates exist!
                {
                    // No duplicate, we can add the gene in the new child
                    firstChildGenes.push_back(secondParent.getChromosomeGenes()[j]);
                    break;
                }
            }
        }
    }

    // Now add the selected gene part to the child
    size_t pos = 0;
    for (size_t i = startGene; i < endGene; i++)
    {
        firstChildGenes.push_back(selectedPartFirstParent[pos]);
        ++pos;
    }

    // Finally, fill the rest of the child chromosome
    for (size_t i = endGene; i < secondParent.getChromosomeGenes().size(); i++)
    {
        if (std::find(selectedPartFirstParent.begin(), selectedPartFirstParent.end(), secondParent.getChromosomeGenes()[i])
                == selectedPartFirstParent.end() &&
                std::find(firstChildGenes.begin(), firstChildGenes.end(), secondParent.getChromosomeGenes()[i])
                == firstChildGenes.end())
        {
            // No duplicate, we can add the gene in the new child
            firstChildGenes.push_back(secondParent.getChromosomeGenes()[i]);
        }
        else
        {
            // We don't want duplication in the gene, look for the next available unique gene
            for (size_t j = i; j < secondParent.getChromosomeGenes().size(); j++)
            {
                if (std::find(selectedPartFirstParent.begin(), selectedPartFirstParent.end(), secondParent.getChromosomeGenes()[j])
                        == selectedPartFirstParent.end() &&
                        std::find(firstChildGenes.begin(), firstChildGenes.end(), secondParent.getChromosomeGenes()[j])
                        == firstChildGenes.end()) // Make sure no duplicates exist!
                {
                    // No duplicate, we can add the gene in the new child
                    firstChildGenes.push_back(secondParent.getChromosomeGenes()[j]);
                    break;
                }
            }
            // It is possible we reached the end without filling up the child gene to the required size
            // Iterate from the beginning of the parent to get the rest of the values
            if (firstChildGenes.size() != secondParent.getChromosomeGenes().size())
            {
                for (size_t i = 0; i < secondParent.getChromosomeGenes().size(); i++)
                {
                    if (std::find(selectedPartFirstParent.begin(), selectedPartFirstParent.end(), secondParent.getChromosomeGenes()[i])
                            == selectedPartFirstParent.end() &&
                            std::find(firstChildGenes.begin(), firstChildGenes.end(), secondParent.getChromosomeGenes()[i])
                            == firstChildGenes.end())
                    {
                        // No duplicate, we can add the gene in the new child
                        firstChildGenes.push_back(secondParent.getChromosomeGenes()[i]);
                    }
                    if (firstChildGenes.size() == secondParent.getChromosomeGenes().size())
                    {
                        break;
                    }
                }
            }
        }
    }

    // Similar process for the second child...
    for (size_t i = 0; i < startGene; i++)
    {
        if (std::find(selectedPartSecondParent.begin(), selectedPartSecondParent.end(), firstParent.getChromosomeGenes()[i])
                == selectedPartSecondParent.end() &&
                std::find(secondChildGenes.begin(), secondChildGenes.end(), firstParent.getChromosomeGenes()[i])
                == secondChildGenes.end())
        {
            // No duplicate, we can add the gene in the new child
            secondChildGenes.push_back(firstParent.getChromosomeGenes()[i]);
        }
        else
        {
            // We don't want duplication in the gene, look for the next available unique gene
            for (size_t j = i + 1; j < firstParent.getChromosomeGenes().size(); j++)
            {
                if (std::find(selectedPartSecondParent.begin(), selectedPartSecondParent.end(), firstParent.getChromosomeGenes()[j])
                        == selectedPartSecondParent.end() &&
                        std::find(secondChildGenes.begin(), secondChildGenes.end(), firstParent.getChromosomeGenes()[j])
                        == secondChildGenes.end()) // Make sure no duplicates exist!
                {
                    // No duplicate, we can add the gene in the new child
                    secondChildGenes.push_back(firstParent.getChromosomeGenes()[j]);
                    break;
                }
            }
        }
    }

    // Now add the selected gene part to the child
    pos = 0;
    for (size_t i = startGene; i < endGene; i++)
    {
        secondChildGenes.push_back(selectedPartSecondParent[pos]);
        ++pos;
    }

    // Finally, fill the rest of the child chromosome
    for (size_t i = endGene; i < firstParent.getChromosomeGenes().size(); i++)
    {
        if (std::find(selectedPartSecondParent.begin(), selectedPartSecondParent.end(), firstParent.getChromosomeGenes()[i])
                == selectedPartSecondParent.end() &&
                std::find(secondChildGenes.begin(), secondChildGenes.end(), firstParent.getChromosomeGenes()[i])
                == secondChildGenes.end())
        {
            // No duplicate, we can add the gene in the new child
            secondChildGenes.push_back(firstParent.getChromosomeGenes()[i]);
        }
        else
        {
            // We don't want duplication in the gene, look for the next available unique gene
            for (size_t j = i; j < firstParent.getChromosomeGenes().size(); j++)
            {
                if (std::find(selectedPartSecondParent.begin(), selectedPartSecondParent.end(), firstParent.getChromosomeGenes()[j])
                        == selectedPartSecondParent.end() &&
                        std::find(secondChildGenes.begin(), secondChildGenes.end(), firstParent.getChromosomeGenes()[j])
                        == secondChildGenes.end()) // Make sure no duplicates exist!
                {
                    // No duplicate, we can add the gene in the new child
                    secondChildGenes.push_back(firstParent.getChromosomeGenes()[j]);
                    break;
                }
            }
            // It is possible we reached the end without filling up the child gene to the required size
            // Iterate from the beginning of the parent to get the rest of the values
            if (secondChildGenes.size() != firstParent.getChromosomeGenes().size())
            {
                for (size_t i = 0; i < firstParent.getChromosomeGenes().size(); i++)
                {
                    if (std::find(selectedPartSecondParent.begin(), selectedPartSecondParent.end(), firstParent.getChromosomeGenes()[i])
                            == selectedPartSecondParent.end() &&
                            std::find(secondChildGenes.begin(), secondChildGenes.end(), firstParent.getChromosomeGenes()[i])
                            == secondChildGenes.end())
                    {
                        // No duplicate, we can add the gene in the new child
                        secondChildGenes.push_back(firstParent.getChromosomeGenes()[i]);
                    }
                    if (secondChildGenes.size() == firstParent.getChromosomeGenes().size())
                    {
                        break;
                    }
                }
            }
        }
    }

    if (firstChildGenes.size() == firstParent.getChromosomeGenes().size())
    {
        firstChild.setChromosomeGenes(firstChildGenes);
        children.push_back(firstChild);
    }
    if (secondChildGenes.size() == secondParent.getChromosomeGenes().size())
    {
        secondChild.setChromosomeGenes(secondChildGenes);
        children.push_back(secondChild);
    }
    return children;
}

Chromosome Reproduction::mutate(const Chromosome& initialChromosome)
{
    Chromosome mutant;

    std::vector < unsigned int > genes = initialChromosome.getChromosomeGenes();

    // Generate random positions to determine where mutation shall occur
    const size_t randomPos1 = static_cast< size_t >(rand()) % genes.size();
    const size_t randomPos2 = static_cast< size_t >(rand()) % genes.size();

    // Now swap the values in the positions determined by the random values
    const auto tmp = genes[randomPos1];
    genes[randomPos1] = genes[randomPos2];
    genes[randomPos2] = tmp;

    mutant.setChromosomeGenes(genes);

    return mutant;
}

} // namespace ga
