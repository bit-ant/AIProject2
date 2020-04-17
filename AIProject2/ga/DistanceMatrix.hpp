#ifndef GA_DISTANCEMATRIX_HPP
#define GA_DISTANCEMATRIX_HPP

#include <map>

namespace ga {

class DistanceMatrix
{
private:
    // The distance matrix is implemented as follows:
    // Assume city 1 and city 2 and their distance.
    // A map contains city 1 as key and its value points to another map.
    // The second map contains city 2 as key and the distance between city 1.
    std::map < unsigned int, std::map < unsigned int, float > > m_Matrix;
public:
    DistanceMatrix(const size_t dimension);

    float getDistanceBetweenCities(const unsigned int first, const unsigned int second) const;
};

} // namespace ga

#endif // GA_DISTANCEMATRIX_HPP
