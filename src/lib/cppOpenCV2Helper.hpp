#ifndef CPP_OPENCV2_HELPER_H
#define CPP_OPENCV2_HELPER_H

cv::Mat convertIMAGEtoMat(IMAGE *img);
void convertMatToIMAGE(cv::Mat mat, IMAGE *img);

#endif /* CPP_OPENCV2_HELPER_H */