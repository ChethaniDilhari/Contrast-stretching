//contrast stretching
#include "opencv2\opencv.hpp"                                                      
#include "opencv2\highgui.hpp"
#include "opencv2\core.hpp"

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) 
{  
	Mat img = imread(argv[1], 1);

	if (!img.data) { //if iamge is not available
		cout << "could not find the image" << endl;
		return -1;
	}

	int height = img.rows;
	int width = img.cols;
	int widthstep = img.step;
	int channels = img.channels();
	
	
	Mat gray(height, width, CV_8UC1, Scalar(0));//CV_*UC1 means one channel, Scaler(0) means color is 0 (black)

	cvtColor(img, gray, COLOR_BGR2GRAY); //convert color image to grayscale
	 
    //get a same image as gray image
	Mat new_image = gray.clone();

	int v;
	int output;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
	
				v = (int)gray.at<uchar>(y, x);

				if ((v >= 0) && (v <= 78)) {
					output = int((5 / 78)*v);
				}
				else if (v > 78 && v <= 195) {
					output = int((245 / 117) * (v - 78) + 5);
				}
				else if (v > 195 && v <= 255) {
					output = int((5 / 60) * (v - 195) + 250);
				}

				//output convert to uchar
				new_image.at<uchar>(y, x) = (uchar)output;

		}
	} 
	
	
	imwrite("new_image.png",new_image);

	//histogram
	//create an array for allocate memory to all intencity values(in gray scal image of 8 bits have 256 intencity values)
	int hist[256];

	//give 0 to all 256 array elements
	for (int a = 0; a < 256; a++) {
		hist[a] = 0;
	}
	int w = new_image.cols;
	int h = new_image.rows;

	//count one by one pixals for all intensity values 
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			hist[(int)new_image.at<uchar>(y, x)] = hist[(int)new_image.at<uchar>(y, x)] + 1;
		}
	}
	//print the counted values
	for (int i = 0; i < 256; i++) {
		cout << i << ":" << hist[i] << endl;
	}


	//create an image to display the histogram
	//give width and height of the histogram
	int hist_width = 512;
	int hist_height = 400;

	//create image with above mentioned width, height and grayscale
	Mat histImage(hist_height, hist_width, CV_8UC1, Scalar(0));

	//find max number of pixals from histogram array
	int max = hist[0];
	for (int i = 0; i < 256; i++) {
		if (max < hist[i]) {
			max = hist[i];
		}
	}

	//normalize values between 0 and height
	for (int i = 0; i < 256; i++) {
		hist[i]=((double)hist[i] / max)*histImage.rows;
	}

		int bin_w = cvRound(double(hist_width / 256)); //height of the histogram is devided by 256 and round that value to get an integer

	for (int i = 0; i < 256; i++) {
		line(histImage, Point(bin_w*(i), hist_height), Point(bin_w*(i), hist_height - hist[i]), Scalar(255), 1, 8, 0);
	}
	//Scaler(255) = line color is black

	imwrite("histogram of stretched image.png", histImage);

	

	namedWindow("image");
	imshow("image", img);
	namedWindow("Gray");
	imshow("Gray", gray);
	namedWindow("New Image");
	imshow("New Image", new_image);
	namedWindow("histogram");
	imshow("histogram", histImage);
	waitKey(0);
	return(0);






}