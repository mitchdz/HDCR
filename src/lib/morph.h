#ifndef MORPH_H
#define MORPH_H

void cleanSpuriousPixels(IMAGE *img);
void thickenImage(IMAGE *img, int n, uint8_t CFGval);
void dilateImage3by3Kernel(IMAGE *img, uint8_t CFGval);
void removeBranchPoints(IMAGE *img, uint8_t CFGval);
void invertImage(IMAGE *img);
void skeletizeImage(IMAGE *img, uint8_t CFGval);
void openImage(IMAGE *img, uint8_t CFGval);
void erodeImage3by3Kernel(IMAGE *img, uint8_t CFGval);


#endif // MORPH_H