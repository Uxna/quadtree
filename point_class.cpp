//Point class, each object represent a point stored
#include <cmath>


class Point{
    public:
        Point();
        ~Point();
        void setXY(double x, double y);

        double distanceToPoint(double xTarget, double yTarget);        
        double getX();
        double getY();

    private:
        double xCoor;
        double yCoor;
};


Point::Point(){}
Point::~Point(){}

void Point::setXY(double x, double y){
    xCoor = x;
    yCoor = y;
}

double Point::distanceToPoint(double xTarget, double yTarget){
    double xDiff = xTarget - xCoor;
    double yDiff = yTarget - yCoor;
    double result = sqrt( xDiff*xDiff + yDiff*yDiff );
    return result;
}

double Point::getX(){
    return xCoor;
}
double Point::getY(){
    return yCoor;
}