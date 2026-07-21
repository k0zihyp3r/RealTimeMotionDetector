# RealTimeMotionDetector

A lightweight, high-performance, **multithreaded C++ application** utilizing the OpenCV 4 library for real-time motion detection. 

The project processes video streams through an asynchronous **Producer-Consumer pipeline** powered by a custom thread pool (`ThreadPool`) and thread-safe task queues (`ThreadQueue`). Moving objects are isolated from static backgrounds using frame differencing (`cv::absdiff`), filtered via Region of Interest (ROI) masking, cleaned using morphological operations, and highlighted with dynamic bounding boxes in the main GUI thread.

## Architecture & Multithreading
- **Thread Pool Engine:** Offloads heavy computer vision tasks (blurring, thresholding, contour detection) across multiple worker threads.
- **Thread-Safe Queues:** Asynchronously passes raw frames to workers and retrieves processed output frames to be safely rendered on the main X11/Wayland event thread.
- **ROI Masking:** Restricts motion evaluation specifically to road boundaries to ignore background disturbances.

## Image Processing Pipeline

The program processes each video frame through a multi-stage computer vision pipeline:
1. **Grayscale Conversion:** Reduces data complexity from 3 channels (BGR) to 1 channel.
2. **Gaussian Blur:** Smooths out digital camera noise.
3. **ROI Masking:** Applies a custom polygon mask to isolate only the target area (e.g., the road).
4. **Frame Differencing (`cv::absdiff`):** Computes absolute pixel differences between consecutive frames to capture movement.
5. **Binary Thresholding:** Segments high-confidence motion areas.
6. **Morphological Closing & Dilation:** Fills internal voids inside vehicles and merges fragmented contours into solid object regions.
7. **Contour Extraction:** Computes geometric shapes and wraps them inside `cv::Rect` bounding boxes on the output frame.

## Prerequisites

To compile and run this project, you need:
- **C++ Compiler** supporting C++17 or higher (GCC, Clang, MSVC).
- **CMake** (v3.10 or higher).
- **OpenCV 4.x** installed and properly configured.
- A test video file placed under the relative directory: `../res/test.mp4`.
