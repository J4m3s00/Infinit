#pragma once



//


#ifdef IN_PLATFORM_WINDOWS
#include <windows.h>
#include <filesystem>
#include <Commdlg.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <iostream>
#include <functional>
#include <memory>
#include <fstream>
#include <future>
#include <thread>
#include <stack>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#define SPDLOG_HEADER_ONLY
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_internal.h>


//////INFINIT HEADER/////////
#include "Core/Core.h"

#include "Util/StringUtil.h"

#include "Core/Log.h"
#include "Core/KeyCodes.h"
#include "Core/Input.h"
#include "Core/Transform.h"
#include "Events/Events.h"
#include "graphics/Window.h"
#include "Core/Resource.h"
#include "Core/ResourceLoader.h"
#include "graphics/RendererAPI.h"
#include "Core/Application.h"

#include "Core/Light.h"
#include "Core/ImGui/ImGuiHelper.h"

#include "graphics/Buffer.h"
#include "graphics/VertexArray.h"
#include "graphics/ShaderUniforms.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/FrameBuffer.h"
#include "graphics/RenderCommandQueue.h"
#include "graphics/RenderContext.h"
#include "graphics/Camera.h"
#include "graphics/Mesh.h"
#include "Core/Material.h"
#include "graphics/Renderer.h"


#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Layer/ImGuiLayer.h"
#include "Core/ECS/Scene.h"
#include "Core/ECS/Component/Components.h"
#include "Core/ECS/GameObject.h"




//PLATFORMS
//OPENGL
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
/////////////////////////////////////////////////