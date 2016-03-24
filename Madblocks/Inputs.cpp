#include "Inputs.h"

void inputKeyboard(Inputs *input) {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(0);
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_LEFT)
				input->left = 1;
			else if (event.key.keysym.sym == SDLK_RIGHT)
				input->right = 1;
			else if (event.key.keysym.sym == SDLK_UP)
				input->up = 1;
			else if (event.key.keysym.sym == SDLK_DOWN)
				input->down = 1;
			else if (event.key.keysym.sym == SDLK_RETURN)
				input->enter = 1;
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				input->escape = 1;
			else if (event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z')
				input->letter = event.key.keysym.sym;
			//	else if (event.key.keysym.sym == SDLK_s)
			//		input->save = 1;
			else if (event.key.keysym.sym == SDLK_DELETE)
				input->erase = 1;
		}

	}
}


void inputJoystick(Inputs *input) {
	SDL_Event event;


	/*if (SDL_JoystickGetButton(infoGame->joystick, JOYSTICK_UP))
	input->up = 1;
	else if (SDL_JoystickGetButton(infoGame->joystick, JOYSTICK_RIGHT))
	input->right = 1;
	else if (SDL_JoystickGetButton(infoGame->joystick, JOYSTICK_DOWN))
	input->down = 1;
	else if (SDL_JoystickGetButton(infoGame->joystick, JOYSTICK_LEFT))
	input->left = 1;
	else if (SDL_JoystickGetButton(infoGame->joystick, JOYSTICK_A))
	input->enter = 1;
	else if (SDL_JoystickGetButton(infoGame->joystick, JOYSTICK_START))
	input->escape = 1;*/

	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_QUIT)
			exit(0);

		if (event.type == SDL_JOYBUTTONDOWN) {
			if (event.jbutton.button == JOYSTICK_UP)
				input->up = 1;
			else if (event.jbutton.button == JOYSTICK_RIGHT)
				input->right = 1;
			else if (event.jbutton.button == JOYSTICK_DOWN)
				input->down = 1;
			else if (event.jbutton.button == JOYSTICK_LEFT)
				input->left = 1;
			else if (event.jbutton.button == JOYSTICK_A)
				input->enter = 1;
			else if (event.jbutton.button == JOYSTICK_START)
				input->escape = 1;
		}
		else if (event.type == SDL_JOYAXISMOTION) {
			if (event.jaxis.which == 0)
			{
				if (event.jaxis.axis == 0)
				{
					if ((event.jaxis.value > -DEAD_ZONE) && (event.jaxis.value < DEAD_ZONE))
					{
						input->right = 0;
						input->left = 0;
					}
					else
					{
						if (event.jaxis.value < -DEAD_ZONE)
						{
							input->right = 0;
							input->left = 1;
						}
						else if (event.jaxis.value > DEAD_ZONE)
						{
							input->right = 1;
							input->left = 0;
						}
					}
				}

				else if (event.jaxis.axis == 1)
				{
					if ((event.jaxis.value > -DEAD_ZONE) && (event.jaxis.value < DEAD_ZONE))
					{
						input->up = 0;
						input->down = 0;
					}
					else
					{
						if (event.jaxis.value < 0)
						{
							input->up = 1;
							input->down = 0;
						}
						else
						{
							input->up = 0;
							input->down = 1;
						}
					}
				}
			}
		}

	}
}

void openJoystick(int indexJoystick) {
	getInfoGame()->joystick = SDL_JoystickOpen(indexJoystick);
	if (getInfoGame()->joystick == NULL)
		printf("Error on opening joystick %d\n", indexJoystick);
}

void getKey(Inputs *input)
{
	if (getInfoGame()->joystick != NULL) {
		inputJoystick(input);
		inputKeyboard(input);
	}
	else {
		if (SDL_NumJoysticks() > 0)
			openJoystick(0);
		inputKeyboard(input);
	}
}

void resetInputs(Inputs *input) {
	input->up = 0;
	input->right = 0;
	input->down = 0;
	input->left = 0;
	input->enter = 0;
	input->escape = 0;
	input->save = 0;
	input->erase = 0;
	input->letter = 0;
}