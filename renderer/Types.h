#pragma once

#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vec4.h"
#include "math/Mat4.h"


struct Color4B;
struct Color4F;


typedef unsigned short IndexBuffer;


struct Color3B
{
	Color3B();
	Color3B(uint8_t _r, uint8_t _g, uint8_t _b);
	explicit Color3B(const Color4B& color);
	explicit Color3B(const Color4F& color);

	bool operator==(const Color3B& right) const;
	bool operator==(const Color4B& right) const;
	bool operator==(const Color4F& right) const;
	bool operator!=(const Color3B& right) const;
	bool operator!=(const Color4B& right) const;
	bool operator!=(const Color4F& right) const;

	bool equals(const Color3B& other) const
	{
		return (*this == other);
	}

	uint8_t r;
	uint8_t g;
	uint8_t b;

	static const Color3B WHITE;
	static const Color3B YELLOW;
	static const Color3B BLUE;
	static const Color3B GREEN;
	static const Color3B RED;
	static const Color3B MAGENTA;
	static const Color3B BLACK;
	static const Color3B ORANGE;
	static const Color3B GRAY;
};


struct Color4B
{
	Color4B();
	Color4B(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a);
	explicit Color4B(const Color3B& color, uint8_t _a = 255);
	explicit Color4B(const Color4F& color);

	inline void set(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	bool operator==(const Color4B& right) const;
	bool operator==(const Color3B& right) const;
	bool operator==(const Color4F& right) const;
	bool operator!=(const Color4B& right) const;
	bool operator!=(const Color3B& right) const;
	bool operator!=(const Color4F& right) const;

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	static const Color4B WHITE;
	static const Color4B YELLOW;
	static const Color4B BLUE;
	static const Color4B GREEN;
	static const Color4B RED;
	static const Color4B MAGENTA;
	static const Color4B BLACK;
	static const Color4B ORANGE;
	static const Color4B GRAY;
};


struct Color4F
{
	Color4F();
	Color4F(float _r, float _g, float _b, float _a);
	explicit Color4F(const Color3B& color, float _a = 1.0f);
	explicit Color4F(const Color4B& color);

	bool operator==(const Color4F& right) const;
	bool operator==(const Color3B& right) const;
	bool operator==(const Color4B& right) const;
	bool operator!=(const Color4F& right) const;
	bool operator!=(const Color3B& right) const;
	bool operator!=(const Color4B& right) const;

	bool equals(const Color4F &other) const
	{
		return (*this == other);
	}

	float r;
	float g;
	float b;
	float a;

	static const Color4F WHITE;
	static const Color4F YELLOW;
	static const Color4F BLUE;
	static const Color4F GREEN;
	static const Color4F RED;
	static const Color4F MAGENTA;
	static const Color4F BLACK;
	static const Color4F ORANGE;
	static const Color4F GRAY;
};

struct Tex2F
{
	Tex2F(float _u, float _v) : u(_u), v(_v) {}

	Tex2F() : u(0.f), v(0.f) {}

	float u;
	float v;
};


struct VertexBase
{
	Vec3 position;
};

struct V3F_C4F : VertexBase
{
	Color4F color;
};


struct V3F_T2F : VertexBase
{
	Tex2F texCoord;
};


struct M4F_M4F_M4F
{
	Mat4 mat1;
	Mat4 mat2;
	Mat4 mat3;

	M4F_M4F_M4F() :
		mat1(Mat4::IDENTITY),
		mat2(Mat4::IDENTITY),
		mat3(Mat4::IDENTITY)
	{}
};