#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <iostream>

int main(){
    std::cout << "ongoing \npress esc to leave" << std::endl;
    std::string path = "../res/test.mp4";
    cv::VideoCapture cap(path);

    if (!cap.isOpened()) {
        std::cerr << "PATH ERROR" << path << std::endl;
        return -1;
    }

    cv::Mat img, imgGray, imgBlur, imgDiff, roiMASK;
    cap.read(img);
    
    cv::Ptr<cv::BackgroundSubtractorMOG2> sub = cv::createBackgroundSubtractorMOG2(500, 50, true);
    cv::Mat kernelNoise = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::Mat kernelCars = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7,7));

    cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC1);
    std::vector<cv::Point> road = {
        {0,745},
        {0,1080},
        {1690,1080},
        {1780,750},
        {1780,400},
        {1080,400},
        {750,0},
        {500,0},
        {435,300}
    };

    cv::fillPoly(mask, std::vector<std::vector<cv::Point>>{road}, cv::Scalar(255));
    
    while (true) {

        if (!cap.read(img)) {
            std::cout << "video has ended" << std::endl;
            break;
        }
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(imgGray, imgBlur, cv::Size(3,3),0);
        imgBlur.setTo(0, mask == 0);
        sub->apply(imgBlur, imgDiff);

        cv::threshold(imgDiff, imgDiff, 127, 255, cv::THRESH_BINARY);
        cv::morphologyEx(imgDiff, imgDiff, cv::MORPH_OPEN,kernelNoise);
        cv::morphologyEx(imgDiff, imgDiff, cv::MORPH_CLOSE, kernelCars);
                
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(imgDiff, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        for (const auto& contour : contours) {
            if (cv::contourArea(contour) < 700) {
                continue;
            }
            cv::drawContours(img, contours, -1, cv::Scalar(255, 0, 0), 2);
            cv::Rect boundingRect = cv::boundingRect(contour);
            cv::rectangle(img, boundingRect, cv::Scalar(0, 255, 0), 2);
        }
        img.copyTo(roiMASK, mask);

        cv::imshow("ROI view", roiMASK);
        cv::imshow("Motion", imgDiff);
        cv::imshow("Mask", mask);
        cv::imshow("Image", img);


        if (cv::waitKey(1) == 27) {
            std::cout << "leaving..." << std::endl;
            break;
        }
    }
    return 0;
}