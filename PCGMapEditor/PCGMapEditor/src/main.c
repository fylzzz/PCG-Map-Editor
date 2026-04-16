#include <raylib.h>
#include <stdio.h>
#include "resource_dir.h"
#include "PCG.h"


int main() {
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PCG Map Editor");

	// Initialize map
	TileType tileArray[MAP_ROWS][MAP_COLUMNS] = { 0 };
	PCG_CreateMap(tileArray);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

		PCG_DrawMap(tileArray, false);
		PCG_DrawGUI(tileArray);

		DrawText("PCG Map Editor", 20, 20, 20, DARKGRAY);
		EndDrawing();
	}

	CloseWindow();

	//printf("Saving to: %s\n", GetWorkingDirectory());
	//PCG_SaveMapData(tileArray, MAP_FILE_NAME);

	return 0;
}