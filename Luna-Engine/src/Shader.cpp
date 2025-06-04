#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	double start = glfwGetTime();

	// Load shader code into strings ready for compilation
	std::string vertexCodeString, fragmentCodeString;
	std::ifstream vertexFile, fragmentFile;

	vertexFile.open(vertexPath); fragmentFile.open(fragmentPath);

	if (!vertexFile)
		std::cerr << "Cannot open file - " << vertexPath << std::endl;
	if(!fragmentFile)
		std::cerr << "Cannot open file - " << fragmentPath << std::endl;

	std::stringstream vertexStream, fragmentStream;
	vertexStream << vertexFile.rdbuf(); fragmentStream << fragmentFile.rdbuf();
	
	vertexFile.close(); fragmentFile.close();

	vertexCodeString = vertexStream.str(); fragmentCodeString = fragmentStream.str();
	const char* vertexCode = vertexCodeString.c_str(); const char* fragmentCode = fragmentCodeString.c_str();

	// Compile shader code and return any errors
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);

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
	glCompileShader(fragmentShader);

	compiled = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != true)
	{
		int log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(fragmentShader, 1024, &log_length, message);

		std::cerr << "SHADER COMPILATION FAILED - FRAGMENT SHADER - " << fragmentPath << std::endl << message << std::endl;
	}

	// Compile whole shader
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	int linked = 0;
	glGetProgramiv(ID, GL_LINK_STATUS, &linked);
	if (linked != true)
	{
		int log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(ID, 1024, &log_length, message);

		std::cerr << "SHADER LINKING FAILED - Vertex Path - " << vertexPath << ", Fragment Path - " << fragmentPath << std::endl << message << std::endl;
	}

	// Cleanup vertex and fragment shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::cout << "Shader loading took " << glfwGetTime() - start << " - " << vertexPath << " : " << fragmentPath << std::endl;
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::BindShader()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
