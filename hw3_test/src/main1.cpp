
//CS 537
//A stub illustarting scene graph creation and a flying camera used to fly around the scene
//The cammera is controlled trhough the keyboard 
// z/Z = roll the camera -- implemented
// c/C yaw the camera  -- you must implement
// x/X pitch the camera -- you must implement
//Sliding the camera 
// up   Special Key = slide forward -- implemented
// down Special Key = slide backward -- you must implement
// The secene 
//Consist of two colored cylinders based on a standard cylinder
// Two different methods and shaders are used to render the cylinders. Note how I stuffed the render methods in the SG nodes. 
// Generate the attributes for a cyllinder mesh of radius 1 and heigth 1
//      Cyl = (x, y, z) s.t. x^2 + y^2 = 1, 0 =< z =< 1
// 
// Mesh descrption: 2*50 side triangulated polygons +  top and bottom polys  (each is a regular 50-gon, rpresented as a fan of 50 trianges)
// Total NumberTriangles = 2*50 + 50 + 50 = 200
// Attributes stored in VBOs
//  Vertex 3D postions: vec4 cylinderData[3*NumberTrinagles]
//  Vertex RGBA colors: vec4 cylinderColor[3*NumberTrinagles]
//
//
//G. Kamberov

#include "../include/Angel.h"
#include <assert.h>



void m_glewInitAndVersion(void);
void reshape(int width, int height);
void specKey(int key, int x, int y);
void keyboard( unsigned char key, int x, int y );
void drawCylinderColored(void);
void drawCylinderMonochrome(void);



//  Define PI in the case it's not defined in the math header file
#ifndef PI
#  define PI  3.14159265358979323846
#endif

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

color4 red=color4(1.0, 0.0, 0.0, 1.0);

#define X               0
#define Y               1
#define Z               2
#define SCALE_VECTOR    1.0
#define SCALE_ANGLE     1.0

/////Camera unv basis///////


Angel::vec4 v = vec4(0.0, 1.0, 0.0, 00);
Angel::vec4 u = vec4(1.0, 0.0, 0.0, 0.0);
Angel::vec4 eye = vec4(0.0, 1.0, 2.0, 1.0);
Angel::vec4 n = Angel::normalize(vec4(0.0, 1.0, 2.0, 0.0));


GLuint buffers[2];
// Create buffer objects

// Create a vertex array object
GLuint vao;


GLuint program[2];

GLuint vPosition;
GLuint vColor;


//------------------------------------
//uniform variables locations

GLuint color_loc;
GLuint proj_loc;
GLuint model_view_loc;

//------------------------------------


int CylNumVertices = 600;

point4 cylinderData[600];
color4 cylinderColor[600];


class MatrixStack {
	int    _index;
	int    _size;
	mat4*  _matrices;

	public:
	MatrixStack( int numMatrices = 32 ):_index(0), _size(numMatrices)
	{ _matrices = new mat4[numMatrices]; }

	~MatrixStack()
	{ delete[]_matrices; }

	void push( const mat4& m ) {
		assert( _index + 1 < _size );
		_matrices[_index++] = m;
	}

	mat4& pop( void ) {
		assert( _index - 1 >= 0 );
		_index--;
		return _matrices[_index];
	}
};


MatrixStack  mvstack;
mat4         model_view=mat4(1.0);
mat4         projmat=mat4(1.0);  

//--------------------------------------

enum {
	baseCyll = 0,
	TranslrotRyStretcedCyll  = 1,
	NumNodes,
	Quit
};

//-------SG DS-------------------------------------------------------------

struct Node {
	mat4  transform;
	void  (*render)( void );
	Node* sibling;
	Node* child;

	Node() :
		render(NULL), sibling(NULL), child(NULL) {}

	Node( mat4& m, void (*render)( void ), Node* sibling, Node* child ) :
		transform(m), render(render), sibling(sibling), child(child) {}
};

Node  nodes[NumNodes];


//----------------------------------------------------------------------------


//----------------------------------------------------------------------------

void traverse( Node* node )
{
	if ( node == NULL ) { return; }

	mvstack.push( model_view );

	model_view *= node->transform;
	node->render();

	if ( node->child ) { traverse( node->child ); }

	model_view = mvstack.pop();

	if ( node->sibling ) { traverse( node->sibling ); }
}

//----------------------------------------------------------------------------


void buildCylinder( ){
	GLfloat x,z,theta;
	int iMax=50, vertexCounter=0;
	//build yellow cylinder top
	for(int i=0;i<iMax;i++){
		theta= (2*PI/(float)iMax)*i; x=cos(theta); z=sin(theta);
		cylinderData[vertexCounter]=vec4(0,1,0,1.0); cylinderColor[vertexCounter]=vec4(2.0,1.0,0.0, 1.0);
		vertexCounter++;
		cylinderData[vertexCounter]=vec4(x,1,z,1.0); cylinderColor[vertexCounter]=vec4(2.0,1.0,0.0, 1.0);
		vertexCounter++;	

		theta= (2*PI/(float)iMax)*(i+1.0); x=cos(theta); z=sin(theta);
		cylinderData[vertexCounter]=vec4(x,1,z,1.0); cylinderColor[vertexCounter]=vec4(1.0,1.0,0.0, 1.0);
		vertexCounter++;
	}
	//build maroon cylinder bottom
	for(int i=0;i<iMax;i++){
		theta= (2*PI/(float)iMax)*i; x=cos(theta); z=sin(theta);
		cylinderData[vertexCounter]=vec4(0,0,0,1.0); cylinderColor[vertexCounter]=vec4(0.5,0.0,0.5, 1.0);
		vertexCounter++;
		cylinderData[vertexCounter]=vec4(x,0,z,1.0); cylinderColor[vertexCounter]=vec4(0.5,0.0,0.5, 1.0);
		vertexCounter++;	
		theta= (2*PI/(float)iMax)*(i+1.0); x=cos(theta); z=sin(theta);
		cylinderData[vertexCounter]=vec4(x,0,z,1.0); cylinderColor[vertexCounter]=vec4(0.5,0.0,0.5, 1.0);
		vertexCounter++;
	}

	//build green cylinder sides

	for(int i=0;i<iMax;i++){
		theta= (2*PI/(float)iMax)*i; x=cos(theta); z=sin(theta);
		cylinderData[vertexCounter]=vec4(x,1,z,1.0); cylinderColor[vertexCounter]=vec4(0.0,1.0,0.0, 0.5);
		vertexCounter++;
		cylinderData[vertexCounter]=vec4(x,0,z,1.0); cylinderColor[vertexCounter]=vec4(0.0,1.0,0.0, 0.5);
		vertexCounter++;

		theta= (2*PI/(float)iMax)*(i+1.0); x=cos(theta); z=sin(theta);
		cylinderData[vertexCounter]=vec4(x,1,z,1.0); cylinderColor[vertexCounter]=vec4(0.0,1.0,0.0, 0.5);
		vertexCounter++;
		cylinderData[vertexCounter]=vec4(x,1,z,1.0); cylinderColor[vertexCounter]=vec4(0.0,1.0,0.0, 0.5);
		vertexCounter++;

		theta= (2*PI/(float)iMax)*i; x=cos(theta); z=sin(theta);
		cylinderData[vertexCounter]=vec4(x,0,z,1.0); cylinderColor[vertexCounter]=vec4(0.0,1.0,0.0, 0.5);
		vertexCounter++;

		theta= (2*PI/(float)iMax)*(i+1.0); x=cos(theta); z=sin(theta);
		cylinderData[vertexCounter]=vec4(x,0,z,1.0); cylinderColor[vertexCounter]=vec4(0.0,1.0,0.0, 0.5);
		vertexCounter++;

	}
}




void initNodes( void )
{
	mat4  m;

	m = mat4(1.0);
	nodes[baseCyll] = Node( m, drawCylinderColored, NULL, &nodes[TranslrotRyStretcedCyll] );


	m = Translate(-1.0, 0.0, 1.0)*RotateZ(45.0)*Scale(0.5, 0.5, 0.5);
	nodes[TranslrotRyStretcedCyll] = Node( m, drawCylinderMonochrome, NULL, NULL);


}



void init(){

	buildCylinder( );


	glGenBuffers(2,&buffers[0]); 


	glGenVertexArrays( 1, &vao );


	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(cylinderData),  cylinderData, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, buffers[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(cylinderColor),  cylinderColor, GL_STATIC_DRAW );

	// Load shaders and use the resulting shader programs
	program[baseCyll] = InitShader( "./glsl/vshaderPerVertColor.glsl", "./glsl/fshaderStock.glsl" );
	program[TranslrotRyStretcedCyll] = InitShader( "./glsl/vshaderUnifColor.glsl", "./glsl/fshaderStock.glsl" );


	glBindVertexArray( vao );

	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	vPosition = glGetAttribLocation( program[baseCyll], "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	glBindBuffer( GL_ARRAY_BUFFER, buffers[1] );
	vColor = glGetAttribLocation( program[baseCyll], "vColor" );
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	glBindVertexArray(0);

	// Initialize tree
	initNodes();

	glClearColor( 1.0, 1.0, 1.0, 0.0 ); 
	glClearDepth( 1.0 ); 
	glEnable( GL_DEPTH_TEST );
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT, GL_FILL);

}

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	projmat = Perspective(90,1.0,1.0, 5.0); 

	model_view = LookAt(eye, eye-n, v); 


	traverse( &nodes[baseCyll]);

	glutSwapBuffers();
}


void idle(void)
{
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

	int
main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( 512, 512 );
	glutCreateWindow( "Scene Graph and a Flying Camera Stub" );



	glewInit();
	init();
	
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( specKey );
	glutIdleFunc(idle);



	glutMainLoop();
	return 0;
}

void keyboard( unsigned char key, int x, int y )
{


	GLfloat xt, yt, zt;
	GLfloat cosine, sine;

	// positive or negative rotation depending on upper or lower case letter
	if(key > 96)  
	{
		cosine = cos(SCALE_ANGLE * PI/-180.0);
		sine = sin(SCALE_ANGLE * PI/-180.0);
	}
	else
	{
		cosine = cos(SCALE_ANGLE * PI/180.0);
		sine = sin(SCALE_ANGLE * PI/180.0);
	}

	switch(key)
	{

		case 'Z': // roll counterclockwise in the xy plane
		case 'z': // roll clockwise in the xy plane
			xt = u[X];
			yt = u[Y];
			zt = u[Z];
			u[X] = xt*cosine - v[X]*sine;
			u[Y] = yt*cosine - v[Y]*sine;
			u[Z] = zt*cosine - v[Z]*sine;
			v[X] = xt*sine + v[X]*cosine;
			v[Y] = yt*sine + v[Y]*cosine;
			v[Z] = zt*sine + v[Z]*cosine;
			break;
			/****** You must write the rest of teh code yourself
			  case 'X': // pitch up
			  case 'x': // pitch down
			// complete
			 ****/
		case 033:  // Escape key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
	}

	glutPostRedisplay();
}

void specKey(int key, int x, int y)
{ 
	switch( key ) {
		case GLUT_KEY_UP: // MOVE FORWARD
			eye[0] -= SCALE_VECTOR * n[0];
			eye[1] -= SCALE_VECTOR * n[1];
			eye[2] -= SCALE_VECTOR * n[2];
			break;
			/****** You must write the rest of teh code yourself
			  case GLUT_KEY_DOWN: // MOVE BACKWARD
			//complete....
			 *********/
		default:
			break;
	}
	glutPostRedisplay();
}


void reshape( int width, int height )
	//the same objects are shown (possibly scaled) w/o shape distortion 
	//original viewport is a square
{

	glViewport( 0, 0, (GLsizei) width, (GLsizei) height );

}

void drawCylinderColored(){

	glUseProgram(program[baseCyll]);

	proj_loc       = glGetUniformLocation(program[baseCyll], "Projection");
	model_view_loc = glGetUniformLocation(program[baseCyll], "ModelView");
	glUniformMatrix4fv(proj_loc, 1, GL_TRUE, projmat);
	glUniformMatrix4fv( model_view_loc, 1, GL_TRUE, model_view);

	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLES, 0, CylNumVertices );

	glUseProgram(0);
	glBindVertexArray(0);

}

void drawCylinderMonochrome(){

	glUseProgram(program[TranslrotRyStretcedCyll]);

	proj_loc       = glGetUniformLocation(program[TranslrotRyStretcedCyll], "Projection");
	model_view_loc = glGetUniformLocation(program[TranslrotRyStretcedCyll], "ModelView");
	glUniformMatrix4fv(proj_loc, 1, GL_TRUE, projmat);
	glUniformMatrix4fv( model_view_loc, 1, GL_TRUE, model_view);
	color_loc = glGetUniformLocation(program[TranslrotRyStretcedCyll], "color");
	glUniform4fv(color_loc, 1, red);

	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] ); 
	vPosition = glGetAttribLocation( program[TranslrotRyStretcedCyll], "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glDrawArrays( GL_TRIANGLES, 0, CylNumVertices );

	glUseProgram(0);

}
