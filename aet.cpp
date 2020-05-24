#include "aet.h"

bool isSlopeZero(const AETNode &node)
{
    return ((node.yMax - node.yMin == 0) ? true : false);
}

void AET::Add(const AETNode node) {
    if (!isSlopeZero(node))
        nodes.push_back(node);
}

AET::AET(Polygon poly)
{
    polygon = poly;
    for (int i = 0; i < polygon.getPointsSize() - 1; i++) {
        AETNode node(polygon.getPoint(i), polygon.getPoint(i+1));
        nodes.push_back(node);
    }
}

