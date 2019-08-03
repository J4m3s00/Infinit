#include "inpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include "Core/Log.h"

#include <Util/StringUtil.h>

namespace Infinit {

	OpenGLShader::OpenGLShader(const string& path)
		: Shader(path), m_RendererID(0)
	{
		CompileShader();
	}

	OpenGLShader::~OpenGLShader()
	{
		IN_CORE_INFO("OpenGL Shader {0} destroyed!", m_RendererID);
		glDeleteProgram(m_RendererID);
	}

	bool OpenGLShader::Reload(const string& filepath)
	{
		if (filepath != "") m_FilePath = filepath;
		if (m_RendererID) glDeleteProgram(m_RendererID);
		CompileShader();
		return true;
	}

	void OpenGLShader::CompileShader()
	{
		LoadShaderFromFile(m_FilePath);
		string* shaderSources = new string[2];

		const char* typeToken = "#shader";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = m_ShaderSource.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = m_ShaderSource.find_first_of("\r\n", pos);
			IN_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = m_ShaderSource.substr(begin, eol - begin);
			IN_CORE_ASSERT(type == "vertex" || type == "fragment", "Invalid shader type specified");

			size_t nextLinePos = m_ShaderSource.find_first_not_of("\r\n", eol);
			pos = m_ShaderSource.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = m_ShaderSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? m_ShaderSource.size() - 1 : nextLinePos));
		}

		ProcessResources();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar *source = (const GLchar *)shaderSources[0].c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			IN_ERROR("{0}", &infoLog[0]);

			// In this simple m_RendererID, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar *)shaderSources[1].c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			IN_ERROR("{0}", &infoLog[0]);

			// In this simple m_RendererID, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a m_RendererID.
		// Get a m_RendererID object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our m_RendererID
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our m_RendererID
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the m_RendererID anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			IN_ERROR("{0}", &infoLog[0]);

			// In this simple m_RendererID, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}


	void OpenGLShader::LoadShaderFromFile(const string& path)
	{
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			m_ShaderSource.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&m_ShaderSource[0], m_ShaderSource.size());
			in.close();
		}
		else
			IN_CORE_WARN("Could not read sahder file {0}", path);
	}

	void OpenGLShader::ProcessResources()
	{
		const char* token;
		const char* source = m_ShaderSource.c_str();

		while (token = FindToken(source, "uniform"))
			ParseUniform(GetStatement(token, &source));
	}

	void OpenGLShader::ParseUniform(const string& statement)
	{
		std::vector<string> tokens = Tokenize(statement);

		if (tokens[1] == "smapler2D" || tokens[1] == "samplerCube");

		string name = tokens[2];
		if (const char* s = std::strstr(name.c_str(), ";"))
			name = string(name.c_str(), s - name.c_str());

		m_Resources.push_back(name);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
#ifdef IN_DEBUG
		glUseProgram(0);
#endif
	}

	uint OpenGLShader::ShaderTypeFromString(const string& type)
	{
		if (type == "vertex")
			return 0;
		else if (type == "fragment")
			return 1;
		IN_CORE_WARN("Shader type unknown!");
		return -1;
	}

	int OpenGLShader::GetUniformLocation(const string& name)
	{
		if (m_UniformBuffer.find(name) != m_UniformBuffer.end())
			return m_UniformBuffer[name];

		int result = glGetUniformLocation(m_RendererID, name.c_str());
		if (result == -1)
			IN_CORE_WARN("Could not find Uniform {0}", name);
		else
			m_UniformBuffer[name] = result;
		return result;
	}

	void OpenGLShader::SetUniform1i(const string& name, const int& value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform1f(const string& name, const float& value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform2f(const string& name, const glm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::SetUniform3f(const string& name, const glm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniform4f(const string& name, const glm::vec4& value)
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformMat4(const string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	int OpenGLShader::GetResourceSlot(const string& name) const
	{
		auto it = std::find(m_Resources.begin(), m_Resources.end(), name);
		if (it == m_Resources.end())
			return -1;
		return std::distance(m_Resources.begin(), it) - 1;
	}
}
