#pragma once

#include <random>
#include <vector>
#include <math.h>

struct Vec2 {
	Vec2(float _x, float _y) : x(_x), y(_y) {};
	float x{ 0.0f };
	float y{ 0.0f };

	inline float dot(const Vec2& other) { return other.x* x + other.y * y; };
};

struct Vec3 {
	Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	float x{ 0.0f };
	float y{ 0.0f };
	float z{ 0.0f };

	inline float dot(const Vec3& other) { return other.x * x + other.y * y + other.z * z; };
};

class Perlin
{
public:
	Perlin();
	~Perlin() {}
	void init();

	float noise1D(float x);
	float noise2D(float x, float y);
	float noise3D(float x, float y, float z);
	float octaves1D(float x, int octaves);
	float octaves2D(float x, float y, int octaves);
	float octaves3D(float x, float y, float z, int octaves);
	void set_frequency(float f) { frequency = f; };
	void set_persistence(float p) { persistence = p; };

private:
	float fade(float t);
	float lerp(float a0, float a1, float t);
	void create_permutation_table();
	void shuffle();
	float get_gradient1D(int x);
	Vec2 get_gradient2D(int x, int y);
	Vec3 get_gradient3D(int x, int y, int z);
	float normalise(float n, int dimensions);

	std::mt19937 r;
	std::random_device seeder;
	float frequency{ 0.01f };
	float persistence{ 0.5f };

	std::vector<int> permutations{};
};

