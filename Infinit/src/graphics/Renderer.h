#pragma once

#include "Core/Core.h"
#include "Mesh.h"
//TEMP
#include "Shader.h"
#include "Camera.h"
#include "Core/Light.h"
#include "RenderCommandQueue.h"

namespace Infinit {

	class Renderer
	{
	public:
		static void Init();

		static void Begin(Camera* camera, const LightMap& lights);
		static void Draw(MeshInstance* mesh, std::shared_ptr<Material>& material, const glm::mat4& modelMatrix = glm::mat4(1.0f));
		static void* Submit(RenderCommandFn fn, uint size) { return s_Instance->m_CommandQueue.Allocate(fn, size); }
		static void End();

		static Renderer* Get() { return s_Instance; }

		void WaitAndRender();
	private:
		static Renderer* s_Instance;

		RenderCommandQueue m_CommandQueue;
		//TEMP
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_CameraPosition;
		LightMap m_LightMap;
	};

}

#define IN_RENDER_PASTE2(a, b) a ## b
#define	IN_RENDER_PASTE(a, b) IN_RENDER_PASTE2(a, b)
#define IN_RENDER_UNIQUE(x) IN_RENDER_PASTE(x, __LINE__)

#define IN_RENDER(code) \
	struct IN_RENDER_UNIQUE(INRenderCommand) \
	{\
		static void Execute(void*)\
		{\
			code\
		}\
	};\
	{\
		auto mem = Infinit::Renderer::Submit(IN_RENDER_UNIQUE(INRenderCommand)::Execute, sizeof(IN_RENDER_UNIQUE(INRenderCommand)));\
		new (mem) IN_RENDER_UNIQUE(INRenderCommand)();\
	}\

#define IN_RENDER1(arg0, code) \
	struct IN_RENDER_UNIQUE(INRenderCommand) \
	{\
		IN_RENDER_UNIQUE(INRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0)\
		:arg0(arg0) {}\
\
		static void Execute(void* argBuffer)\
		{\
			auto& arg0 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg0; \
			code\
		}\
\
typename std::remove_const<typename std::remove_reference<decltype(arg0)>::type>::type arg0;\
	};\
	{\
		auto mem = Infinit::Renderer::Submit(IN_RENDER_UNIQUE(INRenderCommand)::Execute, sizeof(IN_RENDER_UNIQUE(INRenderCommand)));\
		new (mem) IN_RENDER_UNIQUE(INRenderCommand)(arg0);\
	}


#define IN_RENDER2(arg0, arg1, code) \
	struct IN_RENDER_UNIQUE(INRenderCommand) \
	{\
		IN_RENDER_UNIQUE(INRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1)\
		:arg0(arg0), arg1(arg1) {}\
\
		static void Execute(void* argBuffer)\
		{\
			auto& arg0 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg0; \
			auto& arg1 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg1; \
			code\
		}\
\
typename std::remove_const<typename std::remove_reference<decltype(arg0)>::type>::type arg0;\
typename std::remove_const<typename std::remove_reference<decltype(arg1)>::type>::type arg1;\
	};\
	{\
		auto mem = Infinit::Renderer::Submit(IN_RENDER_UNIQUE(INRenderCommand)::Execute, sizeof(IN_RENDER_UNIQUE(INRenderCommand)));\
		new (mem) IN_RENDER_UNIQUE(INRenderCommand)(arg0, arg1);\
	}


#define IN_RENDER3(arg0, arg1, arg2, code) \
	struct IN_RENDER_UNIQUE(INRenderCommand) \
	{\
		IN_RENDER_UNIQUE(INRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2)\
		:arg0(arg0), arg1(arg1), arg2(arg2) {}\
\
		static void Execute(void* argBuffer)\
		{\
			auto& arg0 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg0; \
			auto& arg1 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg1; \
			auto& arg2 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg2; \
			code\
		}\
\
typename std::remove_const<typename std::remove_reference<decltype(arg0)>::type>::type arg0;\
typename std::remove_const<typename std::remove_reference<decltype(arg1)>::type>::type arg1;\
typename std::remove_const<typename std::remove_reference<decltype(arg2)>::type>::type arg2;\
	};\
	{\
		auto mem = Infinit::Renderer::Submit(IN_RENDER_UNIQUE(INRenderCommand)::Execute, sizeof(IN_RENDER_UNIQUE(INRenderCommand)));\
		new (mem) IN_RENDER_UNIQUE(INRenderCommand)(arg0, arg1, arg2);\
	}


#define IN_RENDER4(arg0, arg1, arg2, arg3, code) \
	struct IN_RENDER_UNIQUE(INRenderCommand) \
	{\
		IN_RENDER_UNIQUE(INRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		:arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
\
		static void Execute(void* argBuffer)\
		{\
			auto& arg0 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg0; \
			auto& arg1 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg1; \
			auto& arg2 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg2; \
			auto& arg3 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg3; \
			code\
		}\
\
typename std::remove_const<typename std::remove_reference<decltype(arg0)>::type>::type arg0;\
typename std::remove_const<typename std::remove_reference<decltype(arg1)>::type>::type arg1;\
typename std::remove_const<typename std::remove_reference<decltype(arg2)>::type>::type arg2;\
typename std::remove_const<typename std::remove_reference<decltype(arg3)>::type>::type arg3;\
	};\
	{\
		auto mem = Infinit::Renderer::Submit(IN_RENDER_UNIQUE(INRenderCommand)::Execute, sizeof(IN_RENDER_UNIQUE(INRenderCommand)));\
		new (mem) IN_RENDER_UNIQUE(INRenderCommand)(arg0, arg1, arg2, arg3);\
	}


#define IN_RENDER_S(code) auto self = this;\
	IN_RENDER1(self, code)

#define IN_RENDER_S1(arg0, code) auto self = this;\
	IN_RENDER2(self, arg0, code)

#define IN_RENDER_S2(arg0, arg1, code) auto self = this;\
	IN_RENDER3(self, arg0, arg1, code)

#define IN_RENDER_S3(arg0, arg1, arg2, code) auto self = this;\
	IN_RENDER4(self, arg0, arg1, arg2, code)

