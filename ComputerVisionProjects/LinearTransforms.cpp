// LinearTransforms.cpp
// Program to use the provided Image class to perform linear transformations on "test.gif"
// Author:	Andrew VanKooten

#include "Image.h"
	
#include <cmath>
#include <iostream>
using namespace std;

//--------------------------------interpolate()------------------------------
// Description:		Takes in an Image&, a row, and a column and uses bilinear
//					interpolation to create a new pixel using four pixels 
//					nearest row and column in input and returns that pixel.
// Preconditions:	input must point to an Image and row and column must be
//					within its range. 
// Postconditions:	A pixel is returned using four pixels in input that 
//					corresponds with row and column
pixel interpolate(Image&, double, double);

// Description:		Accepts six arguments in this order: column scale factor, row scale 
//					factor, column translation factor, row translation factor, rotation 
//					factor, and a shear factor. 
//					The shear factor will either shrink or make the image larger; numbers 
//					larger than one will increase the size, a number less than one will 
//					shrink it, and a negative number will flip it. 
//					The translation factors move the image down and to the right; and 
//					negative values will do the opposite. 
//					The rotation factor will rotate the image clockwise around the center;
//					and a negative number will rotate it the other way.
//					The shear factor will shear the image; and a negative number  will shear
//					it the other direction.
// Preconditions:	"test.gif" exists and it is a correctly formatted GIF image 
//					The scale factors cannot be zero.
//					The rotation factor must be in degrees.
//					There must be six arguments.
// Postconditions:	The new image that reflects these transformations will be written to 
//					"output.gif"
int main(int argc, char* argv[])
{
	//Return 1 if there are not enough arguments
	if (argc != 7) {
		cerr << "Incorrect number of arguments." << endl;
		return 1;
	}

	const double scaleCol = atof(argv[1]);			//column scale factor
	const double scaleRow = atof(argv[2]);			//row scale factor
	const double translateCol = atof(argv[3]);		//column translation factor 
	const double translateRow = atof(argv[4]);		//row translation factor 
	const double theta = atof(argv[5]);				//rotation factor 
	const double shear = atof(argv[6]);				//shear factor (k)

	//Return 1 if zero was inputed for scaleCol or scaleRow
	if (scaleCol == 0 || scaleRow == 0) {
		cerr << "Cannot scale by a factor of zero" << endl;
		return 1;
	}

	Image input("test.gif");
	Image output(input.getRows(), input.getCols());
	
	int middleCol = output.getCols() / 2;			//center column
	int middleRow = output.getRows() / 2;			//center row

	const double PI = 3.14159265;					//value of PI
	double cosTheta = cos(theta*PI / 180);			//the cosine of theta
	double sinTheta = sin(theta*PI / 180);			//the sine of theta

	//Fill the output image by taking the cordinates for a pixel in output,
	//	performing linear transofrmations on those cordinates, and using the 
	//	new cordinates to look up the corresponding pixel in image
	for (int row = 0; row < input.getRows(); row++) {
		for (int col = 0; col < input.getCols(); col++) {
			double inputCol, inputRow;

			// Translate input by translateCol columns and translateRow rows
			inputCol = col - translateCol;
			inputRow = row - translateRow;

			// Make the orgin the middle of the image
			inputCol -= middleCol;
			inputRow -= middleRow;

			// Scales input rows by a factor of scaleRow and columns by a factor of scaleCol
			inputCol /= scaleCol;
			inputRow /= scaleRow;

			// Shears input by a factor of shear 
			inputCol -= (shear * inputRow);

			// Rotates input by rotation degrees
			double tempCol = inputCol;
			double tempRow = inputRow;
			inputCol = tempCol * cosTheta + tempRow * sinTheta;
			inputRow = tempRow * cosTheta - tempCol * sinTheta;

			// Make the orgin (0, 0)
			inputCol += middleCol;
			inputRow += middleRow;

			// Check if the given pixel is out of bounds on input
			if (inputCol < 0 || inputCol >= input.getCols() - 1
				|| inputRow < 0 || inputRow >= input.getRows() - 1) {
				continue;
			}
			//get the pixel at the derived coordinates in input and set to output
			pixel p = interpolate(input, inputRow, inputCol);
			output.setPixel(row, col, p);
		}
	}

	output.writeImage("output.gif");
	return 0;
}

//--------------------------------interpolate()------------------------------
// Description:		Takes in an Image&, a row, and a column and uses bilinear
//					interpolation to create a new pixel using four pixels 
//					nearest row and column in input and returns that pixel.
// Preconditions:	input must point to an Image and row and column must be
//					within its range. 
// Postconditions:	A pixel is returned using four pixels in input that 
//					corresponds with row and column
pixel interpolate(Image& input, double row, double column) {
	int c = static_cast<int>(column), r = static_cast<int>(row);
	double alpha = column - c, beta = row - r;
	double red, blue, green;

	//get the value for red
	red = (1 - alpha) * (1 - beta) * input.getPixel(r, c).red
		+ alpha * (1 - beta) * input.getPixel(r + 1, c).red
		+ (1 - alpha) * beta * input.getPixel(r, c + 1).red
		+ alpha * beta * input.getPixel(r + 1, c + 1).red;

	//get the value for blue
	blue = (1 - alpha) * (1 - beta) * input.getPixel(r, c).blue
		+ alpha * (1 - beta) * input.getPixel(r + 1, c).blue
		+ (1 - alpha) * beta * input.getPixel(r, c + 1).blue
		+ alpha * beta * input.getPixel(r + 1, c + 1).blue;

	//get the value for green
	green = (1 - alpha) * (1 - beta) * input.getPixel(r, c).green
		+ alpha * (1 - beta) * input.getPixel(r + 1, c).green
		+ (1 - alpha) * beta * input.getPixel(r, c + 1).green
		+ alpha * beta * input.getPixel(r + 1, c + 1).green;

	pixel p;

	//round the value for each color, cast it to a byte, and store it in pixel p
	p.red = static_cast<byte>(round(red));
	p.blue = static_cast<byte>(round(blue));
	p.green = static_cast<byte>(round(green));

	return p;
}