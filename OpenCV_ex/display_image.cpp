#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;

int main(int argc, char** argv )
{
    
    String path="../GCC_CMake_Example_Tutorial.jpg";
    Mat image;
    image = imread( path, 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}