//Instructions for running: run as normal and have file called noChessBoard.mov. 
//  main.cpp
//  findChessboard
//
//  Created by Sarah Gu on 1/29/19.
//  Copyright © 2019 Sarah Gu. All rights reserved.
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;
/** @function main */
int main(int argc, char** argv)
{
    namedWindow("Video");
    VideoCapture cap;
    cap.open("noChessBoard.mov");
    Mat frame;
    Point c1, c2 , c3 , c4 ;
    c1.x = 1038;
    c2.y = 533;
    c2.x = 940;
    c1.y = 479;
    c3.x = 916;
    c3.y = 439;
    c4.x = 813;
    c4.y = 488;
    int count = 0;
    while(1)
    {
        count += 1;
        cap >> frame;
        if(!frame.data)
            break;

        flip(frame,frame, -1 );
       // cvtColor( frame, frame, COLOR_BGR2GRAY );
        vector<Point2f> corners;
       // bool patternfound = findChessboardCorners(frame, Size(7,7), corners);
        bool patternfound = false;
       /* if(patternfound)
            cornerSubPix(frame, corners, Size(11, 11), Size(-1, -1),TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1)); */
        //cout << corners << endl;
        if(patternfound)
        {
            c1 = corners[16];
            c2 = corners[18];
            c3 = corners[30];
            c4 = corners[32];
        }
        else if(count < 100)
        {
            c1.x -= 1;
            c2.y -= .5;
            c2.x -= 1;
            c1.y -= .5;
            c3.x -= 1;
            c3.y -= .5;
            c4.x -= 1;
            c4.y -= .5;


        }
        else if(count < 200){
            c1.x += 1;
            c2.y -= .5;
            c2.x += 1;
            c1.y -= .5;
            c3.x += 1;
            c3.y -= .5;
            c4.x += 1;
            c4.y -= .5;
        }
        else {
            c1.x += 1;
            c2.y += 1;
            c2.x += 1;
            c1.y += 1;
            c3.x += 1;
            c3.y += 1;
            c4.x += 1;
            c4.y += 1;
        }
        if(count == 200)
        {
            c1.x = 1038;
            c2.y = 533;
            c2.x = 940;
            c1.y = 479;
            c3.x = 916;
            c3.y = 439;
            c4.x = 813;
            c4.y = 488;
        }
       // cout << c1 << endl;
        line(frame, c1, c2, Scalar( 110, 220, 0 ),  2, 8);
        line(frame, c3, c1, Scalar(110,220,0), 2, 8);
        line(frame, c1, Point(c1.x, c1.y - 150), Scalar(110,220,0), 2, 8);
        line(frame, c4, c2, Scalar( 110, 220, 0 ),  2, 8);
        line(frame, c3, c4, Scalar( 110, 220, 0 ),  2, 8);
        line(frame, c2, Point(c2.x, c2.y - 150), Scalar(110,220,0), 2, 8);
        line(frame, c3, Point(c3.x, c3.y - 150), Scalar(110,220,0), 2, 8);
        line(frame, c4, Point(c4.x, c4.y - 150), Scalar(110,220,0), 2, 8);
        line(frame, Point(c3.x, c3.y-150), Point(c4.x, c4.y - 150), Scalar(110,220,0), 2, 8);
        line(frame, Point(c2.x, c2.y-150), Point(c4.x, c4.y - 150), Scalar(110,220,0), 2, 8);
        line(frame, Point(c1.x, c1.y-150), Point(c3.x, c3.y - 150), Scalar(110,220,0), 2, 8);
        line(frame, Point(c2.x, c2.y-150), Point(c1.x, c1.y - 150), Scalar(110,220,0), 2, 8);
      //  line(frame, c1, )
        
      //  drawChessboardCorners(frame, Size(7,7), Mat(corners), patternfound);
        imshow("Video", frame);
        if(waitKey(1) >= 0)
            break;
    }
    return 0;
}

