//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "../include/Angel.h"

#define C30  0.433012702f // const number for the model
#define SCAL 1.0f // scal in homogenous
#define SIZE 3.0f // scal of the model

#define COLOR_LINE 1 // color of line
#define COLOR_FACE 2 // color of face

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
const int NumVertices = 60; //(6 faces)*(2 triangles/face)*(3 vertices/triangle)
point4 points[NumVertices+36];
point4 colors[NumVertices+36]; // Vertices of a unit cube centered at origin, sides aligned with axes


point4 vertices[12] = {
	point4(  0.5*SIZE ,  0.0*SIZE,  0.5, SCAL ),
	point4(  0.25*SIZE,  C30*SIZE,  0.5, SCAL ),
	point4( -0.25*SIZE,  C30*SIZE,  0.5, SCAL ),
	point4( -0.5*SIZE , 0.0*SIZE,  0.5, SCAL ),
	point4( -0.25*SIZE, -C30*SIZE,  0.5, SCAL ),
	point4(  0.25*SIZE, -C30*SIZE,  0.5, SCAL ),
	point4(  0.5*SIZE ,  0.0*SIZE, -0.5, SCAL ),
	point4(  0.25*SIZE,  C30*SIZE, -0.5, SCAL ),
	point4( -0.25*SIZE,  C30*SIZE, -0.5, SCAL ),
	point4( -0.5*SIZE ,  0.0*SIZE, -0.5, SCAL ),
	point4( -0.25*SIZE, -C30*SIZE, -0.5, SCAL ),
	point4(  0.25*SIZE, -C30*SIZE, -0.5, SCAL ),
};

// RGBA olors
color4 vertex_colors[8] = {
	color4( 0.0, 0.0, 0.0, 0.7 ),  // black
	color4( 1.0, 0.0, 0.0, 1.0 ),  // red
	color4( 1.0, 1.0, 0.0, 0.2 ),  // yellow
	color4( 0.0, 1.0, 0.0, 0.1 ),  // green
	color4( 0.0, 0.0, 1.0, 0.1 ),  // blue
	color4( 0.0, 1.0, 1.0, 0.1 ),  // magenta
	color4( 1.0, 1.0, 1.0, 0.1 ),  // white
	color4( 0.5, 0.5, 0.5, 0.1 )   // cyan
};


// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int  Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

int Index = 0; // Index of VAOs

void line(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[d]; Index++;
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[a]; Index++;
}

void quad( int a, int b, int c, int d , int color)
{
	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[d]; Index++;
}

void poly6( int a, int b, int c, int d, int e, int f, int color)
{

	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[d]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[e]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[e]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[f]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[e]; Index++;
}

//----------------------------------------------------------------------------
// generate 12 triangles: 36 vertices and 36 colors
void colorcube()
{
	line(0,6,11,5);
	line(5,11,10,4);
	line(4,10,9,3);
	line(3,9,8,2);
	line(2,8,7,1);
	line(1,7,6,0);
	poly6(5,4,3,2,1,0,COLOR_FACE);
	poly6(11,10,9,8,7,6,COLOR_FACE);
	quad( 0, 5, 11, 6 ,COLOR_FACE);
	quad( 10, 4, 5, 11 ,COLOR_FACE);
	quad( 4, 3, 9, 10 ,COLOR_FACE);
	quad( 9, 8, 2, 3 ,COLOR_FACE);
	quad( 2, 8, 7, 1 ,COLOR_FACE);
	quad( 1, 7, 6, 0 ,COLOR_FACE);
}

//---------------------------------------------------------------------------

// OpenGL initializationi
GLuint buffers[2];
GLuint model_view_loc;
GLuint projection_loc;

void init()
{
	colorcube();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays( 1, &vao);
	glBindVertexArray( vao);

	// Create and initialize a buffer object
	glGenBuffers( 1, buffers);
	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
			NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

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
			BUFFER_OFFSET(sizeof(points)) );

	model_view_loc = glGetUniformLocation( program, "model_view");
	projection_loc = glGetUniformLocation( program, "projection");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable( GL_BLEND );
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------
//Glfloat theta=0.0f;
//Glfloat phi=0.0f;
GLfloat radius=5.0f;
//Glfloat test[6]={0.0f};

mat4 T(1.0f);
void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	point4  eye( radius, 0.0f, 0.0f, 1.0f );
	point4  at( 0.0f,  0.0f, 0.0f, 0.0f );
	vec4    up( 0.0f, 1.0f, 0.0f, 0.0f );
	mat4  mc = Frustum(-0.1024f, 0.1024f, -0.0768f, 0.0768f, 0.01f, 5.0f);	  
	mat4  mv = LookAt( eye, at, up );
	mat4  N = Ortho(-1.0f, 1.0f, -1.0f, 1.0f,  10.0f , -10.0f );
	mat4  transform =mc *N * mv;
	mat4  rote= (   RotateZ( Theta[Zaxis]) *
			RotateY( Theta[Yaxis]) *
			RotateX( Theta[Xaxis]) );

	Theta[0]=0.0f;
	Theta[1]=0.0f;
	Theta[2]=0.0f;
	T=rote*T;


	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, T);	
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, mc * N * mv);

	//	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
	//	glBufferSubData( GL_ARRAY_BUFFER, sizeof(transformed_points),sizeof(colors),colors);
	//	glDrawArrays(GL_LINE_STRIP,0,3);

	glDrawArrays( GL_LINE_STRIP, 0 , 30);
	glDrawArrays( GL_TRIANGLES, 30, NumVertices+30);
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

inline void keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
		case 033: // Escape Key
			exit( EXIT_SUCCESS );

			break;
		case 'q':
			radius+=0.1;
			break;
		case 'e':
			radius-=0.1;
			break;
		case 'w':
			Theta[1]+=10.1;
			break;
		case 's':
			Theta[1]-=10.1;
			break;
		case 'a':
			Theta[2]-=10.1;
			break;
		case 'd':
			Theta[2]+=10.1;
			break;
		break;
	}
		glutPostRedisplay();
}

//----------------------------------------------------------------------------
void mouse( int button, int state, int x, int y )
{
	if ( state == GLUT_DOWN ) {
		switch( button ) {
			case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
			case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
			case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
	}
}

int startx,moving,starty;
GLfloat phi,theta;

inline static void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			phi = 0;
			theta = 0;
			moving = 0;
		}
	}
}
inline static void mouseMotion(int x, int y){
	if (moving) {
		phi = phi + (x - startx);
		theta= theta + (y - starty);
		startx = x;
		starty = y;
		Theta[1] = phi/1024.0f*M_PI*1.5;
		Theta[2] = -theta/768.f*M_PI*1.5;
			
		std::cout<<x<<" "<<y<<std::endl;
		glutPostRedisplay();
	}
}
//----------------------------------------------------------------------------
void idle( void )
{
	if ( Theta[Axis] > 360.0 ) {
		Theta[Axis] -= 360.0;
	}
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
