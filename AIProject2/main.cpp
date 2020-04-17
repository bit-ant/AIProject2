#include <iostream>
#include "ga/Reproduction.hpp"
#include "ga/Fitness.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

int main()
{
    // Init values
    int chromosomeSize = 100; // The number of cities
    int initialPopulationSize = 30; // Initial population size
    int noOfIterations = 50; // The number of iterations to perform before finding a good solution
    int selectedParentAlgorithm = 0;

    // Read values from cin
    std::cout << "Please enter the chromosome size:\n";
    std::cin >> chromosomeSize;
    std::cout << "Please enter the initial population size:\n";
    std::cin >> initialPopulationSize;
    std::cout << "Please enter the number of iterations to be performed:\n";
    std::cin >> noOfIterations;
    std::cout << "Type 0 for Roulette-Wheel Parent Selection or 1 for Ranked Parent selection:\n";
    std::cin >> selectedParentAlgorithm;


    // 1) Create initial population
    std::vector < ga::Chromosome > population;
    std::vector< unsigned int > randomGene;
    // add 0 - chromosomeSize to the vector
    for(int i = 1; i < chromosomeSize + 1; i++)
    {
        randomGene.push_back(static_cast<unsigned int>(i));
    }
    for(int i = 0; i < initialPopulationSize; i++)
    {
        // Shuffle randomly to create the population
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(randomGene.begin(), randomGene.end(), std::default_random_engine(seed));

        ga::Chromosome c;
        c.setChromosomeGenes(randomGene);
        population.push_back(c);
    }

    // 2) Create distance matrix
    ga::DistanceMatrix distanceMatrix{static_cast< size_t >(chromosomeSize)};

    // 3) Create new chromosomes, delete less fit and evaluate
    ga::Reproduction reproduction{};

    // Begin execution time count
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    // Iterate to get the best solution in the given iteration number
    for (int i = 0; i < noOfIterations; i++)
    {
        reproduction.reproduce(population, distanceMatrix, selectedParentAlgorithm == 0 );

        ga::Chromosome result = ga::Fitness::findMostFit(population, distanceMatrix);

        std::cout << "Most fit: ";
        for (const auto& i : result.getChromosomeGenes())
        {
            std::cout << i << ",";
        }
        std::cout << "distance: " << ga::Fitness::calculateTotalDistance(result, distanceMatrix) << &std::endl;
    }

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Duration: " << duration << " ms" << &std::endl;

    return 0;
}
