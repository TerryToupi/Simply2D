#pragma once 

namespace Simply2D
{
	class BoundingBox;

	class BoundingArea
	{
	public:
		virtual bool Intersects(const BoundingArea& area) const = 0;
		virtual bool In(unsigned x, unsigned y) const = 0;
		virtual BoundingArea* Clone(void) const = 0;
		virtual ~BoundingArea() {}

	protected:
		virtual bool Intersects(const BoundingBox& box) const = 0;

	protected:
		friend class BoundingBox;
	};

	class BoundingBox final : public BoundingArea
	{
	public:
		BoundingBox(unsigned _x, unsigned _y, unsigned _w, unsigned _h)
			: x(_x), y(_y), w(_w), h(_h) {
		}

		virtual bool Intersects(const BoundingBox& box) const;

		virtual bool In(unsigned x, unsigned y) const;
		virtual bool Intersects(const BoundingArea& area) const
		{
			return area.Intersects(*this);
		};
		virtual BoundingBox* Clone(void) const
		{
			return new BoundingBox(x, y, w, h);
		}

	protected:
		unsigned x, y, w, h;
	};
}