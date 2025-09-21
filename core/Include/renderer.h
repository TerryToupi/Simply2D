#pragma once

#include <string>
#include <memory>

#include <handle.h>
#include <span.h>
#include <resources.h>

namespace core
{
	// Drawing
	struct DrawCall
	{
		Handle<Texture> texture;
		int src[4] = { 0, 0, 0, 0 };
		int dist[4] = { 0, 0, 0, 0 };
	};
	
	enum class LoadOp : unsigned int
	{
		LOAD = 0,
		CLEAR = 1
	};

	enum class StoreOp : unsigned int
	{
		STORE = 0,
		CLEAR = 1
	};

	struct RenderDescriptor
	{
		Handle<Texture> target = SURFACE;
		LoadOp loadOp = LoadOp::CLEAR;
		StoreOp storeOp = StoreOp::STORE;
		int clearColor[4] = { 0, 0, 0, 255 };
	};

	struct RendererSpecifications
	{
		std::string name = "Simply2D";
		int width = 640;
		int height = 480;
		bool borderless = false;
		bool vSync = true;
	};

	class Renderer
	{
	public:
		static std::shared_ptr<Renderer> Create(const RendererSpecifications& specs);
		Renderer(const RendererSpecifications& specs = RendererSpecifications())
			: m_specifications(specs) { }
		virtual ~Renderer() = default;

		virtual void draw(const RenderDescriptor& desc, Span<DrawCall> calls) = 0;

		//virtual Handle<Texture> createTexture(const TextureDescriptor& desc) = 0;
		//virtual void destroyTexture(Handle<Texture> texture) = 0;

	protected:
		RendererSpecifications m_specifications;
	};
}