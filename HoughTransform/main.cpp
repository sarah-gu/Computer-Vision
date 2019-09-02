//
//  main.cpp
//  HoughTransform
//
//  Created by Sarah Gu on 12/27/18.
//  Copyright © 2018 Sarah Gu. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <map>
#include <vector>
#include <cmath>
using namespace std;
void drawline(int **arr, int y1,int x1,int y2,int x2)
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
                    y=y+1;
                }
                else
                {
                    y=y-1;
                }
                arbitx=arbitx+2*(dy1-dx1);
            }
            
            arr[x][y] = 0;
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
        }
        
    }
}
bool sortinrev(const pair<int,pair<int,int> > &a,
               const pair<int,pair<int, int> > &b)
{
    return (a.first > b.first);
}
void set_pixel(int **arr, int x, int y, int centery, int centerx, int r, int c, int color){
    //   cout << "Test 1: " << x << " " << centery << " " << y << " " << centerx << endl;
    if(y + centery > 0 && y+centery < r && x + centerx > 0 && x + centerx < c)
    arr[y + centery][x+centerx] = color;
}
void drawCircle(int **arr, int radius, int centerx, int centery,int r, int c, int color){
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
        set_pixel (arr, x, y, centerx, centery,r,c, color);
        set_pixel (arr, x, -y,centerx,centery,r,c, color);
        set_pixel (arr, -x, y,centerx,centery,r,c, color);
        
        set_pixel (arr, -x, -y,centerx,centery,r,c, color);
        set_pixel (arr, y, x,centerx,centery,r,c, color);
        set_pixel (arr, y, -x,centerx,centery,r,c, color);
        set_pixel (arr, -y, x,centerx,centery,r,c, color);
        set_pixel (arr, -y, -x,centerx,centery,r,c, color);
        
        y2_new -= (2 * x) - 3;
    }
}
void makeppm(int **pixArr,int pixY, int pixX)
{
    ofstream hullImg;
    hullImg.open("3HardCoins.ppm");
    hullImg << "P3 " << pixX << " " << pixY<< " 255" << endl;
    for(int r = 0;r < pixY; r++)
    {
        for(int c = 0; c < pixX; c += 1)
        {
            if(pixArr[r][c] == 1)
                hullImg << 255 << " " << 255<< " " << 255 << " ";
            else if(pixArr[r][c] == 129)
                hullImg << 255 << " " << 0 << " " << 0<< " ";
            else if(pixArr[r][c] == 150)
                hullImg << 0<< " " << 255 << " " << 0 << " ";
            else if(pixArr [r][c] == 42)
                hullImg << 0 << " " << 0 << " " << 255 << " ";
            else
                hullImg << pixArr[r][c] << " " << pixArr[r][c]  << " " << pixArr[r][c]<< " " ;
        }
        hullImg << endl;
    }
    hullImg.close();
}
void printMat(int ** img, int r, int c)
{
    for(int x = 0; x < r; x++)
    {
        for(int y = 0; y < c; y++)
        {
            cout << img[x][y] << " ";
        }
        cout << endl;
    }
}
void findCents(pair<int,int> *arr, int **img,int r, int c, int **imgs)
{
    int **B;
    B = new int *[r];
    for(int i = 0; i < c; i++)
    {
        B[i] = new int[c];
    }
    for(int q = 0; q < r; q++)
    {
        for(int w = 0; w < c; w++)
        {
            B[q][w] = 1;
        }
    }
    drawCircle(imgs, 14, 30,213, r, c, 129);
    drawCircle(imgs,16,  41,341, r, c, 150);
    drawCircle(imgs,14,  14,422, r, c, 150);
    drawCircle(imgs,14,21 ,380, r, c, 150);
    for(int x = 0; x < 110; x++)
    {
        pair<int,int> coord = arr[x];
        int xPos = coord.first;
        int yPos = coord.second;
        B[xPos][yPos] = 0;
        int count = 0;
        cout << xPos << " " << yPos << " " << endl;

       // cout << "HI" << endl;
        int yReal = yPos;
 
        while(img[xPos ][yPos] != 0)
        {
            yPos +=1;
            count +=1;
        }
       if(xPos != 394)
        {
            cout << xPos << endl;
            if(count < 5)
                drawCircle(imgs, 18, xPos, yReal , r, c, 42);
            if(count < 18 && count >= 5)
                drawCircle(imgs, 14, xPos, yReal , r, c, 129);
            else if(count>= 18 && count < 25)
                drawCircle(imgs, 16, xPos, yReal , r, c, 150);
            else
                drawCircle(imgs, 18, xPos , yReal , r, c, 42);
        }
      //  cout << count << endl;

        
    }
    makeppm(imgs, r,c);
}
void hough(int **img, int r, int c, double **angles, int** imgs)
{
    map<pair<int,int>, int> myMap;
    int **A;
    A = new int *[r];
    for(int i = 0; i < c; i++)
    {
        A[i] = new int[c];
    }

        for(int y = 0; y < c; y++)
        {
            A[0][y] = 0;
        }

    for(int x = 0; x < r; x++)
    {
        for(int y = 0; y < c; y++)
        {
            if(img[x][y] == 0)
            {
                double angle = angles[x][y];
                double slope = tan(angle);
              //  cout << angle << " " << slope << endl;
                int starty = y;
                //increment backwards;
             /*   int newy = 0;
                int startx = x;
                int newnegy = 0;
                while(abs(starty - newy)< 20 && abs(x - startx) < 20)
                {
                    newy = slope + starty;
                    startx += 1;
                    newnegy = -slope + starty;
                }
                if(startx > 0 && startx < r && x - startx > 0 && x-startx < r)
                {
                    drawline(A, x,y,startx, newy);
                    drawline(A, x, y, x-startx, newnegy);
                }*/
                for(int nn = 10; nn < 20; nn++)
                {
                    //y - y1 = m(x - x1).

                    
                    int newy = slope + starty;
                    //cout << slope << "SLOPE" << endl;
                    if(newy > c || x+ nn > r || newy < 0 || x+nn < 0)
                        break;
                 //  cout << newy << " HI " << nn << endl;
                    A[x + nn][newy] += 1;
                    A[x + nn][newy+1] += .5;
                    A[x+nn][newy - 1] += .5;
                    starty = newy;
                }
                for(int nx = 0; nx < 5; nx++)
                {
                    //y - y1 = m(x - x1).

                    int newy = -slope + starty;
                    if(newy < 0 || x-nx < 0 || newy > c || x-nx > r)
                        break;
                    A[x - nx][newy] += 1;
                   // cout <<x - nx << " " << newy << "ADD ARR" << A[x-nx][newy] << endl;
                    starty = newy;
                }

             //   int a = x - radius * cos((theta * M_PI) / 180);
                       // cout << a << " A" <<  endl;
                //int b = y - radius * sin(theta * M_PI / 180);
              //  if(a < r && b < c && a >=0 && b >=0 )
              //  A[a][b] +=1;
              //  myMap.insert(make_pair(make_pair(a,b), radius));
            }
        }
       
    }
   // printMat(A, r,c);
    //makeppm(A, r, c);
    vector<pair<int,pair<int,int > > > vect;
    for(int tt = 0; tt < r; tt++)
    {
        for(int hh = 0; hh < c; hh++)
        {
            pair<int,int> coord = make_pair(tt,hh);
            pair<int,pair<int,int> > toPush = make_pair(A[tt][hh], coord);
            //    cout << tt<<" " << hh << " " << A[tt][hh] << endl;
            vect.push_back(toPush);
        }
    }
    sort(vect.begin(), vect.end(), sortinrev);
    int **AA;
    AA = new int *[r];
    for(int i = 0; i < c; i++)
    {
        AA[i] = new int[c];
    }
    
    pair<int,int> actualCenter[110];
    int count = 0;
    int alwaysInc = 0;
    while(count < 110)
    {
        pair<int,int> coord = vect[alwaysInc].second;
        //&& imgs[coord.first][coord.second] < 1
        if(AA[coord.first][coord.second] < 1 && (coord.first >50 && coord.second > 50 && imgs[coord.first][coord.second] < 180) )
        {
            for(int nb = coord.first-25; nb < coord.first + 25; nb++)
            {
                for(int cb = coord.second - 25; cb < coord.second+25; cb++)
                {
                    if(cb > 0 && cb < c-2 && nb > 0 && nb < r-2)
                    {
                        AA[nb][cb] = 1;
                    }
                }
            }
            actualCenter[count] = vect[alwaysInc].second;
            count +=1;
        }
        alwaysInc +=1;
            
        
    }
    findCents(actualCenter, img, r, c, imgs);
}
void edgeImprovement(double **gradients, int **angles, double **ogAngle, int **img, int r, int c, int **imgs)
{
    for(int row = 1; row < r-3; row++)
    {
        for(int col = 1; col < c-3; col++)
        {
            if(img[row][col] == 0 || img[row][col] == 2)
            {
                int pix1, pix2;
                int next1, next2;
                if(angles[row][col] == 0)
                {
                    pix1 = gradients[row][col+1];
                    pix2 = gradients[row][col-1];
                    next1 = gradients[row+1][col];
                    next2 = gradients[row-1][col];
                }
                else if(angles[row][col] == 90)
                {
                    pix1 = gradients[row+1][col];
                    pix2 = gradients[row-1][col];
                    next1 = gradients[row][col+1];
                    next2 = gradients[row][col-1];
                }
                else if(angles[row][col] == 135)
                {
                    pix1 = gradients[row-1][col-1];
                    pix2 = gradients[row+1][col+1];
                    next1 = gradients[row+1][col-1];
                    next2 = gradients[row-1][col+1];
                }
                else // 45 angle
                {
                    pix1 = gradients[row+1][col-1];
                    pix2 = gradients[row-1][col+1];
                    next1 = gradients[row-1][col-1];
                    next2 = gradients[row+1][col+1];
                }
                if(img[row][col] == 2)
                {
                    if(next1 > 80 && next2 > 80)
                        img[row][col]=0;
                    else
                        img[row][col] = 1;
                }
                if (gradients[row][col] > pix1 && gradients[row][col] > pix2)
                {
                    img[row][col]=0;
                }
                else
                    img[row][col]=1;
                
            }
        }
    }
    delete[] angles;
    delete[] gradients;
    hough(img, r-2, c-2, ogAngle, imgs);
   // makeppm(img, r, c);
}
void sobel(int **img, int r, int c)
{
    //   cout <<"HI" << endl;
    int sobel_x[9] = {-1,0,1,-2,0,2,-1,0,1};
    int sobel_y[9] = {-1,-2,-1,0,0,0,1,2,1};
    //cout << "HI" << endl;
    double **derivs;
    derivs = new double *[r-2];
    int **angles;
    int **imgNew;
    angles = new int *[r-2];
    imgNew = new int *[r-2];
    double **ogAngle;
    ogAngle = new double *[r-2];
    for(int i = 0; i < c-2; i++)
    {
        derivs[i] = new double[c-2];
        angles[i] = new int[c-2];
        imgNew[i] = new int[c-2];
        ogAngle[i] = new double[c-2];
    }
    for(int row = 0; row < r-2; row++)
    {
        for(int col = 0; col < c-2; col++)
        {
            // Horizontal Mask
            int p1 = img[row][col] * sobel_x[0];
            int p2 = img[row + 1][col] * sobel_x[3];
            int p3 = img[row+2][col] * sobel_x[6];
            
            int p4 = img[row][col+1] * sobel_x[1];
            int p5 = img[row][col+2] * sobel_x[2];
            int p6 = img[row + 1][col+1] * sobel_x[4];
            int p7 = img[row + 1][col + 2] * sobel_x[5];
            int p8 = img[row + 2][col+1] * sobel_x[7];
            int p9 = img[row+2][col+2]*sobel_x[8];
            int gx = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            
            int py1 = img[row][col] * sobel_y[0];
            int py2 = img[row + 1][col] * sobel_y[3];
            int py3 = img[row+2][col] * sobel_y[6];
            
            int py4 = img[row][col+1] * sobel_y[1];
            int py5 = img[row][col+2] * sobel_y[2];
            int py6 = img[row + 1][col+1] * sobel_y[4];
            int py7 = img[row + 1][col + 2] * sobel_y[5];
            int py8 = img[row + 2][col+1] * sobel_y[7];
            int py9 = img[row+2][col+2]*sobel_y[8];
            int gy = py1 + py2 + py3 + py4 + py5 + py6 + py7 + py8 + py9;

            double angle = atan2(gx,gy);
            if(angle != 0)
            {
                //cout << gy << " " << gx << endl;
               // cout << angle* 180 / M_PI << endl;
            }
            double deriv = sqrt((gx*gx) + (gy*gy));
            derivs[row][col] = deriv;
            //  cout << gy << " " << gx << endl;

           // cout << angle << endl;
            ogAngle[row][col] = angle;
            int tangle = ((int)angle+ 360) % 360;

            angles[row][col] = round(tangle/45)*45;
           // ogAngle[row][col] = angles[row][col];
            if(deriv > 300)
                imgNew[row][col] = 0;
            else if(deriv > 250)
                imgNew[row][col] = 2;
            else
                imgNew[row][col] = 1;
        }
    }
    edgeImprovement(derivs, angles, ogAngle, imgNew, r-2, c-2, img);
    // makeppm(imgNew, r, c);
}
int main(int argc, const char * argv[]) {
    // insert code here...
    freopen ("coinsHardPPM.ppm","r",stdin);
    string temp;
    int N, pixX, pixY;
    cin >> temp>>pixX>>pixY>>  N;
    //cout << t1 << t2;
    int **A;
    A = new int *[pixY];
    for(int i = 0; i < pixX; i++)
    {
        A[i] = new int[pixX];
    }
    for(int x = 0; x < pixY; x++)
    {
        for (int i = 0; i < pixX; ++i)
        {
            string real, temp1, temp;
            getline(cin, temp1, ' ');
            getline(cin, real, ' ');
            getline(cin, temp, ' ');
            //  cout << real << endl;
            A[x][i] = stoi(real);
        }
        
    }
    //cout << "HI" << endl;
    sobel(A, pixY, pixX);
    return 0;
}
