//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "../include/Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
const int NumVertices = 36+24; //(6 faces)(2 triangles/face)(3 vertices/triangle)
const GLfloat C30 = 0.433012702;
const int scal =1.0;
GLfloat size=2.0;
point4 points[NumVertices+36];
point4 colors[NumVertices+36];
// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[12] = {
	point4(  0.5*size ,  0.0*size,  0.5, scal ),
	point4(  0.25*size,  C30*size,  0.5, scal ),
	point4( -0.25*size,  C30*size,  0.5, scal ),
	point4( -0.5*size , 0.0*size,  0.5, scal ),
	point4( -0.25*size, -C30*size,  0.5, scal ),
	point4(  0.25*size, -C30*size,  0.5, scal ),
	point4(  0.5*size ,  0.0*size, -0.5, scal ),
	point4(  0.25*size,  C30*size, -0.5, scal ),
	point4( -0.25*size,  C30*size, -0.5, scal ),
	point4( -0.5*size ,  0.0*size, -0.5, scal ),
	point4( -0.25*size, -C30*size, -0.5, scal ),
	point4(  0.25*size, -C30*size, -0.5, scal ),
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
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };


//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
int idc=1;
int idp=2;

	void
line(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[idc]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[idc]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[idc]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[idc]; points[Index] = vertices[d]; Index++;
	colors[Index] = vertex_colors[idc]; points[Index] = vertices[a]; Index++;
}

	void
quad( int a, int b, int c, int d )
{
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[d]; Index++;
}

	void
poly6( int a, int b, int c, int d, int e, int f, int idp)
{

	colors[Index] = vertex_colors[idp]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[d]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[e]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[e]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[f]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[idp]; points[Index] = vertices[e]; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
	void
colorcube()

{
	line(0,6,11,5);
	line(5,11,10,4);
	line(4,10,9,3);
	line(3,9,8,2);
	line(2,8,7,1);
	line(1,7,6,0);
	poly6(5,4,3,2,1,0,0);
	poly6(11,10,9,8,7,6,2);
	quad( 0, 5, 11, 6 );
	quad( 10, 4, 5, 11 );
	quad( 4, 3, 9, 10 );
	quad( 9, 8, 2, 3 );
	quad( 2, 8, 7, 1 );
	quad( 1, 7, 6, 0 );
	std::cout<<Index<<" ------------------------------";
//	for(int i=0; i<6; i++)
}

//---------------------------------------------------------------------------

// OpenGL initializationi
GLuint buffers[2];

	void
init()
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
	GLuint program = InitShader( "../glsl/vshader32.glsl", "../glsl/fshader32.glsl" );
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

	glEnable( GL_DEPTH_TEST );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable( GL_BLEND );
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------
float theta=0;
float phi=0;
float radius=5;
float test[6]={0};
float at_x=0;
float at_y=0;
float at_z=0;
	void
display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	mat4  big(size,size,size);

    point4  eye( radius*sin(Theta[Yaxis])*cos(Theta[Xaxis]),
		 radius*sin(Theta[Yaxis])*sin(Theta[Xaxis]),
		 radius*cos(Theta[Yaxis]),
		 1.0 );
    point4  at( at_x, at_y, at_z, 1.0 );
    vec4    up( 0.0, 1.0, 0.0, 0.0 );
    mat4  mc = Frustum(-0.05, 0.05, -0.05, 0.05, test[4] , test[5] );
	mat4  proj = Perspective(test[0],test[1],test[2],test[3]);
    mat4  mv = LookAt( eye, at, up );
	mat4  N = Ortho(-1, 1, -1, 1,  10 , -10 );
	mat4  transform =mc *N * mv * ( RotateX( Theta[Xaxis] ) *
	//		RotateY( Theta[Yaxis] ) *
		RotateZ( Theta[Zaxis]*100 ) );
	std::cout<<test[0]<<' '<<test[1]<<' '<<test[2]<<' '<<test[3]<<' '<<test[4]<<' '<< test[5]<<std::endl;
	point4  transformed_points[NumVertices+36];

	for ( int i = 0; i < NumVertices+36; ++i ) {
		transformed_points[i] = transform * points[i];
	}

	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(transformed_points),
			transformed_points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(transformed_points),sizeof(colors),colors);
	//	glDrawArrays(GL_LINE_STRIP,0,3);

	
	glDrawArrays( GL_LINE_STRIP, 0 , 30);
	
	glDrawArrays( GL_TRIANGLES, 30, NumVertices+30);
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

	void
keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
		case 033: // Escape Key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break;
		case 'a':
			radius+=0.1;
			break;
		case 'z':
			radius-=0.1;
			break;
		case 's':
			Theta[Yaxis] += 0.01;
			break;
		case 'x':
			Theta[Yaxis] -= 0.01;
			break;
		case 'd':
			Theta[Xaxis] += 0.01;
			break;
		case 'c':
			Theta[Xaxis] -= 0.01;
			break;
		case 'f':
			at_x+=0.1;
			break;
		case 'v':
			at_x-=0.1;
			break;
		case 'g':
			test[4]+=0.01;
			break;
		case 'b':
			test[4]-=0.01;
			break;
		case 'h':
			test[5]+=0.01;
			break;
		case 'n':
			test[5]-=0.01;
			break;
	}
}

//----------------------------------------------------------------------------

	void
mouse( int button, int state, int x, int y )
{
	if ( state == GLUT_DOWN ) {
		switch( button ) {
			case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
			case GLUT_MIDDLE_BUTTON:  size+= 0.03;  break;
			case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
	}
}

//----------------------------------------------------------------------------

	void
idle( void )
{
	//Theta[Axis] += 0.01;

	if ( Theta[Axis] > 360.0 ) {
		Theta[Axis] -= 360.0;
	}

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

	int
main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 768, 768 );
	//    glutInitContextVersion( 3, 2 );
	//    glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "Color Cube" );

	glewInit();

	init();

	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutIdleFunc( idle );

	glutMainLoop();
	return 0;
}
