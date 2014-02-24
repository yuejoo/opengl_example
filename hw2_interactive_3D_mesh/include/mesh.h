/////////////////////////////////////////////////////////////////////////
//
// ---------------------mesh.h--------------------------
// Mesh different object
/////////////////////////////////////////////////////////////////////////


#ifndef MESH_H
#define MESH_H

#include "Angel.h"
#include "mycolors.h"
//--------------------------------------------------------------------
//   generate a uncaped_cylinder
//
//--------------------------------------------------------------------
inline
void uncape_cylinder(vec4* t_point, vec4* t_colors, const GLfloat& height, const GLfloat& radius, const vec4& bottom, const vec4& direct, const int& segment, int& index, const int& colindex)
{


//--------------------------------------------------------------------
// compute transform matrix
//--------------------------------------------------------------------
	vec3 ref = cross(vec4(1.0f,0.0f,0.0f,0.0f),normalize(direct));
	if( length(ref) < 0.0001f)
		ref = normalize(cross(vec4(0.0f,0.0f,1.0f,0.0f),direct));		
        else
		ref = normalize(ref);
	vec4 nn=vec4(ref.x,ref.y,ref.z,0.0f);
 	vec3 vv = normalize(cross(nn,direct));
	vec4 v= vec4(vv.x,vv.y,vv.z,0.0f);
		
	std::cout<<ref<<std::endl;	
	mat4 trans=mat4( normalize(v)  , normalize(nn) , normalize(direct) , vec4(0.0f,0.0f,0.0f,1.0f));		
	trans =  Translate(bottom)*trans;
//--------------------------------------------------------------------
// Add
//--------------------------------------------------------------------

	for(int i=0; i<segment; i++)
	{

		GLfloat const d0= 2*M_PI/segment*(float)i;
		GLfloat const d1= 2*M_PI/segment*(float)(i+1);
		 
		t_point[index]=trans * vec4( sin(d0)*radius, cos(d0)*radius, 0.0f ,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;
		
		t_point[index]=trans*vec4( sin(d1)*radius, cos(d1)*radius, 0.0f,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;

		t_point[index]=trans*vec4( sin(d1)*radius, cos(d1)*radius, height,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;
		
		t_point[index]=trans*vec4( sin(d0)*radius, cos(d0)*radius, height,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;
 
		t_point[index]=trans*vec4( sin(d1)*radius, cos(d1)*radius, height,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;

		t_point[index]=trans*vec4( sin(d0)*radius, cos(d0)*radius, 0.0f,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;
	}
}


inline
void cape_cylinder(vec4* t_point, vec4* t_colors, const GLfloat& height, const GLfloat& radius, const vec4& bottom, const vec4& direct, const int& segment, int& index, const int& colindex, const int& coltop, const int& colbot)
{

//--------------------------------------------------------------------
// compute transform matrix
//--------------------------------------------------------------------
//--------------------------------------------------------------------
	vec3 ref = cross(vec4(1.0f,0.0f,0.0f,0.0f),normalize(direct));
	if( length(ref) < 0.0001f)
		ref = normalize(cross(vec4(0.0f,0.0f,1.0f,0.0f),direct));		
        else
		ref = normalize(ref);
	vec4 nn=vec4(ref.x,ref.y,ref.z,0.0f);
 	vec3 vv = normalize(cross(nn,direct));
	vec4 v= vec4(vv.x,vv.y,vv.z,0.0f);
		
	std::cout<<ref<<std::endl;	
	mat4 trans=mat4( normalize(v)  , normalize(nn) , normalize(direct) , vec4(0.0f,0.0f,0.0f,1.0f));		
	trans =  Translate(bottom) * trans;


	for(int i=0; i<segment; i++)
	{

		GLfloat const d0= 2*M_PI/segment*(float)i;
		GLfloat const d1= 2*M_PI/segment*(float)(i+1);
		 
		t_point[index]=trans * vec4( sin(d0)*radius, cos(d0)*radius, 0.0f ,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;
		
		t_point[index]=trans*vec4( sin(d1)*radius, cos(d1)*radius, 0.0f,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;

		t_point[index]=trans*vec4( sin(d1)*radius, cos(d1)*radius, height,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;
		
		t_point[index]=trans*vec4( sin(d0)*radius, cos(d0)*radius, height,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;
 
		t_point[index]=trans*vec4( sin(d1)*radius, cos(d1)*radius, height,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;

		t_point[index]=trans*vec4( sin(d0)*radius, cos(d0)*radius, 0.0f,1.0f);
		t_colors[index]= vertex_colors[colindex];
		index++;
	}
	for(int i=0; i<segment; i++)
	{
		GLfloat const d0= 2*M_PI/segment*(float)i;
		GLfloat const d1= 2*M_PI/segment*(float)(i+1);
		 
		t_point[index]=trans*vec4( sin(d0)*radius, cos(d0)*radius, 0.0f,1.0f);
		t_colors[index]= vertex_colors[colbot];
		index++;
		
		t_point[index]=trans*vec4( 0.0f, 0.0f, 0.0f,1.0f);
		t_colors[index]= vertex_colors[colbot];
		index++;
		
		t_point[index]=trans*vec4( sin(d1)*radius, cos(d1)*radius, 0.0f,1.0f);
		t_colors[index]= vertex_colors[colbot];
		index++;
		
		t_point[index]=trans*vec4( sin(d0)*radius, cos(d0)*radius, height,1.0f);
		t_colors[index]= vertex_colors[coltop];
		index++;

		t_point[index]=trans*vec4( 0.0f, 0.0f, height,1.0f);
		t_colors[index]= vertex_colors[coltop];
		index++;
				
		t_point[index]=trans*vec4( sin(d1)*radius, cos(d1)*radius, height,1.0f);
		t_colors[index]= vertex_colors[coltop];
		index++;
	

	}
}
#endif
