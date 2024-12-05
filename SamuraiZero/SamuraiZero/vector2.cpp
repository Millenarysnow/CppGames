#pragma once

class Vector2
{
public:
	float x = 0;
	float y = 0;

public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y)
		: x(x), y(y) { }

	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}
};