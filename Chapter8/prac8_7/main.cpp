#include "opencv2/opencv.hpp"
#include <iostream>

void mattrix(cv::Mat& src, cv::Mat& dst, cv::Mat M);
void zoom(cv::Mat& src, cv::Mat& dst, float scale);
void rotation(cv::Mat& src, cv::Mat& dst, float rotate);

int main(int argc, char** argv)
{
    cv::Mat img, resizeImg;

    if (argc < 2) {
        throw std::runtime_error("missing input parameters!");
    } else if (argc == 2) {
        img = cv::imread(argv[1]);
    } else {
        throw std::runtime_error("too many input parameters!");
    }

    if (img.empty()) {
		std::cout << "Image load failed!" << std::endl;
		return -1;
	}

    cv::resize(img, resizeImg, cv::Size(1200, 800));
    cv::namedWindow("img");
    
    cv::Mat M = cv::Mat::eye(3, 3, CV_32F);
    cv::imshow("img", resizeImg);
    //std::cout << M << std::endl;

    cv::Mat src, dst;
    float scale = 1;
    float rotate = 0;
    while(1)
    { 
        int inKey = cv::waitKey();
        
        if(inKey >= 49 && inKey <= 57) {             // 1 ~ 9
            int col = (inKey - 49) % 3;
            int row = (inKey - 49) / 3;
            M.at<float>(row, col) += 0.1;
            std::cout << "M : " << M << std::endl;

        } else if (inKey == 0x69) {       // i - ZoomIn
            scale += 0.1;
            
        } else if (inKey == 0x6F) {       // o - ZoomOut
            scale -= 0.1;
            
        } else if (inKey == 0x73) {       // s
            rotate += 10;
            
        } else if (inKey == 0x66) {       // f
            rotate -= 10;
            
        } else if (inKey == 27) {          // esc
            cv::destroyWindow("Perspective Image");
            break;
        }

        cv::Mat tm1, tm2, tm3;
        zoom(resizeImg, tm1, scale);
        rotation(tm1, tm2, rotate);
        mattrix(tm2, tm3, M);

        cv::namedWindow("Perspective Image");
        cv::imshow("Perspective Image", tm3);
    }

    while(1){
        if (cv::waitKey(0) == 27){
            break;
        }
    }
    cv::destroyAllWindows();

    return 0;
}

void mattrix(cv::Mat& src, cv::Mat& dst, cv::Mat M)
{
    cv::warpPerspective(src, dst, M, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
}



void zoom(cv::Mat& src, cv::Mat& dst, float scale)
{
    cv::resize(src, dst, cv::Size(0, 0), scale, scale);
}


void rotation(cv::Mat& src, cv::Mat& dst, float rotate)
{
    cv::Point center = cv::Point(src.cols / 2, src.rows / 2);
    double scale = 1.0;

    cv::Mat rot_mat = cv::getRotationMatrix2D(center, rotate, scale);

    cv::warpAffine(src, dst, rot_mat, src.size());
}

/*
cv::Mat zoomImg, M;
void increase(cv::Mat& src, cv::Mat& dst, int inKey);
void decrease(cv::Mat& src, cv::Mat& dst, int inKey);
void zoomIn(cv::Mat& src, cv::Mat& dst);
void zoomOut(cv::Mat& src, cv::Mat& dst);
void rotationL(cv::Mat& src, cv::Mat& dst);
void rotationR(cv::Mat& src, cv::Mat& dst);


int main(int argc, char** argv)
{
    cv::Mat img, smallImg;

    if (argc < 2) {
        throw std::runtime_error("missing input parameters!");
    } else if (argc == 2) {
        img = cv::imread(argv[1]);
    } else {
        throw std::runtime_error("too many input parameters!");
    }

    if (img.empty()) {
		std::cout << "Image load failed!" << std::endl;
		return -1;
	}

    cv::resize(img, smallImg, cv::Size(1200, 800));
    cv::namedWindow("img");
    
    M = cv::Mat::eye(3, 3, CV_32F);
    cv::namedWindow("Perspective Image");
    cv::imshow("img", smallImg);
    //std::cout << M << std::endl;


    cv::Mat src, dst;
    
    while(1)
    { 
        int inKey = cv::waitKey();
        
        if (inKey == 97)                // a
        {
            int inKey2 = cv::waitKey();
            if(inKey2 >= 0x31 && inKey2 <= 0x39){
                std::cout << "decrease" << std::endl;
                decrease(smallImg, dst, inKey2);
            }

        } else if(inKey >= 49 && inKey <= 57) {             // 1 ~ 9
            std::cout << "increase" << std::endl;
            increase(smallImg, dst, inKey);

        } else if (inKey == 0x69) {       // i
            zoomIn(smallImg, dst);
            
        } else if (inKey == 0x6F) {       // o
            zoomOut(smallImg, dst);
            
        } else if (inKey == 0x73) {       // s
            rotationL(smallImg, dst);
            
        } else if (inKey == 0x66) {       // f
            rotationR(smallImg, dst);
            
        } else if (inKey == 27) {          // esc
            cv::destroyWindow("Perspective Image");
            break;
        }

        cv::imshow("Perspective Image", dst);
    }

    while(1){
        if (cv::waitKey(0) == 27){
            break;
        }
    }
    cv::destroyAllWindows();

    return 0;
}

void increase(cv::Mat& src, cv::Mat& dst, int inKey)
{
    std::cout << "inKey : " << inKey << std::endl;
    int col = (inKey - 49) % 3;
    int row = (inKey - 49) / 3;
    M.at<float>(row, col) += 5.0;

    std::cout << "M : " << M << std::endl;

    cv::warpPerspective(src, dst, M, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());

}

void decrease(cv::Mat& src, cv::Mat& dst, int inKey)
{
    std::cout << "inKey : " << inKey << std::endl;
    int col = (inKey - 49) % 3;
    int row = (inKey - 49) / 3;

    std::cout << "M : " << M << std::endl;

    if(M.at<uchar>(row, col) >= 0){
        M.at<uchar>(row, col) -= 5.0;
    }
    
    cv::warpPerspective(src, dst, M, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());

}

void zoomIn(cv::Mat& src, cv::Mat& dst)
{
    cv::resize(src, dst, cv::Size(0, 0), 1.5, 1.5);
}

void zoomOut(cv::Mat& src, cv::Mat& dst)
{
    cv::resize(src, dst, cv::Size(0, 0), 0.5, 0.5);
}

void rotationL(cv::Mat& src, cv::Mat& dst)
{
    cv::Point center = cv::Point(src.cols / 2, src.rows / 2);
    double angle = -45.0;
    double scale = 1.0;

    cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);

    cv::warpAffine(src, dst, rot_mat, src.size());
}

void rotationR(cv::Mat& src, cv::Mat& dst)
{
    cv::Point center = cv::Point(src.cols / 2, src.rows / 2);
    double angle = 45.0;
    double scale = 1.0;

    cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);

    cv::warpAffine(src, dst, rot_mat, src.size());
}

*/