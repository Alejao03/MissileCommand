#pragma once
class Utils {
public:
	/// <summary>
	/// Devuelve un entero que esta en el intervalo [min,max]
	/// </summary>
	/// <param name="min">minimo del intervalo</param>
	/// <param name="max">maximo del intervalo</param>
	/// <returns>valor que pertence [min,max]</returns>
	static int randomInt(int min, int max);
	static float randomFloat(float min, float max);
};