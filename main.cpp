#include <iostream>
#include <fstream>
#include "pic.h"

int main(){
	bmp_t bmp( 1000, 1000, 0.5 /* 50% of original pic */);
	bmp.box( 250, 250, 300, 300, 0.5, 2, 60, 2 );
	bmp.box( 500, 500, 250, 250, 0.5, 2, 60, 2 );
	std::ofstream opic( "demo.bmp", std::ios::out );
	bmp.output(opic);
	opic.close();
}
