#include <level1.h>
#include <iostream>

level1::level1(std::string level)
	:	Scene(level)
{
	Simply2D::Sprite s1 = createSprite("antonis");
	Simply2D::Sprite s2 = createSprite("kwstas");
	Simply2D::Sprite s3 = createSprite("giwrgis");
}

level1::~level1()
{
}

void level1::event()
{
}

void level1::update(float ts)
{
}

void level1::render()
{
	for (const auto& layer : m_layers)
	{
		int twidth = 0, theight = 0;
		Simply2D::gfx().textureSize(layer->texture(), twidth, theight);

		int swidth = 0, sheight = 0;
		Simply2D::gfx().textureSize(SURFACE, swidth, sheight);

		Simply2D::gfx().draw(
			{
				.target = SURFACE,
				.loadOp = Simply2D::LoadOp::LOAD,
				.storeOp = Simply2D::StoreOp::STORE,
				.clearColor = {0, 0, 0, 255}
			}, 
			{
				{ layer->texture(), Simply2D::Blend::BLEND, {0, 0, twidth, theight}, {0, 0, swidth, sheight} } 
			});
	}
}

