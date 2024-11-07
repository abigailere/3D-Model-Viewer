#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 ourColor;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool z_buffer;
uniform bool z_prime_buffer;
uniform mat4 transform;
//uniform mat4 projection;

float near = 0.5;
float far = 5.0;
//learnopengl.com
float Depth(float depth){
	float z = depth * 2.0 - 1.0;
	float linearDepth = (2.0 * near * far) / (far + near - z * (far - near));
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	vec3 result;
	//Normal = mat3(transpose(inverse(transform))) * aNormal;
//ambient + diffuse + specular = phong
	//ambient
	float ambientConstant = 0.1;
	vec3 ambient = ambientConstant * lightColor;
	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - FragPos);
	float diffComp = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diffComp * lightColor;
	
	//specular
	float specIntensity = 5.0;
	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float specComp = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = specIntensity * specComp * lightColor;
	result = (ambient + diffuse + specular) * ourColor;
	FragColor = vec4(result, 1.0f);
	
	if (z_prime_buffer){
	float depth2 = Depth(gl_FragCoord.z) / far;
	FragColor = vec4(vec3(depth2), 1.0);
	}else if(z_buffer){
	FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
	}
};