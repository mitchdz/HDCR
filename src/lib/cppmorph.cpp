#include "opencv2/highgui.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include <bitset>
#include "cppthresh.hpp"
#include "cppmorph.hpp"
#include "thresh.h"
#include "libhdcr.h"
#include "cppOpenCV2Helper.hpp"


void cpperodeImage(IMAGE *img, uint8_t CFGval)
{
    cv::Mat mat=convertIMAGEtoMat(img);
    //cv::Mat element = cv::getStructuringElement(MORPH_RECT,
                        //Size( 2*)


}


void cppSkeletonize(IMAGE *img, uint8_t CFGval)
{


}

void cppRemoveBranchpoints(IMAGE *img, uint8_t CFGval)
{

    //Mat mat=convertIMAGEtoMat(img);
    //mat=cv::bwmorph(mat, 'skel', inf);
    //convertMatToIMAGE(mat,img);
}