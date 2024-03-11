#pragma once
#pragma once

class Vector2 {

public:
	Vector2();
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	Vector2(float x, float y);
	float magnitude();
	float distance(const Vector2& other);
	void normalize();
	/// <summary>
	/// Setea el valor de x a lo que se le da
	/// </summary>
	/// <param name="x"></param>
	void setX(float x);
	/// <summary>
	/// Setea el valor de y a lo que se le da
	/// </summary>
	/// <param name="y"></param>
	void setY(float y);
	/// <summary>
	/// Obtiene el valor de x
	/// </summary>
	/// <returns></returns>
	float getX() const;
	/// <summary>
	/// Obtiene el valor de y
	/// </summary>
	/// <returns></returns>
	float getY() const;


	// ---------- Operadores ------------

	Vector2 operator+(const Vector2& other);
	Vector2 operator-(const Vector2& other);
	Vector2 operator+=(const Vector2& other);
	Vector2 operator-=(const Vector2& other);

	Vector2 operator*(float c);
	Vector2 operator*=(float c);

private:

	float x, y;
};

