#ifndef AET_H
#define AET_H
#include <list>
#include <aetnode.h>
#include <polygon.h>


class AET
{
public:
    AET(Polygon poly);
    std::list<AETNode> nodes = std::list<AETNode>();
    std::list<AETNode> sortedNodes = std::list<AETNode>();
    int minimalY;
    int maximalY;
    bool compareData(const AETNode& a, const AETNode& b);

private:
    Polygon polygon;
};

#endif // AET_H
