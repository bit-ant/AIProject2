#include "DistanceMatrix.hpp"

namespace ga {

DistanceMatrix::DistanceMatrix(const size_t dimension) : m_Matrix()
{
    for (size_t i = 1; i < dimension; i++)
    {
        std::map < unsigned int, float > secondMap;
        for (size_t j = 1; j < dimension; j++)
        {
            if (i == j)
            {
                secondMap[static_cast< unsigned int >(j)] = 0; // Distance to same city is zero
            }
            else
            {
                secondMap[static_cast< unsigned int >(j)] = rand() % 100 + 1; // Random value from 0 to 100
            }
        }
        m_Matrix[static_cast< unsigned int >(i)] = secondMap;
    }
}

float
DistanceMatrix::getDistanceBetweenCities(const unsigned int first, const unsigned int second) const
{
    float ret = 0;
    if (m_Matrix.count(first))
    {
        const std::map < unsigned int, float > otherCities = m_Matrix.at(first);
        if (otherCities.count(second))
        {
            ret = otherCities.at(second);
        }
    }
    return ret;
}

} // namespace ga
