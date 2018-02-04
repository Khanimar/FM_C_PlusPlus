/*
********************************************************************
**
Copyright (c) 2018, Ali Sharifi Boroujerdi
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
********************************************************************
*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h> 
#include <opencv2/opencv.hpp>

//#include "grid2.h"
//#include "distance2.h"
#include "geo_upwind2.h"
#include "pix.h"
#include "Marli_heap.h"
#include "sobel.h"


# define MAX_X 256// number of pixels in x-direction
# define MAX_Y 256// number of pixels in y-direction

# define STEP_SIZE 1

double fmin(double,double); 

using namespace std;
using namespace cv;

int main()
{
	clock_t t1,t2;
    t1=clock();

	unsigned int extract_count=0;
	unsigned int insert_count=0;
	unsigned int resort_count=0;
	unsigned int heap_count=0;
	unsigned int max_heap_use=0;
	
	int initial_point[1][2]; // containing the initial point coordinates
	float lambda=100000;// distance function coefficient
	initial_point[0][0]=floor(MAX_X/2);
	initial_point[0][1]=floor(MAX_Y/2);
	
    /////////////////// sobel /////////////////////
    
    float **Tx = new float*[MAX_X];// x_gradiant of the input
	for(int i = 0; i < MAX_X; ++i) 
	{
		Tx[i] = new float[MAX_Y];
	}
	
	float **Ty = new float*[MAX_X];// y_gradiant of the input
	for(int i = 0; i < MAX_X; ++i) 
	{
		Ty[i] = new float[MAX_Y];
	}
    
    char *filename="lena.png";
	sobel(filename,Tx,Ty);
    /////////////////// sobel /////////////////////
    
    
//$$$$$$$$$$$$$$$ Reading mask file & creating Holes $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	
	char *filename1="mask.png";
    Mat src1;
	src1 = imread(filename1,0);
	
	float mask[MAX_X][MAX_Y];// field of convolution
	
    
	//internal points---------------------------------------------------
	for (int i=0;i<=MAX_X-1;i++)
		for (int j=0;j<=MAX_Y-1;j++)
		{
			mask[i][j] = src1.at<uchar>(Point(j,i));
			if (mask[i][j]>0)
				mask[i][j]=2;
			//cout<<"img["<<i+1<<"]["<<j+1<<"]="<<img[i+1][j+1]<<endl;
		}
	// creating holes in the mask	
	for (int i=149;i<=187;i++)// First Hole
		for (int j=154;j<=166;j++)
		{
			mask[i][j]=0;
		}
	for (int i=59;i<=79;i++)// Second Hole
		for (int j=59;j<=79;j++)
		{
			mask[i][j]=0; 
		}
//$$$$$$$ Writing the result of mask production in the final_mask.txt file $$$$$$$$$
	ofstream my_mask;
	my_mask.open ("result_mask.txt");
	
	for (int i=2;i<=MAX_X+1;i++)
	for (int j=2;j<=MAX_Y+1;j++)
	{
	
	my_mask << mask[i][j]<<endl;
	} 
	 
	my_mask.close();
	//cout<<border_heap.tail<<endl;
	
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	

	int dmx=MAX_X+4;// domain size in x_direction
	int dmy=MAX_Y+4;// domain size in y_direction
	// domain size= 2 pixels wide border + input size + 2 pixels wide border
	
	int **status = new int*[dmx];// x_direction upwind derivative of distance function
	for(int i = 0; i < dmx; ++i) 
	{
		status[i] =new int[dmy];
	}
	
	for (int i=0;i<=dmx-1;i++)
		for (int j=0;j<=dmy-1;j++)
		{
			if ((i<=1 || i>=dmx-2) || (j<=1 || j>=dmy-2))
				status[i][j]=0;
		    else
				status[i][j]=mask[i-2][j-2];
		}
				
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    
    ////////////////// upwind /////////////////////
    
    double **dx = new double*[MAX_X];// x_direction upwind derivative of distance function
	for(int i = 0; i < MAX_X; ++i) 
	{
		dx[i] = new double[MAX_Y];
	}
	
	double **dy = new double*[MAX_X];// y_direction upwind derivative of distance function
	for(int i = 0; i < MAX_X; ++i) 
	{
		dy[i] = new double[MAX_Y];
	}
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	double **geo_d= new double*[dmx];// y_direction upwind derivative of distance function
	for(int i = 0; i < dmx; ++i) 
	{
		geo_d[i] = new double[dmy];
	}
	//upwind2(dx,dy);
	geo_upwind2(dx,dy,status,geo_d); // execution of upwind derivation process on distance function
		//cout<<"dx="<<dx[176][181]<<endl;
		//cout<<"dy="<<dy[176][181]<<endl;
		//cout<<"geo_d="<<geo_d[178][183]<<endl;
		//cout<<"Dy[128][149].w="<<dy[128][149]<<endl;
		//cout<<"norm_w[200][200].w="<<norm_w[200][200]<<endl;
		//std::cin.get();
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	
	
	////////////////// upwind /////////////////////

	double **norm_w = new double*[MAX_X];// norm of gradiant of w ==> ||w||=||T||+lambda*||D||
	for(int i = 0; i < MAX_X; ++i) 
	{
		norm_w[i] = new double[MAX_Y];
	}
	
	for (int i=0;i<=MAX_X-1;i++)
		for (int j=0;j<=MAX_Y-1;j++)
		{
			norm_w[i][j]= sqrt(pow(Tx[i][j]+(lambda*dx[i][j]),2.0)+pow(Ty[i][j]+(lambda*dy[i][j]),2.0));		
		}
		//cout<<"Tx[128][149].w="<<Tx[128][149]<<endl;
		//cout<<"Ty[128][149].w="<<Ty[128][149]<<endl;
		//cout<<"Dx[128][149].w="<<dx[128][149]<<endl;
		//cout<<"Dy[128][149].w="<<dy[128][149]<<endl;
		//cout<<"Tx="<<fixed<<Tx[21][131]<<endl;
		//cout<<"Ty="<<fixed<<Ty[21][131]<<endl;
		//cout<<"dx="<<fixed<<dx[21][131]<<endl;
		//cout<<"dy="<<fixed<<dy[21][131]<<endl;
		//cout<<"norm_w="<<fixed<<norm_w[21][131]<<endl;
		//std::cin.get();
	for(int i = 0; i < MAX_X; ++i) 
	{
		delete [] Tx[i];
	}
	delete [] Tx;
	
	for(int i = 0; i < MAX_X; ++i) 
	{
		delete [] Ty[i];
	}
	delete [] Ty;
	////////////////////////////////////////////////////////////////////
	for(int i = 0; i < MAX_X; ++i) 
	{
		delete [] dx[i];
	}
	delete [] dx;
	
	for(int i = 0; i < MAX_X; ++i) 
	{
		delete [] dy[i];
	}
	delete [] dy;
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
/*-------------------------------------------------------------------------------------------------------
	int ***grid = new int**[MAX_X];
	for(int i =0; i<MAX_X; i++){
	   grid[i] = new int*[MAX_Y];
	   for(int j =0; j<MAX_Y; j++){
		   grid[i][j] = new int[2];
		   for(int k = 0; k<2;k++){
			  grid[i][j][k] = 0;
		   }
	   }
	}
	
	double **distance = new double*[MAX_X];
	for(int i = 0; i < MAX_X; ++i) 
	{
		distance[i] = new double[MAX_Y];
	}
		
	make_grid2(MAX_X,MAX_Y,grid);	
    make_distance2(initial_point,MAX_X,MAX_Y,grid,distance);
    
    for(int i = 0; i != MAX_X; ++i)
    {
        for(int j = 0; j != MAX_Y; ++j)
        {
            delete[] grid[i][j];
        }
        delete[] grid[i];

    }
    delete[] grid;*/
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$    
//-------------------------------------------------------------------------------------------------------
	//int dmx=MAX_X+4;// domain size in x_direction
	//int dmy=MAX_Y+4;// domain size in y_direction
	// domain size= 2 pixels wide border + input size + 2 pixels wide border

	initial_point[0][0]+=2;// to determine it in domain size
	initial_point[0][1]+=2;
	
	// set of pixels
	//pix pixels[dmx][dmy];
	
	pix **pixels = new pix*[dmx];// norm of gradiant of w ==> ||w||=||T||+lambda*||D||
	for(int i = 0; i < dmx; ++i) 
	{
		pixels[i] = new pix[dmy];
	}
	
	for (int i=0;i<=dmx-1;i++)
		for (int j=0;j<=dmy-1;j++)
		{
			pixels[i][j].x_index = i;// pixels' coordinates
			pixels[i][j].y_index = j;

			if ((i<=1 || i>=dmx-2) || (j<=1 || j>=dmy-2)) // for pixels of borders [ 0 1 .. dmx-2 dmx-1]
			   pixels[i][j].status = 0; // change their status to known to help program to stop on borders
			else // for internal pixels
			{
			   pixels[i][j].c = norm_w[i-2][j-2];
			   pixels[i][j].c =1/pixels[i][j].c; // speed function of internal pixels
			   //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			   pixels[i][j].status = status[i][j];
			   //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			}	
			
			pixels[i][j].w = 100000000000000.0;// initial arriaval time of pixels (infinity or very high)		
		}
	
	for(int i = 0; i < MAX_X; ++i) 
	{
		delete [] norm_w[i];
	}
	delete [] norm_w;
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	for(int i = 0; i < dmx; ++i) 
	{
		delete [] status[i];
	}
	delete [] status;
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	
	// Starting from initial point
	
	int current_x=initial_point[0][0];
	int current_y=initial_point[0][1];
	
	// set its arrival time ( as the starting point of FM ) to 0
	pixels[current_x][current_y].w=0;
	// change its status to "known" (0)
	pixels[current_x][current_y].status=0;
	
	Marli_heap *border_heap=new Marli_heap;// heap structure containing all pixels on the border line
	
	int neighbor_coordinates[4][2];// containing the coordinates of the current pixel's neighbors
	
	pixels[current_x][current_y].find_neighbors(neighbor_coordinates);// extracting the coordinates of the current pixel's neighbors
	//pixels[current_x][current_y].show_neighbors(neighbor_coordinates);

	
	for (int i=0;i<=3;i++)// for all 4 neighbors of the current pixel
	{
	 if (pixels[neighbor_coordinates[i][0]][neighbor_coordinates[i][1]].status==2) 
		{
	     // if the status of neighbors is "uknown" (2) change it to "on border"(1)		
	     pixels[neighbor_coordinates[i][0]][neighbor_coordinates[i][1]].status =1;
	     // update arrival time ((W)) of the neighbors
	     pixels[neighbor_coordinates[i][0]][neighbor_coordinates[i][1]].w=STEP_SIZE/pixels[neighbor_coordinates[i][0]][neighbor_coordinates[i][1]].c;
	     // insert the newly reached border pixels to the heap
	     border_heap->insert(border_heap,pixels[neighbor_coordinates[i][0]][neighbor_coordinates[i][1]]);
	     insert_count++;
	     heap_count++;
	     if (heap_count>max_heap_use)
			max_heap_use=heap_count;
		}
	}
	
	pix   next_point;// implying the next pixel to continue the FM process
	pix   current_neighbor; //processing neighbor of the next point
	double current_c; // speed function value of the processing neighbor of the next point
	int   neighbor_neighbor_coordinates[4][2];// containing the coordinates of the current neighbor's neighbors (neighbors of upper level)
	double ts_1;//
	double ts_2;
	double ts_temp;
	double u_1;
	double u_2;
	int   newly_reached=0;    // flag of new border members
	int   recently_changed=0; // flag of old border members which are newly changed by a better arrival time
	
	while (border_heap->tail!=-1) // do the process until there is no pixel on the border ( in the heap)
	{
		// extract a borther pixel with minimum arrival time as the next point to process
		border_heap->min_extract(border_heap,&next_point);
		extract_count++;
		heap_count--;
		//border_heap.disp(border_heap);
		cout<<endl;
		
		// set the status of the next point in the pixels structure as "known" (0)
		pixels[next_point.x_index][next_point.y_index].status=0;
		// extracting the coordinates of the next point's neighbors
		pixels[next_point.x_index][next_point.y_index].find_neighbors(neighbor_coordinates);
		//pixels[next_point.x_index][next_point.y_index].show_neighbors(neighbor_coordinates);
		
		for (int i=0;i<=3;i++)// for all 4 neighbors of the current pixel (next point)
	    {
			
	     if (pixels[neighbor_coordinates[i][0]][neighbor_coordinates[i][1]].status==2)//for new (unkown) neighbors of the next_point  
		    {
				// change their status from "uknown" (2) to "on border"(1)		
				pixels[neighbor_coordinates[i][0]][neighbor_coordinates[i][1]].status =1;
				newly_reached=1; // the point is the new member of the border line
			}
			
		 if (pixels[neighbor_coordinates[i][0]][neighbor_coordinates[i][1]].status!=0)
			{	
				current_neighbor=pixels[neighbor_coordinates[i][0]][neighbor_coordinates[i][1]];
				current_c=current_neighbor.c;
				// extracting the coordinates of the current neighbor's neighbors
				pixels[current_neighbor.x_index][current_neighbor.y_index].find_neighbors(neighbor_neighbor_coordinates);
				// obtaining minimum arriva timel ((W)) of the vertical neighbors of the current_neighbor 
				ts_1=fmin(pixels[neighbor_neighbor_coordinates[0][0]][neighbor_neighbor_coordinates[0][1]].w,pixels[neighbor_neighbor_coordinates[1][0]][neighbor_neighbor_coordinates[1][1]].w);
				// obtaining minimum arriva timel ((W)) of the horizontal neighbors of the current_neighbor
				ts_2=fmin(pixels[neighbor_neighbor_coordinates[2][0]][neighbor_neighbor_coordinates[2][1]].w,pixels[neighbor_neighbor_coordinates[3][0]][neighbor_neighbor_coordinates[3][1]].w);		    
				
				// put the smaller one in ts_1 (sort them)
				if (ts_2<ts_1)
				{
					ts_temp=ts_2;
					ts_2=ts_1;
					ts_1=ts_temp;
					
				}	
				
				// if wave comes from one direction
				u_1=ts_1+STEP_SIZE/current_c;
				u_2=u_1;
				
				// if wave comes from two directions
				double sum=ts_1+ts_2;
				if (u_1>ts_2)
				{
					u_2=sum+sqrt(pow(sum,2.0)+(2*(pow(STEP_SIZE,2.0)/pow(current_c,2.0)))-(2*(pow(ts_1,2.0)+pow(ts_2,2.0))));
					u_2=u_2/2;
				}
				// final value of the current neighbor arrival time ((W)) 
				// for previously calculated pixels on the border line we have to compare the previous computed arrival time 
				// with the newly computed time and use the smaller one
				if (pixels[current_neighbor.x_index][current_neighbor.y_index].w>u_2)
				 {
				    pixels[current_neighbor.x_index][current_neighbor.y_index].w=u_2;
				    recently_changed=1;   
				 }
				// insert the newly reached border pixels to the heap
				if (newly_reached) // if the point is a new member of border line
					{
					border_heap->insert(border_heap,pixels[current_neighbor.x_index][current_neighbor.y_index]);
					insert_count++;
					heap_count++;
					if (heap_count>max_heap_use)
						max_heap_use=heap_count;
					}
				if ((!newly_reached)&&(recently_changed)) // if the point is an old member of border line but has recently changed arrival time
					{ // resort the position of the recently changed border line member in the heap
					border_heap->resort(border_heap,pixels[current_neighbor.x_index][current_neighbor.y_index]);
					resort_count++;	
					}
		    }
		    newly_reached=0;
		    recently_changed=0;
         }
	}
	
	delete border_heap;
		
	double min=999999;
	double max=-999999;
	//cout<<"before_w="<<fixed<<pixels[21][131].w<<endl;
	//cout<<"geo_d="<<fixed<<geo_d[21][131]<<endl;
		
	for (int i=2;i<=MAX_X+1;i++)
		for (int j=2;j<=MAX_Y+1;j++)
		{
			//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			//pixels[i][j].w=pixels[i][j].w-(lambda*distance[i-2][j-2]);
			if (pixels[i][j].w==100000000000000.0)
				{pixels[i][j].w=0;}
			else
			{pixels[i][j].w=pixels[i][j].w-(lambda*geo_d[i][j]);}
			//if (pixels[i][j].w<0)
				//pixels[i][j].w=0;

			//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			//if (pixels[i][j].w<min){min=pixels[i][j].w;}
			//if (pixels[i][j].w>max){max=pixels[i][j].w;}
		}
	//cout<<"after_w="<<fixed<<pixels[21][131].w<<endl;	
		//cout<<"pixels[128][128].w="<<pixels[128][128].w<<endl;
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		
	//for(int i = 0; i < MAX_X; ++i) 
	//{
		//delete [] distance[i];
	//}
	//delete [] distance;
	for(int i = 0; i < dmx; ++i) 
	{
		delete [] geo_d[i];
	}
	delete [] geo_d;	
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	
		//cout<<"min="<<min<<endl;
		//cout<<"max="<<max<<endl<<endl;	
	
	ofstream myfile;
	myfile.open ("result.txt");
	
	for (int i=2;i<=MAX_X+1;i++)
	for (int j=2;j<=MAX_Y+1;j++)
	{
	
	myfile << pixels[i][j].w<<endl;
	} 
	 
	myfile.close();
	//cout<<border_heap.tail<<endl;
	
	for(int i = 0; i < dmx; ++i) 
	{
		delete [] pixels[i];
	}
	delete [] pixels;		
	
	//cout<<"extract_count="<<extract_count<<endl;
	//cout<<"insert_count="<<insert_count<<endl;
	//cout<<"resort_count="<<resort_count<<endl;
	//cout<<"max_heap_use="<<max_heap_use<<endl<<endl;
	cout<<"Operation is done!"<<endl<<endl;
	t2=clock();
	float diff ((float)t2-(float)t1);
	float seconds = diff / CLOCKS_PER_SEC;
	cout<<"time="<<seconds<<" s"<<endl<<endl;
	
	return 0;
}
////////////////////////////////////////////////////////////////////////
double fmin(double a,double b)
{
	double min=std::numeric_limits<double>::max();
	if(a<min)
	min=a;
	
	if(b<min)
	min=b;
	
	return min;
}

	
/* // how to read pixel intensities of a Mat Object
	int column=102;
	int row=146;
	Scalar intensity_x = sobl_grad_x.at<uchar>(Point(column, row));
	Scalar intensity_y = sobl_grad_y.at<uchar>(Point(column, row));
	cout<<"sobl_grad_x["<<row<<"]["<<column<<"]="<<intensity_x[0]<<endl;
    cout<<"sobl_grad_y["<<row<<"]["<<column<<"]="<<intensity_y[0]<<endl;
*/

