/*
 * Copyright (C) 2020  Christian Berger
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

// Include the single-file, header-only middleware libcluon to create high-performance microservices
#include "cluon-complete.hpp"
// Include the OpenDLV Standard Message Set that contains messages that are usually exchanged for automotive or robotic applications 
#include "opendlv-standard-message-set.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>

#define PI 3.14159265

void createWindow(cv::Mat);
void dilate(cv::Mat);

void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);
	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0,255,0), thickness, 8);
}


void createWindow(cv::Mat img, double* degree)
{
    cv::Mat imgHSV;
    cv::Mat imgColorSpace; //Yellow cones
    cv::Mat imgColorSpace2; //Blue cones    
    cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    
    cv::inRange(imgHSV, cv::Scalar(47, 87, 34), cv::Scalar(126, 255, 86), imgColorSpace2); //Blue
    cv::inRange(imgHSV, cv::Scalar(15, 127, 142), cv::Scalar(120, 255, 255), imgColorSpace); //Yellow
               
    //yellow cones 
    cv::Rect myROI2(1, 200, 620, 280);
    cv::Mat yellowCones = imgColorSpace(myROI2); //smaller frame containing yellow cones
    cv::rectangle(yellowCones, cv::Point(330, 200), cv::Point(450, 280), cv::Scalar(0,0,0), -1);

    //Blue cones
    cv::Rect myROI(1, 200, 620, 280);
    cv::Mat blueCones = imgColorSpace2(myROI); //smaller frame containing blue cones
    
    //Dilates blue cones
    dilate(yellowCones); 
    dilate(blueCones); 
    
    //Blue
    std::vector<std::vector<cv::Point> > blueContours;
    std::vector<cv::Vec4i> blueHierarchy(blueContours.size());        
    findContours( blueCones, blueContours, blueHierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );    
    std::vector<std::vector<cv::Point> > blueApprox( blueContours.size() );
    std::vector<cv::Point2f>blueCenters( blueContours.size() );
    std::vector<float>blueRadius( blueContours.size() );
            
    for( size_t i = 0; i < blueContours.size(); i++ )
    {
        cv::approxPolyDP( blueContours[i], blueApprox[i], 5, true); 
        cv::minEnclosingCircle( blueApprox[i], blueCenters[i], blueRadius[i] );
    }
    
    cv::Mat drawing = cv::Mat::zeros( blueCones.size(), CV_8UC3 );
    cv::Mat drawing2 = cv::Mat::zeros( yellowCones.size(), CV_8UC3 );
    for( size_t i = 0; i< blueContours.size(); i++ )
    {                      
        drawContours( drawing, blueContours, (int)i, cv::Scalar( 255, 255, 255 ), 2, cv::FILLED, blueHierarchy, 0 ); // Not needed
        if(blueRadius[i] < 45 && blueRadius[i] > 10)
        {
            cv::circle( drawing, blueCenters[i], (int)blueRadius[i], cv::Scalar(255,255,0), 2 ); // Draws circles on contours found on the drawing Mat, used to visualize i.e not needed for final
        }
        
        // 3 lines below is just used right now to print coordinates of the middlepoint in the circle that it draws
        std::stringstream temp;
        temp << "X: " << blueRadius[i];
        setLabel(drawing, temp.str(), blueContours[i]);                             
    }    
       
    //Yellow
    std::vector<std::vector<cv::Point> > yellowContours;
    std::vector<cv::Vec4i> yellowHierarchy(yellowContours.size());        
    findContours( yellowCones, yellowContours, yellowHierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );    
    std::vector<std::vector<cv::Point> > yellowApprox( yellowContours.size() );
    std::vector<cv::Point2f>yellowCenters( yellowContours.size() );
    std::vector<float> yellowRadius( yellowContours.size() );
    
    for( size_t i = 0; i < yellowContours.size(); i++ )
    {
        cv::approxPolyDP( yellowContours[i], yellowApprox[i], 5, true); 
        cv::minEnclosingCircle( yellowApprox[i], yellowCenters[i], yellowRadius[i] );
    }
    for( size_t i = 0; i< yellowContours.size(); i++ )
    {                      
        drawContours( drawing2, yellowContours, (int)i, cv::Scalar( 255, 255, 255 ), 2, cv::FILLED, yellowHierarchy, 0 ); // Not needed
        if(yellowRadius[i] < 45 && yellowRadius[i] > 10)
        {
            cv::circle( drawing2, yellowCenters[i], (int)yellowRadius[i], cv::Scalar(255,255,0), 2 ); // Draws circles on contours found on the drawing Mat, used to visualize i.e not needed for final
        }
        
        // 3 lines below is just used right now to print coordinates of the middlepoint in the circle that it draws
        std::stringstream temp;
        temp << "X: " << yellowRadius[i];
        setLabel(drawing2, temp.str(), yellowContours[i]);                             
    }
    
    
    
    std::cout << yellowCenters.size() << std::endl;
    
   *degree = 0;
    
    if(yellowCenters.size() == 0)
    {
        for(uint i = 0; i < blueCenters.size(); i++)
        {
            if(blueRadius[i] < 45 && blueRadius[i] > 10 && blueRadius[i+1] < 45 && blueRadius[i+1] > 10 )
            {
                double m = 0;
                cv::Point value1(static_cast<cv::Point2f>(blueCenters[i]));
                cv::Point value2(static_cast<cv::Point2f>(blueCenters[i+1]));
                cv::line(drawing, value1, // This line just draws a line between 2 coordinates to visualize between what coordinates the angle is calculated
                    value2, cv::Scalar(255, 140, 0), 2, cv::LINE_8);                                                                
                
                m = ((static_cast<float>(value2.y - value1.y)) / (static_cast<float>(value2.x - value1.x)));
                *degree = std::atan (m) * 180 / PI;                               
                *degree = *degree / 100;         
                std::cout << "Degree " << *degree << std::endl;
            }            
        }
    }
    else
    {
        std::cout << "Degree " << *degree << std::endl;
    }
        
    cv::imshow("imgColorspace2", imgColorSpace2);
    cv::imshow("blue cones drawing", drawing);
    cv::imshow("BlueCones", blueCones);
    cv::imshow("YellowCones", yellowCones);
    cv::imshow("Yellow Cones drawing", drawing2);


}

void dilate(cv::Mat cropped)
{
    cv::Mat element = cv::getStructuringElement( 0 ,
                       cv::Size( 5, 5 ),
                       cv::Point( 3, 3 ) );

    cv::dilate(cropped, cropped, element);
}


int32_t main(int32_t argc, char **argv) {
    int32_t retCode{1};
    // Parse the command line parameters as we require the user to specify some mandatory information on startup.
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if ( (0 == commandlineArguments.count("cid")) ||
         (0 == commandlineArguments.count("name")) ||
         (0 == commandlineArguments.count("width")) ||
         (0 == commandlineArguments.count("height")) ) {
        std::cerr << argv[0] << " attaches to a shared memory area containing an ARGB image." << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OD4 session> --name=<name of shared memory area> [--verbose]" << std::endl;
        std::cerr << "         --cid:    CID of the OD4Session to send and receive messages" << std::endl;
        std::cerr << "         --name:   name of the shared memory area to attach" << std::endl;
        std::cerr << "         --width:  width of the frame" << std::endl;
        std::cerr << "         --height: height of the frame" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=253 --name=img --width=640 --height=480 --verbose" << std::endl;
    }
    else {
        // Extract the values from the command line parameters
        const std::string NAME{commandlineArguments["name"]};
        const uint32_t WIDTH{static_cast<uint32_t>(std::stoi(commandlineArguments["width"]))};
        const uint32_t HEIGHT{static_cast<uint32_t>(std::stoi(commandlineArguments["height"]))};
        const bool VERBOSE{commandlineArguments.count("verbose") != 0};

        // Create file to store steering Angle
        char steeringAngleData[ ] = "calculated-steering-angle.csv";
        std::ofstream calAngle;
        calAngle.open(steeringAngleData, std::ios::out | std::ios::app);
        calAngle << "sampleTimeStamp.microseconds;calculatedSteeringAngle;" << std::endl;
        calAngle.close();

        // Attach to the shared memory.
        std::unique_ptr<cluon::SharedMemory> sharedMemory{new cluon::SharedMemory{NAME}};
        if (sharedMemory && sharedMemory->valid()) {
            std::clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << std::endl;

            // Interface to a running OpenDaVINCI session where network messages are exchanged.
            // The instance od4 allows you to send and receive messages.
            cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};
            
            opendlv::proxy::GroundSteeringRequest gsr;
            std::mutex gsrMutex;
            auto onGroundSteeringRequest = [&gsr, &gsrMutex](cluon::data::Envelope &&env){
                std::lock_guard<std::mutex> lck(gsrMutex);
                gsr = cluon::extractMessage<opendlv::proxy::GroundSteeringRequest>(std::move(env));
            };
        
            od4.dataTrigger(opendlv::proxy::GroundSteeringRequest::ID(), onGroundSteeringRequest);

            // Endless loop; end the program by pressing Ctrl-C.
            while (od4.isRunning()) {
                // OpenCV data structure to hold an image.
                cv::Mat img;

                // Wait for a notification of a new frame.
                sharedMemory->wait();

                // Lock the shared memory.
                sharedMemory->lock();
                {
                    // Copy the pixels from the shared memory into our own data structure.
                    cv::Mat wrapped(HEIGHT, WIDTH, CV_8UC4, sharedMemory->data());
                    img = wrapped.clone();
                }                          
                cluon::data::TimeStamp sampleTime {sharedMemory.get()->getTimeStamp().second};
                sharedMemory->unlock();                
                
                double degree;
                createWindow(img, &degree);

                // Save calculated steering angle to file
                int64_t micSecs = cluon::time::toMicroseconds(sampleTime);
                calAngle.open(steeringAngleData, std::ios::out | std::ios::app);
                if (calAngle.is_open()){
                    calAngle << micSecs << ";" << degree << ";" << std::endl;
                }
                calAngle.close();            
                {
                    std::lock_guard<std::mutex> lck(gsrMutex);
                    std::cout << "main: groundSteering = " << gsr.groundSteering() << std::endl;
                }
                    
                // Display image on your screen.
                if (VERBOSE) {
                    cv::imshow(sharedMemory->name().c_str(), img);
                    cv::waitKey(1);
                }
            }
        }
        retCode = 0;
    }
    return retCode;
}

