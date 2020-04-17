#ifndef GA_PARENTSELECTION_HPP
#define GA_PARENTSELECTION_HPP

#include <vector>
#include "Chromosome.hpp"
#include "DistanceMatrix.hpp"

namespace ga {

class ParentSelection
{
public:
    ParentSelection();

    static Chromosome
    rouletteWheelParentSelection(const std::vector< Chromosome >& population, const DistanceMatrix &distanceMatrix);

    static Chromosome
    rankParentSelection(const std::vector< Chromosome >& population, const DistanceMatrix& distanceMatrix);
};

} // namespace ga

#endif // GA_PARENTSELECTION_HPP
