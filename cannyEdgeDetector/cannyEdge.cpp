//
//  main.cpp
//  cannyEdgeFull
//
//  Created by Sarah Gu on 12/6/18.
//  Copyright © 2018 Sarah Gu. All rights reserved.
//


#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <math.h>

using namespace std;
void makeppm(int **pixArr,int pixY, int pixX)
{
    ofstream hullImg;
    hullImg.open("coinsTest.ppm");
    hullImg << "P3 " << pixX << " " << pixY<< " 1" << endl;
    for(int r = 0;r < pixY; r++)
    {
        for(int c = 0; c < pixX; c += 1)
        {
            hullImg << pixArr[r][c] << " " << pixArr[r][c] << " " << pixArr[r][c] << " ";
        }
        hullImg << endl;
    }
    hullImg.close();
}
void edgeImprovement(double **gradients, int **angles, int **img, int r, int c)
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
    makeppm(img, r, c);
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
    for(int i = 0; i < c-2; i++)
    {
        derivs[i] = new double[c-2];
        angles[i] = new int[c-2];
        imgNew[i] = new int[c-2];
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
            double deriv = sqrt((gx*gx) + (gy*gy));
            derivs[row][col] = deriv;
          //  cout << gy << " " << gx << endl;
            int angle = atan2(gy,gx);
          //  cout << angle << endl;
            angle = (angle+ 360) % 360;
            
            angles[row][col] = round(angle/45)*45;
            if(deriv > 200)
                imgNew[row][col] = 0;
            else if(deriv > 180)
                imgNew[row][col] = 2;
            else
                imgNew[row][col] = 1;
        }
    }
    edgeImprovement(derivs, angles, imgNew, r, c);
  // makeppm(imgNew);
}
int main(int argc, const char * argv[]) {
    // insert code here...
    freopen ("coinsEasyPPM.ppm","r",stdin);
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
