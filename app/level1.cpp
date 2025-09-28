#include "level1.h"

void level1::load()
{
	Simply2D::assetDatabase().load(Simply2D::AssetType::SCENE, "scenes/level1/Level1.json");

	generateTileLayers(Simply2D::assetDatabase().get("scenes/level1/Level1.json"));
}

void level1::update()
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

		//calls.push_back(
		//	{ layer->texture(), {0, 0, twidth, theight}, {0, 0, swidth, sheight} } 
		//);
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

void level1::unload()
{
}
