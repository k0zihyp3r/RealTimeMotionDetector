#include "imageTask.hpp"
#include <vector>

void imageTask::execute() {
    if (frame.empty()) return;

    resultFrame = frame.clone();

    cv::Mat gray, blur;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(5, 5), 0);

    if (!mask.empty()) {
        blur.setTo(0, mask == 0);
    }

    static cv::Mat prevBlur;

    if (prevBlur.empty()) {
        blur.copyTo(prevBlur);
        return;
    }

    cv::Mat frameDelta;
    cv::absdiff(prevBlur, blur, frameDelta);

    blur.copyTo(prevBlur);

    cv::Mat thresh;
    cv::threshold(frameDelta, thresh, 100, 255, cv::THRESH_BINARY);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11,11));
    cv::dilate(thresh, thresh, kernel, cv::Point(-1, -1), 6);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        if (cv::contourArea(contour) > 500.0) { 
            cv::Rect boundingBox = cv::boundingRect(contour);
            cv::rectangle(resultFrame, boundingBox, cv::Scalar(0, 255, 0), 2);
        }
    }

    if (outQueue) {
        
        //outQueue->push(thresh); 
        outQueue->push(resultFrame);
    }
}