#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <iostream>
#include "my_lib/include/integral.h"


using namespace std;

int main(){
    cv::Mat image = cv::imread("in.png",0);

    // display original image
    cv::namedWindow("Original Image");
    cv::imshow("Original Image", image);

    // using a fixed threshold
    cv::Mat binaryFixed;
    cv::Mat binaryAdaptive;
    cv::threshold(image, binaryFixed,70,255,cv::THRESH_BINARY);

    //using as adaptive threshold
    int blockSize=21;  //size of the neighborhood
    int threshold=10;  // pixel will be compare to (mean-threshold)

    int64 time;
    time = cv::getTickCount();
    cv::adaptiveThreshold(
            image,  //input image
            binaryAdaptive, // output binary image
            255,  //max value for output
            cv::ADAPTIVE_THRESH_GAUSSIAN_C, // adaptive method
            cv::THRESH_BINARY, //threshold type
            blockSize,
            threshold
    );


    time = cv::getTickCount() - time;
    std::cout<<"time (adaptiveThreshold) =  "<<time<<endl;

    // compute integral image
    IntegralImage<int, 1>integral(image);

    // test integral image
//    cout<<"sum = "<<integral(18, 45,30, 50)<<endl;
//    cv::Mat test(image, cv::Rect(18,45,30,50));
//    cv::Scalar t = cv::sum(test);
//    cout<<"sum test = "<<t[0]<<endl;

    cv::namedWindow("Fixed Threshold");
    cv::imshow("Fixed Threshold", binaryFixed);

    cv::namedWindow("Adaptive Threshold");
    cv::imshow("Adaptive Threshold", binaryAdaptive);

    cv::Mat binary = image.clone();
    time = cv::getTickCount();
    int nl = binary.rows; // number of lines
    int nc = binary.cols; // total number of elements per line

    // compute integral image
    cv::Mat iimage;
    cv::integral(image, iimage, CV_32S);

    //for each row
    int halfSize = blockSize/2;
    for(int j=halfSize; j<nl - halfSize -1;j++ ){
        // get the address of row j
        uchar* data = binary.ptr<uchar>(j);
        int* idata1 = iimage.ptr<int>(j-halfSize); // 滑动窗口上边
        int* idata2 = iimage.ptr<int>(j+halfSize+1); // 滑动窗口下边

        //for pixel of a line
        for(int i=halfSize; i<nc-halfSize-1;i++){
            //compute pix_mean
            int pix_mean = (idata2[i+halfSize+1]-idata2[i-halfSize]-idata1[i+halfSize+1]
            +idata1[i-halfSize])/(blockSize*blockSize);

            //apply adaptive threshold
            if(data[i]<(pix_mean-threshold))
                data[i] = 0;
            else
                data[i]  =255;
        }

    }
    // add white border
    for(int j=0;j<halfSize;j++){
        uchar *data = binary.ptr<uchar>(j);
        for(int i=0; i<binary.cols;i++)
            data[i] = 255;
    }
    for(int j=binary.rows-halfSize-1;j<binary.rows;j++){
        uchar * data = binary.ptr<uchar>(j);
        for(int i=0; i<binary.cols;i++){
            data[i] = 255;
        }
    }
    for(int j=halfSize;j<nl-halfSize-1;j++){
        uchar* data = binary.ptr<uchar>(j);
        for(int i=0; i<halfSize;i++)
            data[i] = 255;
        for(int i=binary.cols-halfSize-1;i<binary.cols;i++)
            data[i] = 255;
    }

    time = cv::getTickCount()-time;
    cout<<"time integral= "<<time<<endl;

    cv::namedWindow("Adaptive Threshold (integral)");
    cv::imshow("Adaptive Threshold (integral)", binary);

    // adaptive threshold using image operators
    time = cv::getTickCount();
    cv::Mat filtered;
    cv::Mat binaryFiltered;
    // box filter compute avg of pixels over a rectangle region
    cv::boxFilter(image, filtered, CV_8U, cv::Size(blockSize,blockSize));
    // check if pixel greater than (mean+shreshold)
    binaryFiltered = image>=(filtered-threshold);
    time = cv::getTickCount()-time;

    cout<<"time filtered= "<<time<<endl;

    cv::namedWindow("Adaptive filtered");
    cv::imshow("Adaptive filtered", binaryFiltered);

    cv::waitKey();

    return 0;

}