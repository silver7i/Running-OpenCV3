#include <fstream>
#include <map>
#include <chrono>
#include "face_detector.h"
#include "scrfd_trt.h"

#define INPUT_H 640
#define INPUT_W 640

const int alpha_slider_max = 10;
std::string trt_path;
cv::Mat image;
cv::Mat filterImg;
int alpha_slider = 0;


int main(int argc, char** argv) {
    
    std::string image_path;
    int repeat_times = 1;
    
    std::string title = "result";
    cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Alpha", title, &alpha_slider, alpha_slider_max);

    if (argc < 3) {
        throw std::runtime_error("missing input parameters!");
    } else if (argc == 3) {
        trt_path = argv[1];
        image_path = argv[2];
    } else if (argc == 4) {
        trt_path = argv[1];
        image_path = argv[2];
        repeat_times = strtol(argv[3], NULL, 10);
    } else {
        throw std::runtime_error("too many input parameters!");
    }

    cv::VideoCapture cap(image_path);
    
    filterImg = cv::imread("../test_images/skeleton.png");
    std::cout << "Image Channels : " << filterImg.channels() << std::endl;

    if (!cap.isOpened())
    {   
        std::cerr << "video open failed!\n";
        return -1;
    }

    double width = cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    double height = cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int count = cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT));  // 비디오파일의 전체 프레임 수
    int fps = cvRound(cap.get(cv::CAP_PROP_FPS));            // 초당 프레임 수

    printf("Frame width: %lf\n", width);
    printf("Frame height: %lf\n", height);
    printf("Frame count: %d\n", count);
    printf("Frame fps: %d\n", fps);

    int framecount = 0;
    SCRFD_TRT* scrfd_trt = new SCRFD_TRT(trt_path);
    
    cv::Mat temp;
    while(1)
    {  
        cap >> image;
        std::cout << "Video Channels : " << image.channels() << std::endl;

        if (image.empty()){
            break;
        }
/*
        if (temp.empty()){
            temp = image.clone();
        }

        image = temp.clone();
*/
        std::vector<FaceObject> faceobjects;
        
        auto start = std::chrono::high_resolution_clock::now();
        for (int i=0; i<repeat_times; i++) {
            scrfd_trt->detect(image, faceobjects, 0.25, 0.4);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        //std::cout << "fps: " << float(repeat_times) / duration.count() * 1000 << "\n";

        std::cout << "framecount  " << ++framecount << std::endl;
        scrfd_trt->draw(image, filterImg, faceobjects, alpha_slider);
    
        cv::imshow(title, image);
        if (cv::waitKey(15) == 27){
            break;
        };

    }
    delete scrfd_trt;
    
    cv::destroyAllWindows();

    return 0;
}

 /*  
    cv::Mat image = cv::imread(image_path.c_str());

    std::vector<FaceObject> faceobjects;
    SCRFD_TRT* scrfd_trt = new SCRFD_TRT(trt_path);
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i=0; i<repeat_times; i++) {
        scrfd_trt->detect(image, faceobjects, 0.25, 0.4);
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "fps: " << float(repeat_times) / duration.count() * 1000 << "\n";
    scrfd_trt->draw(image, faceobjects);

    cv::imshow("result", image);
    cv::waitKey(1);

    return 0;
}

*/