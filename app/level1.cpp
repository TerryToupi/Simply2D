#include <level1.h>
#include <SampleSprite.h>
#include <iostream>

Level1::Level1(Simply2D::Asset level)
	:	Scene(level)
{
	AddSprite<SampleSprite>("Antonis", 0, 0);
}

Level1::~Level1()
{
}

void Level1::event(Simply2D::Event& e)
{
	Simply2D::EventDispatcher dispatch(e);
	dispatch.Dispatch<Simply2D::MouseButtonPressedEvent>([this](Simply2D::MouseButtonPressedEvent& e) { std::cout << e.ToString().c_str() << std::endl;  return true; });
	dispatch.Dispatch<Simply2D::KeyPressedEvent>([this](Simply2D::KeyPressedEvent& e) { std::cout << e.ToString().c_str() << std::endl;  return true; });
}

void Level1::update(float ts)
{
	SampleSprite* antonis = GetSprite<SampleSprite>("Antonis");

	for (auto& sprite : GetAll<Simply2D::Sprite>())
	{
		sprite->update(ts);
	}
}

void Level1::render()
{
	TArray<Simply2D::DrawCall, 6> calls;

	for (unsigned i = 0; i < m_layersCount; ++i)
	{
		int twidth = 0, theight = 0;
		Simply2D::gfx().textureSize(m_layers[i].texture(), twidth, theight);

		int swidth = 0, sheight = 0;
		Simply2D::gfx().textureSize(SURFACE, swidth, sheight);

		calls[i] = {
			.texture = m_layers[i].texture(),
			.blend = Simply2D::Blend::BLEND,
			.alpha = 255,
			.src = { 0, 0, twidth, theight },
			.dist = { 0, 0, swidth, sheight }
		};
	}

	Simply2D::gfx().draw(
		{
			.target = SURFACE,
			.loadOp = Simply2D::LoadOp::LOAD,
			.storeOp = Simply2D::StoreOp::STORE,
			.clearColor = {0, 0, 0, 255}
		},
		Span(calls.data(), calls.size())
	);
}

void Level1::end(float ts)
{
}

