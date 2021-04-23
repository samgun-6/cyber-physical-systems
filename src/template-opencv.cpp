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


void createWindow(cv::Mat img)
{
    cv::Mat imgHSV;
    cv::Mat imgColorSpace2;    
    cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    
    cv::inRange(imgHSV, cv::Scalar(47, 87, 34), cv::Scalar(126, 255, 86), imgColorSpace2); //Blue
    cv::imshow("After Inrange blue", imgColorSpace2);
               
    cv::Rect myROI(1, 200, 620, 280);
    cv::Mat cropped = imgColorSpace2(myROI); //This is the new window with the combined blue and yellow cones
    
    dilate(imgColorSpace2); 
    dilate(cropped); 
                  
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy(contours.size());    

    findContours( cropped, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
    std::vector<std::vector<cv::Point> > approx( contours.size() );
    
    std::vector<cv::Point2f>centers( contours.size() );
    std::vector<float>radius( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    {
        cv::approxPolyDP( contours[i], approx[i], 5, true); 
        cv::minEnclosingCircle( approx[i], centers[i], radius[i] );
    }
    cv::Mat drawing = cv::Mat::zeros( cropped.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {                      
        drawContours( drawing, contours, (int)i, cv::Scalar( 255, 255, 255 ), 2, cv::FILLED, hierarchy, 0 );
        if(radius[i] < 45 && radius[i] > 10)
        {
            cv::circle( drawing, centers[i], (int)radius[i], cv::Scalar(255,255,0), 2 ); // Draws circles on contours found on the drawing Mat
        }
        
        // 3 lines below is just used right now to print coordinates of the middlepoint in the circle that it draws
        std::stringstream temp;
        temp << "X: " << centers[i];
        setLabel(drawing, temp.str(), contours[i]);                             
    }    
    double degree = 0;
    
    if(centers.size() >= 3)
    {
        for(uint i = 0; i < centers.size(); i++)
        {
            if(radius[i] < 45 && radius[i] > 10 && radius[i+1] < 45 && radius[i+1] > 10 )
            {
                double m = 0;
                cv::Point value1(static_cast<cv::Point2f>(centers[0]));
                cv::Point value2(static_cast<cv::Point2f>(centers[1]));
                cv::line(drawing, value1, // This line just draws a line between 2 coordinates to visualize between what coordinates the angle is calculated
                    value2, cv::Scalar(255, 140, 0), 2, cv::LINE_8);                                                                
                
                m = ((static_cast<float>(value2.y - value1.y)) / (static_cast<float>(value2.x - value1.x)));
                degree = std::atan (m) * 180 / PI;                               
                degree = degree / 100;         
                std::cout << "Degree " << degree << std::endl;
            }            
        }
    }
    else
    {
        std::cout << "Degree " << degree << std::endl;
    }
    
    cv::imshow("imgColorspace2", imgColorSpace2);
    cv::imshow("drawing", drawing);
    cv::imshow("Cropped", cropped);

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

        // Attach to the shared memory.
        std::unique_ptr<cluon::SharedMemory> sharedMemory{new cluon::SharedMemory{NAME}};
        if (sharedMemory && sharedMemory->valid()) {
            std::clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << std::endl;

            // Interface to a running OpenDaVINCI session where network messages are exchanged.
            // The instance od4 allows you to send and receive messages.
            cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};

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
                sharedMemory->unlock();                
                createWindow(img);                
                    
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

