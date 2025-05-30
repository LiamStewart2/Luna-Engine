#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// Load shader code into strings ready for compilation
	std::string vertexCodeString, fragmentCodeString;
	std::ifstream vertexFile, fragmentFile;

	vertexFile.open(vertexPath); fragmentFile.open(fragmentPath);

	if (!vertexFile)
		std::cerr << "Cannot open file - " << vertexPath << std::endl;
	if(!fragmentFile)
		std::cerr << "Cannot open file - " << vertexPath << std::endl;

	std::stringstream vertexStream, fragmentStream;
	vertexStream << vertexFile.rdbuf(); fragmentStream << fragmentFile.rdbuf();
	
	vertexFile.close(); fragmentFile.close();

	vertexCodeString = vertexStream.str(); fragmentCodeString = fragmentStream.str();
	const char* vertexCode = vertexCodeString.c_str(); const char* fragmentCode = fragmentCodeString.c_str();

	// Compile shader code and return any errors
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);

	int compiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != true)
	{
		int log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(vertexShader, 1024, &log_length, message);

		std::cerr << "SHADER COMPILATION FAILED - VERTEX SHADER - " << vertexPath << std::endl << message << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);

	compiled = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != true)
	{
		int log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(fragmentShader, 1024, &log_length, message);

		std::cerr << "SHADER COMPILATION FAILED - FRAGMENT SHADER - " << vertexPath << std::endl << message << std::endl;
	}
}

Shader::~Shader()
{
}

void Shader::BindTexture()
{
}

void Shader::SetBool(const std::string name, bool value)
{
}

void Shader::SetInt(const std::string name, int value)
{
}

void Shader::SetFloat(const std::string name, float value)
{
}
