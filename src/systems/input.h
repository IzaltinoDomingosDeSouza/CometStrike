#pragma once
#include "../components/input.h"
#include "../components/movement.h"
#include "../components/player_controller.h"

void input_system_update(entt::registry * world)
{
	const bool * key_state = SDL_GetKeyboardState(NULL);

	//Test only to create a way to game application quit
	//if(key_state[SDL_SCANCODE_ESCAPE]) request_quit();

	auto view = world->view<Input, Movement, PlayerController>();
	for(auto entity : view)
    {
        auto & input = view.get<Input>(entity);
        auto & movement = view.get<Movement>(entity);
        auto & player = view.get<PlayerController>(entity);

        movement.direction = {0.f,0.f};
        
        bool up = false;
        bool down = false;
		if(player.index == 0)
		{
			up = key_state[SDL_SCANCODE_W];
			down = key_state[SDL_SCANCODE_S];
			input.shoot = key_state[SDL_SCANCODE_SPACE];
			
		}else if(player.index == 1)
		{
			up = key_state[SDL_SCANCODE_UP];
			down = key_state[SDL_SCANCODE_DOWN];
			input.shoot = key_state[SDL_SCANCODE_RSHIFT];
		}
		if(up)	   movement.direction.y -= 1.f;
		if(down) movement.direction.y += 1.f;
	}
}
