//
//  main.cpp
//  shortestDistance
//
//  Created by Sarah Gu on 10/29/18.
//  Copyright © 2018 Sarah Gu. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cmath>
#include <stdio.h>

#include <stack>
using namespace std;
class Point
{

public:
    int xcoord;
    int ycoord;
    Point()
    {
        xcoord = 0;
        ycoord = 0;
    }
    Point(int x, int y)
    {
        xcoord = x;
        ycoord = y;
    }
    int getX()
    {
        return xcoord;
    }
    int getY()
    {
        return ycoord;
    }
    void setX(int x)
    {
        xcoord = x;
    }
    void setY(int y)
    {
        ycoord = y;
    }
    void toString()
    {
        cout << "X: " << xcoord << " Y: " << ycoord << " " << endl; ;
    }
    // ~Point();
};
void makeppm(vector<Point> all, vector<Point> convexHull)
{
    int pixArr[800][2400];
    ofstream hullImg;
    hullImg.open("hull.ppm");
    hullImg << "P3 800 800 1" << endl;
    for(int r = 0;r < 800; r++)
    {
        for(int c = 0; c < 2400; c += 3)
        {
            pixArr[r][c] = 1;
            pixArr[r][c+1] = 1;
            pixArr[r][c+2] = 1;
        }
    }
    // cout << pixArr << endl;
    cout << all.size() << endl;
    for (int p = 0; p < all.size(); p++)
    {
        int ycoord = all[p].getX();
        int xcoord = all[p].getY();
        cout << xcoord << " " << ycoord << endl;
        pixArr[xcoord][ycoord*3] = 0;
        pixArr[xcoord][ycoord*3+1]= 0;
        pixArr[xcoord][ycoord*3 + 2] = 0;
    }
    for(int r = 0;r < 800; r++)
    {
        for(int c = 0; c < 2400; c += 1)
        {
            hullImg << pixArr[r][c] << " ";
        }
        hullImg << endl;
    }
    hullImg.close();
}
void makeSet(vector<Point> &vect, int numPoints)
{
    for(int i = 0; i < numPoints; i++)
    {
        int x = rand()%500 + 100;
        int y = rand()%500 + 100;
        Point temp(x,y);
        vect.push_back(temp);
    }
}
float distance(Point p1, Point p2)
{
    float distancex = (p2.getX()-p1.getX())*(p2.getX()-p1.getX());
    float distancey = (p2.getY()-p1.getY()) * (p2.getY()-p1.getY());
    //cout << distancex << " " << distancey << endl;
    float ret = sqrt(distancex + distancey);
   // cout << ret << endl;
    return ret;
}
float bruteForce(vector<Point> &vect)
{
    float shortestPath = 1000;
    Point short1 = vect[0];
    Point short2 = vect[1];
    for(int p1 = 0; p1 < vect.size(); p1++)
    {
        for(int p2 = 0; p2 < vect.size(); p2++)
        {
            if(p1 != p2)
            {
                float dist =distance(vect[p1],vect[p2]);
                if (dist < shortestPath)
                {
                   // cout << dist << " " << shortestPath << endl;
                    shortestPath = dist;
                    short1 = vect[p1];
                    short2 = vect[p2];
                    
                }
            }
        }
    }

    vect.clear();

    vect.push_back(short1);
    vect.push_back(short2);
    return shortestPath;
}
bool comparator(Point &p1, Point &p2)
{
    if(p1.getX() > p2.getX())
        return 0;
    else if (p1.getX() < p2.getX())
        return 1;
    return 0;
}
int compareY(const void* a, const void* b)
{
    Point *p1 = (Point *)a,   *p2 = (Point *)b;
    return (p1->ycoord - p2->ycoord);
}
float min(float x, float y)
{
    return (x < y)? x : y;
}
float stripClosest(vector<Point> strip, int size, float d)
{
    float min = d;  // Initialize the minimum distance as d
    
    qsort(&strip[0], size, sizeof(Point), compareY);
    

    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size && (strip[j].getY() - strip[i].getY()) < min; ++j)
            if (distance(strip[i],strip[j]) < min)
                min = distance(strip[i], strip[j]);
    
    return min;
}
float recurse(vector<Point> &vect)
{
    if(vect.size() ==2)
    {
        return distance(vect[0], vect[1]);
    }
    if(vect.size() == 3)
    {
        float d1 = distance(vect[0],vect[1]);
        float d2 = distance(vect[1], vect[2]);
        float d3 = distance(vect[2], vect[0]);
        if(d1 > d2 && d1 > d3)
        {
            return d1;
        }
        if(d2>d1 && d2 > d3)
        {
            return d2;
        }
        return d3;
    }
    else
    {
        int mid = static_cast<int>(vect.size()/2);
        vector<Point> left(vect.begin(), vect.begin()+vect.size()/2);
        vector<Point> right(vect.begin() + vect.size()/2, vect.end());
        Point midPoint = vect[mid];
        float lval = recurse(left);
        float rval = recurse(right);
        
        float better = min(lval, rval);
        vector<Point> closeStrip;
        int len = 0;
        for(int j = 0; j < vect.size(); j++)
        {
            if(abs(vect[j].getX()-midPoint.getX()) < better)
            {
                closeStrip.push_back(vect[j]);
                len ++;
            }
        }


         return min(better, stripClosest(closeStrip, len, better) );
    }
}
int recurse_helper(vector<Point> &vect)
{
    sort(vect.begin(),vect.end(),&comparator);
    int timestart = clock();
    recurse(vect);
    int end = clock() - timestart;
 //   cout << "Recursive Shortest Distance: " << dist;

    
    return end;
}
int main(int argc, const char * argv[]) {
    // insert code here...
   
    int run[17] = {10,25,100,500,1000,3000,4000,5000,10000,20000,25000, 30000, 32500, 35000,40000,45000, 50000};
    
    for(int runs = 0; runs < 17; runs++)
    {
        int numPoints = run[runs];
        cout << "Number of Points: " << numPoints << endl;
        int timeBrute = 0;
        int timeRecurse = 0;
        for(int rep = 0; rep < 200; rep++)
        {
            if (numPoints < 5100)
            {
                int timestart = clock();
                srand(static_cast<unsigned int>(time(NULL)));
                vector<Point> openSet;
                makeSet(openSet,numPoints);
                vector<Point> ogSet(openSet);
            
                float bruteShort = bruteForce(openSet);
          //  cout << "Brute Force Shortest Distance: " << bruteShort;
           // cout << ", Points: (" << openSet[0].getX()<< "," << openSet[0].getY() << "),";
            //cout << "(" << openSet[1].getX() << "," << openSet[1].getY() << ")" << endl;
                timeBrute += clock()-timestart;

                timeRecurse += recurse_helper(ogSet);
            }
            else
            {
                srand(static_cast<unsigned int>(time(NULL)));
                vector<Point> openSet;
                makeSet(openSet,numPoints);
                timeRecurse += recurse_helper(openSet);
            }
            //cout << ", Points: (" << openSet[0].getX()<< "," << openSet[0].getY() << "),";
            //cout << "(" << openSet[1].getX() << "," << openSet[1].getY() << ")" << endl;


        }
        cout << "Average Time for 200 Trials"<<endl;
        cout << "    Brute Force Method: " << ((double(timeBrute)/(double) CLOCKS_PER_SEC)/200)*1000 << "ms Recursive Method: " << ((double(timeRecurse)/(double) CLOCKS_PER_SEC)/200)*1000 << "ms" << endl;
        cout << endl;
    }
    return 0;
}
