#include "Initialize_component.h"

int brocks_pos[][BLOCK_TYPE_NUM] =
{
	{10, 220},
	{10, 135},
	{10, 60}
};

void menu()
{
	int frame = 0;
	int cache_frame = 0;
	const int Max_frame = 34;
	const int adjust_frame = 8;

	int Background = LoadGraph("Resources\\Background\\background.png", true);
	int Mask_image[Max_frame];
	LoadDivGraph("Resources\\Background\\animation_title.png", 34, 34, 1, 800, 450, Mask_image);

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

		DrawGraph(0, 0, Background, false);
		DrawGraph(0, 0, Mask_image[frame], true);

		if (frame == Max_frame - 1) {
			frame = 0;
		}
		if (cache_frame == adjust_frame) {
			frame++;
			cache_frame = 0;
		}
		cache_frame++;

		if (click) { return; }

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1 || keys[KEY_INPUT_ESCAPE]) { return; }
	}
}

//void Character_Move(int pos[]);
void Start()
{
	//Load World
	World_inport();

	#pragma region 変数宣言/定義
	int World_adjust = 0;

	//background_images
	int BG_X[2] = { INIT_NUM,WIN_WIDTH };
	int BG = LoadGraph("Resources\\Background\\background.png");

	//object_images
	int defoliation_brock[DEFOLIATION_NUM];
	int jump_brock[JUMP_NUM];
	int nomal_block[NOMAL_NUM];
	LoadDivGraph("Resources\\Object\\defoliation-Sheet.png", DEFOLIATION_NUM, DEFOLIATION_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, defoliation_brock);
	LoadDivGraph("Resources\\Object\\jump-Sheet.png", JUMP_NUM, JUMP_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, jump_brock);
	LoadDivGraph("Resources\\Object\\block.png", NOMAL_NUM, NOMAL_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, nomal_block);

	//UI_images
	int UI = LoadGraph("Resources\\UI\\bar.png");
	int scrollArrow = LoadGraph("Resources\\UI\\scroll.png");//z
	bool start_button_flag = false;
	int UI_Button[2] = {
		LoadGraph("Resources\\UI\\start1.png"),
		LoadGraph("Resources\\UI\\stop1.png")
	};
	int UI_Button_pos[2][2] = {
		{25, 300},
		{100, 300}
	};

	int blocks_tmp[BLOCK_TYPE_NUM][BLOCK_TYPE_NUM];
	for (int i = INIT_NUM; i < BLOCK_TYPE_NUM; i++) {
		blocks_tmp[i][BLOCK_X] = brocks_pos[i][BLOCK_X];
		blocks_tmp[i][BLOCK_Y] = brocks_pos[i][BLOCK_Y];
	}

	int arrowWidth = 32;//z
	int arrowHeight = 64;//z
	#pragma endregion

	while (true) {
		#pragma region 初期化
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
		#pragma endregion

		//背景スクロール
		BG_scroll(BG_X, &World_adjust, Mouse_X, Mouse_Y);//z

		#pragma region オブジェクト当たり判定
		//ブロック同士の当たり判定
		if (Mouse == MOUSE_INPUT_LEFT) {
			if (collision_defoliation_mouse() == true) {
				collision_block_otherblock(&Mouse_X, &Mouse_Y, blocks_tmp, 0);
			}
			else if (collision_normal_mouse() == true) {
				collision_block_otherblock(&Mouse_X, &Mouse_Y, blocks_tmp, 1);
			}
			else if (collision_jump_mouse() == true) {
				collision_block_otherblock(&Mouse_X, &Mouse_Y, blocks_tmp, 2);
			}
		}
		#pragma endregion

		#pragma region 描画処理
		//背景描画
		DrawGraph(BG_X[0], 0, BG, TRUE);//z
		DrawGraph(BG_X[1], 0, BG, TRUE);//z

		//既存ワールドデータ描画
		for (auto i = 0; i < world_value; i++) {
			DrawGraph(world[i][BLOCK_X] + World_adjust, world[i][BLOCK_Y], nomal_block[INIT_NUM], true); //通常ブロック
		}

		//インターフェース描画
		DrawGraph(0, 0, UI, true);
		DrawGraph(WIN_WIDTH - arrowWidth, WIN_HEIGHT / 2 - arrowHeight / 2, scrollArrow, TRUE);//前進//z
		DrawTurnGraph(192, WIN_HEIGHT / 2 - arrowHeight / 2, scrollArrow, TRUE);//後退//z

		//再生/停止ボタン描画_当たり判定処理
		for (auto i = 0; i < 2; i++){
			//当たり判定
			bool *collision_start = new bool(Collision.Trigonometric_Fanc(UI_Button_pos[i][0], UI_Button_pos[i][1], 32, Mouse_X, Mouse_Y, 32));
			//描画処理
			DrawGraph(UI_Button_pos[i][0], UI_Button_pos[i][1], UI_Button[i], true);

			//マウスが再生ボタンを押した場合ゲームスタート
			if (i == 0 && *collision_start == true) {
				if (Mouse != MOUSE_INPUT_LEFT && oldMouse == MOUSE_INPUT_LEFT) {
					start_button_flag = true;
				}
			}
			delete collision_start;
		}

		//ブロック描画
		for (auto i = 0; i < 3; i++) {
			if (brocks_pos[i][BLOCK_X] == 10) {
				if (i == 0) {
					DrawGraph(brocks_pos[0][BLOCK_X], brocks_pos[0][BLOCK_Y], defoliation_brock[INIT_NUM], true); //落下ブロック
				}
				else if (i == 1) {
					DrawGraph(brocks_pos[1][BLOCK_X], brocks_pos[1][BLOCK_Y], nomal_block[INIT_NUM], true); //通常ブロック
				}
				else {
					DrawGraph(brocks_pos[2][BLOCK_X], brocks_pos[2][BLOCK_Y], jump_brock[INIT_NUM], true); //ジャンプブロック
				}
			}
			else {
				if (i == 0) {
					DrawGraph(brocks_pos[0][BLOCK_X] + World_adjust, brocks_pos[0][BLOCK_Y], defoliation_brock[INIT_NUM], true); //落下ブロック
				}
				else if (i == 1) {
					DrawGraph(brocks_pos[1][BLOCK_X] + World_adjust, brocks_pos[1][BLOCK_Y], nomal_block[INIT_NUM], true); //通常ブロック
				}
				else {
					DrawGraph(brocks_pos[2][BLOCK_X] + World_adjust, brocks_pos[2][BLOCK_Y], jump_brock[INIT_NUM], true); //ジャンプブロック
				}
			}
		}
		#pragma endregion

		#pragma region フレーム終了処理
		if(start_button_flag == true) {
			for (auto i = 0; i < 3; i++) {
				brocks_pos[i][0] += World_adjust;
			}
			break;
		}

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1 || keys[KEY_INPUT_ESCAPE]) { return; }
		#pragma endregion
	}
}

void stage()
{
	#pragma region 変数宣言/定義
	//ワールドデータ複製
	int world_cache[100][2];
	int user_brock_pos[3][2];
	for (auto i = 0; i < 100; i++) {
		for (auto j = 0; j < 2; j++) {
			//ワールド読み込み時の座標の複製
			world_cache[i][j] = world[i][j];

			//ユーザーが置いたブロック座標の複製
			if (i < 3) {
				user_brock_pos[i][j] = brocks_pos[i][j];
			}
		}
	}

	//背景画像読み込み
	int Background = LoadGraph("Resources\\Background\\background.png", true);
	int BG_position[2] = { 0, WIN_WIDTH };

	//キャラクター画像/フレーム/フラグ
	bool gravity_flag = false;
	bool brock_collision_flag[2] = {false, false};
	bool world_jump_flag[100][2];
	for (auto i = 0; i < 2; i++) {
		world_jump_flag[i][0] = false;
		world_jump_flag[i][1] = false;
	}
	bool brock_flag[3][2] = {
		{ false, false },
		{ false, false },
		{ false, false }
	};

	bool char_animation_flag[2] = { false, false };
	int char_animation_frame[2][2];
	for (auto i = 0; i < 2; i++) {
		char_animation_frame[i][0] = 0;
		char_animation_frame[i][1] = 0;
	}

	int old_block_pos_y = WIN_HEIGHT;
	int old_user_block_pos_y = WIN_HEIGHT;
	short frame = INIT_NUM, frame_cache = INIT_NUM;
	int character_pos_x = 100;
	int character_pos_y = 325;

	int character[CHAR_FRAME];
	int character_jump[8];
	int character_fall[8];
	LoadDivGraph("Resources\\chr\\char1.png", CHAR_FRAME, CHAR_FRAME, 1, 64, 64, character);
	LoadDivGraph("Resources\\chr\\rabbits_jump.png", 8, 8, 1, 64, 64, character_jump);
	LoadDivGraph("Resources\\chr\\rabbits_fall.png", 8, 8, 1, 64, 64, character_fall);

	//object_images
	int defoliation_brock[DEFOLIATION_NUM];
	int jump_brock[JUMP_NUM];
	int nomal_block[NOMAL_NUM];
	LoadDivGraph("Resources\\Object\\defoliation-Sheet.png", DEFOLIATION_NUM, DEFOLIATION_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, defoliation_brock);
	LoadDivGraph("Resources\\Object\\jump-Sheet.png", JUMP_NUM, JUMP_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, jump_brock);
	LoadDivGraph("Resources\\Object\\block.png", NOMAL_NUM, NOMAL_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, nomal_block);
	#pragma endregion

	while (true)
	{
		#pragma region 初期化処理
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
		#pragma endregion

		#pragma region キャラクター処理
		//キャラクタージャンプ処理
		for (auto i = 0; i < world_value; i++) {
			bool char_collision_adjust =
				Collision.box_Fanc(character_pos_x , (double)character_pos_x + 1, (double)character_pos_y + 64, (double)character_pos_y + 65,
					world_cache[i][0], (double)world_cache[i][0] + 64, world_cache[i][1], (double)world_cache[i][1] + 64);

			bool char_collision =
				Collision.box_Fanc(character_pos_x, (double)character_pos_x + 64, character_pos_y, (double)character_pos_y + 64,
					world_cache[i][0], (double)world_cache[i][0] + 64, world_cache[i][1], (double)world_cache[i][1] + 64);

			if (char_collision) {
				world_jump_flag[i][0] = true;
				char_animation_flag[0] = true;
				brock_collision_flag[0] = true;
			}
			else if (character_pos_y < 325 && !(char_collision_adjust)) {
				gravity_flag = true;
			}

			if (world_jump_flag[i][0] == true && world_jump_flag[i][1] == false) {
				character_pos_y -= 64;
				world_jump_flag[i][1] = true;
			}
		}
		#pragma endregion

		if (gravity_flag == true) {
			while (true) {
				if (character_pos_y == 325 || char_animation_flag[0] == true) {
					gravity_flag = false;
					break;
				}
				else {
					character_pos_y++;
				}
			}
		}

		#pragma region ユーザー定義ブロック判定
		/// <summary>
		/// <param name="brock_flag[0]"> 落下ブロック	 </param>
		/// <param name="brock_flag[1]"> 通常ブロック	 </param>
		/// <param name="brock_flag[2]"> ジャンプブロック</param>
		/// </summary>
		//当たり判定
		for (auto i = 0; i < 3; i++) {
			bool b_collision_rear =
				Collision.box_Fanc(character_pos_x, (double)character_pos_x, character_pos_y, (double)character_pos_y + 64,
					user_brock_pos[i][0], (double)user_brock_pos[i][0] + 64, user_brock_pos[i][1], (double)user_brock_pos[i][1] + 64);

			bool b_collision_front =
				Collision.box_Fanc(character_pos_x, (double)character_pos_x + 64, character_pos_y, (double)character_pos_y + 64,
					user_brock_pos[i][0], (double)user_brock_pos[i][0] + 64, user_brock_pos[i][1], (double)user_brock_pos[i][1] + 64);

			if (i == 1 && b_collision_front) {
				brock_flag[i][0] = true;
				brock_collision_flag[1] = true;
			}
			else if (b_collision_rear) {
				brock_flag[i][0] = true;
				brock_collision_flag[1] = true;
			}
			else { brock_collision_flag[1] = false; }
		}

		//落下ブロック機能
		if (brock_flag[0][0] == true && brock_flag[0][1] == false) {
			while (character_pos_y <= 325) {
				character_pos_y++;
			}
			brock_flag[0][1] = true;
			char_animation_flag[1] = true;
		}

		//通常ブロック機能
		if (brock_flag[1][0] == true && brock_flag[1][1] == false) {
			character_pos_y -= 64;
			brock_flag[1][1] = true;
			char_animation_flag[0] = true;
		}

		//ジャンプブロック機能
		if (brock_flag[2][0] == true && brock_flag[2][1] == false) {
			character_pos_y -= 128;
			brock_flag[2][1] = true;
			char_animation_flag[0] = true;
		}
		#pragma endregion

		#pragma region 背景処理
		//背景座標更新処理
		for (auto i = 0; i < 2; i++) {
			if (BG_position[0] == -WIN_WIDTH) {
				BG_position[0] = WIN_WIDTH;
			}
			if (BG_position[1] == -WIN_WIDTH) {
				BG_position[1] = WIN_WIDTH;
			}

			BG_position[0]--;
			BG_position[1]--;

			for (auto brock_x = 0; brock_x < world_value; brock_x++) {
				world_cache[brock_x][0]--;
			}
			for (auto brock_x = 0; brock_x < 3; brock_x++) {
				user_brock_pos[brock_x][0]--;
			}
		}

		//背景描画
		DrawGraph(BG_position[0], 0, Background, true);
		DrawGraph(BG_position[1], 0, Background, true);

		//既存ワールドデータ描画
		for (auto i = 0; i < world_value; i++) {
			DrawGraph(world_cache[i][BLOCK_X], world_cache[i][BLOCK_Y], nomal_block[INIT_NUM], true); //通常ブロック
		}

		//ユーザーが置いたブロックの描画
		DrawGraph(user_brock_pos[0][BLOCK_X], user_brock_pos[0][BLOCK_Y], defoliation_brock[INIT_NUM], true); //落下ブロック
		DrawGraph(user_brock_pos[1][BLOCK_X], user_brock_pos[1][BLOCK_Y], nomal_block[INIT_NUM], true); //通常ブロック
		DrawGraph(user_brock_pos[2][BLOCK_X], user_brock_pos[2][BLOCK_Y], jump_brock[INIT_NUM], true); //ジャンプブロック
		#pragma endregion

		#pragma region キャラクターアニメーション
		if (char_animation_flag[0]) {
			DrawGraph(character_pos_x, character_pos_y, character_jump[char_animation_frame[0][1]], true);
			if (char_animation_frame[0][0] == 3) {
				char_animation_frame[0][1]++;
				char_animation_frame[0][0] = 0;
			}
			if (char_animation_frame[0][1] == 8) {
				char_animation_frame[0][1] = 0;
				char_animation_flag[0] = false;
			}
			char_animation_frame[0][0]++;
		}
		else if (char_animation_flag[1]) {
			DrawGraph(character_pos_x, character_pos_y, character_fall[char_animation_frame[1][1]], true);
			if (char_animation_frame[1][0] == 3) {
				char_animation_frame[1][1]++;
				char_animation_frame[1][0] = 0;
			}
			if (char_animation_frame[1][1] == 8) {
				char_animation_frame[1][1] = 0;
				char_animation_flag[1] = false;
			}
			char_animation_frame[1][0]++;
		}
		else {
			DrawGraph(character_pos_x, character_pos_y, character[frame], true);
			if (frame_cache == 3) {
				frame++;
				frame_cache = 0;
			}
			if (frame == CHAR_FRAME) {
				frame = 0;
			}
			frame_cache++;
		}
		#pragma endregion

		#pragma region フレーム終了処理
		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1 || keys[KEY_INPUT_ESCAPE]) { return; }
		#pragma endregion
	}
}

#pragma region utility_fanction
//背景のスクロール
void BG_scroll(int *BgX, int *adjust, const int &mouseX, const int &mouseY)//z
{
	bool keyLeftInput = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && oldMouse == !(GetMouseInput() & MOUSE_INPUT_LEFT);
	bool scrollLeftFlag = mouseX > 192 && mouseX < 192 + 32 && mouseY > WIN_HEIGHT / 2 - 32 && mouseY < WIN_HEIGHT / 2 + 32;
	bool scrollRightFlag = mouseX > WIN_WIDTH - 32 && mouseX < WIN_WIDTH &&mouseY > WIN_HEIGHT / 2 - 32 && mouseY < WIN_HEIGHT / 2 + 32;

	int scrollVel = 100;

	if (scrollRightFlag && keyLeftInput)
	{
		BgX[0] -= scrollVel;
		BgX[1] -= scrollVel;
		*adjust -= scrollVel;
	}
	if (scrollLeftFlag && keyLeftInput)
	{
		BgX[0] += scrollVel;
		BgX[1] += scrollVel;
		*adjust += scrollVel;
	}
	for (int i = 0; i < 2; ++i)
	{
		if (BgX[i] < -WIN_WIDTH)
		{
			BgX[i] = WIN_WIDTH - scrollVel;
		}
		if (BgX[i] > WIN_WIDTH)
		{
			BgX[i] = -WIN_WIDTH + scrollVel;
		}
	}
}

//落ち葉ブロックと通常ブロックの衝突判定
bool collision_defoliation_normal() {
	return Collision.box_Fanc(
		static_cast<double>(brocks_pos[0][BLOCK_X]),
		static_cast<double>(brocks_pos[0][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[0][BLOCK_Y]),
		static_cast<double>(brocks_pos[0][BLOCK_Y]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[1][BLOCK_X]),
		static_cast<double>(brocks_pos[1][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[1][BLOCK_Y]),
		static_cast<double>(brocks_pos[1][BLOCK_Y]) + BROCKS_SIZE);
}

//通常ブロックとジャンプブロックの衝突判定
bool collision_normal_jump() {
	return Collision.box_Fanc(
		static_cast<double>(brocks_pos[1][BLOCK_X]),
		static_cast<double>(brocks_pos[1][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[1][BLOCK_Y]),
		static_cast<double>(brocks_pos[1][BLOCK_Y]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[2][BLOCK_X]),
		static_cast<double>(brocks_pos[2][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[2][BLOCK_Y]),
		static_cast<double>(brocks_pos[2][BLOCK_Y]) + BROCKS_SIZE);
}

//ジャンプブロックと落ち葉ブロックの衝突判定
bool collision_jump_defoliation() {
	return Collision.box_Fanc(
		static_cast<double>(brocks_pos[0][BLOCK_X]),
		static_cast<double>(brocks_pos[0][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[0][BLOCK_Y]),
		static_cast<double>(brocks_pos[0][BLOCK_Y]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[2][BLOCK_X]),
		static_cast<double>(brocks_pos[2][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[2][BLOCK_Y]),
		static_cast<double>(brocks_pos[2][BLOCK_Y]) + BROCKS_SIZE);
}

//ブロック同士の衝突処理
int collision_block_otherblock(int *mouse_x, int *mouse_y, const int(*pos_tmp)[BLOCK_TYPE_NUM], int num) {
	if (mouse_x == nullptr || mouse_y == nullptr) {
		return true;
	}

	if (collision_defoliation_normal() == true && (num == 0 || num == 1)) {
		brocks_pos[num][BLOCK_X] = pos_tmp[num][BLOCK_X];
		brocks_pos[num][BLOCK_Y] = pos_tmp[num][BLOCK_Y];
	}
	else if (collision_normal_jump() == true && (num == 1 || num == 2)) {
		brocks_pos[num][BLOCK_X] = pos_tmp[num][BLOCK_X];
		brocks_pos[num][BLOCK_Y] = pos_tmp[num][BLOCK_Y];
	}
	else if (collision_jump_defoliation() == true && (num == 2 || num == 0)) {
		brocks_pos[num][BLOCK_X] = pos_tmp[num][BLOCK_X];
		brocks_pos[num][BLOCK_Y] = pos_tmp[num][BLOCK_Y];
	}
	else {
		brocks_pos[num][BLOCK_X] = *mouse_x - BLOCK_SIZE / 2;
		brocks_pos[num][BLOCK_Y] = *mouse_y - BLOCK_SIZE / 2;
	}

	return false;
}

//マウスと落ち葉ブロックの衝突判定
bool collision_defoliation_mouse() {
	return Collision.box_Fanc(
		Mouse_X, Mouse_X,
		Mouse_Y, Mouse_Y,
		static_cast<double>(brocks_pos[0][BLOCK_X]),
		static_cast<double>(brocks_pos[0][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[0][BLOCK_Y]),
		static_cast<double>(brocks_pos[0][BLOCK_Y]) + BROCKS_SIZE);
}

//マウスと通常ブロックの衝突判定
bool collision_normal_mouse() {
	return Collision.box_Fanc(
		Mouse_X, Mouse_X,
		Mouse_Y, Mouse_Y,
		static_cast<double>(brocks_pos[1][BLOCK_X]),
		static_cast<double>(brocks_pos[1][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[1][BLOCK_Y]),
		static_cast<double>(brocks_pos[1][BLOCK_Y]) + BROCKS_SIZE);
}

//マウスとジャンプブロックの衝突判定
bool collision_jump_mouse() {
	return Collision.box_Fanc(
		Mouse_X, Mouse_X,
		Mouse_Y, Mouse_Y,
		static_cast<double>(brocks_pos[2][BLOCK_X]),
		static_cast<double>(brocks_pos[2][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[2][BLOCK_Y]),
		static_cast<double>(brocks_pos[2][BLOCK_Y]) + BROCKS_SIZE);
}
#pragma endregion