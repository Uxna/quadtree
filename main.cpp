#include <iostream>
#include <string>

#include "quadtree_class.cpp"
#include "illegal_exception_class.cpp"

using namespace std;

int main(){
    string command = "";
    cin >> command;
    Quadtree* quadtree = nullptr;
    while (command != "EXIT"){
        try{
        
        if (command == "INIT"){
            int m;
            double x0, y0, x1, y1;
            cin >> m;
            cin >> x0; cin >> y0; cin >> x1; cin >> y1;
            if (x0 < x1 && y0 < y1){
                quadtree = new Quadtree(m, x0, y0, x1, y1);
                cout << "success" << endl;
            }
            else {
                throw( illegal_exception() );
            }
        }
        else if (command == "INSERT"){
            double x, y;
            cin >> x, cin >> y;
            bool success = quadtree->insert(x, y);
            if (success){
                cout << "success";
            }
            else {
                cout << "failure";
            }
            cout << endl;
        }
        else if (command == "SEARCH"){
            double x, y, distance;
            cin >> x; cin >> y; cin >> distance;
            if (quadtree->search(x, y, distance) == true){
                cout << "point exists";
            }
            else {
                cout << "no point exists";
            }
            cout << endl;
        }
        else if (command == "NEAREST"){
            double x, y;
            cin >> x; cin >> y;
            cout << quadtree->nearest(x, y) << endl;
        }
        else if (command == "RANGE"){
            double xMi, yMi, xMx, yMx;
            cin >> xMi; cin >> yMi; cin >> xMx; cin >> yMx;
            if (xMi >= xMx || yMi >= yMx){
                throw ( illegal_exception() );
            }
            else {
                cout << quadtree->range(xMi, yMi, xMx, yMx) << endl;
            }
        }
        else if (command == "NUM"){
            cout << quadtree->numPoints() << endl;
        }
        else {
            throw(illegal_exception());
        }
        }//end of try block
        catch( illegal_exception& e){
            cout << e.getMessage();
        }//end of catch block

        cin >> command;
    }

    if (quadtree != nullptr){
        delete quadtree;
    }
}