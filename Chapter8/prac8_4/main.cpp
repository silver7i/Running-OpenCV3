#include "opencv2/opencv.hpp"
#include <iostream>

int sliderPosition = 0;
int run = 1, dontset = 0;
cv::VideoCapture cap;
void onTrackbarSlide(int pos, void *)
{
    if(!dontset){
        run = 1;
        cap.set(cv::CAP_PROP_POS_FRAMES, pos);
    }
    dontset = 0;
}

int main(int argc, char** argv)
{
    cv::namedWindow("origin", cv::WINDOW_AUTOSIZE);
    cap.open(std::string(argv[1]));
    int frames = (int) cap.get(cv::CAP_PROP_FRAME_COUNT);
    int width = (int) cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    std::cout << "frames" << frames << std::endl;
    std::cout << "width" << width << std::endl;
    std::cout << "height" << height << std::endl;
    
    cv::createTrackbar("Position", "origin", &sliderPosition, frames, onTrackbarSlide);

    cv::Mat frame;
    while(1){
        if(run != 0){
            cap >> frame;
            if(frame.empty()){
                break;
            }

            int current_pos = (int)cap.get(cv::CAP_PROP_POS_FRAMES);
            dontset = 1;
            
            cv::setTrackbarPos("Position", "origin", current_pos);
            cv::imshow("origin", frame);

            run -= 1;
        }

        char c = (char)cv::waitKey(10);
        if(c == 's'){
            run = 1;
            std::cout << "single step, run = " << run << std::endl;
        }
        if(c == 'r'){
            run = -1;
            std::cout << "Run mode, run = " << run << std::endl;
        }
        if(c == 27){
            break;
        }
    }

    return 0;
}

