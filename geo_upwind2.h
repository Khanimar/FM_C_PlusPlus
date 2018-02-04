#ifndef GEO_UPWIND2_H_
#define GEO_UPWIND2_H_

#include <iostream>
#include <cstring>
#include <math.h> 
//#include <opencv2/opencv.hpp>

//#include "grid1.h"
//#include "distance1.h"
#include "pix.h"
#include "Marli_heap.h"

# define MAX_X 256 // number of pixels in x-direction
# define MAX_Y 256// number of pixels in y-direction

# define STEP_SIZE 1

using namespace std;
//using namespace cv;

double max3(double,double,double);
int geo_upwind2(double**,double**,int**,double**);

////////////////////////////////////////////////////////////
int geo_upwind2(double** dx,double** dy,int** status,double** geo_d)
{
	int initial_point[1][2];
	initial_point[0][0]=floor(MAX_X/2);
	initial_point[0][1]=floor(MAX_Y/2);
	
	int max_x=MAX_X;
	int max_y=MAX_Y;
	//int grid[MAX_X][MAX_Y][2];
	//double distance[MAX_X][MAX_Y];
	//double D_extended[MAX_X+2][MAX_Y+2];
	
	//make_grid1(max_x,max_y,grid);
	//show_grid(max_x,max_y,grid);
	//make_distance1(initial_point,max_x,max_y,grid,distance);
	//show_distance(max_x,max_y,distance);
	


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Solving Eikonal$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//-------------------------------------------------------------------------------------------------------
	int dmx=MAX_X+4;// domain size in x_direction
	int dmy=MAX_Y+4;// domain size in y_direction
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

			//if ((i<=1 || i>=dmx-2) || (j<=1 || j>=dmy-2)) // for pixels of borders [ 0 1 .. dmx-2 dmx-1]
			   //pixels[i][j].status = 0; // change their status to known to help program to stop on borders
			//else // for internal pixels
			//{
			   //pixels[i][j].c = 1;
			   //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			pixels[i][j].status = status[i][j];
			   //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			//}	
			
			pixels[i][j].w = 100000000000000.0;// initial arriaval time of pixels (infinity or very high)		
		}
	
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
	     //insert_count++;
	     //heap_count++;
	     //if (heap_count>max_heap_use)
		 //	max_heap_use=heap_count;
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
		//extract_count++;
		//heap_count--;
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
					//insert_count++;
					//heap_count++;
					//if (heap_count>max_heap_use)
					//	max_heap_use=heap_count;
					}
				if ((!newly_reached)&&(recently_changed)) // if the point is an old member of border line but has recently changed arrival time
					{ // resort the position of the recently changed border line member in the heap
					border_heap->resort(border_heap,pixels[current_neighbor.x_index][current_neighbor.y_index]);
					//resort_count++;	
					}
		    }
		    newly_reached=0;
		    recently_changed=0;
         }
	}
	
	delete border_heap;		
		
	for (int i=0;i<=dmx-1;i++)
		for (int j=0;j<=dmx-1;j++)
		{
			geo_d[i][j]=pow(pixels[i][j].w,2.0);
		}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	double D_extended[MAX_X+2][MAX_Y+2];
	// initializing d_exneded with distance except boundaries
	for (int i=1;i<=MAX_X;i++)
		for (int j=1;j<=MAX_Y;j++)
			D_extended[i][j]=geo_d[i+1][j+1];
	
	// boundaries initialization of d_extended matrix
	for (int i=0;i<=MAX_X;i++)
	{
        D_extended[i+1][0]=D_extended[i+1][1]; // initializing left border by its neighbor column
        D_extended[i+1][MAX_Y+1]=D_extended[i+1][MAX_Y]; // initializing right border by its neighbor column
	}
	
	for (int i=0;i<=MAX_Y;i++)
	{
			D_extended[0][i+1]=D_extended[1][i+1]; // initializing up border by its neighbor row
			D_extended[MAX_X+1][i+1]=D_extended[MAX_X][i+1]; // initializing down border by its neighbor row
	}
    
    D_extended[0][0]=D_extended[1][1]; // initializing upper left pixel
    D_extended[0][MAX_Y+1]=D_extended[1][MAX_Y]; // initializing upper right pixel
    D_extended[MAX_X+1][0]=D_extended[MAX_X][1]; // initializing lower left pixel
    D_extended[MAX_X+1][MAX_Y+1]=D_extended[MAX_X][MAX_Y]; // initializing lower right pixel
    
    double up;
    double down;
    double left;
    double right;
    
    for (int i=1;i<=MAX_X;i++)
		for (int j=1;j<=MAX_Y;j++)
		{
		   up=(D_extended[i][j]-D_extended[i-1][j]);
		   down=(D_extended[i][j]-D_extended[i+1][j]);
			
		   dx[i-1][j-1]=max3(up,down,0);
		 
		   if (down>up)
				{
					dx[i-1][j-1]=-dx[i-1][j-1];
				}
		   
		   //------------------------------------------------------------------ 
		   left=(D_extended[i][j]-D_extended[i][j-1]);
		   right=(D_extended[i][j]-D_extended[i][j+1]);
		   
		   dy[i-1][j-1]=max3(left,right,0);

		   
		   if (right>left)
				{dy[i-1][j-1]=-dy[i-1][j-1];}
      
		}
				
	return 0;
		
}
////////////////////////////////////////////////////////////
double max3(double a, double b, double c)
{
	double max=-9999999.0;
	if (a>=max)
		max=a;
	if (b>=max)
		max=b;
	if (c>=max)
		max=c;
		
	return max;		
}
////////////////////////////////////////////////////////////
#endif /* GEO_UPWIND_H_*/
