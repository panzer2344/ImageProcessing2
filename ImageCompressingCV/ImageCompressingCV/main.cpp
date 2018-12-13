#include <opencv\cv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;


//run length coding 
vector<char> rle(Mat& input, int quantumCount) {

	vector<char> tmpVec;

	vector<char> savedBytes;
	savedBytes.push_back(*(input.data));
	bool isSeries = false;

	//strange way, but it works
	for (auto pData = input.data+1; pData != input.dataend; pData++) {
		
		char tmp = savedBytes.at(savedBytes.size() - 1);

		if (tmp == *pData) {
		
			if (!isSeries) {
			
				savedBytes.pop_back();

				if (savedBytes.size() != 0) {

					if (savedBytes.size() < (UCHAR_MAX / 2)) {

						tmpVec.push_back(-(char)savedBytes.size());

					}

					for (int i = 0; i < savedBytes.size(); i++) {

						if (savedBytes.size() > (UCHAR_MAX / 2)) {
							
							tmpVec.push_back(-UCHAR_MAX / 2);

						}

						for (int j = i % (UCHAR_MAX / 2); j < (UCHAR_MAX / 2), i < savedBytes.size(); j++, i++) {
						
							tmpVec.push_back(savedBytes.at(i));
						
						}

					}

				}

				savedBytes.clear();

				savedBytes.push_back(tmp);
				savedBytes.push_back(*pData);

				isSeries = true;

			}
			else {
			
				savedBytes.push_back(*pData);

			}
		
		}
		else {
		
			if (isSeries) {

				if (savedBytes.size() < (UCHAR_MAX / 2)) {

					tmpVec.push_back(savedBytes.size());
					tmpVec.push_back(savedBytes.at(0));

				}
				else {

					int i = 0;

					for (; i < savedBytes.size() / (UCHAR_MAX / 2); i++) {

						tmpVec.push_back((UCHAR_MAX / 2));
						tmpVec.push_back(savedBytes.at(0));

					}

					if (savedBytes.size() % (UCHAR_MAX / 2) != 0) {
					
						tmpVec.push_back(savedBytes.size() % (UCHAR_MAX / 2));
						tmpVec.push_back(savedBytes.at(0));

					}

				}

				
				savedBytes.clear();
				savedBytes.push_back(*pData);

				isSeries = false;

			}
			else {
			
				savedBytes.push_back(*pData);
			
			}
			
		}

		
		
		/*
		
		uchar tmp = savedBytes.at(savedBytes.size() - 1);

		if (*pData == tmp) {
		
			if (prevByteCount < 0) {
			
				outputVec.push_back(prevByteCount);

				for (int i = 0; i < outputVec.size() - 1; i++) {
					outputVec.push_back(savedBytes.at(i));
					savedBytes.pop_back();
				}

				savedBytes.push_back(*pData);
				prevByteCount = 1;

			}
			else {

				prevByteCount++;
			
			}

		}
		else {
		
			if (prevByteCount <= 0) {
				
				savedBytes.push_back(*pData);
				prevByteCount--;

			}
			else {
			


			}

		}*/

	}

	if (savedBytes.size() != 0) {
	
		if (savedBytes.size() >= 2) {
		
			if (savedBytes.at(savedBytes.size() - 1) == savedBytes.at(savedBytes.size() - 2)) {
			
				tmpVec.push_back(savedBytes.size());
				tmpVec.push_back(savedBytes.at(savedBytes.size() - 1));

			}
			else {
			
				tmpVec.push_back(-(char)savedBytes.size());
				
				for (auto elm : savedBytes) {
				
					tmpVec.push_back(elm);

				}
			
			}

		}
		else if(savedBytes.size() == 1 ) {
		
			tmpVec.push_back(-1);
			tmpVec.push_back(savedBytes.at(savedBytes.size() - 1));

		}


	}

	return tmpVec;

}


//run length encoding
void rlde(Mat& output, vector<char> input, int quantumCount) {

	vector<char> tmpVec;

	int i = 0;

	while (i < input.size()) {
	
		char count = input.at(i);

		i++;

		if (count < 0) {
		
			for (int j = 0; j < abs(count); j++) {
			
				tmpVec.push_back(input.at(i + j));
			
			}

			i += abs(count);
		
		}
		else {
		
			for (int j = 0; j < count; j++) {

				tmpVec.push_back(input.at(i));

			}

			i++;
		
		}

	}

	for (auto pData = output.data; pData < output.dataend; pData++) {
	
		*pData = tmpVec.at(pData - output.data);
	
	}

}

int test(Mat& m1, Mat& m2) {

	int diff = 0;

	for (int i = 0; i < m1.rows; i++) {
	
		for (int j = 0; j < m1.cols; j++) {
		
			if (m1.at<uchar>(i, j) != m2.at<uchar>(i, j)) {
			
				diff++;
			}
		
		}
	
	}


	return diff;

}


int main(int argc, char* argv[]) {

	//init
	int k = 32;
	Mat patch = imread("temp.png");


	//converting to ycrcb
	cvtColor(patch, patch, CV_BGR2YCrCb);


	// quanting
	Mat quant = (patch / k);
	quant *= k;

	vector<char> encoded = rle(patch, 1);

	Mat decoded = Mat(quant.rows, quant.cols, quant.type());
	rlde(decoded, encoded, 1);

	cout << test(quant, decoded) << endl;


	//show 
	imshow("patch", patch);
	imshow("quantization", quant);
	waitKey();


	//converting to ycrcb
	cvtColor(patch, patch, CV_YCrCb2BGR);
	cvtColor(quant, quant, CV_YCrCb2BGR);


	//show 
	imshow("patch1", patch);
	imshow("quantization1", quant);
	waitKey();

}

//int main() {
//
//	uchar data[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0};
//
//	Mat m = Mat(4, 4, CV_8U, data);
//
//	vector<char> vec = rle(m, 1);
//
//	Mat m1 = Mat(4, 4, CV_8U);
//
//	rlde(m1, vec, 1);
//
//	std::cout << m << std::endl;
//
//	std::cout << m1 << std::endl;
//
//	return 0;
//
//}