#ifndef AET_H
#define AET_H
#include <list>
#include "aetnode.h"
#include "polygon.h"


class AET
{
public:
    AET(){};
    AET(Polygon poly);
    std::list<AETNode> nodes;
    std::vector<std::pair<int,int> > points;
    int minimalY;
    int maximalY;
    bool compareData(const AETNode& a, const AETNode& b);
    void Add(const AETNode node);
private:
    Polygon polygon;
};

#endif // AET_H
