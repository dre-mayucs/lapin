#pragma once
#include "DxLib.h"
#include "win_config.h"
#include "collision.h"
#include "IO.h"
#include "color.h"
#include "using_conponents.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

void Start(); 

//Grobal class
collision Collision;

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

//main
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	initialize_component();
	Start();

	DxLib_End();
	return 0;
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