#pragma once

#include <string>
#include <memory>

#include <handle.h>
#include <span.h>
#include <resources.h>

namespace Simply2D
{
	struct RendererSpecifications
	{
		std::string name = "Simply2D";
		int width = 640;
		int height = 480;
		bool borderless = false;
		bool vSync = true;
	};

	// Drawing
	enum class Blend : unsigned int
	{
		BLEND = 0,
		ADD = 1,
		MOD = 2,
		NONE = 3
	};

	struct DrawCall
	{
		Handle<Texture> texture;
		Blend blend = Blend::BLEND;
		uint8_t alpha = 255;
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
		uint8_t clearColor[4] = { 0, 0, 0, 255 };
	};

	class Renderer
	{
	public:
		Renderer(const RendererSpecifications& specs = RendererSpecifications())
			: m_specifications(specs) { }
		virtual ~Renderer() = default;

		virtual void draw(const RenderDescriptor& desc, Span<DrawCall> calls) = 0;

		virtual Handle<Texture> createTexture(const TextureDescriptor&& desc) = 0;
		virtual Handle<Texture> createTexture(Handle<Image> image) = 0;
		virtual void destroyTexture(Handle<Texture> texture) = 0;

		virtual void textureSize(Handle<Texture> texture, int& width, int& height) = 0;

	protected:
		RendererSpecifications m_specifications;

	private:
		static std::shared_ptr<Renderer> Create(const RendererSpecifications& specs);
		
		friend class Application;
	};
}