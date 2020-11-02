#include "Initialize_component.h"

void start()
{
	op();
}

void op()
{
	while (true)
	{
		for (auto i = 0; i < 256; i++) { oldkeys[i] = keys[i]; }
		GetHitKeyStateAll(keys);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		ClearDrawScreen(); //ƒNƒŠƒA



		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1 || keys[KEY_INPUT_ESCAPE]) { return; }
	}
}
