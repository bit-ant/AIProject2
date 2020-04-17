#include "Chromosome.hpp"

namespace ga {

Chromosome::Chromosome()
    : m_ChromosomeVec()
{
}

Chromosome::Chromosome(const size_t chromSize)
    : m_ChromosomeVec()
{
    m_ChromosomeVec.reserve(chromSize);
}

std::vector <unsigned int>
Chromosome::getChromosomeGenes() const
{
    return m_ChromosomeVec;
}

void
Chromosome::setChromosomeGenes(const std::vector <unsigned int>& genes)
{
    m_ChromosomeVec = genes;
}

} // namespace ga
