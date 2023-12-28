#include "node_class.cpp"
#include <string>

using namespace std;

class Quadtree{
    public:
        Quadtree(int childrenNum, double Xmin, double Ymin, double Xmax, double Ymax); 
        ~Quadtree();

        bool search(double x, double y, double distance); //return true if there is a point within distance of (x,y)
        bool insert(double x, double y); //return true if the point is successfully inserted into the quadtree
        string nearest(double x, double y); //return "xCoor yCoor" or "no point exists"
        string range(double xMi, double yMi, double xMx, double yMx); //Boundary not included (strict inequality)
        int numPoints(); 


    private:
        int maxPoints;
        int currentPoints;
        double xMax;
        double xMin;
        double yMax;
        double yMin;
        Node* rootNode;
};


/**************************************************
 * Definitions for the quadtree class
**************************************************/

string Quadtree::nearest(double x, double y){
    if (currentPoints == 0){
        return "no point exists";
    }
    return rootNode->findNearest(x, y);
}

string Quadtree::range(double xMi, double yMi, double xMx, double yMx){
    string result = rootNode->printInRange(xMi, xMx, yMi, yMx);
    if (result == ""){
        return "no points within range";
    }
    return result;
}

bool Quadtree::search(double x, double y, double distance){
    return rootNode->existInCircle(x, y, distance);
}

bool Quadtree::insert(double x, double y){
    Point* point = new Point();
    point->setXY(x, y);
    bool success = rootNode->insertPoint(point);
    if (success){
        currentPoints += 1;
    }
    return success;
}
int Quadtree::numPoints(){
    return currentPoints;
}

Quadtree::Quadtree(int num, double Xm, double Ym, double Xx, double Yx): maxPoints{num}, xMin{Xm}, yMin{Ym}, 
xMax{Xx}, yMax{Yx}, currentPoints{0} {
    Node* newRoot = new Node(nullptr, xMin, xMax, yMin, yMax, maxPoints);
    rootNode = newRoot;
}

Quadtree::~Quadtree(){}