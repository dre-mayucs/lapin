#pragma once
#include "DxLib.h"
#include "win_config.h"
#include "collision.h"
#include "key.h"
#include "color.h"
#include "using_conponents.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

void op();
void end();
void stage();
void start();

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
	start();

	DxLib_End();
	return 0;
}