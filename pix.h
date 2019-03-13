/*
********************************************************************
**
Copyright (c) 2018, Maryam Khanian, Ali Sharifi Boroujerdi
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
********************************************************************
*/
#ifndef PIX_H_
#define PIX_H_

#include <iostream>
#include <cstring>
#include <math.h> 
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

// Functions' declerations

class pix{
	
	public:
	int x_index;
	int y_index;
	double c; //speed function
	double w; // arrival time
	short int status; // 0=known, 1=on border, 2=far
	
	public:
	pix() : c(1),status(2) {};
	void find_neighbors(int neighbors_coordinates[4][2]);
	void show_neighbors(int neighbors_coordinates [4][2]);	
	
};
////////////////////////////////////////////////////////////
void pix::find_neighbors(int neighbors_coordinates[4][2])
{
	// UP neighbor x & y
	neighbors_coordinates[0][0]=this->x_index-1;
	neighbors_coordinates[0][1]=this->y_index;
	// DOWN neighbor x & y
	neighbors_coordinates[1][0]=this->x_index+1;
	neighbors_coordinates[1][1]=this->y_index;
	// LEFT neighbor x & y
	neighbors_coordinates[2][0]=this->x_index;
	neighbors_coordinates[2][1]=this->y_index-1;
	// RIGHT neighbor x & y
	neighbors_coordinates[3][0]=this->x_index;
	neighbors_coordinates[3][1]=this->y_index+1;
	
}
//////////////////////////////////////////////////////////// 
void pix::show_neighbors(int neighbors_coordinates [4][2])
{
	cout<<"U_neighbor is:	"<<"["<<neighbors_coordinates[0][0]<<"]"<<"["<<neighbors_coordinates[0][1]<<"]"<<endl;
	cout<<"D_neighbor is:	"<<"["<<neighbors_coordinates[1][0]<<"]"<<"["<<neighbors_coordinates[1][1]<<"]"<<endl;
	cout<<"L_neighbor is:	"<<"["<<neighbors_coordinates[2][0]<<"]"<<"["<<neighbors_coordinates[2][1]<<"]"<<endl;
	cout<<"R_neighbor is:	"<<"["<<neighbors_coordinates[3][0]<<"]"<<"["<<neighbors_coordinates[3][1]<<"]"<<endl;
	
}
//////////////////////////////////////////////////////////// 
#endif /* PIX_H_*/
