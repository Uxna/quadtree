#include "point_class.cpp"

#include <string>
#include <cmath>
#include <iostream>

class Node{
    public:
        Node(Node* parent, double xMin, double xMax, double yMin, double yMax, int maxPoints);
        Node(); //empty default constructor
        ~Node();

        bool isInBoundary(Point* point); //at boundary condition: use left & top
        bool insertPoint(Point* point); //returns 0 if the exact point already exists
        bool existInCircle(double x, double y, double radius); //returns 1 if there is a point stored in the circle
        std::string printInRange(double x0, double x1, double y0, double y1); //return "xp0 yp0 xp1 yp1 ..."
        std::string findNearest(double x, double y, double nearestX = M_PI, double nearestY = M_E);

    private:
        bool isLeafNode;
        double xMin;
        double xMax;
        double yMin;
        double yMax;

        Point** pointsArray;
        Node** p_childNodes;
        int maxPoints;
        int currentPoints;
        Node* p_parentNode;

        void splitNode(); //When the pointsArray is full, split the node into four equal size child nodes
};

/**************************************************
 * Definition for the Node class
**************************************************/

//helper functions in findNearest
double extractX(std::string input);
double extractY(std::string input);

bool Node::isInBoundary(Point* point){
    bool xIn = point->getX()>=xMin && point->getX()<=xMax;
    bool yIn = point->getY()>=yMin && point->getY()<=yMax;
    return xIn && yIn;
}

bool Node::insertPoint(Point* point){
    //Assuming the point is in range of this node or the child of this node
    //If this is not a leaf node, find the correct child to insert the point and call insert on that child
    if (isInBoundary(point) == false){
        return 0;
    }
    if (this->isLeafNode == false){
        for (int i = 0; i < 4; ++i){
            Node* p_currentChild = p_childNodes[i];
            if (p_currentChild->isInBoundary(point) == true){ 
                return p_currentChild->insertPoint(point);
            }
        }
    }
    else { //this means that this is a leaf node
        //go through the points already stored in the node to avoid repetition
        for (int i = 0; i < currentPoints; ++i){
            if (pointsArray[i]->getX() == point->getX() && 
                pointsArray[i]->getY() == point->getY()){ //point already stored
                return 0;
            }
        }
        pointsArray[currentPoints] = point;
        currentPoints += 1;
    }
    // if after insertion the array is full, split the node
    if (currentPoints == maxPoints){
        this->splitNode();
    }
    return 1;
}

std::string Node::printInRange(double x0, double x1, double y0, double y1){
    std::string result = "";
    double xMinRange = x0, xMaxRange = x1;
    double yMinRange = y0, yMaxRange = y1;
    if (x0 < xMin){ xMinRange = xMin;}
    if (x1 > xMax){ xMaxRange = xMax;}
    if (y0 < yMin){ yMinRange = yMin;}
    if (y1 > yMax){ yMaxRange = yMax;}
    if (this->isLeafNode == true){ //add every point within range to result
        for (int i = 0; i < currentPoints; ++i){
            if (pointsArray[i]->getX() > xMinRange && pointsArray[i]->getX() < xMaxRange &&
                pointsArray[i]->getY() > yMinRange && pointsArray[i]->getY() < yMaxRange){
                    result = result + std::to_string(pointsArray[i]->getX()) + " " 
                                + std::to_string(pointsArray[i]->getY()) + " ";
                }
        }
        return result;
    }
    //not a leaf node, go into children nodes to print
    for (int i = 0; i < 4; ++i){
        result = result + p_childNodes[i]->printInRange(xMinRange, xMaxRange, yMinRange, yMaxRange);
    }
    return result;
}

bool Node::existInCircle(double x, double y, double radius){
    bool result = false;
    double xMinRange = x - radius, xMaxRange = x + radius;
    double yMinRange = y - radius, yMaxRange = y + radius;
    if (xMinRange < xMin){ xMinRange = xMin;}
    if (xMaxRange > xMax){ xMaxRange = xMax;}
    if (yMinRange < yMin){ yMinRange = yMin;}
    if (yMaxRange > yMax){ yMaxRange = yMax;}
    if (this->isLeafNode == true){ //check every point to see if it is less than radius away from (x, y)
            for (int i = 0; i < currentPoints; ++i){
            if (pointsArray[i]->getX() > xMinRange && pointsArray[i]->getX() < xMaxRange &&
                pointsArray[i]->getY() > yMinRange && pointsArray[i]->getY() < yMaxRange){
                    if (pointsArray[i]->distanceToPoint(x, y) < radius){ //meaning it is within the circle
                        return true;
                    }
                }
        }
        return false; // no point is less than radius away from (x, y)
    }
    //not a leaf node, go into children nodes to check the points
    for (int i = 0; i < 4; ++i){
        result = result || p_childNodes[i]->existInCircle(x, y, radius);
    }
    return result;
}

std::string Node::findNearest(double x, double y, double x_nearest, double y_nearest){
    // std::cout << "Input: " << x << " " << y << " " << x_nearest << " " << y_nearest << std::endl;
    // First go into the node containing (x, y), if nothing then go to the nodes most adjacent to (x, y) and repeat
    std::string nearestCoor;
    if (x_nearest == M_PI && y_nearest == M_E){
        nearestCoor = "";
    } else {
        nearestCoor = std::to_string(x_nearest) + " " + std::to_string(y_nearest);
    }
    double xCurrentNearest = x_nearest;
    double yCurrentNearest = y_nearest;
    bool foundAPoint = false;
    Point targetPoint;
    targetPoint.setXY(x, y);
    if (isLeafNode == true){ //base case, compare all points to nearest distance
        for (int i = 0; i < currentPoints; ++i){
            Point* p_currentPoint = pointsArray[i]; 
            foundAPoint = true;
            if (xCurrentNearest == M_PI && yCurrentNearest == M_E){ //then this is the first point we have encountered
                xCurrentNearest = p_currentPoint->getX();
                yCurrentNearest = p_currentPoint->getY();
            }
            double currentDistance = targetPoint.distanceToPoint(p_currentPoint->getX(), p_currentPoint->getY());
            double nearestDistance = targetPoint.distanceToPoint(xCurrentNearest, yCurrentNearest);
            if (currentDistance <= nearestDistance){
                // std::cout << "Found a new point: " << p_currentPoint->getX() << " " << p_currentPoint->getY() << std::endl;
                // std::cout << "Nearest point: "<<xCurrentNearest<<" "<<yCurrentNearest<<std::endl;
                // std::cout << "currentDistance: " << currentDistance << std::endl;
                // std::cout << "nearestDistance: " << nearestDistance << std::endl;
                if (currentDistance == nearestDistance){ //tie break with larger x value first, larger y second
                    if (p_currentPoint->getX() > xCurrentNearest){
                        xCurrentNearest = p_currentPoint->getX();
                        yCurrentNearest = p_currentPoint->getY();
                    }
                    else if (p_currentPoint->getY() > yCurrentNearest){
                        xCurrentNearest = p_currentPoint->getX();
                        yCurrentNearest = p_currentPoint->getY();
                    }
                }
                else {
                    xCurrentNearest = p_currentPoint->getX();
                    yCurrentNearest = p_currentPoint->getY();
                }
            }
        }
        if (foundAPoint == false){
            return "";
        }
        nearestCoor = std::to_string(xCurrentNearest) + " " + std::to_string(yCurrentNearest);
        return nearestCoor;
    }
    else { //not a leaf node
        //go into the child node containing (x, y), if nothing is found, or if possible nearer points in other child 
        //nodes, search the other child nodes
        int firstNodeIndex;
        for (int i = 0; i < 4; ++i){
            Point nearestPoint;
            nearestPoint.setXY(xCurrentNearest, yCurrentNearest);
            if (p_childNodes[i]->isInBoundary(&nearestPoint)){
                firstNodeIndex = i;
            }
        }
        nearestCoor = p_childNodes[firstNodeIndex]->findNearest(x, y, xCurrentNearest, yCurrentNearest);
        return nearestCoor;
        //possibility for nearer points in other adjacent nodes if one of the x/y distance to the target is longer 
        //than the target's distance to one of the node edge
        bool possibleInAdjacent = false;
        // std::cout << "nearestCoor: " << nearestCoor <<
        if (nearestCoor != ""){
            //std::cout << "nearestCoor is not empty: ";
            xCurrentNearest = extractX(nearestCoor);
            yCurrentNearest = extractY(nearestCoor);
            double targetToXmin = std::abs(x - p_childNodes[firstNodeIndex]->xMin);
            double targetToXmax = std::abs(p_childNodes[firstNodeIndex]->xMax - x);
            double targetToYmin = std::abs(y - p_childNodes[firstNodeIndex]->yMin);
            double targetToYmax = std::abs(p_childNodes[firstNodeIndex]->yMax - y);
            double minDistanceToEdgeX = (targetToXmin < targetToXmax) ? targetToXmin : targetToXmax;
            double minDistanceToEdgeY = (targetToYmin < targetToYmax) ? targetToYmin : targetToYmax;
            double x_nearestToTarget = std::abs(xCurrentNearest - x);
            double y_nearestToTarget = std::abs(yCurrentNearest - y);
            possibleInAdjacent = (x_nearestToTarget > minDistanceToEdgeX) || (y_nearestToTarget > minDistanceToEdgeY);
            //std::cout << "possibleINAdjacent: " << possibleInAdjacent << std::endl;
        }
        if (possibleInAdjacent || nearestCoor == ""){ 
            //std::cout << "firstNodeIndex: " << firstNodeIndex << std::endl;
            for (int i = (firstNodeIndex+1)%4; i != firstNodeIndex; i = (i+1)%4){
                nearestCoor = p_childNodes[i]->findNearest(x, y, xCurrentNearest, yCurrentNearest);
                if (nearestCoor != ""){
                    xCurrentNearest = extractX(nearestCoor);
                    yCurrentNearest = extractY(nearestCoor);
                    // std::cout << "xCurrentNearest: " << xCurrentNearest<<std::endl;
                    // std::cout << "yCurrentNearest: " << yCurrentNearest<<std::endl;
                }
                //std::cout << "potential update\n";
            }
        }

    }
    return nearestCoor;
}

void Node::splitNode(){ //used when currentPoints == maxPoints
    //Create four child nodes into the p_childNodes array, make sure each point is allocated into the correct child
    double xMid = (xMin + xMax)/2;
    double yMid = (yMin + yMax)/2;
    //0-3 in child array = top left, top right, bottom left, bottom right
    p_childNodes[0] = new Node(this, xMin, xMid, yMid, yMax, maxPoints);
    p_childNodes[1] = new Node(this, xMid, xMax, yMid, yMax, maxPoints);
    p_childNodes[2] = new Node(this, xMin, xMid, yMin, yMid, maxPoints);
    p_childNodes[3] = new Node(this, xMid, xMax, yMin, yMid, maxPoints);
    isLeafNode = false;
    //call insert() to insert the points into the children
    while (currentPoints != 0){
        for (int i = 0; i < 4; ++i){
            if (p_childNodes[i]->insertPoint(pointsArray[currentPoints-1])){
                break; //break once successful insertion into one of the child
            }
        }
        currentPoints -= 1;
    }
    delete [] pointsArray; //All points have been transfered to the children, we no longer need a pointsArray
    pointsArray = nullptr;
}


//*******************************
//Constructor and destructor
Node::Node(){}; //empty default constructor
Node::Node(Node* parent, double xMi, double xMx, double yMi, double yMx, int maxP): 
    p_parentNode{parent}, xMin{xMi}, xMax{xMx}, yMin{yMi}, yMax{yMx}, maxPoints{maxP} {
    //Every node that is created is a leaf node, so set the child nodes to nullptr
    currentPoints = 0;
    Point** initializeArray = new Point*[maxPoints];
    pointsArray = initializeArray;
    isLeafNode = true;
    Node** p_createChild = new Node*[4];
    for (int i = 0; i < 4; ++i){
        p_createChild[i] = nullptr;
    }
    p_childNodes = p_createChild;
}

//Points will be dynamically allocated, and nodes will be dynamically allocated
//Every node should delete the points it holds

Node::~Node(){
    if (this->isLeafNode == true){ //if not a leaf node, nothing to delete
        while (currentPoints != 0){
            delete pointsArray[ currentPoints-1 ];
            currentPoints -= 1;
        }
        delete [] pointsArray;
        pointsArray = nullptr;
    }
}

double extractX(std::string input) {
    size_t spaceIndex = input.find(' ');  // Find the position of the space
    std::string xSubstring = input.substr(0, spaceIndex);  // Extract "x0"
    return std::stod(xSubstring);  // Convert "x0" to double
}

double extractY(std::string input) {
    size_t spaceIndex = input.find(' ');  // Find the position of the space
    std::string ySubstring = input.substr(spaceIndex + 1);  // Extract "y0"
    return std::stod(ySubstring);  // Convert "y0" to double
}