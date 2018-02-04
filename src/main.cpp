//
// Created by Darkstarslyp on 2018/2/4.
//

#include <vector>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <math.h>
#include "ldmarkmodel.h"


using namespace std;
using namespace cv;


int main() {


    ldmarkmodel modelt;
    std::string modelFilePath = "roboman-landmark-model.bin";
    if (!load_ldmarkmodel(modelFilePath, modelt)) {
        std::cout << "init model failed!!!" << std::endl;
        return 0;
    }

    cv::VideoCapture mCamera(0);
    mCamera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    mCamera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    if (!mCamera.isOpened()) {
        std::cout << "Camera opening failed..." << std::endl;
        return 0;
    }
    cv::Mat current_shape;
    cv::Mat srcImg;
    cv::Mat backgroundImg;
    std::string WINDOW_NAME = "FacelessPerson";

    namedWindow(WINDOW_NAME, 1);


    for (;;) {

        mCamera >> srcImg;

        modelt.track(srcImg, current_shape);

        if(current_shape.cols<=0&&backgroundImg.empty()){
            srcImg.copyTo(backgroundImg);
        }else{
            int numLandmarks = current_shape.cols / 2;
            std::vector<cv::Point> landmarks;

            for (int j = 0; j < numLandmarks; j++) {

                int x = current_shape.at<float>(j);
                int y = current_shape.at<float>(j + numLandmarks);

                if (j >= 0 && j <= 16) {
                    landmarks.push_back(cv::Point(x, y));
                }
            }

            if (numLandmarks > 0) {
                for (int j = 67; j >= 0; j--) {
                    int x = current_shape.at<float>(j);
                    int y = current_shape.at<float>(j + numLandmarks);

                    if (j >= 17 && j <= 19) {
                        landmarks.push_back(cv::Point(x, y));
                    }
                    if (j >= 24 && j <= 26) {
                        landmarks.push_back(cv::Point(x, y));
                    }
                }
            }

            if (landmarks.size() > 0&&!backgroundImg.empty()) {

                cv::Mat mask = cv::Mat::zeros(srcImg.size(), CV_8UC1);
                cv::fillConvexPoly(mask, landmarks.data(), landmarks.size(), cv::Scalar(255));

                std::vector<cv::Point> contour;
                std::vector<std::vector<cv::Point>> contours;

                cv::findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
                cv::approxPolyDP(contours[0], contour, 1.0, true);

                int width = srcImg.cols;
                int height = srcImg.rows;

                for(int i=1;i<=height;i++){
                    for(int j=1;j<=width;j++){
                        cv::Point point(j,i);
                        if(cv::pointPolygonTest(contour,point,false)>=0){
                            srcImg.at<cv::Vec4b>(i,j) = backgroundImg.at<cv::Vec4b>(i,j);
                        }
                    }
                }
            }
        }


        cv::imshow(WINDOW_NAME, srcImg);

        if (27 == cv::waitKey(10)) {
            mCamera.release();
            cv::destroyAllWindows();
            break;
        }
    }
    return 0;
}
