// EdgeDetector.cpp
// Program to use the CSS487 Image class to smooth and detect edges on "test.gif"  
// Author: Andrew VanKooten

#include "Image.h"

#include <cmath>
#include <iostream>
using namespace std;

//--------------------------convolute()----------------------------------
// Description:		Takes in an Image&, a second Image& that represents a 
//					kernel, and the row and column for kernel which you 
//					want the convolution to start on. This function takes
//					an image and multiplies each pixel and its neighbors 
//					by the values in the kernel and returns the new Image.
// Preconditions:	source and kernel have to point to Images, centerR and
//					centerC have to be valid pixels in kernel.
// Postconditions:	A new image that is the convolution of source and kernel
//					is returned.
Image convolute(Image&, Image&, int, int);

//-----------------------interpolate()----------------------------------------
// Description:		Takes in an Image&, a row, and a column and uses bilinear 
//					interpolation to create a new float using the four pixels 
//					closest to row and col.
// Preconditions:	source must point to an image.
// Postconditions:	A float is returned using four pixels in input that correspond
//					with row and col.
float interpolate(Image&, float, float);

//-----------------------convertToGrey()--------------------------------------
// Description:		Takes in a float Image& and converts it to a grey image.
// Preconditions:	source must point to an image.
// Postconditions:	source will now be a grey Image.
void convertToGrey(Image&);

//-----------------------main()------------------------------------------------
// Description:		Takes "test.gif", smooths it a certain number of times, and
//					detects edges in it adn writes the smooth and edge images 
//					to "smooth.gif" and "edge.gif".
// Preconditions:	"test.gif" exists and it is a correctly formatted GIF image
//					There will be one argument
// Postconditions:	The image that shows the smoothed image will be written to 
//					"smooth.gif" and the image that shows the edges will be 
//					written to "edge.gif"
int main(int argc, char* argv[]) {

	//Return 1 if there are not the correct number of arguments
	if (argc != 2) {
		cerr << "Incorrect number of arguments" << endl;
		return 1;
	}

	const int repetitions = atoi(argv[1]);		//Number of times to smooth the image
	const float maxThreashold = 10.0;			//Edges are defined by values above this number

	//Useful range for repetition values
	if (repetitions < 0 || repetitions > 20 ) {
		cerr << "Repetitions are out of useful range" << endl;
		return 1;
	}

	Image input("test.gif");
	Image floatInput(input.getRows(), input.getCols());

	//Convert the image to a floating point image
	for (int row = 0; row < floatInput.getRows(); row++) {
		for (int col = 0; col < floatInput.getCols(); col++) {
			floatInput.setFloat(row, col, input.getPixel(row, col).grey);
		}
	}

	Image Sx(1, 3);								//Smoothing kernel for x
	Sx.setFloat(0, 0, .25), Sx.setFloat(0, 1, .5), Sx.setFloat(0, 2, .25);
					
	Image Sy(3, 1);								//Smoothing kernel for y
	Sy.setFloat(0, 0, .25), Sy.setFloat(1, 0, .5), Sy.setFloat(2, 0, .25);

	Image Gx(1, 3);								//Gradient kernel for x
	Gx.setFloat(0, 0, 1), Gx.setFloat(0, 1, 0), Gx.setFloat(0, 2, -1);

	Image Gy(3, 1);								//Gradient kernel for y
	Gy.setFloat(0, 0, 1), Gy.setFloat(1, 0, 0), Gy.setFloat(2, 0, -1);

	//Smooth using Sx
	for (int i = 0; i < repetitions; i++) {
		floatInput = convolute(floatInput, Sx, 0, -1);
	}
	//Smooth using Sy
	for (int i = 0; i < repetitions; i++) {
		floatInput = convolute(floatInput, Sy, -1, 0);
	}

	Image gx(floatInput.getRows(), floatInput.getCols()), gy(floatInput.getRows(), floatInput.getCols());
	
	//create the gradient in X
	gx = convolute(floatInput, Gx, 0, -1);
	//create the gradient in Y
	gy = convolute(floatInput, Gy, -1, 0);

	//create the gradient magnitude
	Image Gmag(floatInput.getRows(), floatInput.getCols());
	for (int row = 0; row < Gmag.getRows(); row++) {
		for (int col = 0; col < Gmag.getCols(); col++) {
			float pixel = (float)sqrt(pow(gx.getFloat(row, col), 2) + pow(gy.getFloat(row, col), 2));
			Gmag.setFloat(row, col, pixel);
		}
	}
	
	Image edge(floatInput.getRows(), floatInput.getCols());

	//Find the edges in Gmag that have the highest intensity
	for (int row = 0; row < Gmag.getRows(); row++) {
		for (int col = 0; col < Gmag.getCols(); col++) {
			float g = Gmag.getFloat(row, col);
			if (g >= maxThreashold) {
				float gCol = gx.getFloat(row, col) / g;
				float gRow = gy.getFloat(row, col) / g;

				float r = interpolate(Gmag, row + gRow, col + gCol);
				float p = interpolate(Gmag, row - gRow, col - gCol);

				if (g > r && g > p) {
					edge.setGrey(row, col, 255);
				}
			}
		}
	}

	//convert to a grey image
	convertToGrey(floatInput);

	floatInput.writeGreyImage("smooth.gif");
	edge.writeGreyImage("edge.gif");
	return 0;
}

//--------------------------convolute()----------------------------------
// Description:		Takes in an Image&, a second Image& that represents a 
//					kernel, and the row and column for kernel which you 
//					want the convolution to start on. This function takes
//					an image and multiplies each pixel and its neighbors 
//					by the values in the kernel and returns the new Image.
// Preconditions:	source and kernel have to point to Images, centerR and
//					centerC have to be valid pixels in kernel.
// Postconditions:	A new image that is the convolution of source and kernel
//					is returned.
Image convolute(Image& source, Image& kernel, int centerR, int centerC) {
	Image Output(source.getRows(), source.getCols());
	float sum;
	int sourceRow, sourceCol;
	
	//For every pixel in source
	for (int row = 0; row < source.getRows(); row++) {
		for (int col = 0; col < source.getCols(); col++) {
			sum = 0;
			//Multiply that pixel and its neighboring pixels by kernel
			for (int r = centerR; r < kernel.getRows() + centerR; r++) {
				for (int c = centerC; c < kernel.getCols() + centerC; c++) {
					sourceRow = row - r, sourceCol = col - c;

					//If the location is out of bounds of the image
					if (sourceRow < 0) sourceRow++;
					if (sourceCol < 0) sourceCol++;
					if (sourceRow > source.getRows() - 1) sourceRow--;
					if (sourceCol > source.getCols() - 1) sourceCol--;

					sum += (kernel.getFloat(r - centerR, c - centerC) * source.getFloat(sourceRow, sourceCol));
				}
			}
			Output.setFloat(row, col, sum);
		}
	}
	return Output;
}

//-----------------------interpolate()----------------------------------------
// Description:		Takes in an Image&, a row, and a column and uses bilinear 
//					interpolation to create a new float using the four pixels 
//					closest to row and col.
// Preconditions:	source must point to an image.
// Postconditions:	A float is returned using four pixels in input that correspond
//					with row and col.
float interpolate(Image& source, float row, float col) {
	int c = (int)col, r = (int)row;
	float beta = col - c, alpha = row - r;

	//If r or c are out of the image set them to the closest pixel in the image
	if (r < 0) r = 0;
	if (c < 0) c = 0;
	if (r >= source.getRows()) r = source.getRows() - 1;
	if (c >= source.getCols()) c = source.getCols() - 1;

	float p = (1 - alpha) * (1 - beta) * source.getFloat(r, c);

	//If (r+1,c+1) are outside the image use (r,c)
	if (r + 1 >= source.getRows() && c + 1 >= source.getCols()) {
		p += alpha * beta * source.getFloat(r, c);
	}
	else {
		if (r + 1 >= source.getRows()) {
			p += alpha * beta * source.getFloat(r, c);
		}
		else if (c + 1 >= source.getCols()) {
			p += alpha * beta * source.getFloat(r, c);
		}
		else {
			p += alpha * beta * source.getFloat(r + 1, c + 1);
		}
	}
	
	//If (r+1,c) is outside the image use (r,c)
	if (r  + 1 >= source.getRows()) {
		p += alpha * (1 - beta) * source.getFloat(r, c);
	}
	else {
		p += alpha * (1 - beta) * source.getFloat(r + 1, c);
	}

	//If (r,c+1) is outside the image use (r,c)
	if ((c + 1) >= (source.getCols())) {
		p += (1 - alpha) * beta * source.getFloat(r, c);
	}
	else {
		p += (1 - alpha) * beta * source.getFloat(r, c + 1);
	}

	return p;
}

//-----------------------convertToGrey()--------------------------------------
// Description:		Takes in a float Image& and converts it to a grey image.
// Preconditions:	source must point to an image.
// Postconditions:	source will now be a grey Image.
void convertToGrey(Image& source) {
	for (int row = 0; row < source.getRows(); row++) {
		for (int col = 0; col < source.getCols(); col++) {
			byte b = (byte)source.getFloat(row, col);
			source.setGrey(row, col, b);
		}
 	}
}
