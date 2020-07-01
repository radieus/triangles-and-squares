#ifndef EDGE_H
#define EDGE_H

#include <QPoint>

class Edge
{
public:
    Edge();
    Edge(QPoint a, QPoint b);
    int getYmax();
    void setYmax(int y);
    int getYmin();

    double getX();
    void setX(double x);

    void makeStep();

    int getNumerator();
    void setNumerator(int num);

    void setDenominator(int num);
    int getDenominator();

    bool operator< (const Edge& edge) const;

private:
    int yMax;
    int yMin;
    double mInv;
    double xVal;
    int dX;
    int dY;
    int sum;
    int sign;
};

#endif // EDGE_H
