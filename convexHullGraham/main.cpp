//
//  main.cpp
//  convexHullGraham
//
//  Created by Sarah Gu on 10/18/18.
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
private:
    int xcoord;
    int ycoord;
public:
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
    void toString()
    {
        cout << "X: " << xcoord << " Y: " << ycoord << endl;
    }
    // ~Point();
};
Point p0(0,0);
void drawline(int arr[800][2400], int y1,int x1,int y2,int x2)
{
    int x,y,deltax,deltay,dx1,dy1,arbitx,arbity,xe,ye,i;
    deltax=x2-x1;
    deltay=y2-y1;
    dx1=fabs(deltax);
    dy1=fabs(deltay)/3;
    arbitx=2*dy1-dx1; //arbitrary values that indicate when to increment y and x pixels
    arbity=2*dx1-dy1;
    if(dy1<=dx1) //decides when to stop the code (i.e when x is reached, because change in x is greater than change in y)
    {
        if(deltax>=0) //if first pixel is to the left of the second pixel
        {
            x=x1;
            y=y1;
            xe=x2;
        }
        else //first pixel to right of the second pixel, so start with the leftmost pixel x2
        {
            x=x2;
            y=y2;
            xe=x1;
        }
        arr[x][y] = 0;
        arr[x][y+1] = 0;
        arr[x][y+2] = 0; //set pixel to white
        for(i=0;x<xe;i++)
        {
            x=x+1;
            if(arbitx<0)
            {
                arbitx=arbitx+2*dy1;
            }
            else
            {
                if((deltax<0 && deltay<0) || (deltax>0 && deltay>0)) //if positive slope :: increment y positively
                {
                    y=y+3;
                }
                else
                {
                    y=y-3;
                }
                arbitx=arbitx+2*(dy1-dx1);
            }
            
            arr[x][y] = 0;
            arr[x][y+1] = 0;
            arr[x][y+2] = 0;
        }
    }
    else //same as above, except change in terms of the x coord (as y will be the ending variable)
    {
        if(deltay>=0)
        {
            x=x1;
            y=y1;
            ye=y2;
        }
        else
        {
            x=x2;
            y=y2;
            ye=y1;
        }
        arr[x][y] = 0;
        arr[x][y+1] = 0;
        arr[x][y+2] = 0;
        for(i=0;y<ye;i++)
        {
            y=y+3;
            if(arbity<=0)
            {
                arbity=arbity+2*dx1;
            }
            else
            {
                if((deltax<0 && deltay<0) || (deltax>0 && deltay>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                arbity=arbity+2*(dx1-dy1);
            }
            
            arr[x][y] = 0;
            arr[x][y+1] = 0;
            arr[x][y+2] = 0;
        }
        
    }
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
Point randPoint(vector<Point> &vect)
{
    
    int rand1 = (int)(rand()%vect.size());
    Point p1 = vect[rand1];
    vect.erase(vect.begin() + (rand1));
    return p1;
}
int triangleArea(Point p1, Point p2, Point p3)
{
    int area = (p1.getX()* (p2.getY() - p3.getY()) + p2.getX()* (p3.getY()-p1.getY()) + p3.getX() * (p1.getY() - p2.getY()))/2;
    return abs(area);
}
void inTriangle(Point p1, Point p2, Point p3, vector<Point> &vect, Point check,int idx)
{
    int wholeTriangle = triangleArea(p1,p2,p3);
    int a1 = triangleArea(p1,p2,check);
    int a2 = triangleArea(p2,p3,check);
    int a3 = triangleArea(p1,p3, check);
    // cout << (a1 + a2 + a3) << endl;
    if((a1 + a2 + a3) == wholeTriangle)
    {
        vect.erase(vect.begin() + idx);
    }
    
}

int distance(Point p1, Point p2)
{
    int distancex = (p2.getX()-p1.getX())*(p2.getX()-p1.getX());
    int distancey = (p2.getY()-p1.getY()) * (p2.getY()-p1.getY());
    int ret = sqrt(distancex + distancey);
    return ret;
}
void swap(Point &p1, Point &p2)
{
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}
int orientation(Point p, Point q, Point r)
{
    if (q.getX() == p.getX() && q.getY() == p.getY())
        return 1;
    int val = (q.getY() - p.getY()) * (r.getX() - q.getX()) -
    (q.getX() - p.getX()) * (r.getY() - q.getY());
    
    if (val == 0) return 0;  // collinear
    return (val > 0)? 1: 2; // clockwise or counterclock wise
}

int compare(const void *vp1, const void *vp2)
{
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;
    
    // Find orientation
    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (distance(p0, *p2) >= distance(p0, *p1))? -1 : 1;
    
    return (o == 2)? -1: 1;
}
void drawHull(vector<Point> &vect)
{
    int ymin = vect[0].getY();
    int minidx = 0;
    for(int i = 1; i < vect.size(); i++) //find leftmost point
    {
        int yCurr = vect[i].getY();
        if (yCurr > ymin){
            ymin = yCurr;
            minidx = i;
        }
    }
    swap(vect[0], vect[minidx]);
    p0 = vect[0];
   // cout << p0.getX() << " " << p0.getY() << "!!!!!!!!";
    qsort(&vect[1], vect.size()-1, sizeof(Point), compare);
}
Point nextToTop(stack<Point> &s)
{
    if(s.size() < 3)
        return s.top();
    Point temp = s.top();
    s.pop();
    Point ret = s.top();
    s.push(temp);
    return ret;
}
void algorithm(vector<Point> &vect, stack<Point> &s, int numPoints)
{
    drawHull(vect); // sorts all points by polar angle
   // cout << "Works" << endl;
    s.push(vect[0]);
    s.push(vect[1]);
    s.push(vect[2]);
    for(int i =3; i < numPoints; i++)
    {
        while(orientation(s.top(), nextToTop(s), vect[i]) !=1)
        {
            s.pop();
        }
        s.push(vect[i]);
        
    }
    
}

void algorithmog(vector<Point> &vect)
{
    int count = 0;
    while(count < 1000 )
    {
        Point p1 = randPoint(vect);
        Point p2 = randPoint(vect);
        Point p3 = randPoint(vect);
        int s1 = vect.size();
        for(int i = 0; i < vect.size(); i++)        {
            Point check = vect[i];
            inTriangle(p1,p2,p3,vect,check,i);
        }
        if(s1 != vect.size())
        {
            count = 0;
        }
        vect.push_back(p1);
        vect.push_back(p2);
        vect.push_back(p3);
        count +=1;
    }
}
void makeppm(vector<Point> all, stack<Point> s)
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

    for (int p = 0; p < all.size(); p++)
    {
        int ycoord = all[p].getX();
        int xcoord = all[p].getY();
       // cout << xcoord << " " << ycoord << endl;
        pixArr[xcoord][ycoord*3] = 0;
        pixArr[xcoord][ycoord*3+1]= 0;
        pixArr[xcoord][ycoord*3 + 2] = 0;
    }
    vector<Point> convexHull;
    while(!s.empty())
    {
        convexHull.push_back(s.top());
        s.pop();
    }
    for (int print = 0; print < convexHull.size()-1; print++)
    {
        drawline(pixArr, convexHull[print].getX()*3, convexHull[print].getY(), \
                 convexHull[print+1].getX()*3, convexHull[print+1].getY());
    }
    drawline(pixArr, convexHull[0].getX()*3, convexHull[0].getY(), \
             convexHull[convexHull.size()-1].getX()*3, convexHull[convexHull.size()-1].getY());
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

int main(int argc, const char * argv[]) {
    // insert code here...

    int run[15] = {10,25,100,500,1000,3000,10000,20000,25000, 30000, 32500, 35000,40000,45000, 50000};
    
    for(int runs = 0; runs < 15; runs++)
    {
        int numPoints = run[runs];
        cout << "Number of Points: " << numPoints << endl;
        int totaltimeng = 0;
        int totaltimeg = 0;
        for(int rep = 0; rep < 200; rep++)
        {
            int timestart = clock();
            srand(time(NULL));
            vector<Point> openSet;
            makeSet(openSet, numPoints);
            vector<Point> ogSet(openSet);
            stack<Point> s;
            algorithm(openSet,s, numPoints);
            totaltimeg += clock()-timestart;
            timestart = clock();
           // cout << "Good" << endl;
            vector<Point> noGraham;
            makeSet(noGraham,numPoints);
            vector<Point> ogSetng(noGraham);
            algorithmog(noGraham);
            totaltimeng += clock()-timestart;
        }
        //cout << totaltimeng << "TEST" << totaltimeg << endl;
        cout << "Average Time for 200 Trials"<<endl;
        cout << "    Triangle Method: " << ((double(totaltimeng)/(double) CLOCKS_PER_SEC)/200)*1000 << "ms Graham Method: " << ((double(totaltimeg)/(double) CLOCKS_PER_SEC)/200)*1000 << "ms" << endl;
        cout << endl;
    }
    //   cout << openSet.size() << endl;
    // cout << "Hello, World!\n";
    return 0;
}
