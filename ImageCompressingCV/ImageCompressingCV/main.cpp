#include <opencv\cv.hpp>
#include <vector>
#include <iostream>
#include <ostream>

using namespace cv;
using namespace std;


//run length coding 
//vector<char> rle(Mat& input, int quantumCount) {
//
//	vector<char> tmpVec;
//
//	vector<char> savedBytes;
//	savedBytes.push_back(*(input.data));
//	bool isSeries = false;
//	int partLen = (UCHAR_MAX / 2);
//	//int partLen = 3;
//
//	//strange way, but it works
//	for (auto pData = input.data+1; pData != input.dataend; pData++) {
//		
//		char tmp = savedBytes.at(savedBytes.size() - 1);
//
//		if (tmp == *pData) {
//		
//			if (!isSeries) {
//			
//				savedBytes.pop_back();
//
//				if (savedBytes.size() != 0) {
//
//					if (savedBytes.size() < partLen) {
//
//						tmpVec.push_back(-(char)savedBytes.size());
//
//					}
//
//					for (int i = 0; i < savedBytes.size(); i++) {
//
//						if (savedBytes.size() > partLen) {
//							
//							tmpVec.push_back(-partLen);
//
//						}
//
//						for (int j = i % (UCHAR_MAX / 2); j < partLen, i < savedBytes.size(); j++, i++) {
//						
//							tmpVec.push_back((uchar)savedBytes.at(i));
//						
//						}
//
//					}
//
//				}
//
//				savedBytes.clear();
//
//				savedBytes.push_back((uchar)tmp);
//				savedBytes.push_back((uchar)(*pData));
//
//				isSeries = true;
//
//			}
//			else {
//			
//				savedBytes.push_back((uchar)(*pData));
//
//			}
//		
//		}
//		else {
//		
//			if (isSeries) {
//
//				if (savedBytes.size() < partLen) {
//
//					tmpVec.push_back((char)savedBytes.size());
//					tmpVec.push_back((uchar)(savedBytes.at(0)));
//
//				}
//				else {
//
//					int i = 0;
//
//					for (; i < savedBytes.size() / partLen; i++) {
//
//						tmpVec.push_back(partLen);
//						tmpVec.push_back((uchar)(savedBytes.at(0)));
//
//					}
//
//					if (savedBytes.size() % partLen != 0) {
//					
//						tmpVec.push_back(savedBytes.size() % partLen);
//						tmpVec.push_back((uchar)(savedBytes.at(0)));
//
//					}
//
//				}
//
//				
//				savedBytes.clear();
//				savedBytes.push_back((uchar)(*pData));
//
//				isSeries = false;
//
//			}
//			else {
//			
//				savedBytes.push_back((uchar)(*pData));
//			
//			}
//			
//		}
//
//	}
//
//	if (savedBytes.size() != 0) {
//	
//		if (savedBytes.size() >= 2) {
//		
//			if (savedBytes.at(savedBytes.size() - 1) == savedBytes.at(savedBytes.size() - 2)) {
//			
//				int i = 0;
//				for (; i < savedBytes.size() / partLen; i++) {
//				
//					tmpVec.push_back(partLen);
//					tmpVec.push_back((uchar)savedBytes.at(savedBytes.size() - 1));
//
//				}
//
//				tmpVec.push_back(savedBytes.size() % partLen);
//				tmpVec.push_back((uchar)savedBytes.at(savedBytes.size() - 1));
//
//				//for (int j = savedBytes.size() % partLen; j + i < savedBytes.size(); j++) {}
//
//
//
//				//tmpVec.push_back(savedBytes.size());
//				//tmpVec.push_back(savedBytes.at(savedBytes.size() - 1));
//
//			}
//			else {
//			
//
//				int i = 0;
//				for (; i < savedBytes.size() / partLen; i++) {
//
//					tmpVec.push_back(-partLen);
//					//tmpVec.push_back(savedBytes.size() - 1);
//					for (int j = 0; j < partLen; j++) {
//					
//						tmpVec.push_back((uchar)savedBytes.at(i + j));
//
//					}
//
//				}
//
//				tmpVec.push_back(savedBytes.size() % partLen);
//
//				for (int j = savedBytes.size() % partLen; j + i < savedBytes.size(); j++) {
//				
//					tmpVec.push_back((uchar)savedBytes.at(i + j));
//
//				}
//
//
//				//tmpVec.push_back(savedBytes.at(savedBytes.size() - 1));
//
//				/*tmpVec.push_back(-(char)savedBytes.size());
//				
//				for (auto elm : savedBytes) {
//				
//					tmpVec.push_back(elm);
//
//				}*/
//			
//			}
//
//		}
//		else if(savedBytes.size() == 1 ) {
//		
//			tmpVec.push_back(-1);
//			tmpVec.push_back((uchar)(savedBytes.at(savedBytes.size() - 1)));
//
//		}
//
//
//	}
//
//	return tmpVec;
//
//}


//run length encoding
//void rlde(Mat& output, vector<char> input, int quantumCount) {
//
//	vector<char> tmpVec;
//
//	int i = 0;
//
//	while (i < input.size()) {
//	
//		char count = input.at(i);
//
//		i++;
//
//		if (count < 0) {
//		
//			for (int j = 0; j < abs(count); j++) {
//			
//				tmpVec.push_back(input.at(i + j));
//			
//			}
//
//			i += abs(count);
//		
//		}
//		else {
//		
//			for (int j = 0; j < count; j++) {
//
//				tmpVec.push_back(input.at(i));
//
//			}
//
//			i++;
//		
//		}
//
//	}
//
//	for (auto pData = output.data; pData != output.dataend; pData++) {
//	
//		*pData = tmpVec.at(pData - output.datastart);
//	
//	}
//
//}
//
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


vector<pair<int, uchar>> rle(Mat& mat) {

	vector<pair<int, uchar>> output;

	for (auto pData = mat.data; pData != mat.dataend;) {
	
		auto pData1 = pData + 1;
		pair<int, uchar> pair = make_pair(1, *pData);

		while (*pData1 == *pData && pData1 != mat.dataend) {
			pData1++;
			pair.first++;
		}

		output.push_back(pair);
		pData += pair.first;

	}

	return output;


}

void rlde(Mat& mat, vector<pair<int, uchar>>& vec) {

	vector<uchar> output;

	for (auto _pair : vec) {
	
		for (int i = 0; i < _pair.first; i++) {

			output.push_back(_pair.second);

		}
	
	}


	for (auto pData = mat.data; pData != mat.dataend; pData++) {
	
		*pData = output.at(pData - mat.datastart);

	}

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



	Mat patch1 = Mat(quant);
	//patch1.convertTo(patch1, CV_8UC3);

	//vector<char> encoded = rle(patch1);
	vector<pair<int, uchar>> encoded = rle(patch1);

	cout << encoded.size() * 5 << endl;
	cout << patch.cols * patch.rows * 3;


	Mat decoded = Mat(patch.rows, patch.cols, CV_8UC3);
	rlde(decoded, encoded);


	cout << test(patch1, decoded) << endl;
	//decoded.convertTo(decoded, CV_YCrCb2BGR);


	//show 
	imshow("patch", patch);
	imshow("quantization", quant);
	waitKey();


	//converting to ycrcb
	cvtColor(patch, patch, CV_YCrCb2BGR);
	cvtColor(quant, quant, CV_YCrCb2BGR);
	cvtColor(decoded, decoded, CV_YCrCb2BGR);


	//show 
	imshow("patch1", patch);
	imshow("quantization1", quant);
	waitKey();

	//show decoded
	imshow("patch2", patch);
	imshow("decoded", decoded);
	waitKey();


}

void randomUCharMat(Mat& mat, uchar min, uchar max) {

	for (auto pData = mat.data; pData != mat.dataend; pData++) {
	
		*pData = (uchar)(rand() % (max - min)  + min);

	}

}




//int main() {
//
//	uchar data[] = {1, 1, 1,  0, 0, 0,  0, 0, 0,  0, 0, 0,
//		0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,
//		0, 0, 0,  1, 1, 1,  1, 1, 1,  1, 1, 1,
//		0, 0, 0,  0, 0, 0,  0, 0, 0, 0, 0, 0};
//
//	//Mat m = Mat(4, 4, CV_8UC3);
//
//	//randomUCharMat(m, 0, 10);
//
//	Mat m = Mat(4, 4, CV_8UC3, data);
//
//	//vector<char> vec = rle(m);
//
//	vector<pair<int, uchar>> vec = rle(m);
//
//	Mat m1 = Mat(4, 4, CV_8UC3);
//
//	//rlde(m1, vec, 1);
//
//	rlde(m1, vec);
//
//	cout << "[ ";
//	for (auto _pair : vec) {
//
//		cout << "(" << _pair.first << "," << (int)(_pair.second) << ")";
//
//	}
//	cout << "]\n";
//
//
//	std::cout << m << std::endl;
//
//	std::cout << m1 << std::endl;
//
//	return 0;
//
//}