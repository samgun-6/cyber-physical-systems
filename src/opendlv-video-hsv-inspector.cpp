/*
 * Copyright (C) 2018  Christian Berger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cluon-complete.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>

void createWindow(cv::Mat);

void dilate(cv::Mat);

void createWindow(cv::Mat img)
{
    cv::Mat imgHSV;
    cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);


    cv::Mat imgColorSpace;
    cv::Mat imgColorSpace2;

//    cv::imshow("Before filter", img);

    //cv::blur( img, imgColorSpace2, cv::Size( 10, 10 ), cv::Point(-1, -1) );                    
//    cv::medianBlur(img, imgColorSpace2, 35);
    

//    cv::imshow("After filter", dst);
       
    cv::inRange(imgHSV, cv::Scalar(15, 6, 113), cv::Scalar(82, 255, 255), imgColorSpace); //Yellow
    cv::inRange(imgHSV, cv::Scalar(47, 87, 34), cv::Scalar(126, 255, 86), imgColorSpace2); //Blue

    cv::imshow("After Inrange yellow", imgColorSpace);
    cv::imshow("After Inrange blue", imgColorSpace2);
        
//    cv::Mat cannyImg;
//    cv::Canny(imgColorSpace, cannyImg, 0, 50, 5);
       
    cv::Mat combined; //Holds the blue and yellow cones in the same picture by cropping

    cv::add(imgColorSpace, imgColorSpace2, combined);
    cv::Rect myROI(1, 200, 620, 280);
    cv::Mat cropped = combined(myROI);

//    cv::imshow("croped", cropped);
//    cv::imshow("YellowCones", cannyImg);
//    cv::imshow("BluecCones", imgColorSpace2);
//    cv::imshow("Combined", combined);
       
    cv::imshow("croped", cropped);
    dilate(cropped);
    
    cv::Mat mask = cv::Mat::zeros(cropped.rows, cropped.cols, CV_64FC1);
    cv::rectangle(cropped, cv::Point(330, 200), cv::Point(450, 280), cv::Scalar(0,0,0), -1);
//    cv::rectangle(mask, (0, 90), (290, 450), 255, -1);
//    cv::bitwise_xor(cropped, cropped, mask);
    
    cv::imshow("dialted", cropped);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    findContours( cropped, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
    cv::Mat drawing = cv::Mat::zeros( cropped.size(), CV_8UC3 );

    std::vector<cv::Point> approx;                
    for( size_t i = 0; i< contours.size(); i++ )
    {                                                
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);      
        if(approx.size() == 3)
        {
        }	
            drawContours( drawing, contours, (int)i, cv::Scalar( 255, 0, 0 ), 2, cv::FILLED, hierarchy, 0 ); 
         // std::cout << "position X = "<< contours[i][0] <<",\t position Y = "<< contours[i][1] << std::endl;          
    }
    cv::imshow("drawing", drawing);
}

void dilate(cv::Mat cropped)
{
    cv::Mat element = cv::getStructuringElement( 0 ,
                       cv::Size( 7, 7 ),
                       cv::Point( 3, 3 ) );

    cv::dilate(cropped, cropped, element);
}


int32_t main(int32_t argc, char **argv) {
    int32_t retCode{1};
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if ( (0 == commandlineArguments.count("name")) ||
         (0 == commandlineArguments.count("width")) ||
         (0 == commandlineArguments.count("height")) ) {
        std::cerr << argv[0] << " attaches to a shared memory area containing an ARGB image and transform it to HSV color space for inspection." << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --name=<name of shared memory area> --width=<W> --height=<H>" << std::endl;
        std::cerr << "         --name:   name of the shared memory area to attach" << std::endl;
        std::cerr << "         --width:  width of the frame" << std::endl;
        std::cerr << "         --height: height of the frame" << std::endl;
        std::cerr << "Example: " << argv[0] << " --name=img.argb --width=640 --height=480" << std::endl;
    }
    else {
        const std::string NAME{commandlineArguments["name"]};
        const uint32_t WIDTH{static_cast<uint32_t>(std::stoi(commandlineArguments["width"]))};
        const uint32_t HEIGHT{static_cast<uint32_t>(std::stoi(commandlineArguments["height"]))};

        // Attach to the shared memory.
        std::unique_ptr<cluon::SharedMemory> sharedMemory{new cluon::SharedMemory{NAME}};
        if (sharedMemory && sharedMemory->valid()) {
            std::clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << std::endl;

            // Create an OpenCV image header using the data in the shared memory.
            IplImage *iplimage{nullptr};
            CvSize size;
            size.width = WIDTH;
            size.height = HEIGHT;

            iplimage = cvCreateImageHeader(size, IPL_DEPTH_8U, 4 /* four channels: ARGB */);
            sharedMemory->lock();
            {
                iplimage->imageData = sharedMemory->data();
                iplimage->imageDataOrigin = iplimage->imageData;
            }
            sharedMemory->unlock();
            
            // Endless loop; end the program by pressing Ctrl-C.
            std::string operation;
            while (cv::waitKey(1)) {
                cv::Mat img;
         
                // Don't wait for a notification of a new frame so that the sender can pause while we are still inspection
                //sharedMemory->wait();

                // Lock the shared memory.
                sharedMemory->lock();
                {
                    // Copy image into cvMat structure.
                    // Be aware of that any code between lock/unlock is blocking
                    // the camera to provide the next frame. Thus, any
                    // computationally heavy algorithms should be placed outside
                    // lock/unlock.
                    img = cv::cvarrToMat(iplimage);
                }
                sharedMemory->unlock();                
                createWindow(img);                
            }

            if (nullptr != iplimage) {
                cvReleaseImageHeader(&iplimage);
            }
        }
        retCode = 0;
    }
    return retCode;
}
