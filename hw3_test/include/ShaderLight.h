#ifndef SHADERLIGHT_H
#define SHADERLIGHT_H


#include "Angel.h"
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;
inline
void shaderlight(GLuint& pro)
{
	point4 light_position( 0.0, 0.0, -1.0, 0.0 );
	color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
	color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
	color4 light_specular( 1.0, 1.0, 1.0, 1.0 );
	color4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
	color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
	color4 material_specular( 1.0, 0.8, 0.0, 1.0 );
	float material_shininess = 100.0;
	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv( glGetUniformLocation(pro, "AmbientProduct"), 1, ambient_product );
	glUniform4fv( glGetUniformLocation(pro, "DiffuseProduct"), 1, diffuse_product );
	glUniform4fv( glGetUniformLocation(pro, "SpecularProduct"), 1, specular_product );
	glUniform4fv( glGetUniformLocation(pro, "LightPosition"), 1, light_position );
	glUniform1f( glGetUniformLocation(pro, "Shininess"), material_shininess );
}





#endif
