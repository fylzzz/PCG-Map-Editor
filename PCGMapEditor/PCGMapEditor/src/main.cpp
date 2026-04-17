#include <raylib.h>
#include <stdio.h>
#include "resource_dir.h"
#include "PCG.h"


int main() {
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(PCG::SCREEN_WIDTH, PCG::SCREEN_HEIGHT, "PCG Map Editor");

	// Initialize map
	PCG::TileType tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS] = { TileType::TILE_TYPE_ROCK };
	//PCG::CreateMap(tileArray);
	PCG::TileMap tileMap;
	//tileMap.CreateMap();
	tileMap.SetMapGenerator(new PCG::NoiseMapGenerator());
	tileMap.GetMapGenerator()->Generate(tilemap.tileArray);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

		//PCG::DrawMap(tileArray, false);
		tileMap.DrawMap();
		//PCG::DrawGUI(tileArray);
		tileMap.DrawGUI();

		DrawText("PCG Map Editor", 20, 20, 20, DARKGRAY);
		EndDrawing();
	}

	CloseWindow();

	//printf("Saving to: %s\n", GetWorkingDirectory());
	//SaveMapData(tileArray, MAP_FILE_NAME);

	return 0;
}