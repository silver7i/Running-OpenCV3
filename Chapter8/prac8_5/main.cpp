#include "opencv2/opencv.hpp"
#include <iostream>

cv::Mat board, draw;
void on_mouse(int event, int x, int y, int flags, void*);

int main(int argc, char** argv)
{
    cv::namedWindow("board", cv::WINDOW_AUTOSIZE);
    
    
    board = cv::Mat(cv::Size(500, 500), CV_8UC1, cv::Scalar(0));
    board.copyTo(draw);

    cv::setMouseCallback("board", on_mouse);
    cv::imshow("board", board);
    
    while(1){
        if(cv::waitKey(0) == 27){
            break;
        };
    }
    return 0;
}


cv::Point ptOld, ptNew;
void on_mouse(int event, int x, int y, int flags, void*)
{
    //std::cout << "flags : " << flags << std::endl;
    
    switch (event) {
        case cv::EVENT_LBUTTONDOWN:
            ptOld = cv::Point(x, y);
            break;

        case cv::EVENT_LBUTTONUP:
            if(flags & cv::EVENT_FLAG_CTRLKEY){
                if(ptNew.x != 0 && ptNew.y != 0){
                    cv::circle(draw, ptOld, abs(ptNew.x-ptOld.x), cv::Scalar(255, 255, 0), 1);
                    cv::imshow("board", draw);
                    ptNew = cv::Point(0, 0);
                }
            } else if(flags & cv::EVENT_FLAG_SHIFTKEY){
                if(ptNew.x != 0 && ptNew.y != 0){
                    cv::ellipse(draw, ptOld, cv::Size(abs(ptNew.x-ptOld.x), abs(ptNew.y-ptOld.y)), 30, 0, 360, cv::Scalar(255, 255, 0), 1);
                    cv::imshow("board", draw);
                    ptNew = cv::Point(0, 0);
                }
            } else if(flags & cv::EVENT_FLAG_ALTKEY){
                if(ptNew.x != 0 && ptNew.y != 0){
                    cv::rectangle(draw, ptOld, ptNew, cv::Scalar(255, 0, 0), 1);
                    cv::imshow("board", draw);
                    ptNew = cv::Point(0, 0);
                }
            }
            break;

        
        case cv::EVENT_RBUTTONDOWN:
            ptOld = cv::Point(x, y);    
            break;

        case cv::EVENT_MOUSEMOVE:
        std::cout << "flags " << flags<<std::endl;
            if(flags == (cv::EVENT_FLAG_LBUTTON | cv::EVENT_FLAG_CTRLKEY)){
                ptNew = cv::Point(x,y);

            } else if (flags == (cv::EVENT_FLAG_LBUTTON | cv::EVENT_FLAG_SHIFTKEY)) {
                ptNew = cv::Point(x, y);

            } else if (flags == (cv::EVENT_FLAG_LBUTTON | cv::EVENT_FLAG_ALTKEY)) {
                ptNew = cv::Point(x, y);

            } else if (flags & cv::EVENT_FLAG_LBUTTON) {
                cv::line(draw, ptOld, cv::Point(x, y), cv::Scalar(255, 255, 255), 2);
                cv::imshow("board", draw);
                ptOld = cv::Point(x, y);

            } else if (flags & cv::EVENT_FLAG_RBUTTON) {
                cv::line(draw, ptOld, cv::Point(x, y), cv::Scalar(0, 0, 0), 5);
                cv::imshow("board", draw);
                ptOld = cv::Point(x, y);
            }
            break;

    }
}