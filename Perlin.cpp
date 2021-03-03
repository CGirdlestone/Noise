#include "Perlin.hpp"

Perlin::Perlin()
{
	r.seed(seeder());
}

void Perlin::init()
{
	create_permutation_table();
}

float Perlin::noise1D(float x)
{
	auto X = static_cast<int>(x) & 255;

	auto xf = x - static_cast<float>(static_cast<int>(x));
	
	auto u = fade(xf);

	auto right = xf - 1.0f;
	auto left = xf;

	auto n = lerp(left * get_gradient1D(X), right * get_gradient1D(X + 1), u);

	return normalise(n, 1);
}

float Perlin::noise2D(float x, float y)
{
	auto X = static_cast<int>(x) & 255;
	auto Y = static_cast<int>(y) & 255;

	auto xf = x - static_cast<float>(static_cast<int>(x));
	auto yf = y - static_cast<float>(static_cast<int>(y));

	auto u = fade(xf);
	auto v = fade(yf);

	auto top_left = Vec2(xf, yf - 1.0f);
	auto top_right = Vec2(xf - 1.0f, yf - 1.0f);
	auto bottom_right = Vec2(xf - 1.0f, yf);
	auto bottom_left = Vec2(xf, yf);

	auto c0 = lerp(bottom_left.dot(get_gradient2D(X, Y)), bottom_right.dot(get_gradient2D(X + 1, Y)), u);
	auto c1 = lerp(top_left.dot(get_gradient2D(X, Y + 1)), top_right.dot(get_gradient2D(X + 1, Y + 1)), u);

	auto n = lerp(c0, c1, v);

	return normalise(n, 2);
}

float Perlin::noise3D(float x, float y, float z)
{
	auto X = static_cast<int>(x) & 255;
	auto Y = static_cast<int>(y) & 255;
	auto Z = static_cast<int>(z) & 255;

	auto xf = x - static_cast<float>(static_cast<int>(x));
	auto yf = y - static_cast<float>(static_cast<int>(y));
	auto zf = z - static_cast<float>(static_cast<int>(z));

	auto u = fade(xf);
	auto v = fade(yf);
	auto w = fade(zf);

	auto top_left_low = Vec3(xf, yf - 1.0f, zf);
	auto top_right_low = Vec3(xf - 1.0f, yf - 1.0f, zf);
	auto bottom_right_low = Vec3(xf - 1.0f, yf, zf);
	auto bottom_left_low = Vec3(xf, yf, zf);

	auto top_left_top = Vec3(xf, yf - 1.0f, zf);
	auto top_right_top = Vec3(xf - 1.0f, yf - 1.0f, zf);
	auto bottom_right_top = Vec3(xf - 1.0f, yf, zf);
	auto bottom_left_top = Vec3(xf, yf, zf);

	auto b0 = lerp(bottom_left_low.dot(get_gradient3D(X, Y, Z)), bottom_right_low.dot(get_gradient3D(X + 1, Y, Z)), u);
	auto b1 = lerp(top_left_low.dot(get_gradient3D(X, Y + 1, Z)), top_right_low.dot(get_gradient3D(X + 1, Y + 1, Z)), u);
	auto c0 = lerp(b0, b1, v);

	auto b2 = lerp(bottom_left_top.dot(get_gradient3D(X, Y, Z + 1)), bottom_right_top.dot(get_gradient3D(X + 1, Y, Z + 1)), u);
	auto b3 = lerp(top_left_top.dot(get_gradient3D(X, Y + 1, Z + 1)), top_right_top.dot(get_gradient3D(X + 1, Y + 1, Z + 1)), u);
	auto c1 = lerp(b2, b3, v);

	auto n = lerp(c0, c1, w);

	return normalise(n, 3);
}

float Perlin::octaves1D(float x, int octaves)
{
	float max_val{ 0.0f };
	float n{ 0.0f };
	float amplitude = 1.0f;
	auto _frequency = frequency;

	for (int _ = 0; _ < octaves; _++) {
		n += amplitude * noise1D(x * _frequency);
		max_val += amplitude;
		amplitude *= persistence;
		_frequency *= 2.0f;
	}

	return n / max_val;
}

float Perlin::octaves2D(float x, float y, int octaves)
{
	float max_val{ 0.0f }; 
	float n{ 0.0f };
	float amplitude = 1.0f;
	auto _frequency = frequency;
	
	for (int _ = 0; _ < octaves; _++) {
		n += amplitude * noise2D(x * _frequency, y * _frequency);
		max_val += amplitude;
		amplitude *= persistence;
		_frequency *= 2.0f;
	}

	return n / max_val;
}

float Perlin::octaves3D(float x, float y, float z, int octaves)
{
	float max_val{ 0.0f };
	float n{ 0.0f };
	float amplitude = 1.0f;
	auto _frequency = frequency;

	for (int _ = 0; _ < octaves; _++) {
		n += amplitude * noise3D(x * _frequency, y * _frequency, z * _frequency);
		max_val += amplitude;
		amplitude *= persistence;
		_frequency *= 2.0f;
	}

	return n / max_val;
}

float Perlin::fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float Perlin::lerp(float a0, float a1, float t)
{
	return a0 + (a1 - a0) * t;
}

void Perlin::create_permutation_table()
{
	for (int i = 0; i < 256; i++) {
		permutations.push_back(i);
	}
	shuffle();
	std::vector<int> p{};
	for (int i = 0; i < 512; i++) {
		p.push_back(permutations[i % 256]);
	}
	permutations = p;
}

void Perlin::shuffle()
{
	std::vector<int> p{};

	while (!permutations.empty()) {
		if (permutations.size() == 1) {
			p.push_back(permutations[0]);
			break;
		}
		auto i = r() % permutations.size();
		p.push_back(permutations[i]);
		auto it = permutations.begin() + i;
		permutations.erase(it);
	}
	permutations = p;
}

float Perlin::get_gradient1D(int x)
{
	auto val = permutations[x] & 1;
	if (val == 0) {
		return 1.0f;
	}
	else  {
		return -1.0f;
	}
}

Vec2 Perlin::get_gradient2D(int x, int y)
{
	int i = permutations[permutations[x] + y];
	auto val = i & 3;
	if (val == 0) {
		return Vec2(1.0f, -1.0f);
	}
	else if (val == 1) {
		return Vec2(-1.0f, 1.0f);
	}
	else if (val == 2) {
		return Vec2(1.0f, 1.0f);
	}
	else {
		return Vec2(-1.0f, -1.0f);
	}
}

Vec3 Perlin::get_gradient3D(int x, int y, int z)
{
	int i = permutations[permutations[permutations[x] + y] + z];
	auto val = i & 7;
	if (val == 0) {
		return Vec3(1.0f, -1.0f, 1.0f);
	}
	else if (val == 1) {
		return Vec3(-1.0f, 1.0f, 1.0f);
	}
	else if (val == 2) {
		return Vec3(1.0f, 1.0f, 1.0f);
	} 
	else if (val == 3) {
		return Vec3(-1.0f, -1.0f, 1.0f);
	}
	else if (val == 4) {
		return Vec3(1.0f, -1.0f, -1.0f);
	}
	else if (val == 5) {
		return Vec3(-1.0f, 1.0f, -1.0f);
	}
	else if (val == 6){
		return Vec3(1.0f, 1.0f, -1.0f);
	}
	else {
		return Vec3(-1.0f, -1.0f, -1.0f);
	}
}

float Perlin::normalise(float n, int dimensions)
{
	if (dimensions == 1) {
		n += 1.0f;
		n /= 2.0f;
	}
	else if (dimensions == 2) {
		n += sqrt(2.0f);
		n /= (2.0f * sqrt(2.0f));
	}
	else {
		n += sqrt(3.0f);
		n /= (2.0f * sqrt(3.0f));
	}

	return n;
}
