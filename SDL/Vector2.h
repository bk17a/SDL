#ifndef VECTOR2_H
#define VECTOR2_H
#include <istream>

class Vector2 {
public:
	float x;
	float y;

	Vector2() : x(0), y(0) {}
	Vector2(const float x, const float y) : x(x), y(y) {}

	// Add member functions for vector operations
	Vector2 operator+(const Vector2& other) const
	{
		return { x + other.x, y + other.y };
	}

	Vector2 operator-(const Vector2& other) const
	{
		return { x - other.x, y - other.y };
	}

	Vector2 operator*(const float scalar) const
	{
		return { x * scalar, y * scalar };
	}

	Vector2 operator/(const float scalar) const
	{
		return { x / scalar, y / scalar };
	}

	Vector2& operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	bool operator==(const Vector2& other) const
	{
		return (x == other.x) && (y == other.y);
	}

	static Vector2 ZERO_VEC()
	{
		return { 0, 0 };
	}

	// Calculate the squared length of the vector
	float calcVecLength() const
	{
		return x * x + y * y;
	}

	void normalize()
	{
		const auto length = static_cast<float>(sqrt(x * x + y * y));
		if (length != 0.0f)
		{
			x /= length;
			y /= length;
		}
	}

	// Overload the >> operator (input stream extraction)
	friend std::istream& operator>>(std::istream& input, Vector2& vec) {
		input >> vec.x >> vec.y;
		return input;
	}

	// Overload the << operator (output stream insertion)
	friend std::ostream& operator<<(std::ostream& output, const Vector2& vec) {
		output << "(" << vec.x << ", " << vec.y << ")";
		return output;
	}
};

#endif 