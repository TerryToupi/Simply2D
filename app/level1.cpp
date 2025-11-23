#include <level1.h>

Level1::Level1(Simply2D::Asset level)
	:	Scene(level)
{
	m_animator.Start(&m_anim1, 0.0f, 0);
 }

Level1::~Level1()
{
}

void Level1::event()
{
}

void Level1::update(float ts)
{
}

void Level1::render()
{
	auto dummy = Allocator::FrameAlloc<int>();
	auto calls = Allocator::FrameAlloc<Simply2D::DrawCall>(m_layersCount);

	(*dummy.ptr) = 12;

	for (unsigned i = 0; i < m_layersCount; ++i)
	{
		int twidth = 0, theight = 0;
		Simply2D::gfx().textureSize(m_layers[i]->texture(), twidth, theight);

		int swidth = 0, sheight = 0;
		Simply2D::gfx().textureSize(SURFACE, swidth, sheight);

		calls[i] = {
			.texture = m_layers[i]->texture(),
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
		Span(calls.ptr, calls.count)
	);
}

void Level1::end(float ts)
{
}

