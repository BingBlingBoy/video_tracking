#include <iostream>
#include <opencv4/opencv2/core/hal/interface.h>
#include <opencv4/opencv2/core/types.hpp>
#include <opencv4/opencv2/core/version.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/video/tracking.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/tracking.hpp>
#include <opencv4/opencv2/tracking/tldDataset.hpp>

int main()
{
    cv::VideoCapture video("cars_around_mountain.mp4");

    if (!video.isOpened())
    {
        std::cout << "Could not read video file" << '\n';
        return 1;
    }

    cv::Mat frame;

    // video resolution
    int frameWidth = video.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = video.get(cv::CAP_PROP_FRAME_HEIGHT);

    cv::VideoWriter output(
        "output.avi",
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
        30,
        cv::Size(800, 600)
    );

    cv::Ptr<cv::Tracker> tracker;
    tracker = cv::TrackerMIL::create();

    video.read(frame);
    cv::resize(frame, frame, cv::Size(800, 600));
    cv::Rect trackingBox = cv::selectROI(frame, false);
    tracker->init(frame, trackingBox);

    while (video.read(frame))
    {
        cv::resize(frame, frame, cv::Size(800, 600));
        bool ok = tracker->update(frame, trackingBox);
        if (ok)
        {
            cv::rectangle(frame, trackingBox, cv::Scalar(255, 0, 0), 2, 8);
        }
        else
        {
            cv::putText(frame, "Tracking failure detected", cv::Point(100, 20), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);
        }

        cv::imshow("Video feed", frame);

        output.write(frame);

        if (cv::waitKey(25) >= 0)
        {
            break;
        }
    }

    output.release();
    video.release();
    cv::destroyAllWindows();

    return 0;
}
