#pragma once
#include <raylib.h>
#include <memory>

namespace PCG {

	// Defines
	constexpr int SCREEN_WIDTH = 1024;
	constexpr int SCREEN_HEIGHT = 1024;
	constexpr int TILE_SIZE = 64;
	constexpr int NOISE_SCALE = 3.0f;

	// Map Dimensions
	constexpr int MAP_COLUMNS = (SCREEN_WIDTH / TILE_SIZE);
	constexpr int MAP_ROWS = (SCREEN_HEIGHT / TILE_SIZE);

	// Tile Types
	typedef enum {
		TILE_TYPE_GRASS,
		TILE_TYPE_ROCK,
		TILE_COUNT
	} TileType;

	// Generator Types
	typedef enum {
		CELLULAR,
		NOISE,
		RANDOM
	} GeneratorType;

	// Visuals & Characters
	constexpr char GRASS_CHAR = '.';
	constexpr char ROCK_CHAR = '#';
	constexpr Color GRASS_COLOR = { 69, 182, 156, 255 };
	constexpr Color ROCK_COLOR{ 114, 147, 160, 255 };
	constexpr Color UNKNOWN_COLOR = WHITE;

	// File Names
	constexpr const char* MAP_FILE_NAME = "pcg_map_data.fyl";

	// UI Constants
	constexpr int BUTTON_WIDTH = 200;
	constexpr int BUTTON_HEIGHT = 50;
	constexpr int BUTTON_X = (SCREEN_WIDTH - BUTTON_WIDTH - 20);
	constexpr int BUTTON_Y = (SCREEN_HEIGHT - BUTTON_HEIGHT - 20);
	constexpr Rectangle RESET_BUTTON_BOUNDS = { BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };


	class MapGenerator {
	public:
		virtual ~MapGenerator() = default;
		virtual void Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) = 0;
	};

	class RandomMapGenerator : public MapGenerator {
	public:
		RandomMapGenerator();
		~RandomMapGenerator();
		void Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) override;
	};

	class NoiseMapGenerator : public MapGenerator {
	public:
		NoiseMapGenerator();
		~NoiseMapGenerator();
		void Generate(TileType _tileType[MAP_ROWS][MAP_COLUMNS]) override;
	};

	class CellularMapGenerator : public MapGenerator {
	public:
		CellularMapGenerator();
		~CellularMapGenerator();
		void Generate(TileType _tileType[MAP_ROWS][MAP_COLUMNS]) override;
	};

	class TileMap {
	public:
		TileMap();
		~TileMap();

		// =============================================
		// Function Declarations
		// =============================================

		// PCG Functions
		void CreateMap();
		void DrawMap(bool isEngine) const;
		void PrintMap() const;
		void DrawGUI();

		// File I/O Functions
		void SaveMapData(const char* fileName) const;
		void LoadMapData(const char* fileName);

		// Accessors
		void SetTile(int x, int y, PCG::TileType tileType);
		Color GetTileColor(TileType tileType) const;
		char GetTileChar(TileType tileType) const;

		void ChangeTile();

		TileType(&GetTileData())[MAP_ROWS][MAP_COLUMNS]{
			return tileArray;
		}

		void SetMapGenerator(std::unique_ptr<MapGenerator> generator);
		MapGenerator* GetMapGenerator() const;
		void CycleMapGenerator();

	private:
		TileType tileArray[MAP_ROWS][MAP_COLUMNS] = { PCG::TileType::TILE_TYPE_ROCK };
		GeneratorType currentGeneratorType = PCG::GeneratorType::CELLULAR;
		std::unique_ptr<MapGenerator> mapGenerator;
	};
}