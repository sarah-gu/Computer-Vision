//Instructions for running: run as normal and have file called withChessBoard.mov. 
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
    cap.open("withChessBoard.mov");
    Mat frame;
    Point c1, c2, c3, c4;
    while(1)
    {
        cap >> frame;
        if(!frame.data)
            break;

        flip(frame,frame, -1 );
       // cvtColor( frame, frame, COLOR_BGR2GRAY );
        vector<Point2f> corners;
        bool patternfound = findChessboardCorners(frame, Size(7,7), corners);
       /* if(patternfound)
            cornerSubPix(frame, corners, Size(11, 11), Size(-1, -1),TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1)); */
        //cout << corners << endl;
        if(patternfound)
        {
            c1 = corners[16];
            cout << c1.x << " " << c1.y << endl;
            c2 = corners[18];
             cout << c2.x << " " << c2.y << endl;
            c3 = corners[30];
             cout << c3.x << " " << c3.y << endl;
            c4 = corners[32];
             cout << c4.x << " " << c4.y << endl;
            cout << endl; 
        }
        else
        {
            patternfound = findChessboardCorners(frame, Size(3,3), corners);
            if(patternfound){

                c1 = corners[0];
            cout << c1.x << " " << c1.y << endl;
            c2 = corners[2];
            cout << c2.x << " " << c2.y << endl;
            c3 = corners[6];
            cout << c3.x << " " << c3.y << endl;
            c4 = corners[8];
            cout << c4.x << " " << c4.y << endl;
            }
            cout <<"FOUND" << corners << endl;
            c1.x -= 1;
            c2.y -= 2;
            c2.x -= 1;
            c1.y -= 2;
            c3.x -= 1;
            c3.y -= 2;
            c4.x -= 1;
            c4.y -= 2;


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

