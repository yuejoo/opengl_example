#include <sys/time.h>

#include "../include/Angel.h"
#include "../include/glmath.h"
#include "../include/mycolors.h"
#include "../include/mesh.h"
#define C30  0.433012702f // const number for the model
#define SCAL 1.0f // scal in homogenous
#define SIZE 3.0f // scal of the model

#define COLOR_LINE 0 // color of line
#define COLOR_FACE 2 // color of face
#define CYLINDER_SEG 30 // segments of cylinder

#define NUM_CAPE_CYLINDER 12 // vertices needed per cape cylinder
#define NUM_UNCAPE_CYLINDER 6 // vertices needed per uncape cylinder


typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
const int NumVertices = CYLINDER_SEG * (NUM_CAPE_CYLINDER * 3 + NUM_UNCAPE_CYLINDER); // 4 vertices per face;
const int points_size = sizeof(point4)*(NumVertices+8);
point4 *points;
point4 *colors; // Vertices of a unit cube centered at origin, sides aligned with axes
point4 ref_points[2];
point4 ref_colors[2];
int Index=0; 


//----------------------------------------------------------------------------
// generate mesh
//----------------------------------------------------------------------------

void mesh()
{


	//----------------LINES---------------	


	//----------------FACES----------------	
	//------Cannon's wheels

	cape_cylinder(points,colors, 0.05f, 0.25f, vec4(0.30f,0.0f,0.0f,1.0f), vec4(-1.0f,0.0f,0.0f,0.0f),CYLINDER_SEG, Index, 0,8,8);
	cape_cylinder(points,colors, 0.05f, 0.25f, vec4(-0.30f,0.0f,0.0f,1.0f), vec4(1.0f,0.0f,0.0f,0.0f),CYLINDER_SEG, Index, 0,8,8);

	//------Cannon's Axel	
	uncape_cylinder(points,colors, 0.9f, 0.05f, vec4(0.45f,0.0f,0.0f,1.0f), vec4(-1.0f,0.0f,0.0f,0.0f),CYLINDER_SEG, Index, 9);
	//------Cannon
	cape_cylinder(points,colors, 2.0f, 0.15f, vec4(0.0f,-0.4f,0.1f,1.0f), vec4(0.0f,1.0f,0.0f,0.0f),CYLINDER_SEG, Index, 1,0,8);
}

//---------------------------------------------------------------------------
// Draw the reference Aixses
//----------------------------------------------------------------------------
void draw_aixs()
{

	// for the reference line
	ref_points[0] = point4(0.0f,0.0f,0.0f,1.0f); ref_colors[0] = vertex_colors[1];  
	ref_points[1] = point4(0.0f,0.0f,0.0f,1.0f); ref_colors[1] = vertex_colors[1];
	points[Index] = point4(1.0f,0.0f,0.0f,1.0f);  colors[Index] = vertex_colors[1];  Index++;
	points[Index] = point4(-1.0f,0.0f,0.0f,1.0f);  colors[Index] = vertex_colors[1];  Index++;
	points[Index] = point4(0.0f,1.0f,0.0f,0.001f);  colors[Index] = vertex_colors[3];  Index++;
	points[Index] = point4(0.0f,-1.0f,0.0f,0.001f);  colors[Index] = vertex_colors[3];  Index++;
	points[Index] = point4(0.0f,0.0f,-1.0f,0.001f);  colors[Index] = vertex_colors[4];  Index++;
	points[Index] = point4(0.0f,0.0f,1.0f,0.001f);  colors[Index] = vertex_colors[4];  Index++;
}
//----------------------------------------------------------------------------
// OpenGL initializationi
//----------------------------------------------------------------------------
GLuint buffers[2];
GLuint model_view_loc;
GLuint projection_loc;

void init()
{
	//std::cout<<Index;
	points=  (vec4*) malloc(points_size);
	colors=  (vec4*) malloc(points_size);

	draw_aixs();
	mesh();
	// Create a vertex array object
	//GLuint vao;
	//glGenVertexArrays( 1, &vao);
	//glBindVertexArray( vao);

	// Create and initialize a buffer object
	glGenBuffers( 1, buffers);
	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	glBufferData( GL_ARRAY_BUFFER, points_size*2+sizeof(ref_points)*2,
			NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(ref_points), ref_points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(ref_points), points_size, points );

	glBufferSubData( GL_ARRAY_BUFFER, sizeof(ref_points)+points_size, sizeof(ref_points), ref_colors );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(ref_points)*2+points_size, points_size, colors );

	// Load shaders and use the resulting shader program
	GLuint program = InitShader( "./glsl/vshader32.glsl", "./glsl/fshader32.glsl" );
	glUseProgram( program );

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0) );

	GLuint vColor = glGetAttribLocation( program, "vColor" ); 
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(points_size+sizeof(ref_points)) );

	model_view_loc = glGetUniformLocation( program, "model_view");
	projection_loc = glGetUniformLocation( program, "projection");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable( GL_BLEND );
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int startx=512,moving,starty=384;
// 
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------Transformation matrix declare----------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

GLfloat radius=3.0f;
point4 click_point3d;


//----------------------------------------------------------------------------
//-----------------------------------------
// Array of rotation angles (in degrees) for each coordinate axis
//-----------------------------------------
//----------------------------------------------------------------------------
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int  Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
mat4 T(1.0f);
point4 d_moving(0.0f), mouse_last(0.0f);
int first_click = 0;
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Define some control flags
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
bool flag_reference=false;

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	//Rotation Matrix
	mat4  rote= (   RotateY( Theta[Xaxis]) *
			RotateZ( Theta[Zaxis]) );
	point4  eye( 0.0f, 0.0f, radius,  1.0f );

	T=rote*T;
	//eye=T*eye;	
	point4  at( 0.0f,  0.0f, 0.0f, 1.0f );
	vec4    up( 0.0f, 0.0f, 1.0f, 0.0f );

	mat4  mc = Frustum(-0.1024f, 0.1024f, -0.0768f, 0.0768f, 0.1f, 5.0f);	  
	mat4  mv = LookAt( eye, at, up );

	Theta[0]=0.0f;
	Theta[1]=0.0f;
	Theta[2]=0.0f;

	click_point3d=sol_inter_sphereandline(reprojection(mc*mv,pixels2homo( startx , starty , HAFW_WIDTH, HAFW_HEIGHT, 5.0f, 0.1f, 2.0)),3.0f);	
	//std::cout<<click_point3d<<std::endl;
	//std::cout<<mc*mv*click_point3d/2.0f<<std::endl;
	//std::cout<<startx<<" "<<starty<<std::endl;	

	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, mv*T);	
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, mc );

	//	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
	//	glBufferSubData( GL_ARRAY_BUFFER, sizeof(transformed_points),sizeof(colors),colors);
	//	glDrawArrays(GL_LINE_STRIP,0,3);

	if(moving)
	{
		d_moving= click_point3d - mouse_last;
		if(first_click<3)
			d_moving=0.0f;

		mouse_last = click_point3d;

		ref_points[0] =  gluInvertMatrix(T)* click_point3d;
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(ref_points), ref_points );
	}
	if(flag_reference)
	{	
		glDrawArrays( GL_LINES,  0, 2);
		glDrawArrays( GL_LINES,  2 , 6);
	}
	glDrawArrays( GL_TRIANGLES, 8, NumVertices);	
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

inline void keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
		case 033: // Escape Key
			free(points);
			free(colors);
			exit( EXIT_SUCCESS );
			break;
		case 'q':
			radius+=0.02;
			break;
		case 'e':
			radius-=0.02;
			break;
		case 'w':
			Theta[0]=1.1;
			break;
		case 's':
			Theta[0]=-1.1;
			break;
		case 'a':
			Theta[2]=1.1;
			break;
		case 'd':
			Theta[2]=-1.1;
			break;
		case 'r':
			flag_reference = !flag_reference;	
			break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
void mouse( int button, int state, int x, int y )
{
	if ( state == GLUT_DOWN ) {
	}
}


inline static void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			moving = 0;
			d_moving = point4(0,0,0,0);
			first_click=0;
			glutPostRedisplay();
		}
	}
}
inline static void mouseMotion(int x, int y){
	if (moving) {
		startx = x;
		starty = y;
		first_click++;	
		Theta[2]=180*d_moving[1]/M_PI;
		Theta[0]=-180*d_moving[2]/M_PI;
		glutPostRedisplay();
	}
}
//----------------------------------------------------------------------------
void idle( void )
{
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 1024, 768 );
	//    glutInitContextVersion( 3, 2 );
	//    glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "Color Cube" );

	glewInit();

	init();

	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouseButton );
	glutMotionFunc( mouseMotion );
	glutMainLoop();
	return 0;
}
