#include <iostream>
#include <opencv2/opencv.hpp>
#include <memory>
#include "threadPool.hpp"
#include "imageTask.hpp"
#include "threadQueue.hpp"

int main() {
    cv::VideoCapture cap("../res/test.mp4");
    if (!cap.isOpened()) {
        std::cerr << "missing video file!" << std::endl;
        return -1;
    }

    threadPool pool(4);
    
    auto outQueue = std::make_shared<threadQueue<cv::Mat>>();

    cv::Mat firstFrame;
    cap.read(firstFrame);
    cv::Mat mask = cv::Mat::zeros(firstFrame.size(), CV_8UC1);
    std::vector<cv::Point> road = {{0,745}, {0,1080}, {1690,1080}, {1780,750}, {1780,400}, {1080,400}, {750,0}, {500,0}, {435,300}};
    cv::fillPoly(mask, std::vector<std::vector<cv::Point>>{road}, cv::Scalar(255));

    int taskId = 0;
    cv::Mat frame;

    while (true) {
        if (cap.read(frame) && !frame.empty()) {
            cv::Mat frameMask = mask.clone();
            auto task = std::make_unique<imageTask>(
                taskId++, Priority::high, std::move(frame), std::move(frameMask), outQueue
            );
            pool.submit(std::move(task));
        }

        if (!outQueue->empty()) {
            cv::Mat readyFrame;
            outQueue->waitAndPop(readyFrame);
            
            if (!readyFrame.empty()) {
                cv::imshow("processed image", readyFrame);
            }
        }

        if (cv::waitKey(20) == 27) { // ESC
            break; 
        }
    }

    std::cout << "closing.." << std::endl;

    cv::destroyAllWindows();
    cv::waitKey(1);

    return 0;
}