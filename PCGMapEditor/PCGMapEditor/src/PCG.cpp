#define _CRT_SECURE_NO_WARNINGS

#include "PCG.h"
#include "perlin.h"
#include "save_dialog.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static float s_offsetX = 0.0f;
static float s_offsetY = 0.0f;


// =============================================
// RandomMapGenerator
// =============================================
// Constructor
PCG::RandomMapGenerator::RandomMapGenerator() {
    // nothing to initialize for now, but you could seed a random generator here if you want reproducible maps
}

// Destructor
PCG::RandomMapGenerator::~RandomMapGenerator() {
    // nothing to clean up for now, but if you had allocated resources (like noise generators) you would release them here
}

void PCG::RandomMapGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            _tileArray[y][x] = (TileType)GetRandomValue(0, TILE_COUNT - 1);
        }
    }
}

// =============================================
// NoiseGenerator
// =============================================
// Constructor
PCG::NoiseMapGenerator::NoiseMapGenerator() {
    
}

// Destructor
PCG::NoiseMapGenerator::~NoiseMapGenerator() {
    // nothing to clean up for now, but if you had allocated resources (like noise generators) you would release them here
}

void PCG::NoiseMapGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) {
    s_offsetX = (float)GetRandomValue(0, 10000);
    s_offsetY = (float)GetRandomValue(0, 10000);
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            float noise = perlinNoise(s_offsetX + (float)x / MAP_COLUMNS * NOISE_SCALE, s_offsetY + (float)y / MAP_ROWS * NOISE_SCALE);
            _tileArray[y][x] = (noise > 0.0f) ? TILE_TYPE_GRASS : TILE_TYPE_ROCK;
            printf("%f ", noise);
        }
    }
}

// =============================================
// CellularGenerator
// =============================================
// Constructor
PCG::CellularMapGenerator::CellularMapGenerator() {

}

// Destructor
PCG::CellularMapGenerator::~CellularMapGenerator() {
    // nothing to clean up for now, but if you had allocated resources (like noise generators) you would release them here
}

void PCG::CellularMapGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) {
    float fillChance = 40;
    int generations = 5;
    int minRocks = 5;

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            float choice = GetRandomValue(0, 100);
            _tileArray[y][x] = (choice < fillChance) ? TILE_TYPE_ROCK : TILE_TYPE_GRASS;
        }
    }

    TileType tileBuffer[MAP_ROWS][MAP_COLUMNS];

    for (int g = 0; g < generations; g++) {
        for (int y = 0; y < MAP_ROWS; y++) {
            for (int x = 0; x < MAP_COLUMNS; x++) {
                int rockCount = 0;

                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dy == 0 && dx == 0) continue;

                        int ny = y + dy;
                        int nx = x + dx;

                        if (ny >= 0 && ny < MAP_ROWS && nx >= 0 && nx < MAP_COLUMNS) {
                            if (_tileArray[ny][nx] == TILE_TYPE_ROCK) {
                                rockCount++;
                            }
                        }
                        else {
                            rockCount++;
                        }
                    }
                }

                tileBuffer[y][x] = (rockCount >= minRocks) ? TILE_TYPE_ROCK : _tileArray[y][x];
            }
        }

        memcpy(_tileArray, tileBuffer, sizeof(tileBuffer));
    }
}

// ============================================
// TileMap constructor
// ============================================

PCG::TileMap::TileMap()
{
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            tileArray[y][x] = TILE_TYPE_GRASS;
        }
    }
}

// =============================================
// TileMap destructor
// =============================================

PCG::TileMap::~TileMap()
{
    
}

// =============================================
// SetMapGenerator
// =============================================
void PCG::TileMap::SetMapGenerator(std::unique_ptr<MapGenerator> generator) {
    mapGenerator = std::move(generator);
}

// =============================================
// GetMapGenerator
// =============================================
PCG::MapGenerator* PCG::TileMap::GetMapGenerator() const {
    return mapGenerator.get();
}

// =============================================
// CycleMapGenerator
// =============================================
void PCG::TileMap::CycleMapGenerator() {
    switch (currentGeneratorType) {
    case GeneratorType::CELLULAR:
        SetMapGenerator(std::make_unique<NoiseMapGenerator>());
        currentGeneratorType = GeneratorType::NOISE;
        break;
    case GeneratorType::NOISE:
        SetMapGenerator(std::make_unique<RandomMapGenerator>());
        currentGeneratorType = GeneratorType::RANDOM;
        break;
    case GeneratorType::RANDOM:
        SetMapGenerator(std::make_unique<CellularMapGenerator>());
        currentGeneratorType = GeneratorType::CELLULAR;
        break;
    }
    GetMapGenerator()->Generate(GetTileData());
}

// ============================================= 
// CreateMap
// ============================================= 
void PCG::TileMap::CreateMap() {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            //_tileArray[y][x] = (TileType)GetRandomValue(0, TILE_COUNT - 1);
			float noise = perlinNoise(s_offsetX + (float)x / MAP_COLUMNS * NOISE_SCALE, s_offsetY + (float)y / MAP_ROWS * NOISE_SCALE);
			tileArray[y][x] = (noise > 0.0f) ? TILE_TYPE_GRASS : TILE_TYPE_ROCK;
			printf("%f ", noise);
        }
		printf("\n");
    }
}

// ============================================= 
// SetTile(int x, int y, TileType tileType)
// ============================================= 

void PCG::TileMap::SetTile(int x, int y, TileType tileType)
{
    if (x >= 0 && x < MAP_COLUMNS && y >= 0 && y < MAP_ROWS) {
        tileArray[y][x] = tileType;
    }
}

// ============================================= 
// GetTileColor
// ============================================= 
Color PCG::TileMap::GetTileColor(PCG::TileType _tileType) const {
    switch (_tileType) {
    case TILE_TYPE_GRASS: return GRASS_COLOR;
    case TILE_TYPE_ROCK: return ROCK_COLOR;
    default: return UNKNOWN_COLOR;
    }
}

// ============================================= 
// DrawMap
// ============================================= 
void PCG::TileMap::DrawMap(bool isEngine) const {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            if (isEngine) {
                Vector3 position = { x, 0.0f, y };
                DrawCube(position, 1.0f, 1.0f, 1.0f, GetTileColor(tileArray[y][x]));
            }
            else {
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, GetTileColor(tileArray[y][x]));
            }
        }
    }
}

// ============================================= 
// PrintMap
// ============================================= 
void PCG::TileMap::PrintMap() const {
    printf("\n-------Map Layout:--------\n");
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            if (tileArray[y][x] == PCG::TILE_TYPE_GRASS) {
                printf("%c", GRASS_CHAR);
            }
            else {
                printf("%c", ROCK_CHAR);
            }
        }
        printf("\n");
    }
    printf("--------------------------\n");
}

// =============================================
// GetTileChar
// =============================================
char PCG::TileMap::GetTileChar(TileType _tileType) const {
    switch (_tileType) {
    case TILE_TYPE_GRASS: return GRASS_CHAR;
    case TILE_TYPE_ROCK: return ROCK_CHAR;
    default: return '?';
    }
}

// =============================================
// SaveMapData
// =============================================
void PCG::TileMap::SaveMapData(const char* fileName) const {
	char filePath[260] = "";

    if (!ShowSaveDialog(filePath, sizeof(filePath), fileName)) {
        printf("Save cancelled.\n");
        return;
    }

    FILE* file = fopen(filePath, "w");
    if (file == NULL) {
        printf("Error opening file for writing: %s\n", filePath);
        return;
	}

    // Save offsets on the first line
    fprintf(file, "%f %f\n", s_offsetX, s_offsetY);

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
			fputc(GetTileChar(tileArray[y][x]), file);
        }
        fputc('\n', file);
    }
	fclose(file);
	printf("Map saved successfully to: %s\n", filePath);
}

// =============================================
// LoadMapData
// =============================================
void PCG::TileMap::LoadMapData(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file for reading: %s\n", fileName);
        return;
    }

    // Read offsets from the first line
    fscanf(file, "%f %f\n", &s_offsetX, &s_offsetY);

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            int ch = fgetc(file);
            while (ch == '\n' || ch == '\r') {
                ch = fgetc(file);
			}

            if (ch == GRASS_CHAR) {
                tileArray[y][x] = TILE_TYPE_GRASS;
            }
            else if (ch == ROCK_CHAR) {
                tileArray[y][x] = TILE_TYPE_ROCK;
            }
        }
    }
    fclose(file);
}

// =============================================
// DrawGUI
// =============================================
void PCG::TileMap::DrawGUI() {
	//Reset Button
    if (GuiButton(RESET_BUTTON_BOUNDS, "Reset Map")) {
        //SetRandomSeed(GetTime());
        GetMapGenerator()->Generate(GetTileData());
    }

	// Save Button
    Rectangle saveRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 140, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(saveRect, "Save Map")) {
        SaveMapData(MAP_FILE_NAME);
	}

    //Load Button
    Rectangle loadRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 70, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(loadRect, "Load Map")) {
        LoadMapData(MAP_FILE_NAME);
	}

    //Change Generator Button
    Rectangle genRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 210, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(genRect, "Change Generator")) {
        if (GetMapGenerator() != nullptr) {
            CycleMapGenerator();
        }
    }

    //Tell user current GeneratorType
    const char* generatorName = "";
    switch (currentGeneratorType) {
    case GeneratorType::CELLULAR: generatorName = "Cellular"; break;
    case GeneratorType::NOISE:    generatorName = "Noise";    break;
    case GeneratorType::RANDOM:   generatorName = "Random";   break;
    }
    DrawText(TextFormat("Generator: %s", generatorName), 20, 50, 20, WHITE);
}