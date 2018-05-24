#ifndef PIC_H
#define PIC_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bmp.h"

class bmp_t{
public:
	BMPHEADER header;
	BMPINFO info;
	RGBTRIPLE* rgb;
	int padding;
bmp_t(int w, int h, double rate):rgb(NULL){
	set( w*rate, h*rate );
};
~bmp_t(){
	if( rgb!=NULL ) delete [] rgb;
}
void set( int X , int Y, int init=(255+(255<<8)+(255<<16))){
	X += (4-(X%4))%4;
	int pixel_size= Y*X*sizeof(RGBTRIPLE);
	rgb= new RGBTRIPLE [pixel_size];
	memset( rgb , init , sizeof(RGBTRIPLE)*pixel_size );
	header.         bfType= 19778;
	header.         bfSize= pixel_size+54;
	header.    bfReserved1= 0;
	header.    bfReserved2= 0;
	header.     bfOffbytes= 54;
	
	info.         biSize= 40;
	info.        biWidth= X;
	info.       biHeight= Y;
	info.       biPlanes= 1;
	info.     biBitCount= 24;
	info.  biCompression= 0;
	info.    biSizeImage= pixel_size;
	info.biXPelsPerMeter= 0;
	info.biYPelsPerMeter= 0;
	info.      biClrUsed= 0;
	info. biClrImportant= 0;
	}

void output( std::ostream& ofile ){
	ofile.write( (char*)&header , sizeof(BMPHEADER) );
	ofile.write( (char*)&info , sizeof(BMPINFO) );
	ofile.write( (char*)rgb , info.    biSizeImage );
}

void gradient_box( int x , int y , int w , int h , int color , int transparent , int bound ){
	int y_lim= ( (y+h) < info.biHeight )?( y+h ): (info.biHeight);
	int x_lim= ( (x+w) < info.biWidth )?( x+w ): (info.biWidth);
	
	BYTE R,G,B;
	B= color%255;
	G= (color>>8)%255;
	R= (color>>16)%255;
	for( int i=y ; i<y_lim ; i++ ){
		int y_offset= i*(info.biWidth);
		for( int j=x ; j<x_lim ; j++ ){
			rgb[ y_offset + j ].rgbBlue= B;
			rgb[ y_offset + j ].rgbGreen= G;
			rgb[ y_offset + j ].rgbRed= R;
		}
	}
	
}

void box( int org_x , int org_y , int org_w , int org_h ,double rate , int color , int transparent , int bound ){
	int x = org_x * rate;
	int y = org_y * rate;
	int y_lim = (org_y+org_h)*rate;
	int x_lim = (org_x+org_w)*rate;
	int w = org_w * rate;
	int h = org_h * rate;
	y_lim= ( (y_lim) < info.biHeight )?( y_lim ): (info.biHeight);
	x_lim= ( (x_lim) < info.biWidth )?( x_lim ): (info.biWidth);
	switch(color){
		case 0:
			for( int i=y ; i<y_lim ; i++ ){
				int y_offset= i*(info.biWidth);
				for( int j=x ; j<x_lim ; j++ ){
					dec( rgb[ y_offset + j ].rgbGreen , transparent );
					dec( rgb[ y_offset + j ].rgbBlue , transparent );
				}
			}
			break;
		case 1:
			for( int i=y ; i<y_lim ; i++ ){
				int y_offset= i*(info.biWidth);
				for( int j=x ; j<x_lim ; j++ ){
					dec( rgb[ y_offset + j ].rgbRed , transparent );
					dec( rgb[ y_offset + j ].rgbBlue , transparent );
				}
			}
			break;
		case 2:
			for( int i=y ; i<y_lim ; i++ ){
				int y_offset= i*(info.biWidth);
				for( int j=x ; j<x_lim ; j++ ){
					dec( rgb[ y_offset + j ].rgbRed , transparent );
					dec( rgb[ y_offset + j ].rgbGreen , transparent );
					
				}
			}
			break;
	}
	
	line( x , y , bound , h , -1 , transparent );
	line( x+w-bound , y , bound , h , -1 , transparent );
	line( x , y , w , bound , -1 , transparent );
	line( x , y+h-bound , w , bound , -1 , transparent );
	
}

void line( int x , int y , int w , int h , int color , int transparent ){
	int y_lim= ( y+h < info.biHeight )?( y+h ): (info.biHeight);
	int x_lim= ( x+w < info.biWidth )?( x+w ): (info.biWidth);
	switch(color){
		case 0:
			for( int i=y ; i<y_lim ; i++ ){
				int y_offset= i*(info.biWidth);
				for( int j=x ; j<x_lim ; j++ ){
					dec( rgb[ y_offset + j ].rgbGreen , transparent );
					dec( rgb[ y_offset + j ].rgbBlue , transparent );
				}
			}
			break;
		case 1:
			for( int i=y ; i<y_lim ; i++ ){
				int y_offset= i*(info.biWidth);
				for( int j=x ; j<x_lim ; j++ ){
					dec( rgb[ y_offset + j ].rgbRed , transparent );
					dec( rgb[ y_offset + j ].rgbBlue , transparent );
				}
			}
			break;
		case 2:
			for( int i=y ; i<y_lim ; i++ ){
				int y_offset= i*(info.biWidth);
				for( int j=x ; j<x_lim ; j++ ){
					dec( rgb[ y_offset + j ].rgbRed , transparent );
					dec( rgb[ y_offset + j ].rgbGreen , transparent );
				}
			}
			break;
		case -1:
			for( int i=y ; i<y_lim ; i++ ){
				int y_offset= i*(info.biWidth);
				for( int j=x ; j<x_lim ; j++ ){
					dec( rgb[ y_offset + j ].rgbRed , transparent );
					dec( rgb[ y_offset + j ].rgbGreen , transparent );
					dec( rgb[ y_offset + j ].rgbBlue , transparent );
				}
			}
			break;
	}
	
}
void dec( BYTE& tar , int trans=1 ){
	tar= ( tar-trans>0 )?(tar-trans):0;
}

};// end of bmp_t
/**
int main(){
	int height= 400;
	int width = 400;
	int pixel = height * width;
	
	bmp_t bmp;
	bmp.set(height,width);
	bmp.box( 12 , 54 , 650 , 73 , 2 , 50 , 1 );
	bmp.box( 42 , 24 , 73 , 65 , 2 , 50 , 1 );
	
	
	std::ofstream file( "new pic.bmp" , std::ios::out );
	bmp.output( file );
	file.close();
	
}
/**/
#endif
