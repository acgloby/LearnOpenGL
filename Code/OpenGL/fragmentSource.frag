#version 330 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
//	vec4 texColor1 = texture(texture1, TexCoord);
	vec4 texColor2 = texture(texture2, TexCoord);

	vec3 worldNormal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - WorldPos);
	vec3 viewDir = normalize(viewPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, worldNormal);
	
	//环境光
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

	//漫反射
	float diff = max(dot(worldNormal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//高光反射
	float specularStrength = 0.5;
	float specularShininess = 32;
	float spec = pow(max(dot(viewDir,reflectDir),0.0),specularShininess);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * texColor2.rgb;
	FragColor = vec4(result, 1.0);
}