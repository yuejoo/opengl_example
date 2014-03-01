//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
// Implementation of relative function for the trajectory
//
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "Angel.h"
#include <ctime>
#include "glmath.h"
//-------------------------------------------------------------------
// Struct of the node list
//-------------------------------------------------------------------
struct tnode
{
	vec3 point;
	tnode *next;
	tnode(const vec3& temp):point(temp){ next=NULL;}
};

tnode *end = new tnode(vec3(0.0,0,0));

tnode *head = end;




//-------------------------------------------------------------------
// Random displace of each step
//-------------------------------------------------------------------


	inline
void randomdiplace(const GLfloat& step)
{
	GLfloat angle_1 =  ((GLfloat)rand()/(GLfloat)RAND_MAX) * (2.0 * M_PI);
	GLfloat angle_2 = ((GLfloat)rand()/(GLfloat)RAND_MAX) * (2.0 * M_PI);
	//std::cout<<angle_1<<" "<<angle_2<<std::endl; 
	end-> next = new tnode(vec3(step*sin(angle_1)*cos(angle_2)+end->point.x,
				step*sin(angle_1)*sin(angle_2)+end->point.y,
				step*cos(angle_2)+end->point.z));
	end = end->next;
	//std::cout<<end->point<<std::endl;
}
//-------------------------------------------------------------------
// Bound checker, need the models normals and reference point
//-------------------------------------------------------------------
	inline
int checkbound(const vec3& vertice, vec3 (&nor)[13], vec4 (&poi)[39])
{
	for(int i=0; i<13; i++)
	{
		vec3 ppoi(poi[i*3].x,poi[i*3].y,poi[i*3].z);
		vec3 temp= ppoi-vertice;
		GLfloat res = dot(temp,nor[i]);
		if( res < 0 )
		{
			res=-res;
			//std::cout<<"out"<<std::endl;
		}
		if( res < 0.02f )
		{

			return i;	
		}
	}
	return -1;
}

	inline
int checkinside(const vec3& vertice, vec3 (&nor)[13], vec4 (&poi)[39])
{
	int countt=0;
	for(int i=0; i<13; i++)
	{
		vec3 ppoi(poi[i*3].x,poi[i*3].y,poi[i*3].z);
		vec3 temp= ppoi-vertice;
		GLfloat res = dot(temp,nor[i]);
		if( res < 0 )
		{
	//		++countt;
			
			res=-res;
		}
		else
			++countt;
		if( res < 0.05f )
		{

			return -1;	
		}
	}
	std::cout<<countt<<std::endl;

	if(countt==13)	
		return 1;
	else
		return -1;
}
#define BLACK vec4(0,0,0,1.0f)


//-------------------------------------------------------------------
// Make the object face to be black
//-------------------------------------------------------------------
	inline
int pointertofaces(const int& i, vec4 *t_color)
{
	if(i<6 && i>-1)
	{
		for(int j=0; j<3; j++ ) 
			t_color[48+i*3+j]=BLACK;	
	}
	if(i==6)
	{
		for(int j=0; j<12; j++ ) 
			t_color[66+j]=BLACK;	
	}
	if(i>6)
	{

		for(int j=0; j<6; j++ ) 
			t_color[78+(i-7)*6+j]=BLACK;	
	}
	return 0;
}

//-------------------------------------------------------------------
// Copy the list to the array
//-------------------------------------------------------------------
	inline
vec4 *copytoarray(const int& count)
{
	tnode *temp=head;
	vec4 *line;
	if(count>=0)
	{
		line = new vec4[count];
	}
	int i=0;
	while(i<count)
	{
		line[i]=vec4(temp->point.x,temp->point.y,temp->point.z,1);//vec4( temp->point[0],temp->point[1],temp->point[2],1.0f);
		temp=temp->next;
		//	std::cout<<line[i]<<std::endl;
		i++;
	}
	return line;
}





	inline
void randomstart(tnode *start, vec3 (&nor)[13], vec4 (&poi)[39], GLfloat step)
{
	GLfloat angle_1 =  ((GLfloat)rand()/(GLfloat)RAND_MAX) * (2.0 * M_PI);
	GLfloat angle_2 = ((GLfloat)rand()/(GLfloat)RAND_MAX) * (2.0 * M_PI);
	start->point = vec3(step*sin(angle_1)*cos(angle_2),
			step*sin(angle_1)*sin(angle_2),
			step*cos(angle_2)+0.5);
	int flag = checkinside(start->point,nor,poi);
	while(flag!=1)
	{
		time_t timer;
		srand((unsigned)time(&timer));
		angle_1 =  ((GLfloat)rand()/(GLfloat)RAND_MAX) * (2.0 * M_PI);
		angle_2 = ((GLfloat)rand()/(GLfloat)RAND_MAX) * (2.0 * M_PI);
		start->point = vec3(step*sin(angle_1)*cos(angle_2),
				step*sin(angle_1)*sin(angle_2),
				step*cos(angle_2));
		std::cout<<flag<<std::endl;

		flag = checkinside(start->point,nor,poi);

	}

}


#endif //trajectory
