#pragma once

#include <optional>
#include <variant>

#include "Math/mathUtils.h"
#include "Types/String.h"
#include "Types/Function.h"

#include "boundingArea.h"
#include "motionQuantizer.h"
#include "gravity.h"

namespace Simply2D
{
	class Sprite 
	{
	public:
		using Mover	= TFunction<void(Rect& r, int* dx, int* dy)>;

	public:
		Sprite(const char* _name, int _x, int _y);
		virtual ~Sprite() = 0;

	// scripting
	public:
		virtual void begin(float ts)  { }
		virtual void update(float ts) { }
		virtual void end(float ts)    { }

		virtual BoundingArea* OnGetCollider() { return nullptr; }
		virtual	void		  OnCollision(Sprite& other) { }

	// setters and getters
	public:
		const char* GetID();

		void	 SetBox(int _width, int _height);
		Rect	 GetBox();
		void	 SetMover(const Mover& move);
		Sprite&  Move(int dx, int dy);
		void     SetPos(int _x, int _y);
		int		 GetPosX() const;
		int		 GetPosY() const;
		void	 SetColiderBox(unsigned _w, unsigned _h);
		void	 SetMotionQuantizerUse(bool v);
		void	 SetQuanntizerVertHorz(int v, int h);

		GravityHandler& GetGravityHandler(void);
		Sprite&			SetHasDirectMotion(bool v);
		bool			GetHasDirectMotion(void) const;

	private:
		TString m_id;

		// persistent data
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