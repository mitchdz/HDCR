% read the RGB image
% RGB = imread('circuit1_pencil.png');
RGB = imread('Circuit2-1.png');
% RGB = imread('personal.png');

% convert to grayscale
I = rgb2gray(RGB);

% adaptive threshold with Otsu's method
level = graythresh(I);
BW = imbinarize(I,level);

imwrite(BW,'AfterThresholding.png','png');

% erode with 4x4 disk
% SE = strel("disk",2);
% BW2 = imerode(BW, SE);
% imwrite(J,'AfterEroding.png','png');


% remove spurious pixels (single pixels)
J = bwmorph(J,'spur');

% thicken 3 times
J = bwmorph(BW,'thicken', 3);
imwrite(J,'AfterThickening.png','png');

% have to dilate before finding branchpoints
% this is because the joints for each component have 3 or more neighbors
% so the branchpoint operation is not happy
J = imdilate(J, true(3));


% find and erase branchpoints
componentImage = bwmorph(J, 'branchpoints');

imwrite(componentImage,'ComponentImage.png','png');

% complement image because regionprops looks for white blobs
componentImageComplement = imcomplement(componentImage);

% find centroids of connected components
s = regionprops(componentImageComplement,'Centroid');

% store x and y coordinates of centroids into two-column matrix
centroids = cat(1,s.Centroid);
centroids(1,:) = []; % remove first centroid which is the entire image

% remove duplicate centroids
nRows = size(centroids,1);
keep = true(nRows,1);
for rowId = 2 : nRows
    if any(all(repelem(centroids(rowId,:),rowId-1, 1) == centroids(1:rowId-1,:), 2))
        keep(rowId) = false;
    end
end
centroids = centroids(keep, :);

% display binary image with centroid locations superimposee
imshow(componentImage)
hold on
plot(centroids(:,1),centroids(:,2),'b*')
hold off

% print off the table of each centroid
T1 = table(centroids)
  


BoundingBox = regionprops(componentImageComplement,'BoundingBox');
BB = cat(1,BoundingBox.BoundingBox);
BB(1,:) = []; % remove first bounding box which is the entire image

BBsize = size(BB,1);
hold on
for i = 1:BBsize
   x = BB(i,1);
   y = BB(i,2);
   hwidth = BB(i,3);
   vheight = BB(i,4);
   rectangle('Position', [x,y,hwidth,vheight], 'EdgeColor','r','Linewidth',3)
end
hold off

imwrite(componentImage,'BB.png','png');

F = getframe;
imwrite(F.cdata, 'CC.png','png');


% extract each bounding box and compare to roster

% structural similarity index measure (SSIM)




% centroidsMatrixSize = size(centroids,1)
% for i = 1:centroidsMatrixSize
%     x = centroids(i,1);
%     y = centroids(i,2);
%     
%     
% end






