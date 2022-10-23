#version 430

layout (location=0) in vec3 position;
in vec4 vColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

uniform float rcolor;
uniform float gcolor;
uniform float bcolor;

out vec4 varyingColor;

void main(void)
{	
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	varyingColor = vec4(rcolor, gcolor, bcolor, 1.0f);
} 
