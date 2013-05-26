#include <Shad\Shader.h>
#include <fstream>
#include <iostream>

#define ERROR_BUFSIZE 1024

std::map<std::string, GLuint> Shader::Shaders = std::map<std::string, GLuint>();

Shader::Shader() :
	_path(""),
	_vertexShaderID(0),
	_fragmentShaderID(0),
	_programID(0),
	_loaded(false) { }

Shader::Shader(const std::string& path) :
	_path(path),
	_vertexShaderID(0),
	_fragmentShaderID(0),
	_programID(0),
	_loaded(false) {

	const GLchar* source[1];
    GLint length;

	// Load the vertex shader and compile
	std::vector<char> vertexSource = readSource(path + ".vert");
	source[0] = &vertexSource.front();
    length = vertexSource.size()-1;
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertexShaderID, 1, source, &length);
    glCompileShader(_vertexShaderID);
	GLint vertCompileStatus;
	glGetShaderiv(_vertexShaderID, GL_COMPILE_STATUS, &vertCompileStatus);
	
	// Load the fragment shader and compile
	std::vector<char> fragmentSource = readSource(path + ".frag");
	source[0] = &fragmentSource.front();
    length = fragmentSource.size()-1;
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragmentShaderID, 1, source, &length);
    glCompileShader(_fragmentShaderID);
	GLint fragCompileStatus;
	glGetShaderiv(_fragmentShaderID, GL_COMPILE_STATUS, &fragCompileStatus);
	
	// Create the vertex program
	_programID = glCreateProgram();
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);
	glLinkProgram(_programID);
	
	// Error checking
	GLint linkStatus;
	glGetProgramiv(_programID, GL_LINK_STATUS, &linkStatus);
    //glGetShaderiv(vertexShaderID_, GL_COMPILE_STATUS, (GLint*)&loaded_);
	if (!linkStatus || !vertCompileStatus || !fragCompileStatus) {
        GLchar tempErrorLog[ERROR_BUFSIZE];
		GLsizei length;
		glGetShaderInfoLog(_fragmentShaderID, ERROR_BUFSIZE, &length, tempErrorLog);
        _errors += "Fragment shader errors:\n";
		_errors += std::string(tempErrorLog, length) + "\n";
		glGetShaderInfoLog(_vertexShaderID, ERROR_BUFSIZE, &length, tempErrorLog);
        _errors += "Vertex shader errors:\n";
		_errors += std::string(tempErrorLog, length) + "\n";
		glGetProgramInfoLog(_programID, ERROR_BUFSIZE, &length, tempErrorLog);
        _errors += "Linker errors:\n";
		_errors += std::string(tempErrorLog, length) + "\n";
		_loaded = false;
    } else {
		Shaders.insert(std::pair<std::string, GLuint>(path, _programID));
		_loaded = true;
	}
}

Shader::~Shader() {
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
	glDeleteProgram(_programID);
}

std::vector<char> Shader::readSource(const std::string& path) {

	// Open the file
	std::vector<char> source;
	std::ifstream in(path.c_str());
	if (in.fail()) {
		source.push_back(0);
		return source;
	}
	
	// Seek to the end of the file to get the size
	in.seekg(0, std::ios::end);
	source.reserve((unsigned)(1 + in.tellg()));
	source.resize((unsigned)in.tellg());
	in.seekg(0, std::ios::beg);
	if (source.empty()) {
		source.push_back(0);
		return source;
	}
	
	// Now read the whole buffer in one call, and don't 
	// forget to null-terminate the vector with a zero
	in.read(&source.front(), source.size());
	source.push_back(0);
	
	return source;
}

const std::string& Shader::path() const {
	return _path;
}

GLuint Shader::programID() const {
	return _programID;
}

const std::string& Shader::errors() const {
	return _errors;
}

bool Shader::loaded() const {
	return _loaded;
}
