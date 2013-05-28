#include <Shad\Shader.h>
#include <fstream>
#include <iostream>

#define ERROR_BUFSIZE 1024

std::map<std::string, Shader*> Shader::Shaders = std::map<std::string, Shader*>();

Shader::Shader() :
	_path(""),
	_errors(""),
	_vertexShaderID(0),
	_fragmentShaderID(0),
	_tessControlShaderID(0),
	_tessEvalShaderID(0),
	_geometryShaderID(0),
	_programID(0),
	_loaded(false) { }

Shader::Shader(const std::string& path) :
	_path(path),
	_errors(""),
	_vertexShaderID(0),
	_fragmentShaderID(0),
	_tessControlShaderID(0),
	_tessEvalShaderID(0),
	_geometryShaderID(0),
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

	// Check for tesselation and geometry shaders and compile
	bool hasTessC = false, hasTessE = false, hasGeom = false;
	GLint tesscCompileStatus = 1, tesseCompileStatus = 1, geomCompileStatus = 1;
	std::ifstream inTessC((path + ".tessc").c_str());
	if (!inTessC.fail())
	{
		hasTessC = true;
		std::vector<char> tessControlSource = readSource(path + ".tessc");
		source[0] = &tessControlSource.front();
		length = tessControlSource.size()-1;
		_tessControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(_tessControlShaderID, 1, source, &length);
		glCompileShader(_tessControlShaderID);
		glGetShaderiv(_tessControlShaderID, GL_COMPILE_STATUS, &tesscCompileStatus);
	}
	std::ifstream inTessE((path + ".tesse").c_str());
	if (!inTessE.fail())
	{
		hasTessE = true;
		std::vector<char> tessEvalSource = readSource(path + ".tesse");
		source[0] = &tessEvalSource.front();
		length = tessEvalSource.size()-1;
		_tessEvalShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(_tessEvalShaderID, 1, source, &length);
		glCompileShader(_tessEvalShaderID);
		glGetShaderiv(_tessEvalShaderID, GL_COMPILE_STATUS, &tesseCompileStatus);
	}
	std::ifstream inGeom((path + ".geom").c_str());
	if (!inGeom.fail())
	{
		hasGeom = true;
		std::vector<char> geometrySource = readSource(path + ".geom");
		source[0] = &geometrySource.front();
		length = geometrySource.size()-1;
		_geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(_geometryShaderID, 1, source, &length);
		glCompileShader(_geometryShaderID);
		glGetShaderiv(_geometryShaderID, GL_COMPILE_STATUS, &geomCompileStatus);
	}

	// Create the program
	_programID = glCreateProgram();
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);
	if (hasTessC)
		glAttachShader(_programID, _tessControlShaderID);
	if (hasTessE)
		glAttachShader(_programID, _tessEvalShaderID);
	if (hasGeom)
		glAttachShader(_programID, _geometryShaderID);
	glLinkProgram(_programID);
	
	// Error checking
	GLint linkStatus;
	glGetProgramiv(_programID, GL_LINK_STATUS, &linkStatus);
	//glGetShaderiv(vertexShaderID_, GL_COMPILE_STATUS, (GLint*)&loaded_);
	if (!linkStatus || !vertCompileStatus || !fragCompileStatus || !tesscCompileStatus || !tesseCompileStatus || !geomCompileStatus) {
		GLchar tempErrorLog[ERROR_BUFSIZE];
		GLsizei length;
		glGetShaderInfoLog(_fragmentShaderID, ERROR_BUFSIZE, &length, tempErrorLog);
		_errors += "Fragment shader errors:\n";
		_errors += std::string(tempErrorLog, length) + "\n";
		glGetShaderInfoLog(_vertexShaderID, ERROR_BUFSIZE, &length, tempErrorLog);
		_errors += "Vertex shader errors:\n";
		_errors += std::string(tempErrorLog, length) + "\n";
		if (hasTessC) {
			glGetProgramInfoLog(_tessControlShaderID, ERROR_BUFSIZE, &length, tempErrorLog);
			_errors += "Tesselation Control shader errors:\n";
			_errors += std::string(tempErrorLog, length) + "\n";
		}
		if (hasTessE) {
			glGetProgramInfoLog(_tessEvalShaderID, ERROR_BUFSIZE, &length, tempErrorLog);
			_errors += "Tesselation Evaluation shader errors:\n";
			_errors += std::string(tempErrorLog, length) + "\n";
		}
		if (hasGeom) {
			glGetProgramInfoLog(_geometryShaderID, ERROR_BUFSIZE, &length, tempErrorLog);
			_errors += "Geometry shader errors:\n";
			_errors += std::string(tempErrorLog, length) + "\n";
		}
		glGetProgramInfoLog(_programID, ERROR_BUFSIZE, &length, tempErrorLog);
		_errors += "Linker errors:\n";
		_errors += std::string(tempErrorLog, length) + "\n";
		_loaded = false;
	} else {
		_loaded = true;
	}
	Shaders.insert(std::pair<std::string, Shader*>(path, this));
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
