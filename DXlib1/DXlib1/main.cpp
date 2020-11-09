#include "Initialize_component.h"

void menu()
{
	while (true)
	{
		//キー情報取得
		for (auto i = 0; i < 256; i++) { oldkeys[i] = keys[i]; }
		GetHitKeyStateAll(keys);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//マウス座標/状態取得
		oldMouse = Mouse;
		Mouse = GetMouseInput();
		GetMousePoint(&Mouse_X, &Mouse_Y);

		//画面クリア
		ClearDrawScreen();

		

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1 || keys[KEY_INPUT_ESCAPE]) { return; }
	}
}

void Character_Move(int pos[]);
void Start()
{
	//character_animation frame
	short *frame = INIT_NUM, frame_cache = INIT_NUM;
	int character_pos[2] = {
		250, 250
	};

	//character_resrouces
	int character[CHAR_FRAME];
	LoadDivGraph("Resources\\chr\\char1.png", CHAR_FRAME, CHAR_FRAME, 1, 64, 64, character);

	//background_images
	int BG_X[2] = { INIT_NUM ,WIN_WIDTH };
	int BG = LoadGraph("Resources\\Background\\background.png");

	//object_images
	int defoliation_brock[DEFOLIATION_NUM];
	LoadDivGraph("Resources\\Object\\defoliation-Sheet.png", DEFOLIATION_NUM, DEFOLIATION_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, defoliation_brock);
	int jump_brock[JUMP_NUM];
	LoadDivGraph("Resources\\Object\\jump-Sheet.png", JUMP_NUM, JUMP_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, jump_brock);
	int nomal_block[NOMAL_NUM];
	LoadDivGraph("Resources\\Object\\block.png", NOMAL_NUM, NOMAL_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, nomal_block);

	//UI_images
	int UI = LoadGraph("Resources\\UI\\bar.png");

	int brocks_pos[][BLOCK_TYPE_NUM] =
	{
		{10, 220},
		{10, 135},
		{10, 60}
	};

	while (true)
	{
		//キー情報取得
		for (auto i = 0; i < 256; i++) { oldkeys[i] = keys[i]; }
		GetHitKeyStateAll(keys);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//マウス座標/状態取得
		oldMouse = Mouse;
		Mouse = GetMouseInput();
		GetMousePoint(&Mouse_X, &Mouse_Y);

		//画面クリア
		ClearDrawScreen();

		DrawGraph(0, 0, BG, true);
		for (auto i = INIT_NUM; i < BLOCK_TYPE_NUM; i++) {
			if (Mouse == MOUSE_INPUT_LEFT &&
				Collision.box_Fanc(
					Mouse_X, Mouse_X,
					Mouse_Y, Mouse_Y,
					(double)brocks_pos[i][BLOCK_X], (double)brocks_pos[i][BLOCK_X] + BROCKS_SIZE,
					(double)brocks_pos[i][BLOCK_Y], (double)brocks_pos[i][BLOCK_Y] + BROCKS_SIZE))
			{
				brocks_pos[i][0] = Mouse_X - BLOCK_SIZE / 2;
				brocks_pos[i][1] = Mouse_Y - BLOCK_SIZE / 2;
			}
		}
		DrawGraph(0, 0, UI, true);
		DrawGraph(brocks_pos[0][BLOCK_X], brocks_pos[0][BLOCK_Y], defoliation_brock[INIT_NUM], true);
		DrawGraph(brocks_pos[1][BLOCK_X], brocks_pos[1][BLOCK_Y], nomal_block[INIT_NUM], true);
		DrawGraph(brocks_pos[2][BLOCK_X], brocks_pos[2][BLOCK_Y], jump_brock[INIT_NUM], true);

		Character_Animation(frame, );
		DrawGraph(character_pos[0], character_pos[1], character[frame], true);

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1 || keys[KEY_INPUT_ESCAPE]) { return; }
	}
}

void Character_Move(int pos[])
{
	pos[0] += 10;
}