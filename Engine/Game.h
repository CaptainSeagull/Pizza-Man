#ifndef GAME_H
#define GAME_H
	#include <Bass\bass.h>
	#include <map>
#include "UniformBufferObject.h"
#include "LightManager.h"
#include "Material.h"

	#define COLLISION_BOXES 0

#if COLLISION_BOXES
struct Mesh;
#endif

	struct AABB
	{
		glm::vec3 position;
		glm::vec3 size;
		#if COLLISION_BOXES
			static Mesh wireframeMesh;
		#endif

		AABB()
		{
			position = glm::vec3(0.0f);
			size = glm::vec3(1.0f);
		}
		AABB(const AABB& aabb)
		{
			position = aabb.position;
			size = aabb.size;
		}

		const AABB operator = (const AABB &aabb)
		{
			position = glm::vec3(aabb.position);
			size = glm::vec3(aabb.size);
			return *this;
		}
	};

	struct Mesh
	{
		GLuint *vaos;
		GLuint *numOfIndices;
		GLuint numMeshes;
		AABB roofAABB, bodyAABB;
	};

	struct Camera
	{
		AABB aabb;
		glm::vec3 pos;
		glm::vec3 previousPos;
		glm::vec3 startPos;
		glm::vec3 lookAt;

		float pitch, yaw; 
		float vspeed;
		float currentMovement;

		int32_t currentBuilding;

		uint32_t score;
		uint32_t numberOfPizzaSlices;
		uint32_t currentLevel;

		bool onBuilding;
	};

	struct Building
	{
		AABB aabb_body;
		AABB aabb_roof;

		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 rotation;

		bool isInitialized;
	};

	struct Enemy
	{
		glm::vec3 pos;
		glm::vec3 scale;
		AABB aabb;
		
		float vspeed;
		float hspeed;
		int32_t currentBuilding;

		bool isInitialized;
		bool isDead;
	};

	struct Shaders
	{
		GLuint main;
		#if COLLISION_BOXES
			GLuint wireFrame;
		#endif
		GLuint skybox;
		GLuint hud;
		GLuint normalMap;
		GLuint arrow;
		GLuint main_fogless;
	};

	struct Textures
	{
		GLuint building;
		GLuint skyboxCubemap;
		GLuint textSheet;
		GLuint blank;
		GLuint mainMenuTitle;
		GLuint mainMenuStart;
		GLuint mainMenuHighScore;
		GLuint mainMenuHelp;
		GLuint mainMenuQuit;
		GLuint mainMenuBackground;
		GLuint helpScreen;
	};


	// NOTE(Jonny): Hud only uses GL struct instead of Mesh because I was in a rush...
	//				but it'd probably be better using Mesh
	struct GL
	{
		GLuint VBOarray[2];
		GLuint mesh;
	};
	struct Meshes
	{
		Mesh cube;
		Mesh buildings[10];
		Mesh enemy;
		Mesh arrow;
		Mesh pizzaSlice;
		GL hud;
	};

	// NOTE(Jonny): This is set to 11 so that the players score can get added on the end
	//				however only ten are drawn, so it just skips out the last element
	#define NumberOfHighScores 11
	struct HighScoreTable
	{	
		uint32_t score[NumberOfHighScores];
		char *name[NumberOfHighScores];
	};

	#define NumberOfMainMenuItems 3
	struct MainMenu
	{
		GL gl;
		GL background_gl;
		HighScoreTable highScoreTable;


		// NOTE(Jonny): 0 is startGame, 1 is highscore, 2 is help, 3 is quit
		int8_t currentlySelectedItem;

		bool isActive;
		bool isHelpShown;
		bool isHighScoreShowing;
	};
	
	struct Level
	{
		glm::vec3 startPosition;
		Building building[255];
		Enemy enemy[255];
		int endBuilding;
		int numEnemies = 0;
	};

	struct FogParameters
	{
		float density, startHeight;
		glm::vec4 colour;
	};

	struct PizzaSlice
	{
		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 playerPosWhenthrowing;

		AABB aabb;

		float maxSpeed;
		float playerPitchWhenThrowing;
		float playerYawWhenThrowing;

		float vspeed;
		float hspeed;
	
		bool isInitialized;
	};

	struct GameState
	{
		Level levels[255];
		int32_t currentLevel;
		int32_t maxNumberOfLevels;
		Camera camera;
		PizzaSlice pizzaSlice;
		FogParameters buildingFog, skyboxFog;

		Shaders shaders;
		Textures textures;
		Meshes mesh;

		UniformBufferObject *lightsUBO, *matricesUBO;
		LightManager *lightManager;
		Material *randomMaterial;

		MainMenu mainMenu;
		HSAMPLE sounds[2];

		uint32_t textSheetWidth;
		uint32_t textSheetHeight;
		float timer;

		bool isEnterNameMessageShowing;
		char *currentName;

		glm::vec3 ArrowColour;
	};


	// NOTE(Jonny): Because C++...
	struct Input;

	void initGame(GameState *gameState);
	void update(GameState *gameState, Input input, float delta);
	void draw(GameState *gameState);

#endif