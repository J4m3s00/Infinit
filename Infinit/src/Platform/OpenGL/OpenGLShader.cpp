#include "inpch.h"

#define IN_SHOW_UNIFORM_WARN 0

namespace Infinit {

	OpenGLShader::OpenGLShader()
		: Shader(""), m_RendererID(0), m_UniformBufferSize(0), m_UniformBuffer(nullptr), m_ShaderSource("ShaderSource", this, "")
	{
	}

	OpenGLShader::OpenGLShader(const string& path)
		: Shader(path), m_RendererID(0), m_UniformBufferSize(0), m_ShaderSource("ShaderSource", this, "")
	{
		Reload(path);
	}

	OpenGLShader::OpenGLShader(const string& vertexSource, const string& fragmentSource)
		: Shader("", "Default Shader"), m_UniformBufferSize(0), m_ShaderSource("ShaderSource", this, "")
	{
		m_ShaderSource.SetValue("#shader vertex\n" + vertexSource + "#shader fragment\n" + fragmentSource);
		CompileShader();
	}

	OpenGLShader::~OpenGLShader()
	{
		IN_CORE_INFO("OpenGL Shader {0} destroyed!", m_RendererID);
		delete m_UniformBuffer;
		IN_RENDER_S({
				glDeleteProgram(self->m_RendererID);
			})
	}

	void OpenGLShader::OnDeserialize(const json& json_object)
	{
		Shader::OnDeserialize(json_object);
		//Init all to zero
		m_UniformBufferSize = 0;
		m_UniformCache.clear();
		m_Uniforms.clear();
		m_Structs.clear();
		m_Resources.clear();
		if (m_UniformBuffer)
			delete[] m_UniformBuffer;
		m_UniformBuffer = nullptr;
		m_ShaderSource.SetValue("");
		////////
		if (m_RendererID != 0) {
			glDeleteProgram(m_RendererID);
			m_RendererID = 0;
		}

		CompileShader();
	}


	bool OpenGLShader::Reload(const string& filepath)
	{
		//Init all to zero
		m_UniformBufferSize = 0;
		m_UniformCache.clear();
		delete[] m_UniformBuffer;
		m_UniformBuffer = nullptr;
		m_Uniforms.clear();
		m_Structs.clear();
		m_Resources.clear();
		m_ShaderSource.SetValue("");
		////////
		if (filepath != "") m_FilePath.SetValue(filepath);
		std::ifstream inFile(m_FilePath.GetValue());
		json json_object;
		inFile >> json_object;
		if (!json_object.is_null())
		{
			Deserialize(json_object);
		}
		else
		{
			if (m_RendererID != 0) {
				glDeleteProgram(m_RendererID);
				m_RendererID = 0;
			}
			if (GetFilePath() != "")
				LoadShaderFromFile(m_FilePath.GetValue());

			CompileShader();
		}
		return true;
	}

	void OpenGLShader::CompileShader()
	{
		string* shaderSources = new string[2];

		const char* typeToken = "#shader";
		size_t typeTokenLength = strlen(typeToken);
		const string& shaderSource = m_ShaderSource.GetValue();
		size_t pos = shaderSource.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = shaderSource.find_first_of("\r\n", pos);
			IN_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = shaderSource.substr(begin, eol - begin);
			IN_CORE_ASSERT(type == "vertex" || type == "fragment", "Invalid shader type specified");

			size_t nextLinePos = shaderSource.find_first_not_of("\r\n", eol);
			pos = shaderSource.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = shaderSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? shaderSource.size() - 1 : nextLinePos));
		}



		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		IN_RENDER_S1(shaderSources, {
		const GLchar * source = (const GLchar*)shaderSources[0].c_str();
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

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
		self->m_RendererID = glCreateProgram();

		// Attach our shaders to our m_RendererID
		glAttachShader(self->m_RendererID, vertexShader);
		glAttachShader(self->m_RendererID, fragmentShader);

		// Link our m_RendererID
		glLinkProgram(self->m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(self->m_RendererID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(self->m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(self->m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the m_RendererID anymore.
			glDeleteProgram(self->m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			IN_ERROR("{0}", &infoLog[0]);

			// In this simple m_RendererID, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(self->m_RendererID, vertexShader);
		glDetachShader(self->m_RendererID, fragmentShader);
			})

		ProcessResources();
	}


	void OpenGLShader::LoadShaderFromFile(const string& path)
	{
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			string shaderSource;
			shaderSource.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&shaderSource[0], shaderSource.size());
			in.close();
			m_ShaderSource.SetValue(shaderSource);
		}
		else
			IN_CORE_WARN("Could not read sahder file {0}", path);
	}

	void OpenGLShader::ProcessResources()
	{
		const char* token;
		const char* source = m_ShaderSource.GetValue().c_str();

		while (token = FindToken(source, "struct"))
			ParseUniformStruct(GetBlock(token, &source));

		source = m_ShaderSource.GetValue().c_str();
		while (token = FindToken(source, "uniform"))
			ParseUniform(GetStatement(token, &source));

		m_UniformBuffer = (byte*) malloc(m_UniformBufferSize);
		memset(m_UniformBuffer, 0, m_UniformBufferSize);
	}

	byte* OpenGLShader::GetUniformBuffer(const string& name) const
	{
		byte* result = m_UniformBuffer;
		for (const ShaderUniform& uni : m_Uniforms)
		{
			if (uni.Name == name)
				return result;
			else if (uni.Name.find(".") != string::npos)
			{
				string ss;
				for (uint i = 0; i < uni.Name.size(); i++)
				{
					if (uni.Name[i] == '.') break;
					ss += uni.Name[i];
				}
				if (ss == name)
					return result;
			}
		
			result += ShaderDataTypeSize(uni.Type);
		}
#if IN_SHOW_UNIFORM_WARN
		IN_CORE_WARN("Could not find Uniform {0} in uniformBuffer", name);
#endif
		return nullptr;
	}

	void OpenGLShader::SetUniformBuffer(const string& name, byte* value, size_t size)
	{
		byte* ptr = GetUniformBuffer(name);
		IN_RENDER3(ptr, value, size, {
			if (ptr)
				memcpy(ptr, value, size);
			});
	}

	void OpenGLShader::ParseUniform(const string& statement)
	{
		std::vector<string> tokens = Tokenize(statement);
		unsigned int index = 0;
		index++; // "uniform"
		string typeString = tokens[index++];
		string name = tokens[index++];

		// Strip ; from name if present
		if (const char* s = strstr(name.c_str(), ";"))
			name = string(name.c_str(), s - name.c_str());

		short count = 1;
		const char* namestr = name.c_str();
		if (const char* s = strstr(namestr, "["))
		{
			name = string(namestr, s - namestr);

			const char* end = strstr(namestr, "]");
			string c(s + 1, end - s);
			count = atoi(c.c_str());
		}


		ShaderDataType type = ShaderDataTypeFromString(typeString);
		uint size = ShaderDataTypeSize(type);

		if (type == ShaderDataType::Texture2D || type == ShaderDataType::TextureCube)
			m_Resources.push_back(name);


		if (type == ShaderDataType::None)
		{
			ShaderStruct str;
			//Find struct
			if (FindStruct(typeString, &str))
			{
				for (StructMember& mem : str.Members)
				{
					size = ShaderDataTypeSize(mem.Type);
					string uniformName = name + "." + mem.Name;
					m_Uniforms.push_back({ uniformName, mem.Type });
					m_UniformBufferSize += count * size;
				}
			}
		}
		else
		{
			m_Uniforms.push_back({ name, type });
			m_UniformBufferSize += count * size;
		}
	}

	bool OpenGLShader::FindStruct(const string& name, ShaderStruct* str)
	{
		if (!str)
		{
			IN_CORE_WARN("To find a struct give a valid struct to store data in!");
			return false;
		}

		for (ShaderStruct& s : m_Structs)
			if (s.Name == name)
				*str = s;
		return (bool)str;
	}

	void OpenGLShader::ParseUniformStruct(const string& block)
	{
		std::vector<string> tokens = Tokenize(block);
		std::vector<StructMember> members;
		uint structSize = 0;
		unsigned int index = 0;
		index++; // "struct"
		string name = tokens[index++];
		index++; //;
		while (index < tokens.size())
		{
			if (tokens[index] == "}")
				break;
			string typeString = tokens[index++];
			string memberName = tokens[index++];

			// Strip ; from name if present
			if (const char* s = strstr(memberName.c_str(), ";"))
				memberName = string(memberName.c_str(), s - memberName.c_str());

			short count = 1;
			const char* namestr = memberName.c_str();
			if (const char* s = strstr(namestr, "["))
			{
				memberName = string(namestr, s - namestr);

				const char* end = strstr(namestr, "]");
				string c(s + 1, end - s);
				count = atoi(c.c_str());
			}
			ShaderDataType type = ShaderDataTypeFromString(typeString);
			uint size = ShaderDataTypeSize(type);

			structSize += count * size;
			members.push_back({type, memberName});
		}
		m_Structs.push_back({ name, members, structSize });
	}

	void OpenGLShader::Bind() const
	{
		IN_RENDER_S({
				glUseProgram(self->m_RendererID);
			})
	}

	void OpenGLShader::Unbind() const
	{
#ifdef IN_DEBUG
		IN_RENDER({
				glUseProgram(0);
			})
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
		if (m_UniformCache.find(name) != m_UniformCache.end())
			return m_UniformCache[name];

		int result = glGetUniformLocation(m_RendererID, name.c_str());
		if (result == -1)
		{
#if IN_SHOW_UNIFORM_WARN
			IN_CORE_WARN("Could not find Uniform {0}", name);
#endif
		}
		else
			m_UniformCache[name] = result;
		return result;
	}

	const std::vector<ShaderUniform>& OpenGLShader::GetUniforms() const
	{
		return m_Uniforms;
	}

	const std::vector<ShaderStruct>& OpenGLShader::GetStructs() const
	{
		return m_Structs;
	}

	void OpenGLShader::UploadUniformBuffer()
	{
		IN_RENDER_S({
			byte* buffer = self->m_UniformBuffer;
			for (ShaderUniform& uniform : self->m_Uniforms)
			{
				switch (uniform.Type)
				{
				case ShaderDataType::Matrix3: self->SetUniformMat3(uniform.Name, *((const glm::mat3*)buffer)); break;
				case ShaderDataType::Matrix4: self->SetUniformMat4(uniform.Name, *((const glm::mat4*)buffer)); break;
				case ShaderDataType::Float: self->SetUniform1f(uniform.Name, *((const float*)buffer)); break;
				case ShaderDataType::Float2: self->SetUniform2f(uniform.Name, *((const glm::vec2*)buffer)); break;
				case ShaderDataType::Float3: self->SetUniform3f(uniform.Name, *((const glm::vec3*)buffer)); break;
				case ShaderDataType::Float4: self->SetUniform4f(uniform.Name, *((const glm::vec4*)buffer)); break;
				case ShaderDataType::Int: self->SetUniform1i(uniform.Name, *((const int*)buffer)); break;
				case ShaderDataType::Bool: self->SetUniform1i(uniform.Name, *((const bool*)buffer)); break;
				case ShaderDataType::Texture2D:
				case ShaderDataType::TextureCube: self->SetUniform1i(uniform.Name, (int&)*((int*)buffer)); break;
				default:
					IN_CORE_WARN("Cant decide what uniform type to upload");
					break;
				}
				buffer += ShaderDataTypeSize(uniform.Type);
			}
		})
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

	void OpenGLShader::SetUniformMat3(const string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
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
		return std::distance(m_Resources.begin(), it);
	}

	void OpenGLShader::ImGuiDraw()
	{
		Shader::ImGuiDraw();
		ImGui::Text("%s\n", m_ShaderSource.GetValue().c_str());
	}
}
