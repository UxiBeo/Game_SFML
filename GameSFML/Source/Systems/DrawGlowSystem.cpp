#include "..\..\System\DrawGlowSystem.h"
#include "../../GlowComponent.h"
#include "../../Graphics.h"
void DrawGlowSystem::Update(entt::registry& ECS) const
{
	auto& ge = ECS.ctx<GlowEffect>();
	auto view = ECS.ctx<Graphics>().GetView();
	ge.offscreen.setView(view);
	ge.sp3.setPosition(view.getCenter());

	ge.offscreen.clear();
	ge.blurPass.clear();

	
	ECS.view<GlowComponent, sf::Sprite>().each([&ge](auto entity, auto, const sf::Sprite& sp) {
		ge.offscreen.draw(sp);
		});
	ge.offscreen.display();

	ge.state.blendMode = sf::BlendNone;
	assert(ge.winSize.x != 0);
	ge.blurPass.draw(ge.sp1);
	ge.blurPass.display();
	ge.state.shader = &ge.blurShader;
	ge.blurShader.setUniform("offsetFactor", sf::Vector2f(0.0f, 1.0f / ge.winSize.y));
	ge.blurPass.draw(ge.sp2, ge.state);
	ge.blurShader.setUniform("offsetFactor", sf::Vector2f(1.0f / ge.winSize.x, 0.0f));
	ge.blurPass.draw(ge.sp2, ge.state);
	ge.blurPass.display();
	ge.state.shader = &ge.addShader;
	ge.state.blendMode = sf::BlendAdd;
}

void DrawGlowSystem::BeginPlay(entt::registry& ECS) const
{
	GlowEffect& ge = ECS.set<GlowEffect>();
	ge.winSize = ECS.ctx<Graphics>().getRenderWindow().getSize();
	ge.scale = 4.0f;
	int radius = 5;
	float sigma = 3.0f;
	ge.SetMyKernelGauss(5, 3.0f);

	ge.blurShader.loadFromFile("Data/Shader/GuassianBlur.frag", sf::Shader::Fragment);
	ge.blurShader.setUniformArray("coefficients", &ge.k.coefficients[0], 15);
	ge.blurShader.setUniform("r", radius);
	ge.blurShader.setUniform("source", sf::Shader::CurrentTexture);

	ge.addShader.loadFromFile("Data/Shader/Add.frag", sf::Shader::Fragment);
	ge.offscreen.create(ge.winSize.x, ge.winSize.y);
	ge.blurPass.create(ge.winSize.x / ge.scale, ge.winSize.y / ge.scale);
	ge.blurPass.setSmooth(true);

	ge.sp1.setTexture(ge.offscreen.getTexture());
	ge.sp1.scale({ 1.0f / ge.scale, 1.0f / ge.scale });

	ge.sp2.setTexture(ge.blurPass.getTexture());
	ge.sp3.setTexture(ge.offscreen.getTexture());
	ge.sp3.setOrigin((float)ge.winSize.x / 2.0f, (float)ge.winSize.y / 2.0f);
	ge.addShader.setUniform("source", ge.offscreen.getTexture());
	ge.addShader.setUniform("bloom", ge.blurPass.getTexture());
}
