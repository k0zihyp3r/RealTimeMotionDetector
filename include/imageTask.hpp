#ifndef IMAGETASK_HPP
#define IMAGETASK_HPP

#include "task.hpp"
#include "threadQueue.hpp"
#include <opencv2/opencv.hpp>
#include <memory>

class imageTask : public Task {
public:
    imageTask(int id, Priority p, cv::Mat f, cv::Mat m, std::shared_ptr<threadQueue<cv::Mat>> outQ)
        : Task(id, p), frame(std::move(f)), mask(std::move(m)), outQueue(outQ) {}

    void execute() override;

private:
    cv::Mat frame;
    cv::Mat mask;
    cv::Mat resultFrame;
    std::shared_ptr<threadQueue<cv::Mat>> outQueue;
};

#endif