#version 330 core
struct Material
{
	sampler2D diffuse;	//漫反射贴图
    sampler2D specular; //高光贴图
	float shininess;	//高光范围
};


struct DirLight
{
	vec3 color;
	vec3 direction;		//光照方向
	
	vec3 ambient;		//环境光
	vec3 diffuse;		//漫反射
	vec3 specular;		//高光反射
};

struct PointLight
{
	vec3 color;
	vec3 position;		//光源位置
	float constant;		//衰减常数
	float linear;		//衰减一次常数
	float quadratic;	//衰减二次常数

	vec3 ambient;		//环境光
	vec3 diffuse;		//漫反射
	vec3 specular;		//高光反射
};

struct SpotLight
{
	vec3 color;
	vec3 position;		//光源位置
	vec3 direction;		//光照方向
	float cutOff;		//内切光角度
	float outerCutOff;	//外切光角度

	vec3 ambient;		//环境光
	vec3 diffuse;		//漫反射
	vec3 specular;		//高光反射
};

#define NR_POINT_LIGHTS 1

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];;
uniform SpotLight spotLight;

out vec4 FragColor;
in vec4 vertexColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light,vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 worldNormal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - WorldPos);
	vec3 result = CalcDirLight(dirLight, worldNormal, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], worldNormal, viewDir);
	}
	result += CalcSpotLight(spotLight, worldNormal, viewDir);

	FragColor = vec4(result, 1.0);
}

//计算平行光
vec3 CalcDirLight(DirLight light,vec3 normal, vec3 viewDir)
{
	vec4 diffuseTex = texture(material.diffuse, TexCoords);
	vec4 specularTex = texture(material.specular, TexCoords);

	vec3 lightDir = normalize(-light.direction); //光照计算需求一个从片段至光源的光线方向,所以取反
	vec3 reflectDir = reflect(-lightDir, normal);
	
	//环境光
	vec3 ambient = light.ambient * vec3(diffuseTex) * light.color;

	//漫反射
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(diffuseTex)* light.color;

	//高光反射
	float specularShininess = 32;
	float spec = pow(max(dot(viewDir,reflectDir),0.0),specularShininess);
	vec3 specular = light.specular * spec * vec3(specularTex)* light.color;

	return ambient + diffuse + specular;
}

//计算点光
vec3 CalcPointLight(PointLight light,vec3 normal, vec3 viewDir)
{
	float distance = length(light.position - WorldPos);
	//衰减值
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec4 diffuseTex = texture(material.diffuse, TexCoords);
	vec4 specularTex = texture(material.specular, TexCoords);
	vec3 lightDir = normalize(light.position - WorldPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	//环境光
	vec3 ambient = light.ambient * vec3(diffuseTex)* light.color;

	//漫反射
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(diffuseTex)* light.color;

	//高光反射
	float specularShininess = 32;
	float spec = pow(max(dot(viewDir,reflectDir),0.0),specularShininess);
	vec3 specular = light.specular * spec * vec3(specularTex)* light.color;

	//点光源衰减值
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular; 
}

//计算聚光
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	vec4 diffuseTex = texture(material.diffuse, TexCoords);
	vec4 specularTex = texture(material.specular, TexCoords);
	vec3 lightDir = normalize(light.position - WorldPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	//聚光强度
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
	
	//环境光
	vec3 ambient = light.ambient * vec3(diffuseTex)* light.color;

	//漫反射
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(diffuseTex)* light.color;

	//高光反射
	float specularShininess = 32;
	float spec = pow(max(dot(viewDir,reflectDir),0.0),specularShininess);
	vec3 specular = light.specular * spec * vec3(specularTex) * light.color;

	//聚光灯软化边缘
	diffuse  *= intensity;
	specular *= intensity;
	ambient *= intensity;

	return ambient + diffuse + specular; 
}