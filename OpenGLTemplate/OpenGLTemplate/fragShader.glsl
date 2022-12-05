#version 430

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec2 tc;

out vec4 fragColor;

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

//纹理贴图
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
uniform mat4 view_matrix;

uniform mat4 norm_matrix;

uniform int flag;  //判断是否使用纹理
//layout (binding=0) uniform sampler2D samp;

void main(void)
{	// normalize the light, normal, and view vectors:
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);
	
	// compute light reflection vector, with respect N:
	vec3 R = normalize(reflect(-L, N));
	
	// get the angle between the light and surface normal:
	float cosTheta = dot(L,N);
	
	// angle between the view vector and reflected light:
	float cosPhi = dot(V,R);
	
	if(flag==0){
	   vec3 ambient = ((globalAmbient * material0.ambient) + (light.ambient * material0.ambient)).xyz;
       vec3 diffuse = light.diffuse.xyz * material0.diffuse.xyz * max(cosTheta,0.0);
	   vec3 specular = light.specular.xyz * material0.specular.xyz * pow(max(cosPhi,0.0), material0.shininess);
	   fragColor = vec4((ambient + diffuse + specular), 1.0);
	}
	else{
	   /*vec3 diffuse = light.diffuse.xyz * max(cosTheta,0.0) * vec3(texture(material1.diffuse, tc));
	   vec3 ambient = (globalAmbient * vec4(texture(material1.diffuse, tc)) +light.ambient * vec4(texture(material1.diffuse, tc))).xyz;
	   vec3 specular = light.specular.xyz * material1.specular.xyz * pow(max(cosPhi,0.0), material1.shininess);
	   fragColor = vec4((ambient + diffuse + specular), 1.0);*/
	   
	   vec3 ambient = ((globalAmbient * material0.ambient) + (light.ambient * material0.ambient)).xyz;
       vec3 diffuse = light.diffuse.xyz * material0.diffuse.xyz * max(cosTheta,0.0);
	   vec3 specular = light.specular.xyz * material0.specular.xyz * pow(max(cosPhi,0.0), material0.shininess);
	   vec4 lightColor = vec4((ambient + diffuse + specular), 1.0);
	   fragColor = 0.5 * lightColor + 0.5 * vec4(vec3(texture(material1.diffuse, tc)), 1.0);
	}
}