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
