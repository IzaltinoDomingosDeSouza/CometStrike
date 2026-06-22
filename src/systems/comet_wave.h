#pragma once
#include "../components/wave.h"

#include <random>
std::mt19937 random_engine;

void comet_wave_system_update(entt::registry * world, float delta_time, ResourceManager * resource_manager,
							 std::function<void(entt::registry &, ResourceManager *,
							 TextureHandle, Transform, Velocity, Health, Damage)> create_comet,
							 TextureHandle * comet_textures, Vec2f screen_size)
{
	auto view = world->view<Wave>();
	for(auto entity : view)
	{
		auto & wave = view.get<Wave>(entity);

		wave.timer -= delta_time;

		if(wave.timer <= 0.0f)
		{
			float comet_damage[] = {10.0f, 25.0f, 75.0f, 100.0f};

			//Tiny = 50%, Small = 30%, Med = 15%, Big = 5%
			std::vector<double> frequency = {50.0, 30.0, 15.0, 5.0};
			std::discrete_distribution<size_t> comet_type_distribuition(frequency.begin(), frequency.end());

			size_t comet_type = comet_type_distribuition(random_engine);
			auto texture = comet_textures[comet_type];

			auto texture_info = resource_manager->get_texture_info(texture);
			std::uniform_real_distribution<float> comet_position_distribuition(texture_info.height, screen_size.y - texture_info.height);

			Vec2f comet_position = {screen_size.x + texture_info.width, comet_position_distribuition(random_engine)};
			create_comet(*world,resource_manager, texture,
						Transform{comet_position, 0}, Velocity{-100, 0.f},
						Health{100.f,100.f}, Damage{.amount = comet_damage[comet_type]});
			wave.timer = wave.spawn_rate;
		}
	}
}
