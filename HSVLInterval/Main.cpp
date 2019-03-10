#include <stdio.h>

// OpenCV headers.
#include "opencv/highgui.h"

// min and max might cause incompatibilities with GCC...
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif // !max
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif // !min

/*
Convert from RGB to HSL.

double red : (IN) Red between 0..255.
double green : (IN) Green between 0..255.
double blue : (IN) Blue between 0..255.
double* pH : (OUT) Pointer to the hue between 0..240.
double* pS : (OUT) Pointer to the saturation between 0..240.
double* pL : (OUT) Pointer to the luminance between 0..240.

Return : Nothing.
*/
inline void RGB2HSL_MSPaint(double red, double green, double blue, double* pH, double* pS, double* pL)
{
	double minval = min(red, min(green, blue));
	double maxval = max(red, max(green, blue));
	double mdiff = maxval-minval;
	double msum = maxval+minval;
	double hue = 0.0;
	double saturation = 0.0;
	double luminance = msum/510.0;

	if (maxval != minval)
	{
		double tmp = 60.0/mdiff;
		if (red == maxval)
		{
			hue = (green-blue)*tmp;
			if (hue < 0) hue += 360.0;
		}
		else if (green == maxval)
			hue = (blue-red)*tmp+120.0;
		else if (blue == maxval)
			hue = (red-green)*tmp+240.0;
		saturation = (luminance <= 0.5)? (mdiff/msum): (mdiff/(510.0-msum));
	}

	// Microsoft Paint limits for H,S,L are 0..240.

	*pH = hue*240.0/360.0;
	*pS = saturation*240.0;
	*pL = luminance*240.0;
}

/*
Convert from RGB to HSV.

double red : (IN) Red between 0..255.
double green : (IN) Green between 0..255.
double blue : (IN) Blue between 0..255.
double* pH : (OUT) Pointer to the hue between 0..240.
double* pS : (OUT) Pointer to the saturation between 0..240.
double* pV : (OUT) Pointer to the value between 0..240.

Return : Nothing.
*/
inline void RGB2HSV_MSPaint_Fake(double red, double green, double blue, double* pH, double* pS, double* pV)
{

	// From https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur.

	double minval = min(red, min(green, blue));
	double maxval = max(red, max(green, blue));
	double mdiff = maxval-minval;
	double hue = 0.0;
	double saturation = (maxval == 0)? 0: (1-minval/maxval);
	double value = maxval/255.0;

	if (maxval != minval)
	{
		double tmp = 60.0/mdiff;
		if (red == maxval)
		{
			hue = (green-blue)*tmp;
			if (hue < 0) hue += 360.0;
		}
		else if (green == maxval)
			hue = (blue-red)*tmp+120.0;
		else if (blue == maxval)
			hue = (red-green)*tmp+240.0;
	}

	*pH = hue*240.0/360.0;
	*pS = saturation*240.0;
	*pV = value*240.0;
}

int main(int argc, char* argv[])
{
	int i = 0, j = 0;
	unsigned char rmin = 255, rmax = 0, gmin = 255, gmax = 0, bmin = 255, bmax = 0;
	unsigned char hmin = 240, hmax = 0, smin = 240, smax = 0, lmin = 240, lmax = 0, 
		svmin = 240, svmax = 0, vmin = 240, vmax = 0;
	unsigned char hxmin = 0, hxmax = 240;//, sxmin = 0, sxmax = 240, lxmin = 0, lxmax = 240,
		//svxmin = 0, svxmax = 240, vxmin = 0, vxmax = 240;
	IplImage* image = cvLoadImage("image.png");
	IplImage* image_bin = cvLoadImage("image_bin.png");

	// Convert image->imageData from char* to unsigned char* to work with color values in 0..255.
	unsigned char* data = reinterpret_cast<unsigned char*>(image->imageData);
	unsigned char* data_bin = reinterpret_cast<unsigned char*>(image_bin->imageData);

	for (i = 0; i < image->height; i++)
	{
		for (j = 0; j < image->width; j++)
		{
			int index = 3*(j+image->width*i);
			double b = data[0+index], g = data[1+index], r = data[2+index];
			double h = 0, s = 0, l = 0, sv = 0, v = 0;
			RGB2HSL_MSPaint(r, g, b, &h, &s, &l);
			RGB2HSV_MSPaint_Fake(r, g, b, &h, &sv, &v);
			// Select the pixels with the right color.
			if ((data_bin[0+index] > 0)||(data_bin[1+index] > 0)||(data_bin[2+index] > 0))
			{
				rmin = (unsigned char)min(rmin, r); gmin = (unsigned char)min(gmin, g); bmin = (unsigned char)min(bmin, b);
				rmax = (unsigned char)max(rmax, r); gmax = (unsigned char)max(gmax, g); bmax = (unsigned char)max(bmax, b);
				hmin = (unsigned char)min(hmin, h); smin = (unsigned char)min(smin, s); lmin = (unsigned char)min(lmin, l);
				hmax = (unsigned char)max(hmax, h); smax = (unsigned char)max(smax, s); lmax = (unsigned char)max(lmax, l);
				svmin = (unsigned char)min(svmin, sv); vmin = (unsigned char)min(vmin, v);
				svmax = (unsigned char)max(svmax, sv); vmax = (unsigned char)max(vmax, v);
				hxmin = (h < 120)? (unsigned char)max(hxmin, h): hxmin; hxmax = (h >= 120)? (unsigned char)min(hxmax, h): hxmax;
				//sxmin = (s < 120)? (unsigned char)max(sxmin, s): sxmin; sxmax = (s >= 120)? (unsigned char)min(sxmax, s): sxmax;
				//lxmin = (l < 120)? (unsigned char)max(lxmin, l): lxmin; lxmax = (l >= 120)? (unsigned char)min(lxmax, l): lxmax;
				//svxmin = (sv < 120)? (unsigned char)max(svxmin, sv): svxmin; svxmax = (sv >= 120)? (unsigned char)min(svxmax, sv): svxmax;
				//vxmin = (v < 120)? (unsigned char)max(vxmin, v): vxmin; vxmax = (v >= 120)? (unsigned char)min(vxmax, v): vxmax;
			}
		}
	}
#pragma endregion

	printf("MS Paint convention : r,g,b in [0,255], h,s,l in [0,240], h,s,v in [0,240] (not provided by MS Paint), singularity for red (h=0 and h=240)\n");
	printf("r = [%d,%d], g = [%d,%d], b = [%d,%d]\n", (int)rmin, (int)rmax, (int)gmin, (int)gmax, (int)bmin, (int)bmax);
	printf("h = [%d,%d] (exclusive : [%d,%d]), s = [%d,%d], l = [%d,%d]\n", (int)hmin, (int)hmax, (int)hxmin, (int)hxmax, (int)smin, (int)smax, (int)lmin, (int)lmax);
	printf("h = [%d,%d] (exclusive : [%d,%d]), s = [%d,%d], v = [%d,%d]\n", (int)hmin, (int)hmax, (int)hxmin, (int)hxmax, (int)svmin, (int)svmax, (int)vmin, (int)vmax);

	cvReleaseImage(&image_bin);
	cvReleaseImage(&image);

	return 0;
}
