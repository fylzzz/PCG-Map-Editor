#define _CRT_SECURE_NO_WARNINGS

#include <raylib.h>
#include "PCG.h"


int main() {
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1920, 1080, "3D Engine");
	SetTargetFPS(60);

	TileType tileArray[MAP_COLUMNS][MAP_ROWS] = { (TileType)0 };
	
	Camera3D camera = { 0 };
	camera.position = Vector3{ 50.0f, 50.0f, 50.0f };
	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	PCG_LoadMapData(tileArray, MAP_FILE_NAME);
	PCG_PrintMap(tileArray);


	while (!WindowShouldClose()) {

		UpdateCamera(&camera, CAMERA_ORBITAL);

		BeginDrawing();
		ClearBackground(RAYWHITE);

			BeginMode3D(camera);
			PCG_DrawMap(tileArray, true);
			EndMode3D();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}