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
	checkCompileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");

	// Compile whole shader
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	// Cleanup vertex and fragment shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::cout << "Shader loading took " << glfwGetTime() - start << " - " << vertexPath << " : " << fragmentPath << std::endl;
}

Shader::~Shader()
{

	std::cout << "Shader destructor called! ID: " << ID << std::endl;
	//glDeleteProgram(ID);
}

void Shader::BindShader()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}