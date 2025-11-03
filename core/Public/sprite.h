#pragma once

#include <boundingArea.h>
#include <mathUtils.h>
#include <motionQuantizer.h>
#include <gravity.h>

#include <optional>
#include <variant>

namespace Simply2D
{
	class Sprite 
	{
	public:
		using Mover				= std::function<void(Rect& r, int* dx, int* dy)>;
		using BoundingVariant	= std::variant<BoundingBox>;
		using OptionalBounding	= std::optional<BoundingVariant>;
		using CollisionCallback = std::function<void(Sprite*)>;

	public:
		Sprite(const std::string& _name, int _x, int _y);
		virtual ~Sprite() = default;

		void	 SetBox(int _width, int _height);
		Rect	 GetBox();
		void	 SetMover(const Mover& move);
		Sprite&  Move(int dx, int dy);
		void     SetPos(int _x, int _y);
		int		 GetPosX() const;
		int		 GetPosY() const;
		void	 SetBoundingArea(const BoundingVariant& area);
		void	 SetColiderBox(unsigned _w, unsigned _h);
		auto	 GetBoundingArea() -> const OptionalBounding& { return m_boundingArea; }
		void	 SetMotionQuantizerUse(bool v);
		void	 SetQuanntizerVertHorz(int v, int h);
		
		void	 SetCollisionCallback(const CollisionCallback& callback);
		auto	 GetCollisionCallback() -> const CollisionCallback& { return m_collisionCallback; }

		GravityHandler& GetGravityHandler(void);
		Sprite&			SetHasDirectMotion(bool v);
		bool			GetHasDirectMotion(void) const;

	private:
		// for deriving
		std::string	m_name;

		// colisions
		OptionalBounding  m_boundingArea;
		CollisionCallback m_collisionCallback;
		
		// persitent data
		bool m_emptyFrameBox = true;
		int	 m_width = 0, m_height = 0;
		int	 m_x = 0, m_y = 0;
		bool m_isVisible = false;

		// Moving
		Mover			m_mover;
		MotionQuantizer m_quantizer;
		GravityHandler	m_gravity;
		bool			m_directMotion = false;
	};
}