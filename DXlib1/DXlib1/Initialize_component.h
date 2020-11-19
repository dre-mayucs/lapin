#pragma once
#include "DxLib.h"
#include "win_config.h"
#include "collision.h"
#include "IO.h"
#include "color.h"
#include "using_conponents.h"
#include "Effects.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

void Start();
void menu();
void stage();

//Grobal class
collision Collision;
Effects effect;

//Prototype
void BG_scroll(int *BgX, int *adjust, const int &mouseX, const int &mouseY);

int collision_block_otherblock(int *mouse_x, int *mouse_y, const int(*pos_tmp)[BLOCK_TYPE_NUM], int num);

bool collision_defoliation_normal();
bool collision_normal_jump();
bool collision_jump_defoliation();
bool collision_defoliation_mouse();
bool collision_normal_mouse();
bool collision_jump_mouse();

//initialize main system
int initialize_component()
{
	// ウィンドウモード
	ChangeWindowMode(true);
	SetWindowSizeChangeEnableFlag(false, false);
	SetMainWindowText(TITLE);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(0, 0, 0);

	if (DxLib_Init() == -1) { return -1; }
	SetDrawScreen(DX_SCREEN_BACK);

	srand(time(NULL));
}

std::vector<std::string> Split(std::string str, char key)
{
	int first = 0;
	int last = str.find_first_of(key);

	std::vector<std::string> result;

	while (first < str.size())
	{
		std::string cache(str, first, last - first);
		result.push_back(cache);

		first = last + 1;
		last = str.find_first_of(key, first);

		if (last == std::string::npos)
		{
			last = str.size();
		}
	}

	return result;
}

int world_value = 0;
int world[100][2];
void World_inport()
{
	int count = 0;
	int count2 = 0;
	std::string w_cache, cache[4], cache_2[100];
	std::ifstream stream_File("Resources\\scenes\\stage.scene");

	for (auto i = 0; i < 100; i++) {
		world[i][0] = 0;
		world[i][1] = 0;
	}

	//シーンファイル読み込み
	stream_File >> w_cache;

	//第２キャッシュへ格納
	for (auto cache_str : Split(w_cache, '|')) {
		std::istringstream stream = std::istringstream(cache_str);
		stream >> cache_2[count++];
	}
	world_value = count;

	for (auto i = 0; i < count; i++) {
		for (auto cache_str : Split(cache_2[i], ',')) {
			std::istringstream stream = std::istringstream(cache_str);
			stream >> cache[count2++];
		}

		world[i][0] = std::stoi(cache[0]);
		world[i][1] = std::stoi(cache[2]);

		for (auto i = 0; i < 4; i++) {
			cache[i] = "null";
		}
		count2 = 0;
	}
}

//main
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	initialize_component();
	menu();
	Start();
	stage();

	DxLib_End();
	return 0;
}