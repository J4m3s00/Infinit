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
		static void Draw(MeshInstance* mesh, const glm::mat4& modelMatrix = glm::mat4(1.0f));
		static void* Submit(RenderCommandFn fn, uint size);
		static void End();

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
		static void Executre(void*)\
		{\
			code\
		}\
	};\
	{\
		auto mem ::Infinit::Renderer::Submit(HZ_RENDER_UNIQUE(INRenderCommand)::Execute, sizeof(IN_RENDER_UNIQUE(IN_RenderCommand)));\
		new (mem) IN_RENDER_UNIQUE(INRenderCommand)();\
	}\

#define IN_RENDER1(arg0, code) \
	struct IN_RENDER_UNIQUE(INRenderCommand) \
	{\
		IN_RENDER_UNIQUE(INRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0)\
		:arg0(arg0) {}\
\
		static void Executre(void*)\
		{\
			auto& arg0 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg0; \
			code\
		}\
\
typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
	};\
	{\
		auto mem ::Infinit::Renderer::Submit(HZ_RENDER_UNIQUE(INRenderCommand)::Execute, sizeof(IN_RENDER_UNIQUE(IN_RenderCommand)));\
		new (mem) IN_RENDER_UNIQUE(INRenderCommand)();\
	}\

#define IN_RENDER2(arg0, arg1, code) \
	struct IN_RENDER_UNIQUE(INRenderCommand) \
	{\
		IN_RENDER_UNIQUE(INRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1)\
		:arg0(arg0), arg1(arg1) {}\
\
		static void Executre(void*)\
		{\
			auto& arg0 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg0; \
			auto& arg1 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg1; \
			code\
		}\
\
typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0; \
typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
	};\
	{\
		auto mem ::Infinit::Renderer::Submit(HZ_RENDER_UNIQUE(INRenderCommand)::Execute, sizeof(IN_RENDER_UNIQUE(IN_RenderCommand)));\
		new (mem) IN_RENDER_UNIQUE(INRenderCommand)();\
	}\


#define IN_RENDER3(arg0, arg1, arg2, code) \
	struct IN_RENDER_UNIQUE(INRenderCommand) \
	{\
		IN_RENDER_UNIQUE(INRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2)\
		:arg0(arg0), arg1(arg1), arg2(arg2) {}\
\
		static void Executre(void*)\
		{\
			auto& arg0 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg0; \
			auto& arg1 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg1; \
			auto& arg2 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg2; \
			code\
		}\
\
typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0; \
typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
	};\
	{\
		auto mem ::Infinit::Renderer::Submit(HZ_RENDER_UNIQUE(INRenderCommand)::Execute, sizeof(IN_RENDER_UNIQUE(IN_RenderCommand)));\
		new (mem) IN_RENDER_UNIQUE(INRenderCommand)();\
	}\



#define IN_RENDER4(arg0, arg1, arg2, arg3, code) \
	struct IN_RENDER_UNIQUE(INRenderCommand) \
	{\
		IN_RENDER_UNIQUE(INRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2, typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		:arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
\
		static void Executre(void* argBuffer)\
		{\
			auto& arg0 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((IN_RENDER_UNIQUE(INRenderCommand)*)argBuffer)->arg3;\
			code\
		}\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0; \
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2; \
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
	};\
	{\
		auto mem ::Infinit::Renderer::Submit(HZ_RENDER_UNIQUE(INRenderCommand)::Execute, sizeof(IN_RENDER_UNIQUE(IN_RenderCommand)));\
		new (mem) IN_RENDER_UNIQUE(INRenderCommand)();\
	}\

