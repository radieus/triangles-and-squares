#include "aetnode.h"
#include <cmath>

AETNode::AETNode(QPoint p1, QPoint p2)
{
    if (p1.y() < p2.y()) {
        xVal = p1.x();
        xMax = p2.x();
        yMin = p1.y();
        yMax = p2.y();
    }
    else if (p1.y() > p2.y()) {
        xVal = p2.x();
        xMax = p1.x();
        yMin = p2.y();
        yMax = p1.y();
    }
    else {
        yMin = yMax = p1.y();
        xVal = (p1.x() <= p2.x()) ? p1.x() : p2.x();
        xMax = (p1.x() <= p2.x()) ? p2.x() : p1.x();
    }

    signCoeff = (xMax > xVal) ? 1 : -1;
    dx = abs(p1.x() - p2.x());
    dy = abs(p1.y() - p2.x());

}

double AETNode::getMinv()
{
    return ((xMax - xVal) / (yMax - yMin));
}

int AETNode::getYmin() const {return yMin;}
void AETNode::setYmin(int y) {yMin = y;}

int AETNode::getYmax() const {return yMax;};
void AETNode::setYmax(int y) {yMax = y;}

double AETNode::getXval() const {return xVal;};
void AETNode::setXval(double x) {xVal = x;}

double AETNode::getXmax() const {return xMax;};
void AETNode::setXmax(double x) {xMax = x;}

int AETNode::getDx() const {return dx;};
void AETNode::setDx(int dx_) {dx = dx_;}

int AETNode::getDy() const {return dy;};
void AETNode::setDy(int dy_) {dy = dy_;}

