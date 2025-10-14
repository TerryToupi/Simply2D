#pragma once

#include <boundingArea.h>

#include <optional>
#include <variant>

namespace Simply2D
{
	class Sprite
	{
	public:
		using BoundingVariant = std::variant<BoundingBox>;
		using OptionalBounding = std::optional<BoundingVariant>;
		//using Mover = std::function<void(Rect& r, int* dx, int* dy)>;

	public:
		Sprite(const std::string& _name, int _x, int _y);

		//void		 SetMover(const Mover& move);
		//SpriteClass& Move(int dx, int dy);
		void		 SetPos(int _x, int _y);
		int			 GetPosX() const;
		int			 GetPosY() const;
		void		 SetBoundingArea(const BoundingVariant& area);
		void		 SetColiderBox(unsigned _w, unsigned _h);
		auto		 GetBoundingArea() -> const BoundingVariant& { return m_boundingArea.value(); }
		void		 SetMotionQuantizerUse(bool v);
		void	     SetQuanntizerVertHorz(int v, int h);

		//const std::string& GetTypeId();
		//void				SetVisibility(bool v);
		//bool				IsVisible(void) const;
		//bool				CollisionCheck(const Sprite s);
		//GravityHandler& GetGravityHandler(void);
		//SpriteClass& SetHasDirectMotion(bool v);
		//bool				GetHasDirectMotion(void) const;

	private:
		std::string m_name;
		//byte m_frameNo = 0;
		//Rect m_frameBox{ 0, 0, 0, 0 }; // inside the film 
		bool m_emptyFrameBox = true;
		int m_x = 0, m_y = 0;
		bool m_isVisible = false;
		OptionalBounding m_boundingArea;
		//Mover m_mover;
		//MotionQuantizer m_quantizer;
		//GravityHandler m_gravity;
		bool m_directMotion = false;
	};
}