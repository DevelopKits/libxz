#pragma once

namespace geometry
{
	class Point
	{
	public:
		float x;
		float y;

	public:
		Point();
		Point(float x, float y);
		Point(const Point& other);
		Point& operator= (const Point& other);
		void setPoint(float x, float y);
		bool equals(const Point& target) const;
	};

	class Size
	{
	public:
		float w;
		float h;

	public:
		Size();
		Size(float width, float height);
		Size(const Size& other);
		Size& operator= (const Size& other);
		void setSize(float width, float height);
		bool equals(const Size& target) const;
	};

	class Rect
	{
	public:
		Point origin;
		Size  size;

	public:
		Rect();    
		Rect(float x, float y, float width, float height);
		Rect(const Rect& other);
		Rect& operator= (const Rect& other); 
		void setRect(float x, float y, float width, float height);
		float getMinX() const; /// return the leftmost x-value of current rect
		float getMidX() const; /// return the midpoint x-value of current rect
		float getMaxX() const; /// return the rightmost x-value of current rect
		float getMinY() const; /// return the bottommost y-value of current rect
		float getMidY() const; /// return the midpoint y-value of current rect
		float getMaxY() const; /// return the topmost y-value of current rect
		bool equals(const Rect& rect) const;   
		bool containsPoint(const Point& point) const;
		bool intersectsRect(const Rect& rect) const;
	};



	/**
	 *	[ a, c, tx ] [x]   [a*x + c*y + tx]
	 *  [ b, d, ty ] [y] = [b*x + d*y + ty]
	 *  [ 0, 0, 1  ] [1]   [1             ]
	 * 
	 */
	class Transform
	{
	public:
		Transform();
		Transform(float a, float b, float c, float d, float tx, float ty);

		bool IsEqual(const Transform& t) const;

		//����任
		Point Apply(const Point& point) const;
		Size Apply(const Size& size) const;
		Rect Apply(const Rect& rect) const;

		/**
		*	ƽ�Ʊ任
		*
		*	    [ 1, 0, tx ] 
		*	t *	[ 0, 1, ty ] 
		*	    [ 0, 0, 1  ]
		*
		* @param tx x��ƽ����
		* @param ty y��ƽ����
		*/
		Transform Translate(float x, float y) const;

		/**
		*	��ת�任
		*	    [ cos(anAngle), -sin(anAngle), 0 ]
		*	t *	[ sin(anAngle),  cos(anAngle), 0 ]
		*	    [            0,             0, 1 ]
		*
		*  @param  anAngle ˳ʱ��Ƕ�
		*/
		Transform Rotate(float anAngle) const;

		/**
		*	���ű任
		*		[ sx,  0, 0 ] 
		*	t *	[  0, sy, 0 ] 
		*		[  0,  0, 1 ]
		*
		*  @param sx/sy x/y�����ű���
		*/
		Transform Scale(float sx, float sy) const;

		/**
		*	�������
		*/
		Transform Concat(const Transform& t) const;

		/**
		*	����ת
		*/
		Transform Invert() const;

		XFORM ToXFORM() const;
	private:
		float a, b, c, d;
		float tx, ty;
	};

	

	

	

	
	
	/**
	 *	����ת
	 */
	Transform TransformInvert(const Transform& t);

	/// @}  �任������
}

#define PointMake(x, y) geometry::Point((float)(x), (float)(y))
#define SizeMake(width, height) geometry::Size((float)(width), (float)(height))
#define RectMake(x, y, width, height) geometry::Rect((float)(x), (float)(y), (float)(width), (float)(height))



const geometry::Point kPointZero = PointMake(0,0);

const geometry::Size kSizeZero = SizeMake(0,0);

const geometry::Rect kRectZero = RectMake(0,0,0,0);

void ToRECT(const geometry::Rect& rect, LPRECT lprect);
