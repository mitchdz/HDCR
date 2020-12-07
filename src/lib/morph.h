#ifndef MORPH_H
#define MORPH_H

void cleanSpuriousPixels(IMAGE *img);
void thickenImage(IMAGE *img, int n, uint8_t CFGval);
void dilateImage3by3Kernel(IMAGE *img, uint8_t CFGval);
void removeBranchPoints(IMAGE *img);
void invertImage(IMAGE *img);

#endif // MORPH_H