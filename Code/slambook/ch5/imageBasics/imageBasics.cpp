#include <iostream>
#include <chrono>

using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char** argv) {
    // read the image from argv[1]
    cv::Mat image;
    image = cv::imread(argv[1]);

    if (image.data == nullptr) {
        cerr << "file " << argv[1] << " doesn't exist." << endl;
        return 0;
    }

    // Basic image info
    cout << "Image width: " << image.cols << ", height: " << image.rows
         << ", channels: " << image.channels() << endl;
    cv::imshow("image", image);
    cv::waitKey(1000);  //ms

    // judge image type
    if (image.type() != CV_8UC1 && image.type() != CV_8UC3) {
        cout << "Image doesn't meet requirements, please input a color or gray-scale iamge." << endl;
        return 0;
    }

    // traverse image
    // use std::chrono for algorithm timing
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    for (size_t y = 0; y < image.rows; ++y) {
        // use cv::Mat::ptr for the row pointer
        unsigned char* row_ptr = image.ptr<unsigned char>(y);
        for (size_t x = 0; x < image.cols; ++x) {
            unsigned char* data_ptr = &row_ptr[x* image.channels()];
            for (int c = 0; c != image.channels(); ++c)
                unsigned char data = data_ptr[c];
        }
    }
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>> (t2-t1);
    cout << "Traversing the image cost: " << time_used.count() << " seconds." << endl;

//    // copy of cv::Mat
//    cv::Mat image_another = image;
//    // modifying image_another will cause the change to iamge
//    image_another(cv::Rect(0, 0, 100, 100)).setTo(0);
//    cv::imshow("image", image);
//    cv::waitKey(1000);

    // using clone()
    cv::Mat image_clone = image.clone();
    image_clone(cv::Rect(0, 0, 100, 100)).setTo(255);
    cv::imshow("image", image);
    cv::imshow("image_clone", image_clone);
    cv::waitKey(0);

    cv::destroyAllWindows();

    return 0;
}
