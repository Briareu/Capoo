#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location=2) in vec2 texCoord;
out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec2 tc;

struct PositionalLight
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material0
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	float shininess;
};

//������ͼ
struct Material1
{	sampler2D diffuse;  
	vec3 specular;  
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material0 material0;
uniform Material1 material1;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform mat4 view_matrix;

uniform int flag;  //�ж��Ƿ�ʹ������
//layout (binding=0) uniform sampler2D samp;

void main(void)
{	varyingVertPos = (mv_matrix * vec4(vertPos,1.0)).xyz;
	varyingLightDir = light.position - varyingVertPos;
	varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz;

	gl_Position = proj_matrix * view_matrix*mv_matrix * vec4(vertPos,1.0);
	tc = texCoord;
}