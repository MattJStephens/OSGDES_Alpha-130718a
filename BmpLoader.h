#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#include <string>

class BmpLoader
{
public:
	BmpLoader(void);
	~BmpLoader(void);
	bool loadBmp(std::string filename); 
	int	mWidth;
	int	mHeight;
	unsigned char *mRGB;
};

#endif
