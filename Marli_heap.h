/*
********************************************************************
**
Copyright (c) 2018, Maryam Khanain, Ali Sharifi Boroujerdi
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
********************************************************************
*/
#ifndef MARLI_HEAP_
#define MARLI_HEAP_

#include <iostream>
#include <cstring>
#include <math.h> 

#include "pix.h"

# define SIZE 3000

using namespace std;

class Marli_heap 
{
	public:
	int size;
	int tail;
	pix elements[SIZE];

	// Methods
	public:
	Marli_heap();
	void  disp(Marli_heap* );
	void  insert(Marli_heap* ,pix);
	void  min_extract(Marli_heap* ,pix*);
	void  resort(Marli_heap* ,pix);
};

// methods definition
Marli_heap::Marli_heap()
{
tail=-1;
size=SIZE;
}
///////////////////////////////////////////////////
void Marli_heap::disp(Marli_heap *heap)
{
   for (int i=0;i<=heap->tail;i++)
                cout<<"Element["<<i<<"]="<<heap->elements[i].w<<"		["<<heap->elements[i].x_index<<"]["<<heap->elements[i].y_index<<"]"<<endl;
}
///////////////////////////////////////////////////
void Marli_heap::insert(Marli_heap *heap,pix item)
{
	int child_index;
	int father_index;
	pix temp;
	// if the heap is full writing a new item is not possible
	if (heap->tail==heap->size)
	 cout<<"The heap is full!"<<endl;
	else
	 {
		//increase tail_index before insertion 
		heap->tail++;
		// insert the inputed item to the last (tail) location
		heap->elements[heap->tail]=item;
			//heap->elements[heap->tail].x_index=item.x_index;
			//heap->elements[heap->tail].y_index=item.y_index;
			//heap->elements[heap->tail].c=item.c;
			//heap->elements[heap->tail].w=item.w;
			//heap->elements[heap->tail].status=item.status;
		// save tail number in another variable to prevent it from changing
		child_index=heap->tail;
		/* extract the index of the father node of the inserted item
           the father of the root is itself!*/
		if (child_index>0)
		 {
		   father_index=floor((child_index-1)/2);
	     }
		else
		{
		   father_index=0;
	    }
		   /* check if the father's ((w)) is greater than item' ((w)) (his child) swap
              them (temp is an auxiliary variable for swapping)
              continue swapping until father's w < child's w
              WE DON NOT NEED TO DO ANYTHING ELSE WITH THE FIRST
              ELEMENT(ROOT)*/   
		while ((heap->elements[father_index].w > heap->elements[child_index].w)&&(child_index!=0))
		{
		 // swapping
		 temp=heap->elements[child_index];
		 heap->elements[child_index]=heap->elements[father_index];
		 heap->elements[father_index]=temp;
		 // update father and child indeces for moving up in the heap
		 child_index=father_index;
		 if (child_index>0)
		  {
			father_index=floor((child_index-1)/2);
	      }
		  else
		  {
		    father_index=0;
		  }
		 }	
	 }//else	 
 }//Marli_heap::insert
///////////////////////////////////////////////////
void Marli_heap::min_extract(Marli_heap *heap,pix *min)
{
   int father_index;
   int left_child_index;
   int right_child_index;
   int min_child_index;
   int done;
   pix temp;
   // if the heap is empty there is nothing to extract!
   if (heap->tail==-1)
   {
	cout<<"The heap is empty!"<<endl;  
   }
   else
   {
	// extracting the minimum from the fist node (root)
	*min=heap->elements[0];
	// replacing the root with the last element
	heap->elements[0]=heap->elements[heap->tail];
	// decreasing the tail number because we have removed one node from the list
	heap->tail--;
	// going down through the heap and check the heap property
	// correct it if it is damaged
	if (heap->tail!=-1) // if the extracted item was not the root 
	{
	 father_index=0;
	 left_child_index =(2*father_index)+1;
	 right_child_index=(2*father_index)+2;
	 //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	 while (father_index<=heap->tail)
	 {
		 done=0; // flag of donig one step
		 //----------------------------------------------------
		 // (1) if father has two children
		 
		 if (right_child_index<=heap->tail)
		 { 
			// compare two chidldrens' ((W)) and find min
			if (heap->elements[left_child_index].w<heap->elements[right_child_index].w)
				{
					min_child_index=left_child_index;
				}
			else
				{
					min_child_index=right_child_index;
				}
				
			// if father is bigger than min_child then swap the values (temp is an auxiliary variable for swapping)
			if (heap->elements[father_index].w>heap->elements[min_child_index].w)
			{
				temp=heap->elements[min_child_index];
				heap->elements[min_child_index]=heap->elements[father_index];
				heap->elements[father_index]=temp;
			}
			// go down though the heap by updating indeces
			father_index=min_child_index;
			left_child_index=(2*father_index)+1;;
			right_child_index=(2*father_index)+2;
			done=1;// step is done * do not go into the next if body
		 }
		 //----------------------------------------------------
		 // (2) if father has one (left) child but not two
		 if ((left_child_index<=heap->tail) && (!done))
		 {
			  min_child_index=left_child_index;
			  // if father's ((W)) is bigger than min_child's ((W)) then swap them (temp is an auxiliary variable for swapping)
			  if (heap->elements[father_index].w>heap->elements[min_child_index].w)
			  {
				  temp=heap->elements[min_child_index];
				  heap->elements[min_child_index]=heap->elements[father_index];
				  heap->elements[father_index]=temp;
			  }
			  // go down though the heap by updating indeces
			  father_index=min_child_index;
			  left_child_index=(2*father_index)+1;
			  right_child_index=(2*father_index)+2;
			  done=1;// step is done * do not go into the next if body
		 }
		 //----------------------------------------------------
		 // (3) if father has no child
		 if (!done)
		 {
			  // beark the while loop condition to exit
			  father_index=heap->tail+1;
		 }
		 
		 //----------------------------------------------------
	  } // (while)
	 //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	} //  heap->tail!=-1
   }// first if heap->tail==0 
}// min_extract
///////////////////////////////////////////////////
///////////////////////////////////////////////////
void Marli_heap::resort(Marli_heap *heap,pix item)
{	
	int item_index_in_heap;
	int item_father_index;
	pix temp;
	int i;
	
	for (i=0;i<=heap->tail;i++) // find the position (index) of the item in the heap
		{
			if ((heap->elements[i].x_index==item.x_index)&&(heap->elements[i].y_index==item.y_index))
			{
				item_index_in_heap=i;
				break;
			}
		}
		
	heap->elements[item_index_in_heap].w=item.w; // assign the new arrival time to the corresponding position of the item in the heap
	item_father_index=floor((item_index_in_heap-1)/2);
	while ((heap->elements[item_father_index].w>heap->elements[item_index_in_heap].w)&&(item_father_index>=0))
	{
		// swap the values (temp is an auxiliary variable for swapping)
		temp=heap->elements[item_index_in_heap];
		heap->elements[item_index_in_heap]=heap->elements[item_father_index];
		heap->elements[item_father_index]=temp;
		// go up through the heap
		item_index_in_heap=item_father_index;
		item_father_index=floor((item_index_in_heap-1)/2);
	}
	 
}//Marli_heap::resort
///////////////////////////////////////////////////
#endif /* MARLI_HEAP_*/

