#include <GL\glew.h>
#include <GL\glew.h>

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <tinyxml2.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "OpenGLHelper.h"
#include "Game.h"
#include "Input.h"
#include "Junk.h"
#include "GetLetterPosition.cpp"

// NOTE(Christie): Load Sample into the game
static HSAMPLE loadSample(char * filename)
{
	HSAMPLE music;
	music = BASS_SampleLoad(FALSE, filename, 0, 0, 3, BASS_SAMPLE_LOOP);
	return music;
}

// NOTE(Christie): Creates Music
static void createGameMusic(HSAMPLE sounds[])
{
	if(!BASS_Init(-1, 44100, 0, 0, NULL))
	{
		std::cout << "Cant't initialise sound." << std::endl;
	}
	sounds[0] = loadSample("Sounds//Keep_The_Pace.mp3");
	sounds[1] = loadSample("Sounds//Metal_Race.mp3");
	
#if 0
	sounds[0] = loadSample("Sounds//Metal_Race.mp3");
	sounds[1] = loadSample("Sounds//Switch_It_Up.mp3");
	sounds[2] = loadSample("Sounds//Keep_The_Pace.mp3");
	sounds[3] = loadSample("Sounds//Morning_Stroll.mp3");
	sounds[4] = loadSample("Sounds//Funeral_March_by_Chopin.mp3");
	sounds[5] = loadSample("Sounds//You_Make_Me_Feel_Good.mp3");
	sounds[6] = loadSample("Sounds//Crowd_Applause.wav");

	std::cout << "Press P to pause music and R to resume music" << std::endl;
#endif
}

// NOTE(Christie): Deleted music to free up memory
static void deleteMusic()
{
	BASS_Stop();
	BASS_Free();
}


// NOTE(Christie): Plays Music:
//						0 - Game
//						1 - Highscore
//						2 - LevelComplete
//						3 - Mainmenu
static void playMusic(HSAMPLE sound)
{
	HCHANNEL song = BASS_SampleGetChannel(sound, FALSE);
	BASS_ChannelSetAttributes(song, -1 , 50, (rand()%201)-100);
	BASS_ChannelPlay(song, TRUE);

	if (!BASS_ChannelPlay(song, FALSE))
	{
		std::cout << "Can't play sample" << std::endl;
	}
}

#if COLLISION_BOXES
	Mesh AABB::wireframeMesh;
#endif

static void setFogParameters(const GLuint shader, const FogParameters fogParams)
{
	glUseProgram(shader);
	int location = glGetUniformLocation(shader, "fogParams.colour");
	glUniform4f(location, fogParams.colour.x, fogParams.colour.y, fogParams.colour.z, fogParams.colour.w);
	location = glGetUniformLocation(shader, "fogParams.startHeight");
	glUniform1f(location, fogParams.startHeight);
	location = glGetUniformLocation(shader, "fogParams.density");
	glUniform1f(location, fogParams.density);
}

static void initSkybox(GameState *gameState)
{
	gameState->shaders.skybox = initShaders("Shaders/skyboxCubemap.vert", "Shaders/skyboxCubemap.frag");

	setFogParameters(gameState->shaders.skybox, gameState->skyboxFog);

	gameState->textures.skyboxCubemap = loadCubemap("Textures/nec_city",
	                                                "city_bk.tga",
	                                                "city_ft.tga",
	                                                "city_rt.tga",
	                                                "city_lf.tga",
	                                                "city_up.tga",
	                                                "cityp_dn.tga");
	gameState->mesh.cube = loadMesh("Models/cube.3ds");
}

static Building createBuilding(glm::vec3 pos, glm::vec3 scale, AABB defaultBody, AABB defaultRoof)
{
	Building result = {};

	result.pos = pos;
	result.scale = scale;
	result.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	result.aabb_roof = defaultRoof;
	result.aabb_roof.position += pos;
	result.aabb_body = defaultBody;
	result.aabb_body.position += pos;
	result.aabb_body.size = result.aabb_body.size * scale;
	result.aabb_roof.size = result.aabb_roof.size * scale;

	result.isInitialized = true;

	float offset = 5.0f;
	result.aabb_roof.position.y -= offset;
	result.aabb_body.position.y -= offset;

	return result;
}

static const HighScoreTable initialiseDefaultHighScoreTable()
{
	HighScoreTable table = {};
	for (int i = 0; i < 10; i++)
	{
		table.name[i] = "AAA";
		table.score[i] = 0;
	}
	return table;
}


// NOTE(Kyle): Returns a HighScoreTable object which contains the highscores
//			   and names present in the scores saved to file. It is unsorted. 
static const HighScoreTable loadHighscoreFile(const char *file)
{
	tinyxml2::XMLDocument highscoreFile;
	HighScoreTable table = {};

	if (highscoreFile.LoadFile(file) != tinyxml2::XML_NO_ERROR)
	{
		return initialiseDefaultHighScoreTable();
	}

	tinyxml2::XMLElement *highscores = highscoreFile.FirstChildElement()->FirstChildElement();
	for (int i = 0; i < NumberOfHighScores; ++i)
	{
		int score = highscores->IntAttribute("Score");
		const char* name = highscores->Attribute("Name");

		int l = StringLength((char *)name);
		table.name[i] = (char *)malloc(sizeof(char) * l + 1);
		for (int k = 0; k < l; k++)
		{
			table.name[i][k] = name[k];
		}
		table.name[i][l + 1] = '\0';

		table.score[i] = score;

		if (i < NumberOfHighScores - 1)
			highscores = highscores->NextSiblingElement();
	}
	return table;
}

// NOTE(Kyle): Saves the provided highscore table to file
static void saveHighScoreTable(const HighScoreTable &table, const char *file)
{
	tinyxml2::XMLDocument highscoreFile;
	tinyxml2::XMLElement *rootElement = highscoreFile.NewElement("Scores");
	highscoreFile.InsertFirstChild(rootElement);
	for (int i = 0; i < NumberOfHighScores; ++i)
	{
		tinyxml2::XMLElement *scoreElement = highscoreFile.NewElement("Score");
		rootElement->InsertFirstChild(scoreElement);
		scoreElement->SetAttribute("Score", table.score[i]);
		scoreElement->SetAttribute("Name", table.name[i]);
	}
	highscoreFile.SaveFile(file, false);
}

static void LoadLevel(const char *file, GameState *gameState, int32_t no)
{
	// NOTE(Kyle): Open file and print error and return if there was an error.
	tinyxml2::XMLDocument level;
	if (level.LoadFile(file) != tinyxml2::XML_NO_ERROR)
	{
		std::cout << "Failed to load level at " << file << std::endl << "Error: " << level.ErrorName() << std::endl;
		return;
	}

	// NOTE(Kyle): Get the element holding all the models used
	tinyxml2::XMLElement *models = level.FirstChildElement();
	models = models->FirstChildElement();
	// NOTE(Kyle): Get the first child element and get its name and file path
	tinyxml2::XMLElement *building = models->FirstChildElement();
	// NOTE(Kyle): Get the number of models and loop through them
	int count = models->IntAttribute("Count");
	for (int i = 0; i < count; i++)
	{
		// NOTE(Kyle): Get its name and file path
		const char *name = building->Attribute("Name");
		const char *path = building->Attribute("Path");
		// NOTE(Kyle): Then get the size of it
		tinyxml2::XMLElement *positionElement = building->FirstChildElement();
		float x = positionElement->FloatAttribute("x");
		float y = positionElement->FloatAttribute("y");
		float z = positionElement->FloatAttribute("z");
		glm::vec3 position = glm::vec3(x, y, z);
		// NOTE(Kyle): Then get the size of it
		tinyxml2::XMLElement *sizeElement = positionElement->NextSiblingElement();
		x = sizeElement->FloatAttribute("x");
		y = sizeElement->FloatAttribute("y");
		z = sizeElement->FloatAttribute("z");
		glm::vec3 size = glm::vec3(x, y, z);

		// NOTE(Kyle): Load in the model and create its bounding boxes. 
		Mesh newMesh = loadMesh(path);
		AABB body = AABB();
		AABB roof = AABB();
		body.size = size;
		body.position = position;

		roof.size = body.size;
		roof.size.y = 15.0f;
		roof.position = body.position;
		roof.position.y = roof.size.y / 2.0f;
		
		newMesh.bodyAABB = body;
		newMesh.roofAABB = roof;
		gameState->mesh.buildings[i] = newMesh;


		// NOTE(Kyle): Move to next element
		if (i != count - 1)
		{
			building = building->NextSiblingElement();
		}
	}

	// NOTE(Kyle): Get the element holding all game objects
	tinyxml2::XMLElement *objects = models->NextSiblingElement();
	building = objects->FirstChildElement();
	// NOTE(Kyle): Get the number of objects and loop through them
	count = objects->IntAttribute("Count");
	for (int i = 0; i < count; i++)
	{
		const char *name = building->Attribute("Name");
		bool startBuilding = building->BoolAttribute("StartBuilding");
		bool endBuilding = building->BoolAttribute("EndBuilding");
		// NOTE(Kyle): Get the buildings position
		tinyxml2::XMLElement *posElement = building->FirstChildElement();
		float x = posElement->FloatAttribute("x");
		float y = posElement->FloatAttribute("y");
		float z = posElement->FloatAttribute("z");
		glm::vec3 position = glm::vec3(x, y, z);
		// NOTE(Kyle): Size
		tinyxml2::XMLElement *sizeElement = posElement->NextSiblingElement();
		x = sizeElement->FloatAttribute("x");
		y = sizeElement->FloatAttribute("y");
		z = sizeElement->FloatAttribute("z");
		glm::vec3 modelSize = gameState->mesh.buildings[0].bodyAABB.size;
		glm::vec3 size = glm::vec3(x, y, z) / modelSize;

		gameState->levels[no].building[i] = createBuilding(position, size, gameState->mesh.buildings[0].bodyAABB, gameState->mesh.buildings[0].roofAABB);

		// NOTE(Kyle): 50% chance of creating an enemy on each roof. 
		if (rand() % 2 == 0)
		{
			Enemy newEnemy = {};
			newEnemy.pos = position;
			float xOffset = rand() % (int)(modelSize.x) - modelSize.x / 2.0f;
			float zOffset = rand() % (int)(modelSize.z) - modelSize.z / 2.0f;
			newEnemy.pos.x += xOffset;
			newEnemy.pos.y += modelSize.y / 2.0f + 100.0f;
			newEnemy.pos.z += zOffset;
			float scale = 10.0;
			newEnemy.scale = glm::vec3(scale, scale, scale);
			newEnemy.aabb.position = newEnemy.pos;
			newEnemy.aabb.size = newEnemy.scale * 1.1f;
			newEnemy.isInitialized = true;
			// TODO(Jonny): This didn't used to read .enemy[0] but for debugging I changed it
			//				And now I can't remember what it was. KYLE, FIX IT!!! :'(
			gameState->levels[no].enemy[gameState->levels[no].numEnemies++] = newEnemy;
		}

		if (startBuilding)
		{
			gameState->levels[no].startPosition = position;
			gameState->levels[no].startPosition.y += modelSize.y / 2.0f + 100.0f;
		}
		if (endBuilding)
		{
			gameState->levels[no].endBuilding = i;
		}

		if (i != count - 1)
			building = building->NextSiblingElement();
	}

	++gameState->maxNumberOfLevels;
}

#if COLLISION_BOXES
	static void
	initCollisionBoxMesh()
	{
		GLfloat vertices[] = {	
			-0.5f, 0.5f, -0.5f,		// NOTE(Kyle): Top back left
			0.5f, 0.5f, -0.5f,		// NOTE(Kyle): Top back right
			-0.5f, -0.5f, -0.5f,	// NOTE(Kyle): Bottom back left
			0.5f, -0.5f, -0.5f,		// NOTE(Kyle): Bottom back right
			-0.5f, 0.5f, 0.5f,		// NOTE(Kyle): Top front left
			0.5f, 0.5f, 0.5f,		// NOTE(Kyle): Top front right
			-0.5f, -0.5f, 0.5f,		// NOTE(Kyle): Bottom front left
			0.5f, -0.5f, 0.5f,		// NOTE(Kyle): Bottom front right
		};
		GLuint indices[] = {
			0, 1, 0, 2, 0, 4,
			1, 3, 1, 5,
			2, 3, 2, 6,
			3, 7,
			4, 5, 4, 6,
			5, 7,
			6, 7
		};

		AABB::wireframeMesh.vaos = new GLuint[1];
		AABB::wireframeMesh.numMeshes = 1;
		AABB::wireframeMesh.numOfIndices = new GLuint[1];
		AABB::wireframeMesh.numOfIndices[0] = 24;
		GLuint VBOs[2];
		glGenVertexArrays(1, AABB::wireframeMesh.vaos);
		glGenBuffers(2, VBOs);
		glBindVertexArray(AABB::wireframeMesh.vaos[0]);

		// NOTE(Kyle): Send Vertices
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		// NOTE(Kyle): Send Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	static void
	drawCollisionBox(AABB collisionBox, GLuint wireframeShader, glm::mat4 projMat, glm::mat4 viewMat)
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(wireframeShader);
		glBindVertexArray(AABB::wireframeMesh.vaos[0]);
		int colourLoc = glGetUniformLocation(wireframeShader, "colour");
		if (colourLoc != -1)
		{
			glUniform3f(colourLoc, 1.0f, 0.0f, 0.0f);
		}
		int modelViewLoc = glGetUniformLocation(wireframeShader, "modelViewMat");
		int projectionLoc = glGetUniformLocation(wireframeShader, "projectionMat");

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, collisionBox.position);
		modelMatrix = glm::scale(modelMatrix, collisionBox.size);
		glm::mat4 modelViewMatrix = viewMat * modelMatrix;

		setUniform(modelViewLoc, modelViewMatrix);
		setUniform(projectionLoc, projMat);

		glDrawElements(GL_LINES, AABB::wireframeMesh.numOfIndices[0], GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
#endif

static HighScoreTable BubblesortHighScore(HighScoreTable highScoreTable)
{
	HighScoreTable result = highScoreTable;

	for (int i = 0; i<NumberOfHighScores; i++)
	{
		for (int k = NumberOfHighScores - 1; k >= 0; k--)
		{
			if (k != NumberOfHighScores - 1)
			{
				if (result.score[k] < result.score[k + 1])
				{
					int swapScore = result.score[k];
					result.score[k] = result.score[k + 1];
					result.score[k + 1] = swapScore;

					char *swapName = result.name[k];
					result.name[k] = result.name[k + 1];
					result.name[k + 1] = swapName;
				}
			}
		}
	}

	return result;
}

static void initialiseUBOs(GameState *gameState)
{
	const GLchar * matriceNames[] =
	{
		"modelMat",
		"viewMat",
		"modelViewMat",
		"normalMat",
		"projectionMat",
		"mvpMat",
		"eyePos"
	};
	gameState->matricesUBO = new UniformBufferObject("MatrixBlock", matriceNames, 7, gameState->shaders.main, 0, GL_DYNAMIC_DRAW);

	const GLchar * lightsNames[] =
	{
		"numLights",
		"position",
		"direction",
		"ambient",
		"diffuse",
		"specular",
		"attenuation",
		"cosCutoffAngle",
		"secondaryCutoffAngle"
	};
	gameState->lightsUBO = new UniformBufferObject("LightBlock", lightsNames, 9, gameState->shaders.main, 1, GL_DYNAMIC_DRAW);
}

void initGame(GameState *gameState)
{
	std::srand((unsigned)time(NULL));
	//NOTE(Christie): TEST FOR MUSIC
	createGameMusic(gameState->sounds);
	playMusic(gameState->sounds[0]); // NOTE(Christie): Music for game

	gameState->buildingFog.colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	gameState->buildingFog.density = 0.02f;
	gameState->buildingFog.startHeight = 100.0f;

	gameState->skyboxFog.colour = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	gameState->skyboxFog.density = 0.2f;
	gameState->skyboxFog.startHeight = 0.0f;

	gameState->shaders.main = initShaders("Shaders/TextureAndFog.vert", "Shaders/TextureAndFog.frag");
	gameState->shaders.main_fogless = initShaders("Shaders/Texture.vert", "Shaders/Texture.frag");

	initialiseUBOs(gameState);
	gameState->lightsUBO->setUniform("numLights", 0);

	gameState->lightManager = new LightManager(gameState->lightsUBO);
	AmbientLight *ambientLight = new AmbientLight(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	gameState->lightManager->addLight(ambientLight);

	Pointlight *light = new Pointlight(
		glm::vec4(50.0f, 500.0f, 200.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 0.2f, 1.0f),
		glm::vec4(1.0f, 1.0f, 0.2f, 1.0f));
	gameState->lightManager->addLight(light);

	gameState->randomMaterial = new Material(
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
		1.0f);

	setFogParameters(gameState->shaders.main, gameState->buildingFog);

	#if COLLISION_BOXES
		gameState->shaders.wireFrame = initShaders("Shaders/Wireframe.vert", "Shaders/Wireframe.frag");
		initCollisionBoxMesh();
	#endif

	gameState->mesh.enemy = loadMesh("Models/obeseMale/obese_male.obj");

	gameState->shaders.arrow = initShaders("Shaders/arrow.vert", "Shaders/arrow.frag");
	gameState->ArrowColour = glm::vec3(0.0f, 1.0f, 0.0f);
	int location = glGetUniformLocation(gameState->shaders.arrow, "colour");
	glUniform3f(location, gameState->ArrowColour.x, gameState->ArrowColour.y, gameState->ArrowColour.z);
	gameState->mesh.arrow = loadMesh("Models/arrows/arrow4.obj");
	gameState->mesh.pizzaSlice = loadMesh("Models/PizzaSlice/PizzaSlice.FBX");

	gameState->textures.building = loadImage("Textures/Bricks2_FIX.png", 4);
	gameState->textures.blank = loadImage("Textures/checks.png", 4);
	
	gameState->shaders.hud = initShaders("Shaders/hud.vert", "Shaders/hud.frag");
	gameState->textures.textSheet = loadImage("Textures/freemono.png", 4,
	                                          gameState->textSheetWidth,
	                                          gameState->textSheetHeight);
	generate2DMesh(gameState->mesh.hud.VBOarray,
	               gameState->mesh.hud.mesh,
	               (float)gameState->textSheetWidth,
	               (float)gameState->textSheetHeight,
				   16.0f, 16.0f);
	glUseProgram(gameState->shaders.hud);
	GLuint transMat = glGetUniformLocation(gameState->shaders.hud, "u_trans");
	glm::mat4 empty = {};
	setUniform(transMat, empty);


	initSkybox(gameState);
	gameState->currentLevel = 0;
	// NOTE(Jonny): The Levels are 0 based, not 1. So the First Level is Level 0...
	LoadLevel("Levels/Lv1.pza", gameState, 0);
	LoadLevel("Levels/Lv2.pza", gameState, 1);
	LoadLevel("Levels/Lv3.pza", gameState, 2);
	LoadLevel("Levels/Lv4.pza", gameState, 3);
	LoadLevel("Levels/Lv5.pza", gameState, 4);

	gameState->camera = {};
	gameState->camera.pos = gameState->levels[0].startPosition;
	gameState->camera.startPos = gameState->camera.pos;
	gameState->camera.lookAt = glm::vec3(0.0f, 0.0f, -1.0f);
	gameState->camera.aabb.position = gameState->camera.pos;
	gameState->camera.aabb.size = glm::vec3(10.0f, 40.0f, 10.0f);
	gameState->camera.currentBuilding = -1;
	gameState->camera.score = 50;
	gameState->camera.numberOfPizzaSlices = 6;

	gameState->pizzaSlice = {};
	gameState->pizzaSlice.pos = gameState->camera.pos;
	gameState->pizzaSlice.scale = glm::vec3(1.0f);
	gameState->pizzaSlice.aabb.position = gameState->pizzaSlice.pos;
	gameState->pizzaSlice.aabb.size = gameState->pizzaSlice.scale * 50.0f;
	gameState->pizzaSlice.maxSpeed = 1.0f;
	gameState->pizzaSlice.isInitialized = false;

	gameState->mainMenu.isActive = true;
	uint32_t temp;
	gameState->textures.mainMenuStart 		= loadImage("Textures/start.png", 4, temp, temp);
	gameState->textures.mainMenuHighScore 	= loadImage("Textures/highscore.png", 4, temp, temp);
	gameState->textures.mainMenuHelp 		= loadImage("Textures/help.png", 4, temp, temp);
	gameState->textures.mainMenuQuit 		= loadImage("Textures/quit.png", 4, temp, temp);

	gameState->textures.mainMenuTitle 		= loadImage("Textures/title.png", 4, temp, temp);
	gameState->textures.mainMenuBackground 	= loadImage("Textures/main_menu_background.jpg", 4, temp, temp);

	// NOTE(Kyle): Help
	gameState->textures.helpScreen = loadImage("Textures/HELP_SCREEN.png", 4, temp, temp);


	generate2DMesh(gameState->mainMenu.gl.VBOarray,
	               gameState->mainMenu.gl.mesh,
	               1.0f, 1.0f,
	               2.0f, 1.0f);
	generate2DMesh(gameState->mainMenu.background_gl.VBOarray,
	               gameState->mainMenu.background_gl.mesh,
	               1.0f, 1.0f,
	               1.0f, 1.0f);
#if 0
	gameState->mainMenu.highScoreTable.name[0] = "AAA";
	gameState->mainMenu.highScoreTable.name[1] = "AAA";
	gameState->mainMenu.highScoreTable.name[2] = "AAA";
	gameState->mainMenu.highScoreTable.name[3] = "AAA";
	gameState->mainMenu.highScoreTable.name[4] = "AAA";
	gameState->mainMenu.highScoreTable.name[5] = "AAA";
	gameState->mainMenu.highScoreTable.name[6] = "AAA";
	gameState->mainMenu.highScoreTable.name[7] = "AAA";
	gameState->mainMenu.highScoreTable.name[8] = "AAA";
	gameState->mainMenu.highScoreTable.name[9] = "AAA";
	gameState->mainMenu.highScoreTable.name[10] = "AAA";

	gameState->mainMenu.highScoreTable.score[0] = 0;
	gameState->mainMenu.highScoreTable.score[1] = 0;
	gameState->mainMenu.highScoreTable.score[2] = 0;
	gameState->mainMenu.highScoreTable.score[3] = 0;
	gameState->mainMenu.highScoreTable.score[4] = 0;
	gameState->mainMenu.highScoreTable.score[5] = 0;
	gameState->mainMenu.highScoreTable.score[6] = 0;
	gameState->mainMenu.highScoreTable.score[7] = 0;
	gameState->mainMenu.highScoreTable.score[8] = 0;
	gameState->mainMenu.highScoreTable.score[9] = 0;
	gameState->mainMenu.highScoreTable.score[10] = 0;
#endif

	BubblesortHighScore(gameState->mainMenu.highScoreTable);

	gameState->mainMenu.highScoreTable = loadHighscoreFile("high.scores");
	BubblesortHighScore(gameState->mainMenu.highScoreTable);

				   
	gameState->timer = 0.0f;
	gameState->isEnterNameMessageShowing = false;
	gameState->currentName = "";
}

inline glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	glm::vec3 result = glm::vec3(pos.x + d*std::sin(angle*Deg2Rad32), pos.y, pos.z - d*std::cos(angle*Deg2Rad32));
	return result;
}

inline glm::vec3 moveRight(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	glm::vec3 result = glm::vec3(pos.x + d*std::cos(angle*Deg2Rad32), pos.y, pos.z + d*std::sin(angle*Deg2Rad32));
	return result;
}

static bool checkAABB(AABB one, AABB other)
{
	if (one.position.x - one.size.x/2 <= other.position.x + other.size.x/2 &&
		one.position.y - one.size.y / 2 <= other.position.y + other.size.y / 2 &&
		one.position.z - one.size.z / 2 <= other.position.z + other.size.z / 2 &&
		one.position.x + one.size.x/2 >= other.position.x - other.size.x/2 &&
		one.position.y + one.size.y / 2 >= other.position.y - other.size.y / 2 &&
		one.position.z + one.size.z / 2 >= other.position.z - other.size.z / 2)
		return true;

	return false;
}


inline float distance(float x_1, float x_2,
					  float y_1, float y_2)
{
	float result = sqrt(Square(x_1-x_2) + Square(y_1-y_2));
	return result;
}


// TODO(Jonny): There is a *mathimatically correct* way of doing this; using Delta, and
//				angles, and stuff. Just cound't be bothered doing it...
static glm::vec3 moveTowardsPoint(glm::vec3 current, glm::vec3 target, float speed)
{
	glm::vec3 result = current;

	glm::vec3 direction = target - current;
	result = current + speed * glm::normalize(direction);
#if 0
	result.x += speed * sin(Deg2Rad32 * ((atan2(current.z-target.z, current.x-target.x) * 180.0f / Pi32) + 270.0f));
	result.z += speed * cos(Deg2Rad32 * ((atan2(current.z-target.z, current.x-target.x) * 180.0f / Pi32) + 90.0f));
#endif
	return result;
}

static glm::vec3 moveTowardsPointWithY(glm::vec3 current, glm::vec3 target, float speed)
{
	glm::vec3 result = current;
	
	result.x += speed * sin(Deg2Rad32 * ((atan2(current.z-target.z, current.x-target.x) * 180.0f / Pi32) + 270.0f));
	result.y += speed * sin(Deg2Rad32 * ((atan2(current.z-target.z, current.x-target.x) * 180.0f / Pi32) + 90.0f));
	result.z += speed * cos(Deg2Rad32 * ((atan2(current.z-target.z, current.x-target.x) * 180.0f / Pi32) + 90.0f));

	return result;
}



static void DoHighScoreThingsAndStuff(GameState *gameState, Input input)
{
	if (input.KeyDown[A])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "A");
	}
	if (input.KeyDown[B])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "B");
	}
	if (input.KeyDown[C])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "C");
	}
	if (input.KeyDown[D])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "D");
	}
	if (input.KeyDown[E])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "E");
	}
	if (input.KeyDown[F])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "F");
	}
	if (input.KeyDown[G])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "G");
	}
	if (input.KeyDown[H])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "H");
	}
	if (input.KeyDown[I])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "I");
	}
	if (input.KeyDown[J])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "J");
	}
	if (input.KeyDown[K])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "K");
	}
	if (input.KeyDown[L])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "L");
	}
	if (input.KeyDown[M])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "M");
	}
	if (input.KeyDown[N])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "N");
	}
	if (input.KeyDown[O])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "O");
	}
	if (input.KeyDown[P])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "P");
	}
	if (input.KeyDown[Q])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "Q");
	}
	if (input.KeyDown[R])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "R");
	}
	if (input.KeyDown[S])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "S");
	}
	if (input.KeyDown[T])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "T");
	}
	if (input.KeyDown[U])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "U");
	}
	if (input.KeyDown[V])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "V");
	}
	if (input.KeyDown[W])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "W");
	}
	if (input.KeyDown[X])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "X");
	}
	if (input.KeyDown[Y])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "Y");
	}
	if (input.KeyDown[Z])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, "Z");
	}
	if (input.KeyDown[SPACE])
	{
		gameState->currentName = ConcatonateStrings(gameState->currentName, " ");
	}

	if ((input.KeyDown[DEL]) || (input.KeyDown[BACKSPACE]))
	{
		uint32_t length = StringLength(gameState->currentName);
		if (length > 0)
		{
			gameState->currentName[length-1] = '\0';
		}
	}
}

static void setLevel(GameState *gameState, int32_t levelNo)
{
	if (levelNo <= gameState->maxNumberOfLevels)
	{
		gameState->currentLevel = levelNo;
		gameState->camera.pos = gameState->levels[levelNo].startPosition;
		gameState->camera.score -= (uint32_t)gameState->timer;
		gameState->camera.score *= (uint32_t)(gameState->camera.numberOfPizzaSlices / 2.0f);
		gameState->camera.numberOfPizzaSlices = 6;
		gameState->timer = 0.0f;
	}
}


void update(GameState *gameState, Input input, float delta)
{
	if (gameState->mainMenu.isActive)
	{
		if (input.KeyDown[ESCAPE])
		{
			if (gameState->mainMenu.isHelpShown)
			{
				gameState->mainMenu.isHelpShown = false;
			}
			else if (gameState->mainMenu.isHighScoreShowing) 
			{
				gameState->mainMenu.isHighScoreShowing = false;
			}
		}

		if (input.KeyDown[UP] || input.KeyDown[W])
		{
			gameState->mainMenu.currentlySelectedItem--;
		}
		else if (input.KeyDown[DOWN] || input.KeyDown[S])
		{
			gameState->mainMenu.currentlySelectedItem++;
		}

		if (gameState->mainMenu.currentlySelectedItem < 0)
		{
			gameState->mainMenu.currentlySelectedItem = NumberOfMainMenuItems;
		}
		else if (gameState->mainMenu.currentlySelectedItem > NumberOfMainMenuItems)
		{
			gameState->mainMenu.currentlySelectedItem = 0;
		}
		if (input.Key[SPACE])
		{
			switch(gameState->mainMenu.currentlySelectedItem)
			{
				case 0:
				{
					gameState->mainMenu.isActive = false;
				} break;

				case 1:
				{
					gameState->mainMenu.isHighScoreShowing = true;
				} break;
				case 2:
				{
					gameState->mainMenu.isHelpShown = true;
				} break;
				case 3:
				{
					exit(1);
				} break;
			}
		}
	}
	else
	{
		if (input.KeyDown[ESCAPE])
		{
			gameState->mainMenu.isActive = true;
			return;
		}

		if (!gameState->isEnterNameMessageShowing)
		{
			gameState->timer += delta;

			float movement = 100.0f * delta;

			if (input.Key[E])
			{
				movement *= 2.0f;
			}

			if (input.Key[W])
			{
				gameState->camera.pos = moveForward(gameState->camera.pos,
													gameState->camera.yaw,
													-movement);
			}
			if (input.Key[S])
			{
				gameState->camera.pos = moveForward(gameState->camera.pos,
													gameState->camera.yaw,
													movement);
			}

			if (input.Key[A])
			{
				gameState->camera.pos = moveRight(gameState->camera.pos,
												  gameState->camera.yaw,
												  movement);
			}
			if (input.Key[D])
			{
				gameState->camera.pos = moveRight(gameState->camera.pos,
												  gameState->camera.yaw,
												  -movement);
			}


			glm::vec2 mouseDistance = input.MouseMoveDistance;

			gameState->camera.pitch += mouseDistance.y;
			if (gameState->camera.pitch < -180.0f)
			{
				gameState->camera.pitch = -180.0f;
			}
			if (gameState->camera.pitch > 20)
			{
				gameState->camera.pitch = 20;
			}

			gameState->camera.yaw += mouseDistance.x;
			if (gameState->camera.yaw >= 360)
			{
				gameState->camera.yaw -= 360;
			}
			if (gameState->camera.yaw <= -360)
			{
				gameState->camera.yaw += 360;
			}

			if (input.MouseButtonDown[MOUSE_LEFT])
			{
				if (gameState->camera.numberOfPizzaSlices > 0)
				{
					gameState->pizzaSlice.pos = gameState->camera.pos;
					gameState->pizzaSlice.playerPosWhenthrowing = gameState->camera.pos;
					gameState->pizzaSlice.pos = moveForward(gameState->pizzaSlice.pos,
															gameState->camera.yaw, -10.0f);
					gameState->pizzaSlice.playerYawWhenThrowing = gameState->camera.yaw;
					gameState->pizzaSlice.playerPitchWhenThrowing = gameState->camera.pitch;
					gameState->pizzaSlice.isInitialized = true;
					--gameState->camera.numberOfPizzaSlices;
				}
			}
			glm::vec3 start = gameState->pizzaSlice.playerPosWhenthrowing;
			glm::vec3 target = moveForward(gameState->pizzaSlice.playerPosWhenthrowing, gameState->pizzaSlice.playerYawWhenThrowing, -10.0f);
			target.y -= cos(Deg2Rad32 * gameState->pizzaSlice.playerPitchWhenThrowing) * 5.0f;
			glm::vec3 current = gameState->pizzaSlice.pos;

			glm::vec3 direction = start-target;
			gameState->pizzaSlice.pos += glm::normalize(direction) * -10.0f;

			gameState->pizzaSlice.aabb.position = gameState->pizzaSlice.pos;

			gameState->camera.onBuilding = false;

			uint32_t noOfBuildings = ArrayCount(gameState->levels[gameState->currentLevel].building);
			ULoop(noOfBuildings)
			{
				if (gameState->levels[gameState->currentLevel].building[index].isInitialized == true)
				{
					// NOTE(Jonny): Not 100% sure why I need to divide it by 10
					//				for buildings, but who cares at this point????
					AABB temp = gameState->pizzaSlice.aabb;
					temp.size /= 10.0f;
					if ((checkAABB(temp, gameState->levels[gameState->currentLevel].building[index].aabb_body)) ||
						(checkAABB(temp, gameState->levels[gameState->currentLevel].building[index].aabb_roof)))
					{
						gameState->pizzaSlice.isInitialized = false;
					}




					if (!checkAABB(gameState->camera.aabb,
					    gameState->levels[gameState->currentLevel].building[index].aabb_body))
					{
						if (checkAABB(gameState->camera.aabb,
						    gameState->levels[gameState->currentLevel].building[index].aabb_roof))
						{
							gameState->camera.onBuilding = true;
							gameState->camera.currentBuilding = index;
							if (index == gameState->levels[gameState->currentLevel].endBuilding)
							{
								// NOTE(Kyle): Next level!!!
								setLevel(gameState, ++gameState->currentLevel);
							}
						}
					}
					else
					{
						// NOTE(Jonny): Hacky-est code you've ever seen!!
						//				Makes the player *bounce* off the side of buildings
						//				I tried using the players previousTime for this, but it
						//				made the player *stick* to the building. This is Plan B.
						if (input.Key[W])
						{
							gameState->camera.pos = moveForward(gameState->camera.pos,
																gameState->camera.yaw,
																-movement * 10.0f);
						}
						if (input.Key[S])
						{
							gameState->camera.pos = moveForward(gameState->camera.pos,
																gameState->camera.yaw,
																movement * 10.0f);
						}

						if (input.Key[A])
						{
							gameState->camera.pos = moveRight(gameState->camera.pos,
															  gameState->camera.yaw,
															  -movement * 10.0f);
						}
						if (input.Key[D])
						{
							gameState->camera.pos = moveRight(gameState->camera.pos,
															  gameState->camera.yaw,
															  movement * 10.0f);
						};
					}
				}
			}


			// NOTE(Jonny): This is Debug-only code!!!!!!!!!!!!!!!!
			if (input.KeyDown[P])
			{
				setLevel(gameState, ++gameState->currentLevel);
			}
			else if (input.KeyDown[O])
			{
				setLevel(gameState, --gameState->currentLevel);
			}

			float gravityPull = 9.8f;
			float mass = 2.0f;
			if (gameState->camera.onBuilding == true)
			{
				if (gameState->camera.vspeed < 0.0f)
				{
					gameState->camera.vspeed = 0.0f;
				}
			}

			gameState->camera.pos.y += gameState->camera.vspeed * delta;

			gameState->camera.lookAt.y += gameState->camera.vspeed * delta;
			gameState->camera.aabb.position = gameState->camera.pos;

			gameState->camera.vspeed -= Square(gravityPull) * mass * delta;
		


			if ((gameState->camera.onBuilding) && (input.Key[SPACE]))
			{
				gameState->camera.vspeed = 175.0f;
			}
		

			if (gameState->camera.pos.y < -60.0f)
			{
				gameState->camera.pos = gameState->levels[gameState->currentLevel].startPosition;
				gameState->camera.lookAt = gameState->camera.startPos - glm::vec3(0.0f, 0.0f, 1.0f);
				gameState->camera.aabb.position = gameState->camera.startPos;
				gameState->camera.vspeed = 0.0f;
				gameState->timer += 10.0f;
			}

			uint32_t numberOfEnemies = ArrayCount(gameState->levels[gameState->currentLevel].enemy);
			UForeach(numberOfEnemies, enemyIndex)
			{
				bool isEnemyOnBuilding = false;

				gameState->levels[gameState->currentLevel].enemy[enemyIndex].aabb.position =
					gameState->levels[gameState->currentLevel].enemy[enemyIndex].pos;

				if (checkAABB(gameState->camera.aabb, gameState->levels[gameState->currentLevel].enemy[enemyIndex].aabb))
				{
					gameState->camera.pos = gameState->levels[gameState->currentLevel].startPosition;
					gameState->camera.lookAt = gameState->camera.startPos - glm::vec3(0.0f, 0.0f, 1.0f);
					gameState->camera.aabb.position = gameState->camera.startPos;
					gameState->camera.vspeed = 0.0f;
					gameState->timer += 10.0f;
				}
			
				if (gameState->pizzaSlice.isInitialized)
				{
					if (checkAABB(gameState->pizzaSlice.aabb, gameState->levels[gameState->currentLevel].enemy[enemyIndex].aabb))
					{
						gameState->levels[gameState->currentLevel].enemy[enemyIndex].isDead = true;
						gameState->pizzaSlice.isInitialized = false;
					}
				}

				uint32_t numberOfBuildings = ArrayCount(gameState->levels[gameState->currentLevel].building);
				UForeach(numberOfBuildings, buildingIndex)
				{
					if (gameState->levels[gameState->currentLevel].building[buildingIndex].isInitialized == true)
					{
						if(!checkAABB(gameState->levels[gameState->currentLevel].enemy[enemyIndex].aabb,
						   gameState->levels[gameState->currentLevel].building[buildingIndex].aabb_body))
						{
							if (checkAABB(gameState->levels[gameState->currentLevel].enemy[enemyIndex].aabb,
								gameState->levels[gameState->currentLevel].building[buildingIndex].aabb_roof))
							{
								isEnemyOnBuilding = true;
								gameState->levels[gameState->currentLevel].enemy[enemyIndex].currentBuilding = buildingIndex;
							}
						}
					}
				}

				if (isEnemyOnBuilding == true)
				{
					if (gameState->levels[gameState->currentLevel].enemy[enemyIndex].vspeed < 0.0f)
					{
						gameState->levels[gameState->currentLevel].enemy[enemyIndex].vspeed = 0.0f;
					}
				}

				gameState->levels[gameState->currentLevel].enemy[enemyIndex].pos.y +=
					gameState->levels[gameState->currentLevel].enemy[enemyIndex].vspeed * delta;
				gameState->levels[gameState->currentLevel].enemy[enemyIndex].vspeed -=
					Square(gravityPull) * mass * delta;

				if (gameState->levels[gameState->currentLevel].enemy[enemyIndex].currentBuilding ==
				    gameState->camera.currentBuilding)
				{
					//Get the new position of the enemy and constrain its y-axis. 
					glm::vec3 position = gameState->levels[gameState->currentLevel].enemy[enemyIndex].pos;
					glm::vec3 newPosition = moveTowardsPoint(position, gameState->camera.pos, 10.0f * delta);
					newPosition.y = position.y;

					gameState->levels[gameState->currentLevel].enemy[enemyIndex].pos = newPosition;
				}
				gameState->levels[gameState->currentLevel].enemy[enemyIndex].aabb.position =
					gameState->levels[gameState->currentLevel].enemy[enemyIndex].pos;
			}

			gameState->camera.previousPos = gameState->camera.pos;

			if (gameState->currentLevel == gameState->maxNumberOfLevels)
			{
				gameState->isEnterNameMessageShowing = true;
				gameState->currentName = "\0";
			}
		}
		else
		{
			DoHighScoreThingsAndStuff(gameState, input);
			if (input.KeyDown[RETURN])
			{
				gameState->mainMenu.highScoreTable = BubblesortHighScore(gameState->mainMenu.highScoreTable);

				gameState->mainMenu.highScoreTable.name[NumberOfHighScores-1] = gameState->currentName;
				gameState->mainMenu.highScoreTable.score[NumberOfHighScores-1] = gameState->camera.score;

				gameState->mainMenu.highScoreTable = BubblesortHighScore(gameState->mainMenu.highScoreTable);
				saveHighScoreTable(gameState->mainMenu.highScoreTable, "high.scores");

				gameState->isEnterNameMessageShowing = false;
				gameState->mainMenu.isActive = true;
				gameState->mainMenu.isHighScoreShowing = true;
			}
		}
	}
}

static glm::mat4 getViewMatrix(const Camera &camera)
{
	glm::vec3 cameraLookAt = glm::vec3(camera.pos.x + sin(glm::radians(camera.yaw)),
									   camera.pos.y + cos(glm::radians(camera.pitch)),
									   camera.pos.z - cos(glm::radians(camera.yaw)));

	return glm::lookAt(cameraLookAt, camera.pos, glm::vec3(0.0f, 1.0f, 0.0f));
}

static Vector2f GetLetterPosition(char letter);

static void drawLetter(char letter, float x, float y, float scale, GLuint shader)
{
	float oneFrame = 1.0f / 16.0f;
	Vector2f letterPos = GetLetterPosition(letter);

	GLuint uniformTextureOffset = glGetUniformLocation(shader, "u_offset");
	setUniform(uniformTextureOffset, glm::vec2(-(oneFrame*letterPos.x),
       						 		   		   -(oneFrame*letterPos.y)));

	glm::mat4 pos = {};
	pos = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
	pos = glm::scale(pos, glm::vec3(scale, scale, 0.0f));

	GLuint uniformPosition = glGetUniformLocation(shader, "u_pos");
	setUniform(uniformPosition, pos);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static void drawWord(char *text, float x, float y, float scale, GLuint shader)
{
	uint32_t wordLength = StringLength(text);

	ULoop(wordLength)
	{
		drawLetter(text[index], x+(index*scale), y, scale, shader);
	}
}

static float GetAngleTowardsPoint(glm::vec3 pointOne, glm::vec3 pointTwo)
{
	float result = atan2((pointTwo.x-pointOne.x), (pointTwo.z-pointOne.z));

	return result;
}


#define BUTTON_SCALE_X 0.2f
#define BUTTON_SCALE_Y 0.1f

void draw(GameState *gameState)
{
	if (gameState->mainMenu.isActive)
	{
		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(gameState->shaders.hud);
		GLuint uniformTextureOffset = glGetUniformLocation(gameState->shaders.hud, "u_offset");
		GLuint uniformPosition = glGetUniformLocation(gameState->shaders.hud, "u_pos");
		glm::mat4 pos = {};
		
		glBindVertexArray(gameState->mainMenu.background_gl.mesh);
		setUniform(uniformTextureOffset, glm::vec2(0.0f, 0.0f));
		
		// NOTE(Jonny): Background
		{
			pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			setUniform(uniformPosition, pos);
			
			glBindTexture(GL_TEXTURE_2D, gameState->textures.mainMenuBackground);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		// NOTE(Jonny): Title
		{	
			pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.4f, 0.0f));
			pos = glm::scale(pos, glm::vec3(0.4f, 0.4f, 0.0f));
			setUniform(uniformPosition, pos);
			
			glBindTexture(GL_TEXTURE_2D, gameState->textures.mainMenuTitle);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}


		
		glBindVertexArray(gameState->mainMenu.gl.mesh);

		// NOTE(Jonny): Start game
		{		
			if (gameState->mainMenu.currentlySelectedItem == 0)
			{
				setUniform(uniformTextureOffset, glm::vec2(0.0f, 0.0f));
		    }
		    else
		    {
				setUniform(uniformTextureOffset, glm::vec2(0.5f, 0.0f));
		    }
			pos = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.2f, 0.0f));
			pos = glm::scale(pos, glm::vec3(BUTTON_SCALE_X, BUTTON_SCALE_Y, 0.0f));
			setUniform(uniformPosition, pos);
			
			glBindTexture(GL_TEXTURE_2D, gameState->textures.mainMenuStart);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
		
		// NOTE(Jonny): Highscore game
		{	
			if (gameState->mainMenu.currentlySelectedItem == 1)
			{
				setUniform(uniformTextureOffset, glm::vec2(0.0f, 0.0f));
		    }
		    else
		    {
				setUniform(uniformTextureOffset, glm::vec2(0.5f, 0.0f));
		    }

			pos = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.1f, 0.0f));
			pos = glm::scale(pos, glm::vec3(BUTTON_SCALE_X, BUTTON_SCALE_Y, 0.0f));
			setUniform(uniformPosition, pos);
			
			glBindTexture(GL_TEXTURE_2D, gameState->textures.mainMenuHighScore);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		// NOTE(Jonny): Help game
		{	
			if (gameState->mainMenu.currentlySelectedItem == 2)
			{
				setUniform(uniformTextureOffset, glm::vec2(0.0f, 0.0f));
		    }
		    else
		    {
				setUniform(uniformTextureOffset, glm::vec2(0.5f, 0.0f));
		    }

			pos = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.4f, 0.0f));
			pos = glm::scale(pos, glm::vec3(BUTTON_SCALE_X, BUTTON_SCALE_Y, 0.0f));
			setUniform(uniformPosition, pos);
			
			glBindTexture(GL_TEXTURE_2D, gameState->textures.mainMenuHelp);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		// NOTE(Jonny): Quit game
		{	
			if (gameState->mainMenu.currentlySelectedItem == 3)
			{
				setUniform(uniformTextureOffset, glm::vec2(0.0f, 0.0f));
		    }
		    else
		    {
				setUniform(uniformTextureOffset, glm::vec2(0.5f, 0.0f));
		    }
			pos = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.7f, 0.0f));
			pos = glm::scale(pos, glm::vec3(BUTTON_SCALE_X, BUTTON_SCALE_Y, 0.0f));
			setUniform(uniformPosition, pos);
			
			glBindTexture(GL_TEXTURE_2D, gameState->textures.mainMenuQuit);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		// NOTE(Kyle): HELP
		if (gameState->mainMenu.isHelpShown == true)
		{
			glBindVertexArray(gameState->mainMenu.background_gl.mesh);
			setUniform(uniformTextureOffset, glm::vec2(0.0f, 0.0f));

			pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			//pos = glm::scale(pos, glm::vec3(BUTTON_SCALE_X, BUTTON_SCALE_Y, 0.0f));
			setUniform(uniformPosition, pos);

			glBindTexture(GL_TEXTURE_2D, gameState->textures.helpScreen);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		// NOTE(Jonny): High score screen
		if (gameState->mainMenu.isHighScoreShowing == true)
		{
			glBindVertexArray(gameState->mainMenu.background_gl.mesh);
			setUniform(uniformTextureOffset, glm::vec2(0.0f, 0.0f));

			pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			setUniform(uniformPosition, pos);

			glBindTexture(GL_TEXTURE_2D, gameState->textures.mainMenuBackground);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			
			glBindTexture(GL_TEXTURE_2D, gameState->textures.textSheet);
			glBindVertexArray(gameState->mesh.hud.mesh);
			
			float yPosition = 0.0f;
			ULoop(NumberOfHighScores-1)
			{
				//gameState->mainMenu.highScoreTable = BubblesortHighScore(gameState->mainMenu.highScoreTable);				


				char *beforeThingy = ConcatonateIntAndString((index+1), ". ");
				char* name = ConcatonateStrings(beforeThingy, gameState->mainMenu.highScoreTable.name[index]);
				char *outputName = ConcatonateStrings(name, ": \t");
				char *outputScore = ConcatonateStringAndInt(outputName, gameState->mainMenu.highScoreTable.score[index]);

				drawWord(outputScore, -0.7f, yPosition, 0.05f, gameState->shaders.hud);

				free(outputScore);
				free(outputName);
				free(name);
				free(beforeThingy);

				yPosition -= 0.1f;
			}
		}
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_BACK);

		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 1.0f, 10000.0f);
		glm::mat4 modelMatrix(1.0f), viewMatrix(1.0f), modelViewMatrix(1.0f);
		gameState->matricesUBO->setUniform("projectionMat", projection);

		gameState->lightManager->updateUniforms();

		// NOTE(Kyle): Draw Skybox
		viewMatrix = getViewMatrix(gameState->camera);
		gameState->matricesUBO->setUniform("viewMat", viewMatrix);
		gameState->matricesUBO->setUniform("eyePos", gameState->camera.pos);
		glm::mat4 rotationOnlyMat = glm::mat4(glm::transpose(glm::inverse(glm::mat3(viewMatrix))));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
		modelViewMatrix = rotationOnlyMat * modelMatrix;
		glUseProgram(gameState->shaders.skybox);
		int viewMatLocation = glGetUniformLocation(gameState->shaders.skybox, "viewMat");
		setUniform(viewMatLocation, viewMatrix);
		GLuint modelMatLocation = glGetUniformLocation(gameState->shaders.skybox, "modelMat");
		GLuint uniformSkyboxModelViewMat = glGetUniformLocation(gameState->shaders.skybox, "modelViewMat");
		GLuint uniformSkyboxProjectionMat = glGetUniformLocation(gameState->shaders.skybox, "projectionMat");
		glDepthMask(GL_FALSE);
		glCullFace(GL_FRONT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, gameState->textures.skyboxCubemap);
		setUniform(modelMatLocation, modelMatrix);
		setUniform(uniformSkyboxModelViewMat, modelViewMatrix);
		setUniform(uniformSkyboxProjectionMat, projection);
		drawMesh(gameState->mesh.cube);
		glCullFace(GL_BACK);
		glDepthMask(GL_TRUE);

		// NOTE(Kyle): Draw the target arrow
		glUseProgram(gameState->shaders.arrow);
		viewMatLocation = glGetUniformLocation(gameState->shaders.arrow, "viewMat");
		setUniform(viewMatLocation, viewMatrix);
		GLuint uniformModelViewMat = glGetUniformLocation(gameState->shaders.arrow, "modelViewMat");
		GLuint uniformProjectionMat = glGetUniformLocation(gameState->shaders.arrow, "projectionMat");
		setUniform(uniformProjectionMat, projection);
		glBindTexture(GL_TEXTURE_2D, 0);
		glm::vec3 arrowPosition = gameState->levels[gameState->currentLevel].building[gameState->levels[gameState->currentLevel].endBuilding].aabb_roof.position;
		arrowPosition.y += 600.0f;
		glm::vec3 arrowScale = glm::vec3(100.0f, 500.0f, 100.0f);
		glm::vec3 arrowRotation = glm::vec3(0.0f, 0.0f, -90.0f);
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, arrowPosition);
		modelMatrix = glm::scale(modelMatrix, arrowScale);
		modelMatrix = glm::rotate(modelMatrix, arrowRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, arrowRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, arrowRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		modelViewMatrix = viewMatrix * modelMatrix;
		setUniform(uniformModelViewMat, modelViewMatrix);
		GLuint arrowColourLocation = glGetUniformLocation(gameState->shaders.arrow, "colour");
		glUniform3f(arrowColourLocation, gameState->ArrowColour.x, gameState->ArrowColour.y, gameState->ArrowColour.z);
		drawMesh(gameState->mesh.arrow);


		// NOTE(Jonny): Render Buildings and Enemies
		glUseProgram(gameState->shaders.main);
		gameState->matricesUBO->bind();
		gameState->randomMaterial->setUniforms(gameState->shaders.main);
		setFogParameters(gameState->shaders.main, gameState->buildingFog);
		modelMatLocation = glGetUniformLocation(gameState->shaders.main, "modelMat");
		viewMatLocation = glGetUniformLocation(gameState->shaders.main, "viewMat");
		uniformModelViewMat = glGetUniformLocation(gameState->shaders.main, "modelViewMat");
		uniformProjectionMat = glGetUniformLocation(gameState->shaders.main, "projectionMat");
		setUniform(viewMatLocation, viewMatrix);
		setUniform(uniformProjectionMat, projection);

		glBindTexture(GL_TEXTURE_2D, gameState->textures.blank);
		uint32_t numberOfEnemies = ArrayCount(gameState->levels[gameState->currentLevel].enemy);

		ULoop(numberOfEnemies)
		{
			if (gameState->levels[gameState->currentLevel].enemy[index].isInitialized)
			{
				if (!gameState->levels[gameState->currentLevel].enemy[index].isDead)
				{
					modelMatrix = glm::mat4(1.0f);
					modelMatrix = glm::translate(modelMatrix, gameState->levels[gameState->currentLevel].enemy[index].pos);
					modelMatrix = glm::scale(modelMatrix, gameState->levels[gameState->currentLevel].enemy[index].scale);
					float rotation = GetAngleTowardsPoint(gameState->levels[gameState->currentLevel].enemy[index].pos,
														  gameState->camera.pos);
					modelMatrix = glm::rotate(modelMatrix,
											  rotation,
											  glm::vec3(0.0f, 1.0f, 0.0f));
					modelViewMatrix = viewMatrix * modelMatrix;

					glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

					setUniform(uniformModelViewMat, modelViewMatrix);
					setUniform(modelMatLocation, modelMatrix);
					gameState->matricesUBO->setUniform("modelMat", modelMatrix);
					gameState->matricesUBO->setUniform("modelViewMat", modelViewMatrix);
					gameState->matricesUBO->setUniform("normalMat", normalMat);
					drawMesh(gameState->mesh.enemy);
				}
			}
		}

		glBindTexture(GL_TEXTURE_2D, gameState->textures.building);
		glUseProgram(gameState->shaders.main);
		gameState->matricesUBO->bind();
		gameState->randomMaterial->setUniforms(gameState->shaders.main);
		setFogParameters(gameState->shaders.main, gameState->buildingFog);
		modelMatLocation = glGetUniformLocation(gameState->shaders.main, "modelMat");
		viewMatLocation = glGetUniformLocation(gameState->shaders.main, "viewMat");
		uniformModelViewMat = glGetUniformLocation(gameState->shaders.main, "modelViewMat");
		uniformProjectionMat = glGetUniformLocation(gameState->shaders.main, "projectionMat");
		setUniform(viewMatLocation, viewMatrix);
		setUniform(uniformProjectionMat, projection);

		uint32_t buildingNo = ArrayCount(gameState->levels[gameState->currentLevel].building);
		ULoop(buildingNo)
		{
			if (gameState->levels[gameState->currentLevel].building[index].isInitialized == true)
			{
				modelMatrix = glm::mat4(1.0f);
				modelMatrix = glm::translate(modelMatrix, gameState->levels[gameState->currentLevel].building[index].pos);
				modelMatrix = glm::rotate(modelMatrix,
										  glm::radians(gameState->levels[gameState->currentLevel].building[index].rotation.x),
										  glm::vec3(1.0f, 0.0f, 0.0f));
				modelMatrix = glm::rotate(modelMatrix,
										  glm::radians(gameState->levels[gameState->currentLevel].building[index].rotation.y),
										  glm::vec3(0.0f, 1.0f, 0.0f));
				modelMatrix = glm::rotate(modelMatrix,
										  glm::radians(gameState->levels[gameState->currentLevel].building[index].rotation.z),
										  glm::vec3(0.0f, 0.0f, 1.0f));
				modelMatrix = glm::scale(modelMatrix, gameState->levels[gameState->currentLevel].building[index].scale);
				modelViewMatrix = viewMatrix * modelMatrix;
				glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

				setUniform(uniformModelViewMat, modelViewMatrix);
				setUniform(modelMatLocation, modelMatrix);
				gameState->matricesUBO->setUniform("modelMat", modelMatrix);
				gameState->matricesUBO->setUniform("modelViewMat", modelViewMatrix);
				gameState->matricesUBO->setUniform("normalMat", normalMat);
				drawMesh(gameState->mesh.buildings[0]);

				#if COLLISION_BOXES
					drawCollisionBox(gameState->levels[gameState->currentLevel].building[index].aabb_roof,
					                 gameState->shaders.wireFrame,
					                 projection,
					                 viewMatrix);
					drawCollisionBox(gameState->levels[gameState->currentLevel].building[index].aabb_body,
					                 gameState->shaders.wireFrame,
					                 projection,
					                 viewMatrix);
				#endif
			}
		}

		if (gameState->pizzaSlice.isInitialized)
		{
			glBindTexture(GL_TEXTURE_2D, gameState->textures.blank);
			glm::mat4 viewMatrix = getViewMatrix(gameState->camera);
			glUseProgram(gameState->shaders.main);
			gameState->matricesUBO->bind();
			gameState->randomMaterial->setUniforms(gameState->shaders.main);
			modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, gameState->pizzaSlice.pos);
			modelMatrix = glm::scale(modelMatrix, gameState->pizzaSlice.scale);
			//float rotation = GetAngleTowardsPoint(gameState->pizzaSlice.pos, gameState->camera.pos);
			//modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			modelViewMatrix = viewMatrix * modelMatrix;
			glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

			setUniform(uniformModelViewMat, modelViewMatrix);
			setUniform(modelMatLocation, modelMatrix);
			gameState->matricesUBO->setUniform("modelMat", modelMatrix);
			gameState->matricesUBO->setUniform("modelViewMat", modelViewMatrix);
			gameState->matricesUBO->setUniform("normalMat", normalMat);
			drawMesh(gameState->mesh.pizzaSlice);
		}


		glUseProgram(gameState->shaders.hud);
		glBindTexture(GL_TEXTURE_2D, gameState->textures.textSheet);
		glBindVertexArray(gameState->mesh.hud.mesh);

		char *outputScore = ConcatonateStringAndInt("Score: ", gameState->camera.score);
		drawWord(outputScore, 0.7f, 0.9f, 0.03f, gameState->shaders.hud);
		free(outputScore);

		char *numberOfPizzaSlicesLeft = ConcatonateStringAndInt("Slices Left: ",
																gameState->camera.numberOfPizzaSlices);
		drawWord(numberOfPizzaSlicesLeft, -0.9f, 0.9f, 0.03f, gameState->shaders.hud);
		free(numberOfPizzaSlicesLeft);

		char *currentLevel = ConcatonateStringAndInt("Level: ", gameState->currentLevel+1);
		drawWord(currentLevel, 0.0f, 0.9f, 0.03f, gameState->shaders.hud);
		free(currentLevel);

		char *timerOutput = ConcatonateStringAndInt("Time: ", (int)gameState->timer);
		drawWord(timerOutput, -0.9f, 0.7f, 0.08f, gameState->shaders.hud);
		free(timerOutput);

		if (gameState->timer < 3.0f && !gameState->isEnterNameMessageShowing)
		{
			int actualLevelNumber = gameState->currentLevel;
			++actualLevelNumber;
			char *level = ConcatonateStringAndInt("Level: ", actualLevelNumber);
			drawWord(timerOutput, -0.6f, 0.0f, 0.125f, gameState->shaders.hud);
			free(level);
		}


		if (gameState->isEnterNameMessageShowing)
		{
			char *output = ConcatonateStrings("Enter Name: ", gameState->currentName);
			drawWord(output, -0.9f, 0.0f, 0.05f, gameState->shaders.hud);
			free(output);
		}


		#if DRAW_BOXES
			drawAABB(gameState->camera.aabb, gameState->shaders.wireFrame, projection, viewMatrix);
		#endif
	}
}