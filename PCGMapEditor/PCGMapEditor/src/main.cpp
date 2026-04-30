#if defined(_WIN32) && !defined(_DEBUG)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include "resource_dir.h"
#include "PCG.h"


int main() {
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(PCG::SCREEN_WIDTH, PCG::SCREEN_HEIGHT, "PCG Map Editor");
	

	// Initialize map
	PCG::TileType tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS] = { PCG::TileType::TILE_TYPE_ROCK };
	//PCG::CreateMap(tileArray);
	PCG::TileMap tileMap;
	//tileMap.CreateMap();
	tileMap.SetMapGenerator(std::make_unique<PCG::CellularMapGenerator>());
	tileMap.GetMapGenerator()->Generate(tileMap.GetTileData());

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

		tileMap.ChangeTile();

		//PCG::DrawMap(tileArray, false);
		tileMap.DrawMap(false);
		//PCG::DrawGUI(tileArray);
		tileMap.DrawGUI();

		DrawText("PCG Map Editor", 20, 20, 20, DARKGRAY);
		DrawText("Hold [LSHIFT] + Left Click to change tiles manually", 20, (PCG::SCREEN_HEIGHT - 30), 20, DARKGRAY);
		EndDrawing();
	}

	CloseWindow();

	//printf("Saving to: %s\n", GetWorkingDirectory());
	//SaveMapData(tileArray, MAP_FILE_NAME);

	return 0;
}