#ifndef MORPH_H
#define MORPH_H

void cleanSpuriousPixels(IMAGE *img);
void thickenImage(IMAGE *img, int n);
void dilateImage3by3Kernel(IMAGE *img);
void removeBranchPoints(IMAGE *img);

#endif // MORPH_H