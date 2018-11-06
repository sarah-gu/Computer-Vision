#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <math.h>
#include <ctime>
#include <cmath>
using namespace std;

#define pixelWidth 2400
#define pixelHeight 800
int* makeRand()
{
    static int randList[6];
    for (int i = 0; i < 6; i++){

        if (i%2 == 0)
        {
            if (i == 0)
            {
                randList[i] = rand()%100 + 150;
            }
            else if (i == 2)
            {
                randList[i] = rand()%350 + 200;
            }
            else
            {
                randList[i] = rand()%150 + 300;
            }
        }
        else
        {
            if (i == 1)
            {
                randList[i] = 3*(rand()%100 + 50);
            }
            else if (i == 3)
            {
                randList[i] = 3*(rand()%250 + 200);
            }
            else
            {
                randList[i] = 3*(rand()%250 + 150);
            }
        }
    }
    return randList;
    
    
}
void getSlope(int x1, int y1, int x2, int y2, double &a)
{
    double ydelt = y2 -y1;
    double xdelt = x2-x1;
    cout << ydelt << " " << xdelt << endl;
    a = (ydelt)/xdelt;
 //   cout << "A" << a << endl;

   cout << a << " " << endl;
    
}
void getPerpBisector(int x1, int y1, int x2, int y2, double &a, double &b){
    int midx = (x1 + x2)/2;
    int midy = (y1 + y2)/2;
    cout <<"Midpoints" <<  midx << " " << midy << endl;
    double temp = 1/a;
    a = -1*temp;
    b = midy - a*midx;
    cout << "Perp Bisector: " << a << " " << b << endl;
}
void lineIntersection(double a1, double b1, double a2, double b2, int &finx, int &finy){
    cout << "TEST " << a1 << " " << a2 << " " << b1 << " " << b2 << endl;
    //a1x + b1 = a2x + b2, a2x-a1x=b1-b2
    finx = (b2-b1)/(a1-a2);
    finy = a1 * finx + b1;
    cout << "Intersection: " << finx << " " << finy<< endl;
}
void drawline(int arr[pixelHeight][pixelWidth], int y1,int x1,int y2,int x2)
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
void set_pixel(int arr[pixelHeight][pixelWidth], int x, int y, int centery, int centerx){
 //   cout << "Test 1: " << x << " " << centery << " " << y << " " << centerx << endl;
    arr[y + centery][3*x+centerx] = 0;

    arr[y + centery][3*x+centerx +1] = 0;
    arr[y + centery][3*x+centerx + 2] = 0;
}
void drawEuler(int arr[pixelHeight][pixelWidth], int x, int y, int x2, int y2)
{
    double a;
    getSlope(x/3, y, x2/3, y2, a);
    if( y > y2){
        drawline(arr, x*-a, y+a, x2*a, y-a);
    }
    else{
        drawline(arr, x*a, y-a, x2*-a, y+a);
    }
}
void drawCircle(int arr[pixelHeight][pixelWidth], int radius, int centery, int centerx){
    int x, y, xmax, y2, y2_new, ty;
   // cout << "Radius: " << radius << endl;
    xmax = (int) (radius * 0.70710678); // maximum x at radius/sqrt(2)
    y = radius;
    y2 = y * y;
    ty = (2 * y) - 1;
    y2_new = y2;
    for (x = 0; x <= xmax; x += 1) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }
        set_pixel (arr, x, y, centerx, centery);
        set_pixel (arr, x, -y,centerx,centery);
        set_pixel (arr, -x, y,centerx,centery);

        set_pixel (arr, -x, -y,centerx,centery);
        set_pixel (arr, y, x,centerx,centery);
        set_pixel (arr, y, -x,centerx,centery);
        set_pixel (arr, -y, x,centerx,centery);
        set_pixel (arr, -y, -x,centerx,centery);
        
        y2_new -= (2 * x) - 3;
    }
}
int distance(int x1, int y1, int x2, int y2)
{
    int distancex = (x2-x1)/3*(x2-x1)/3;
    int distancey = (y2-y1) * (y2-y1);
    int ret = sqrt(distancex + distancey);
    cout << "Radius: " << ret << endl;
    return ret;
}
void incenter(int x1, int y1, int x2, int y2, int x3, int y3, int &inx, int &iny, int &inrad)
{
    int a;
    a=distance(x2,y2, x3,y3);
    int b = distance(x1,y1,x3,y3);
    int c = distance(x1, y1, x2, y2);
    int p = (a + b + c);
    int s = .5*p;
    inx = ((a*x1) + (b*x2) + (c*x3))/p;
    iny = ((a*y1) + (b*y2) + (c*y3)) /p;
    cout << "Centers: " << inx << " "<< iny << endl;
    inrad = sqrt(((s-a)*(s-b)*(s-c))/s);
}

int main(){
    int pixArr[pixelHeight][pixelWidth];
    ofstream triangleImg;
    triangleImg.open("triangle.ppm");
    srand( time(NULL));
    if(triangleImg.fail())
    {
        cout << "Dislay Failed" << endl;
        return 0;
    }
    {
        triangleImg << "P3" << " " << pixelHeight << " " << pixelWidth/3 << " " << 1 << endl;
    }
    {
        for (int row = 0; row < pixelHeight; row++)
        {
            for (int col = 0; col < pixelWidth; col += 3)
            {
                pixArr[row][col] = 1;
                pixArr[row][col+1] = 1;
                pixArr[row][col+2] = 1;
            }
        }
        
    }
    int *coord =  makeRand();
    for (int i = 0; i < 6; i++)
    {
        cout << coord[i] << " ";
    }
    cout << endl;
   // int coord[6] = {200,300, 180,600,100, 360}; //{y,x}
    drawline(pixArr, coord[5], coord[4], coord[1], coord[0]);
    drawline(pixArr, coord[1], coord[0], coord[3], coord[2]);
    drawline(pixArr, coord[5], coord[4], coord[3], coord[2]);
    double a,b,e,f;
    int finx,finy;
    getSlope(coord[1]/3, coord[0], coord[3]/3, coord[2], a);//
    getSlope(coord[3]/3, coord[2], coord[5]/3, coord[4], e);
   // drawline(pixArr, 1000,450,100,0);
    getPerpBisector(coord[1]/3, coord[0], coord[3]/3, coord[2], a,b);
    getPerpBisector(coord[3]/3, coord[2], coord[5]/3, coord[4], e,f);
  //  drawline(pixArr, 600,250, 400,650); // {x,y}
  //  drawline(pixArr, 1000,575, 0,75);
    lineIntersection(a,b,e,f,finx,finy);
  //  drawline(pixArr, 1000,100,1000,700);
 //   drawline(pixArr, 10,250, 2300,250);
 //   drawline(pixArr, 10, 500, 2300, 500);
  // drawline(pixArr, 1200,484, 1500,82);
   // drawline(pixArr, 350,100,350,700);
   // drawline(pixArr, 10,550, 1000,550);
   // cout << finx << " " << finy << endl;
   int circumradius;
   circumradius = distance(finx*3, finy, coord[1], coord[0]);
   // cout << circumradius << endl;
    drawCircle(pixArr, circumradius, finx*3,finy);
    int inx, iny, inrad;
    incenter(coord[1], coord[0], coord[3], coord[2], coord[5], coord[4], inx,iny,inrad);
    drawCircle(pixArr, inrad, inx, iny);
    drawline(pixArr, finx*3, finy, inx, iny);
   // drawEuler(pixArr, finx*3, finy, inx, iny);
    /*getSlope(50, 100, 60, 500, a,b,c);
    getSlope(60, 500, 300, 50, e,f,g);
    getPerpBisector(50, 100, 60, 500, a,b,c);
    getPerpBisector(60, 500, 300, 50, e,f,g);

    lineIntersection(a,b,c,e,f,g,finx,finy);
   
    int circumradius = 0;
    int num1 = 50;
    int num2 = 100;
    circumradius =distance(300,600, 50, 100);
    drawCircle(pixArr, finx,finy, circumradius); */
    {
        for (int r = 0; r < pixelHeight; r++)
        {
            for (int c = 0; c < pixelWidth;c++)
            {
                int value = pixArr[r][c];
                triangleImg << value << " ";
            }
            triangleImg << endl;
        }
    }
    triangleImg.close();
  //  delete pixArr;
  //  pixArr = NULL;
    return 0;
   // delete pixArr;

}
 //rasterized circle

//  //
// displayImage.cpp

 
