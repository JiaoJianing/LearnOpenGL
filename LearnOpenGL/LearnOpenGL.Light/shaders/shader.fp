#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outterCutOff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

void main()
{
	//判断当前片元是否在聚光灯范围内
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outterCutOff;
	float intensity = clamp((theta - light.outterCutOff)/epsilon, 0.0f, 1.0f);

	//执行光照计算
	//环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//漫反射光
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//镜面高光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	//vec3 emission = vec3(texture(material.emission, TexCoords));

	diffuse *= intensity;
	specular *= intensity;

	color = vec4(ambient + diffuse + specular, 1.0f);
}