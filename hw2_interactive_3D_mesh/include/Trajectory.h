#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "Angel.h"
#include <ctime>
#include "glmath.h"
struct tnode
{
	vec3 point;
	tnode *next;
	tnode(const vec3& temp):point(temp){ next=NULL;}
};

tnode *end = new tnode(vec3(0.8,0,0));

tnode *head = end;

inline
void randomdiplace(const GLfloat& step)
{
	GLfloat angle_1 = -0.5;// ((GLfloat)rand()/(GLfloat)RAND_MAX) * (2.0 * M_PI);
        GLfloat angle_2 = -2.1;//((GLfloat)rand()/(GLfloat)RAND_MAX) * (2.0 * M_PI);
	//std::cout<<angle_1<<" "<<angle_2<<std::endl; 
	end-> next = new tnode(vec3(step*sin(angle_1)*cos(angle_2)+end->point.x,
				   step*sin(angle_1)*sin(angle_2)+end->point.y,
			           step*cos(angle_2)+end->point.z));
	end = end->next;
	//std::cout<<end->point<<std::endl;
}

inline
int checkbound(const vec3& vertice, vec3 nor[13], vec4 poi[39])
{
	GLfloat res=0;
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

#define BLACK vec4(0,0,0,1.0f)


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

#endif //trajectory
