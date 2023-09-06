#include "opencv2/opencv.hpp"
#include <iostream>

//void eachWindow(cv::VideoCapture& cap);
//void combinedWindow(cv::Mat &reframe, cv::Mat &Gray3channel, cv::Mat &canny3channel);
void on_mouse(int event, int x, int y, int flags, void*);
void draggedImg(cv::Mat &image);


class Window{
private:
    cv::Mat frame, reframe;
    cv::Mat bgrToGray, Gray3channel;
    cv::Mat cannyEdge, canny3channel;
    

public:
    cv::Mat combineImg;

    void eachWindow(cv::VideoCapture& cap);
    void combinedWindow(cv::Mat &reframe, cv::Mat &Gray3channel, cv::Mat &canny3channel);

};

int main(int argc, char** argv)
{
    cv::namedWindow("origin", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("grayscale", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("canny", cv::WINDOW_AUTOSIZE);

    std::string video_path;

    if (argc < 2) {
        throw std::runtime_error("missing input parameters!");
    } else if (argc == 2) {
        video_path = argv[1];
    } else {
        throw std::runtime_error("too many input parameters!");
    }

    cv::VideoCapture cap(video_path);

    if (!cap.isOpened())
    {
        printf("Can't open the camera");
        return -1;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "fps : " << fps << std::endl;
    std::cout << "width : " << width << std::endl;
    std::cout << "height : " << height << std::endl;

    //eachWindow(cap);
    Window window;
    window.eachWindow(cap);
    
    cv::destroyAllWindows();

    return 0;
}

void Window::eachWindow(cv::VideoCapture& cap)
{
    while(1)
    {
        cap >> frame;
        cv::resize(frame, reframe, cv::Size(640, 360));  // Size(cols, rows)
        
        if(reframe.empty())
        {
            break;
        }

        cv::cvtColor(reframe, bgrToGray, cv::COLOR_BGR2GRAY);           // Gray
        cv::cvtColor(bgrToGray, Gray3channel, cv::COLOR_GRAY2BGR);
        cv::Canny(Gray3channel, cannyEdge, 50, 100);                         // Canny
        cv::cvtColor(cannyEdge, canny3channel, cv::COLOR_GRAY2BGR);

        cv::imshow("origin", reframe);
        cv::imshow("grayscale", Gray3channel);
        cv::imshow("canny", canny3channel);

        combinedWindow(reframe, Gray3channel, canny3channel);

        if(cv::waitKey(25) == 27)
        {
            break;
        }
    }

}

void Window::combinedWindow(cv::Mat &reframe, cv::Mat &Gray3channel, cv::Mat &canny3channel)
{
    combineImg = cv::Mat::zeros(cv::Size(1920, 360), CV_8UC3);         // Size(cols, rows)
    //std::cout << "rows : " << combineImg.rows << ", " << "cols : " << combineImg.cols << std::endl;
    cv::namedWindow("combine", cv::WINDOW_AUTOSIZE);
    
    cv::Mat firstImg = combineImg(cv::Rect(0, 0, 640, 360));
    cv::Mat secondImg = combineImg(cv::Rect(640, 0, 640, 360));
    cv::Mat thirdImg = combineImg(cv::Rect(1280, 0, 640, 360));
    
    reframe.copyTo(firstImg);
    Gray3channel.copyTo(secondImg);
    canny3channel.copyTo(thirdImg);

    cv::imshow("combine", combineImg);
    cv::setMouseCallback("combine", on_mouse, &combineImg);


}


cv::Point ptOld, ptNew, ptDragged;
cv::Point NewX, NewY;
void on_mouse(int event, int x, int y, int flags, void* imgData)
{
    cv::Mat* ptrcombine = (cv::Mat*)imgData;
    cv::Mat combineWindow = *ptrcombine;

    cv::Vec3b& pixel = combineWindow.at<cv::Vec3b>(ptOld);
    int b1 = pixel[0];
    int g1 = pixel[1];
    int r1 = pixel[2];

    cv::Vec3b& ptrPixel = combineWindow.at<cv::Vec3b>(ptDragged);
    int b2, g2, r2;

    switch (event) {
        case cv::EVENT_LBUTTONDOWN:             // 마우스 왼쪽 버튼 누른 경우
            ptOld = cv::Point(x, y);
            std::cout << "EVENT_LBUTTONDOWN: " << x << ", " << y << std::endl;
            cv::putText(combineWindow, cv::format("X : %d, Y : %d", x, y), ptOld, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1.5);

            std::cout << "B : " << b1 << ", G : " << g1 << ", R : " << r1 << std::endl;
            cv::putText(combineWindow, cv::format("B : %d, G : %d, R : %d", b1, g1, r1), cv::Point(x, y+15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1.5);           
            
            cv::imshow("combine", combineWindow);   

            break;
/*
        case cv::EVENT_LBUTTONUP:     

            ptNew = cv::Point(x, y);        
            
            if(ptOld != ptNew){
                draggedImg(combineWindow);
                cv::imshow("combine", combineWindow);
                if(cv::waitKey(0))
                {
                    cv::destroyWindow("dragged");
                }
            }
            ptDragged = cv::Point(0, 0);

            break;
*/
        case cv::EVENT_MOUSEMOVE:             // 마우스 움직이는 경우

            if(flags & cv::EVENT_FLAG_LBUTTON){
                ptDragged = cv::Point(x, y);
                cv::putText(combineWindow, cv::format("X : %d, Y : %d", x, y), ptDragged, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1.5);

                b2 = ptrPixel[0];
                g2 = ptrPixel[1];
                r2 = ptrPixel[2];
                cv::putText(combineWindow, cv::format("B : %d, G : %d, R : %d", b2, g2, r2), cv::Point(x, y+15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1.5);            
                
                if (ptDragged.x != 0 && ptDragged.y != 0) {
                    cv::rectangle(combineWindow, ptOld, ptDragged, cv::Scalar(0, 255, 255));
                    cv::imshow("combine", combineWindow);
                }
                
            }
            break;

    
        default:
            break;

    }

    
}

void draggedImg(cv::Mat &image)
{
    cv::Mat draggedImg = image.clone();

    cv::Mat newImg = draggedImg(cv::Rect(ptOld, ptNew));
    cv::Mat highlight;
    newImg.convertTo(highlight, -1, 1, 100);

/*
    cv::Mat highlight, newImg;
    draggedImg.convertTo(highlight, -1, 1, 100);
    newImg = highlight(cv::Rect(ptOld, ptNew));
*/
    cv::imshow("dragged", highlight);
    cv::imshow("combine", newImg);    

}



/*
void eachWindow(cv::VideoCapture& cap)
{
    cv::Mat frame;
    cv::Mat reframe;
    cv::Mat bgrToGray, Gray3channel;
    cv::Mat cannyEdge, canny3channel;
    while(1)
    {
        cap >> frame;
        cv::resize(frame, reframe, cv::Size(640, 360));  // Size(cols, rows)
        
        if(reframe.empty())
        {
            break;
        }

        //bgrToGray = ~reframe;
        cv::cvtColor(reframe, bgrToGray, cv::COLOR_BGR2GRAY);           // Gray
        cv::cvtColor(bgrToGray, Gray3channel, cv::COLOR_GRAY2BGR);
        cv::Canny(Gray3channel, cannyEdge, 50, 100);                         // Canny
        cv::cvtColor(cannyEdge, canny3channel, cv::COLOR_GRAY2BGR);


        cv::imshow("origin", reframe);
        cv::imshow("grayscale", Gray3channel);
        cv::imshow("canny", canny3channel);

        combinedWindow(reframe, Gray3channel, canny3channel);

        if(cv::waitKey(15) == 27)
        {
            break;
        }
    }

}

void combinedWindow(cv::Mat &reframe, cv::Mat &Gray3channel, cv::Mat &canny3channel)
{
    //cv::Mat blankImg;
    //blankImg =
    cv::Mat blankImg = cv::Mat::zeros(cv::Size(1920, 360), CV_8UC3);         // Size(cols, rows)
    //std::cout << "rows : " << blankImg.rows << ", " << "cols : " << blankImg.cols << std::endl;
    cv::namedWindow("combine", cv::WINDOW_AUTOSIZE);
    
    cv::Mat firstImg = blankImg(cv::Rect(0, 0, 640, 360));
    cv::Mat secondImg = blankImg(cv::Rect(640, 0, 640, 360));
    cv::Mat thirdImg = blankImg(cv::Rect(1280, 0, 640, 360));
    
    reframe.copyTo(firstImg);
    Gray3channel.copyTo(secondImg);
    canny3channel.copyTo(thirdImg);
    //reframe.copyTo(secondImg);
    //reframe.copyTo(thirdImg);

    //cv::setMouseCallback("combine", on_mouse);
    cv::imshow("combine", blankImg);

}
*/