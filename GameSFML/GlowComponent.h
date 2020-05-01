#pragma once
#include "MaxxUtility.h"
#include "SFML/Graphics.hpp"
struct GlowComponent{};

struct GlowEffect
{
    struct Kernel
    {
        static constexpr int maxRadius = 7;
        int nTaps;
        std::array<float, maxRadius * 2 + 1> coefficients;
    };
    void SetMyKernelGauss(int radius, float sigma)
    {
        assert(radius <= k.maxRadius);
        const int nTaps = radius * 2 + 1;
        k.nTaps = nTaps;
        float sum = 0.0f;
        for (int i = 0; i < nTaps; i++)
        {
            const auto x = float(i - radius);
            const auto g = MaxU::gauss(x, sigma);
            sum += g;
            k.coefficients[i] = g;
        }
        for (int i = 0; i < nTaps; i++)
        {
            k.coefficients[i] = (float)k.coefficients[i] / sum;
        }
    }
	float scale = 1.0f;
	sf::Vector2u winSize;
	Kernel k;
	sf::Shader blurShader;
    sf::Shader addShader;
	sf::RenderStates state;
	sf::RenderTexture offscreen;
	sf::RenderTexture blurPass;
	sf::Sprite sp1;
	sf::Sprite sp2;
	sf::Sprite sp3;
};