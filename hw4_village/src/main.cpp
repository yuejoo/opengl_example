
#include "../include/Angel.h"
#include <assert.h>
#include "../include/mesh.h"
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

const int NumVertices = 9; //(6 faces)(2 triangles/face)(3 vertices/triangle)
point4 points[NumVertices];
int index = 0;

//Stack----------------------------------------------------------------------------

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

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

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

//----------------------------------------------------------------------------
//Draw method()---------------------------------------------------------------

MatrixStack mvstack;
mat4 model_view=mat4(1.0f);
GLuint projection_loc, color_loc, model_view_loc;
//----------------------------------------------------------------------------

void rectangleY()
{
	glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
	glUniform4fv(color_loc,1,vertex_colors[2]);
	glDrawArrays( GL_TRIANGLES, 0 , 6 );
}

void rectangleR()
{
	glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
	glUniform4fv(color_loc,1,vertex_colors[1]);
	glDrawArrays( GL_TRIANGLES, 0 , 6 );
}

void rectangleM()
{
	glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
	glUniform4fv(color_loc,1,vertex_colors[0]);
	glDrawArrays( GL_TRIANGLES, 0 , 6 );
}

void triangle()
{
	glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
	glUniform4fv(color_loc,1,vertex_colors[1]);
	glDrawArrays( GL_TRIANGLES, 6 , 9 );
}
//----------------------------------------------------------------------------
//Node init()------------------------------------------------------------------
//----------------------------------------------------------------------------

void blank()
{};

Node* inithouseNode()
{
	Node *head = new Node[11];
	mat4 m(1.0f);

	head[0] = Node(m, blank,NULL,&head[1]);
	m = Translate(1.5, -0.5, 0.0)*RotateY(-90.0);

	head[1] = Node( m, rectangleY, &head[2], NULL);
	m = Translate( 0.0, -0.5, 0.0)*RotateY(-90.0);

	head[2] = Node( m, rectangleY, &head[3], NULL);
	m = Translate(0.0, -0.5, 0.0)*RotateX(90.0)*Scale(1.5,1,1);

	head[3] = Node( m, rectangleY, &head[4], NULL);
	m = Translate(0.0, 0.5, 0.0)*RotateX(90.0)*Scale(1.5,1,1);

	head[4] = Node( m, rectangleY, &head[5], NULL);
	m = Translate(0.0, 0.0, 0.0);

	head[5] = Node( m, blank, NULL, &head[6]);
	m = Translate( 0.0, 0.0, 1.0)*RotateY(-90.0);

	head[6] = Node( m, triangle, &head[7], NULL);
	m = Translate( 1.5, 0.0, 1.0)*RotateY(-90.0);

	head[7] = Node( m, triangle, &head[8], NULL);
	m = Translate(0.0, -0.5, 1.0)*RotateX(45)*Scale(1.5, sqrt(0.5), 1.0);

	head[8] = Node( m, rectangleM, &head[9], NULL);
	m = Translate(0.0, 0.5, 1.0)*RotateX(135)*Scale(1.5, sqrt(0.5), 1.0);

	head[9] = Node( m, rectangleM, NULL, NULL);

	return &head[0];
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void traverse(Node *node)
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
//----------------------------------------------------------------------------

void initbase()
{
	points[index] = point4(0.0f,0.0f,0.0f,1.0f); ++index;
	points[index] = point4(1.0f,0.0f,0.0f,1.0f); ++index;
	points[index] = point4(1.0f,1.0f,0.0f,1.0f); ++index;
	points[index] = point4(0.0f,0.0f,0.0f,1.0f); ++index;
	points[index] = point4(0.0f,1.0f,0.0f,1.0f); ++index;
	points[index] = point4(1.0f,1.0f,0.0f,1.0f); ++index;
	points[index] = point4(0.0f,-0.5f,0.0f,1.0f); ++index;
	points[index] = point4(0.0f,0.5f,0.0f,1.0f); ++index;
	points[index] = point4(0.5f,0.0f,0.0f,1.0f); ++index;
}

GLuint buffer;
Node  *head;
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void init( void )
{


	initbase();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// Create and initialize a buffer object
	glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

	// Load shaders and use the resulting shader program
	GLuint program = InitShader( "./glsl/vshader83.glsl", "./glsl/fshader83.glsl" );
	glUseProgram( program );

	GLuint vPosition;
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0 ,BUFFER_OFFSET(0));

	//----------------------------------------------------------------------------
	model_view_loc = glGetUniformLocation( program, "modelview");
	color_loc = glGetUniformLocation(program, "vColor");
	projection_loc = glGetUniformLocation(program, "projection");
	point4  eye( 4.0f, 0.0f, 4.0f, 1.0f );
	point4  at( 0.0f,  0.0f, 0.0f, 1.0f );
	vec4    up( 0.0f, 0.0f, 1.0f, 0.0f );
	mat4  mc = Frustum(-0.1024f, 0.1024f, -0.1024f, 0.1024f, 0.1f, 50.0f);
	mat4  mv = LookAt( eye, at, up );

	//----------------------------------------------------------------------------
	glUniformMatrix4fv(projection_loc,1,GL_TRUE,mc*mv);
	head = inithouseNode();

	glEnable( GL_DEPTH_TEST );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
		case 033: // Escape Key
		case 'q': case 'Q':
			delete [] head;
			exit( EXIT_SUCCESS );
			break;
	}
}

void idle(void)
{
	glutPostRedisplay();
}

void display(void)
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	
	for(int j=-50; j<=50 ; j++)	
	for(int i=-50; i<=50 ; i++)
	{
		model_view = mat4(1.0f) * Translate(j*2.5,1.7*i,0);
		traverse(head);
	}
	model_view = mat4(1.0f);	
	glutSwapBuffers();
}


//----------------------------------------------------------------------------
int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( 512, 512 );
	glutCreateWindow( "robot" );

	glewInit();

	
	init();
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutIdleFunc(idle);


	glutMainLoop();
	return 0;
}
