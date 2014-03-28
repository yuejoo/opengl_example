#version 150 

in  vec4 vPosition;
out vec4 color;
attribute vec4 vNormal;

uniform vec4 vColor;
uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 camview;

uniform vec4 campos;
uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 specular;

void main() 
{
    vec4 vpos = modelview*vPosition;
    vec4 vnor = modelview*vNormal;
    vec3 E = normalize((campos - vpos).xyz);
    vec3 N = normalize(vnor.xyz);
    vec3 L = normalize(vec3(20,0,20)-vpos.xyz);

    vec3 H = normalize(L+E);
    
    if(dot(N,L) < 0)
        H = vec3(0,0,0);
    
  
    vec4 tc = ambient  + pow(max(dot(N,H),0),5.1)*specular + max(0,dot(N,L)) * diffuse;
    
    
    color = vec4(tc.xyz,1.0f);
    //color = vPosition;
    gl_Position =projection * camview * modelview * vPosition;
} 
