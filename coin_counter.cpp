// Elise Ong
// Last modified: 2/19/19

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat orig, gray, finalimg, edges;

int edgeThresh = 1;
int lowThreshold = 30;
int ratio = 3;
int kernel_size = 3;
double dollar = 1.00;
double quarter = 0.25;
double dime = 0.10;
double nickel = 0.05;
double penny = 0.01;
double TOTALMONEY = 0;

void FullCanny(int, void*)
{
    blur(gray, edges, Size(kernel_size, kernel_size));  // reduces noise
    Canny(edges, edges, lowThreshold, lowThreshold*ratio, kernel_size);
    finalimg = Scalar::all(0);

    orig.copyTo(finalimg, edges);
}

int main(int argc, char** argv)
{    
    orig = imread(argv[1], 1);

    if(!orig.data)
        return -1;
    
    finalimg.create(orig.size(), orig.type());

    // Convert it to gray
    cvtColor(orig, gray, CV_BGR2GRAY);
    
    // Reduce the noise so we avoid false circle detection
    GaussianBlur(gray, gray, Size(9, 9), 2, 2);

    FullCanny(0, 0);

    vector<Vec3f> circles;  // 3d vect
                                                        // mindist, thH, thL, minR, maxR
    // coin1
    HoughCircles(edges, circles, CV_HOUGH_GRADIENT, 1, 25, 40, 20, 20, 40);  // lowThreshold = 30
    
    // coin2
    // HoughCircles(edges, circles, CV_HOUGH_GRADIENT, 1, 23, 30, 15, 15, 35);  // lowThreshold = 40
    
    // coin3
    // HoughCircles(edges, circles, CV_HOUGH_GRADIENT, 1, 24, 30, 15, 17, 38);  // lowThreshold = 17
    
    
    /// Draw the circles detected
    for(size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cout << radius << "\n";
        
        // COIN 1, 2
        if(radius >= 28){
            TOTALMONEY+=quarter;
            circle(finalimg, center, radius, Scalar(0,0,255), 1, 8, 0);  // red
        }
        else if(radius < 28 && radius >= 23){
            TOTALMONEY+=nickel;
            circle(finalimg, center, radius, Scalar(0,255,0), 1, 8, 0);  // green
        }
        else if(radius < 23){
            TOTALMONEY+=penny;
            circle(finalimg, center, radius, Scalar(255,0,0), 1, 8, 0);  // blue
        }
        
        /*  COIN 3
        if(radius >= 34){
            TOTALMONEY+=dollar;
            circle(finalimg, center, radius, Scalar(0,0,255), 2, 8, 0);  // red
        }
        else if(radius < 34 && radius >= 22){
            TOTALMONEY+=nickel;
            circle(finalimg, center, radius, Scalar(0,255,255), 2, 8, 0);  // yellow
        }
        else if(radius < 22 && radius >= 19){
            TOTALMONEY+=penny;
            circle(finalimg, center, radius, Scalar(255,0,0), 2, 8, 0);  // blue
        }
        else if(radius < 19){
            TOTALMONEY+=dime;
            circle(finalimg, center, radius, Scalar(0,255,0), 2, 8, 0);  // green
        }
        */
    }
    cout << "TOTALMONEY: " << TOTALMONEY << "\n";
    
    imshow("Hough Transform", finalimg);
    imwrite("finalimg-coin1.jpg", finalimg); 
    waitKey(0);
    return 0;
    
    // coin1 estimate: $5.11
    // coin2 estimate: $8.82
    // coin3 estimate: $11.45
}