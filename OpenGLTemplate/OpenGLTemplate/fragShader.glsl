#version 430

in vec4 varyingColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

uniform float rcolor;
uniform float gcolor;
uniform float bcolor;

void main(void)
{
	gl_FragColor = varyingColor;
}
