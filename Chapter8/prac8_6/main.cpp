#include "opencv2/opencv.hpp"
#include <iostream>

cv::Point ptOld, ptNew;
cv::Mat img, cloneImg;
int enterKey;

void on_mouse(int event, int x, int y, int flags, void*){
    //std::cout << "flags: " << flags << std::endl;

    switch(event){
        case cv::EVENT_LBUTTONDOWN: {
            cv::imshow("img", img);

            ptOld = cv::Point(x, y);
            std::cout << "ptOld: " << ptOld << std::endl;

            std::cout << "Input Text"<< std::endl;

            cv::Size sizeText;

            img.copyTo(cloneImg);
    
            int i = 0;
            while(1){
                int inKey = cv::waitKey();
                std::cout << "inKey : " << inKey << std::endl;
                
                if(inKey >= 48 && inKey <= 57){
                    inKey -= 48;
                    cv::String sKey = std::to_string(inKey);

                    sizeText = cv::getTextSize(sKey, 1, 1, 1, 0);
                    std::cout << "sizeText width : " << sizeText.width << std::endl;
                    std::cout << "sizeText height : " << sizeText.height << std::endl;

                    std::cout << "String : " << sKey << std::endl;
                    cv::putText(cloneImg, sKey, cv::Point(ptOld.x+i, ptOld.y), 1, 1, cv::Scalar(255, 255, 0), 1, 8);


                    i += sizeText.width;
                } else if(inKey == 8 && ptOld.x+i != ptOld.x){                     // backspace
                    i -= sizeText.width;
                    cv::rectangle(cloneImg, cv::Rect(cv::Point(ptOld.x+i, ptOld.y), cv::Point(ptOld.x+i+sizeText.width, ptOld.y-sizeText.height)), cv::Scalar(0, 0, 0), -1);

                } else if(inKey == 13){             // 'enter'
                    cloneImg.copyTo(img);
                    cv::imshow("img", img);
                    break;

                } else if(inKey == 70 || inKey == 102){             // 'f'
                    break;

                }

                cv::imshow("img", cloneImg);
            }
            std::cout << "putText End"<< std::endl;
            break;
        }

        default:
            break;

    }
}

int main(int argc, char** argv)
{
    img = cv::Mat(800, 800, CV_8UC1, cv::Scalar(0));

    cv::namedWindow("img");

    cv::setMouseCallback("img", on_mouse);

    cv::imshow("img", img);

    while(1){
        if(cv::waitKey(0)==27)
            break;
    }

    
    cv::destroyAllWindows();

    return 0;
}

