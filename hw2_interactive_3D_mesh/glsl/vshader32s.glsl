#version 150

in  vec4 vPosition;
in  vec4 vColor;
in  vec3 vNormal;
out vec4 color;
uniform mat4 model_view;
uniform mat4 projection;

uniform vec4 LightPosition;
uniform float Shininess;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;

void main() 
{
	vec3 pos = (model_view*vPosition).xyz;
	vec3 L = normalize(( model_view*vec4(0.0,0.0,-1.0,1)).xyz - pos );
	vec3 E = normalize( -pos );
	vec3 H = normalize( L + E );


	// Transform vertex normal into eye coordinates
	vec4 nn=  model_view*vec4(vNormal,1.0);
	vec3 N = normalize( nn.xyz/nn[3]);

	float Kd = max( dot(L, N), 0.0 );
	
	vec4 diffuse = Kd*DiffuseProduct;
	
	float Ks = pow( max(dot(N, H), 0.0), Shininess );
	
	vec4 specular = Ks * SpecularProduct;
	
	if( dot(L, N) < 0.0 ) specular = vec4(0.0, 0.0, 0.0, 1.0);


	gl_Position = projection*model_view*vPosition;
	color = AmbientProduct + diffuse + specular;

	color.a = 1.0;




} 
