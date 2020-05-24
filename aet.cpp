#include "aet.h"

bool isSlopeZero(const AETNode &node)
{
    return ((node.getYmax() - node.getYmin() == 0) ? true : false);
}


bool CompareNodes(const AETNode& a, const AETNode& b)
{
        //ascending order
        if (a.getYmin() < b.getYmin()) return true;
        if (b.getYmin() < a.getYmin()) return false;

        // a = b for primary condition, go to secondary
        if (a.getYmax() < b.getYmax()) return true;
        if (b.getYmax() < a.getYmax()) return false;

        // a = b for secondary condition, go to third
        if (a.getXval() < b.getXval()) return true;
        if (b.getXval() < a.getXval()) return false;

        return false;
}

AET::AET(Polygon poly)
{
    polygon = poly;
    for (int i = 0; i < polygon.getPointsSize() - 1; i++) {
        AETNode newNode(polygon.getPoint(i), polygon.getPoint(i+1));
        nodes.push_back(newNode);
    }

    nodes.remove_if(isSlopeZero);
    nodes.sort(CompareNodes);

    maximalY = std::max_element(nodes.begin(), nodes.end(), [](const AETNode& a, const AETNode& b)
                                 {
                                     return a.getYmax() < b.getYmax();
                                 })->getYmax();
    minimalY = std::min_element(nodes.begin(), nodes.end(), [](const AETNode& a, const AETNode& b)
                                 {
                                     return a.getYmin() < b.getYmin();
                                 })->getYmin();
}
