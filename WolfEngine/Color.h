#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	Color() {}
	Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
	~Color() {}

	void SetColor(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	void SetColor(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

public:
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;
};

namespace Colors
{
	extern Color Black;
	extern Color Blue;
	extern Color Green;
	extern Color Aqua;
	extern Color Red;
	extern Color Fuchsia;
	extern Color Yellow;
	extern Color White;
}

#endif // !COLOR_H