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

void createWindow(cv::Mat, double* degree, double* temp);
void dilate(cv::Mat);
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);  

void mainAlgordom(std::vector<cv::Point2f>& yellowCenters,std::vector<cv::Point2f>& blueCenters,std::vector<float>& yellowRadius, std::vector<float>& blueRadius,  cv::Mat &drawing, double* degree, double* temp  )

{
    if(yellowCenters.size() == 0 && blueCenters.size() != 0 )
    {
        if(blueRadius[0] < 45 && blueRadius[0] > 10 && blueRadius[1] < 45 && blueRadius[1] > 10)
        {
            double m = 0;
            cv::Point value1(static_cast<cv::Point2f>(blueCenters[0]));
            cv::Point value2(static_cast<cv::Point2f>(blueCenters[1]));
            cv::line(drawing, value1, //This line just draws a line between 2 coordinates to visualize between what coordinates the angle is calculated
                value2, cv::Scalar(255, 140, 0), 2, cv::LINE_8);                                                                

            m = ((static_cast<float>(value2.y - value1.y)) / (static_cast<float>(value2.x - value1.x)));
            *degree = std::atan (m) * 180 / PI;                               
            *degree = *degree / 100;

            if(*degree > 0.60 && *degree < 0.70) //In some cases we get a way bigger angle because of noise, thus we divide the large number by reasonable number
            {
                *degree = * degree / 4;
            }
            else if(*degree > 0.70) //If degree is above 70 divide by 5
            {
                *degree = * degree / 5;
            }
            
           else if(*degree < -0.60 && *degree > -0.70) //In some cases we get a way bigger angle because of noise, thus we divide the large number by reasonable number
            {
                *degree = * degree / 4;
            }
            else if(*degree < -0.70) //If degree is above 70 divide by 5
            {
                *degree = * degree / 5;
            }


            *temp = *degree;
        }
    }

else if(blueCenters.size() ==0 && yellowCenters.size() != 0 ){

  if(yellowRadius[0] < 45 && yellowRadius[0] > 10 && yellowRadius[1] < 45 && yellowRadius[1] > 10)
        {
            double m = 0;
            cv::Point value1(static_cast<cv::Point2f>(yellowCenters[0]));
            cv::Point value2(static_cast<cv::Point2f>(yellowCenters[1]));
            cv::line(drawing, value1, //This line just draws a line between 2 coordinates to visualize between what coordinates the angle is calculated
                value2, cv::Scalar(255, 140, 0), 2, cv::LINE_8);                                                                

            m = ((static_cast<float>(value2.y - value1.y)) / (static_cast<float>(value2.x - value1.x)));
            *degree = std::atan (m) * 180 / PI;                               
            *degree = *degree / 100;

            if(*degree > 0.60 && *degree < 0.70) //In some cases we get a way bigger angle because of noise, thus we divide the large number by reasonable number
            {
                *degree = * degree / 4;
            }
            else if(*degree > 0.70) //If degree is above 70 divide by 5
            {
                *degree = * degree / 5;
            }

            //for minus degrees

            else if(*degree < -0.60 && *degree > -0.70) //In some cases we get a way bigger angle because of noise, thus we divide the large number by reasonable number
            {
                *degree = * degree / 4;
            }
            else if(*degree < -0.70) //If degree is above 70 divide by 5
            {
                *degree = * degree / 5;
            }   

            *temp = *degree;
         
        }
   }




    else if(*degree == *temp)
    {
        return;
    }
    else
    {
        *degree = 0;
    }
}

void bCones(std::vector<std::vector<cv::Point> >& blueContours, std::vector<cv::Vec4i>& blueHierarchy, std::vector<std::vector<cv::Point> >& blueApprox,
        std::vector<cv::Point2f>& blueCenters, std::vector<float>& blueRadius, cv::Mat &drawing )
{
    for( size_t i = 0; i < blueContours.size(); i++ )
    {
        cv::approxPolyDP( blueContours[i], blueApprox[i], 5, true); 
        cv::minEnclosingCircle( blueApprox[i], blueCenters[i], blueRadius[i] );
    }
    
    
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
}

void yCones(std::vector<std::vector<cv::Point> >& yellowContours, std::vector<cv::Vec4i>& yellowHierarchy, std::vector<std::vector<cv::Point> >& yellowApprox,
        std::vector<cv::Point2f>& yellowCenters, std::vector<float>& yellowRadius, cv::Mat &drawing2  )
{
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
}


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


void createWindow(cv::Mat img, double* degree, double* temp, const bool* VERBOSE)
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
    cv::Mat drawing = cv::Mat::zeros( blueCones.size(), CV_8UC3 ); //Drawing blue cones
    bCones(blueContours, blueHierarchy, blueApprox, blueCenters, blueRadius, drawing);        
    
       
    //Yellow
    std::vector<std::vector<cv::Point> > yellowContours;
    std::vector<cv::Vec4i> yellowHierarchy(yellowContours.size());        
    findContours( yellowCones, yellowContours, yellowHierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );       
    std::vector<std::vector<cv::Point> > yellowApprox( yellowContours.size() );
    std::vector<cv::Point2f>yellowCenters( yellowContours.size() );
    std::vector<float> yellowRadius( yellowContours.size() );
    cv::Mat drawing2 = cv::Mat::zeros( yellowCones.size(), CV_8UC3 ); //Drawing yellow cones
    yCones(yellowContours, yellowHierarchy, yellowApprox, yellowCenters, yellowRadius, drawing2);
    
    mainAlgordom(yellowCenters,blueCenters,yellowRadius, blueRadius, drawing, degree, temp);


    if(*VERBOSE){    
        cv::imshow("imgColorspace2", imgColorSpace2);
        cv::imshow("blue cones drawing", drawing);
        cv::imshow("BlueCones", blueCones);
        cv::imshow("YellowCones", yellowCones);
        cv::imshow("Yellow Cones drawing", drawing2);
    }
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
                double temp;
                createWindow(img, &degree, &temp, &VERBOSE);

                // Save calculated steering angle to file
                int64_t micSecs = cluon::time::toMicroseconds(sampleTime);
                calAngle.open(steeringAngleData, std::ios::out | std::ios::app);
                if (calAngle.is_open()){
                    calAngle << micSecs << ";" << degree << ";" << std::endl;
                }
                calAngle.close();

                // Print required information to console
                std::cout << "Group_11;" << micSecs << ";" << degree << std::endl;            
                    
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

