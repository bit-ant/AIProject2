#ifndef GA_CHROMOSOME_HPP
#define GA_CHROMOSOME_HPP

#include <vector>

namespace ga {

class Chromosome
{
private:
    // A chromosome is described as a vector of numbers, each number respresenting a city
    std::vector <unsigned int> m_ChromosomeVec;
public:
    Chromosome();
    Chromosome(const size_t chromSize);

    std::vector <unsigned int>
    getChromosomeGenes() const;

    void
    setChromosomeGenes(const std::vector <unsigned int>& genes);
};

} // namespace ga

#endif // GA_CHROMOSOME_HPP
