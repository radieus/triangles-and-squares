#ifndef AETNODE_H
#define AETNODE_H
#include <QPoint>

class AETNode
{
public:
    AETNode(QPoint p1, QPoint p2);
    double getMInv();
    bool getIsSlopeZero();

    int getYmin() const;
    void setYmin(int y);

    int getYmax() const;
    void setYmax(int y);

    double getXval() const;
    void setXval(double x);

    double getXmax() const;
    void setXmax(double x);

    int getDx() const;
    void setDx(int dx);

    int getDy() const;
    void setDy(int dy);

    double getMinv();

private:
    int yMin;
    int yMax;
    double xVal;
    double xMax;
    int dx;
    int dy;
    int signCoeff;
    int dummy = 0;
};

#endif // AETNODE_H
