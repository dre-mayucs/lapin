void start()
{
	while (true)
	{
		GetHitKeyStateAll(keys);
		for (auto i = 0; i < 256; i++) { oldkeys[i] = keys[i]; }
		ClearDrawScreen(); //ƒNƒŠƒA

		if (keys[KEY_INPUT_A])
		{
			break;
		}

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1) { break; }
	}
}