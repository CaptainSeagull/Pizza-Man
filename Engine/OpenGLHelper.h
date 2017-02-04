#ifndef OPENGL_HELPER_H
#define OPENGL_HELPER_H

#include <GL\glew.h>

#include "stb_image.c"

#include <fstream>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <string>
#include "Junk.h"

#include "Game.h"

	struct Vector2
	{
		float x, y;
		void Set(float x, float y) { this->x = x; this->y = y; }
	};

    #define OPENGL_VERTEX       0
    #define OPENGL_TEXCOORD     1


    inline void setUniform(GLuint index, float data)
    {
        // TODO(Jonny): Do an assert to make sure the index is valid
        glUniform1f(index, data);
    }

    inline void setUniform(GLuint index, glm::vec2 data)
    {
        // TODO(Jonny): Do an assert to make sure the index is valid
        glUniform2f(index, data.x, data.y);
    }

    inline void setUniform(GLuint index, glm::mat4 data)
    {
        // TODO(Jonny): Do an assert to make sure the index is valid
        glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(data));
    }



    char * loadFile(const char *fileName, GLint &fileSize)
    {
        int size = 0;
        char *memblock = NULL;

        std::ifstream file (fileName, std::ios::in|std::ios::binary|std::ios::ate);
        if (file.is_open())
        {
            size = (int) file.tellg();
            fileSize = (GLint) size;
            //memblock = new char [size+1];
			memblock = (char *)malloc(sizeof(char *) * (size*1));
            memblock[size] = '\n';
            file.seekg (0, std::ios::beg);
            file.read (memblock, size);
            file.close();
        }
        else
        {
            fileSize = 0;
            std::cout << "Failed to Load File: " << fileName << std::endl;
        }

        return memblock;
    }

    void printShaderError(const GLint shader)
    {
        int maxLength = 0;
        int logLength = 0;
        GLchar *logMessage;

        
        if (!glIsShader(shader))
        {
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        }
        else
        {
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        }

        if (maxLength > 0)
        {
			logMessage = (GLchar*)malloc(maxLength * sizeof(char));
            if (!glIsShader(shader))
            {
                glGetProgramInfoLog(shader, maxLength, &logLength, logMessage);
            }
            else
            {
                glGetShaderInfoLog(shader,maxLength, &logLength, logMessage);
            }

            std::cout << logMessage << std::endl;
			free(logMessage);
        }

        // TODO(Jonny):     Perform additional checks for OpenGL errors here

    }




    GLuint initShaders(const char *vertFile, const char *fragFile)
    {
        GLuint result = 0;

        GLuint fragmentShader   = 0;
        GLuint vertexShader     = 0;

        char *vertexText    = NULL;
        char *fragmentText  = NULL;

        vertexShader    = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader  = glCreateShader(GL_FRAGMENT_SHADER);    

        GLint vertexlength      = 0;
        GLint fragmentlength    = 0;
        vertexText      = loadFile(vertFile, vertexlength);
        fragmentText    = loadFile(fragFile, fragmentlength);
        
        const char *const_vertexText    = vertexText;
        const char *const_fragmentText  = fragmentText;

        glShaderSource(vertexShader, 1, &const_vertexText, &vertexlength);
        glShaderSource(fragmentShader, 1, &const_fragmentText, &fragmentlength);
        
        GLint vertexCompiled    = 0;
        GLint fragmentCompiled  = 0;

        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);

        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompiled);


        if ((vertexCompiled) && (fragmentCompiled))
        {   
            result = glCreateProgram();
                
            glAttachShader(result, vertexShader);
            glAttachShader(result, fragmentShader);

            glBindAttribLocation(result, OPENGL_TEXCOORD, "in_TexCoord");
            glBindAttribLocation(result, OPENGL_VERTEX, "in_Position");

            glLinkProgram(result);
            glUseProgram(result);

			GLuint matricesIndex = glGetUniformBlockIndex(result, "MatrixBlock");
			if (matricesIndex == GL_INVALID_INDEX)
				std::cout << "Could not get index for uniform block MatrixBlock" << std::endl;
			glUniformBlockBinding(result, matricesIndex, 0);

			GLuint lightsIndex = glGetUniformBlockIndex(result, "LightBlock");
			if (lightsIndex == GL_INVALID_INDEX)
				std::cout << "Could not get index for uniform block LightBlock" << std::endl;
			glUniformBlockBinding(result, lightsIndex, 1);
        }
        else if (!vertexCompiled)
        {
            printShaderError(vertexShader);
        }
        else
        {
            printShaderError(fragmentShader);       
        }

        // NOTE(Jonny): Delete Memory Allocated in loadFile function
        free(vertexText);
        free(fragmentText);

        return result;
    }




    GLuint loadImage(char *fileName, int req_comp, uint32_t &width, uint32_t &height)
    {
        GLuint result = 0;

        int widthAsInt;
		int heightAsInt;
        int componentsInImageFile;
        unsigned char* image = NULL;
        image = stbi_load(fileName,
                          &widthAsInt,
                          &heightAsInt,
                          &componentsInImageFile,
						  req_comp);

		width = widthAsInt;
		height = heightAsInt;

        if (image != NULL)
        {
            glGenTextures(1, &result);

            glBindTexture(GL_TEXTURE_2D, result);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

			if ((req_comp == 2) || (req_comp == 4))
            {
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             GL_RGBA,
							 widthAsInt, heightAsInt,
                             0,
                             GL_RGBA,
                             GL_UNSIGNED_BYTE,
							 image);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             GL_RGB,
							 widthAsInt, heightAsInt,
                             0,
                             GL_RGB,
                             GL_UNSIGNED_BYTE,
							 image);
            }

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(image);
        }
        else
        {
            std::cout << "Failed to Load Image: " << fileName << std::endl;
		}

        return result;
    }

    inline GLuint loadImage(char *fileName, int req_comp)
    {
    	uint32_t stuff;
    	return loadImage(fileName, req_comp, stuff, stuff);
    }


	GLuint loadCubemap(std::string folder, std::string back, std::string front, std::string right, std::string left, std::string up, std::string down)
	{
		GLuint id = 0;
		glGenTextures(1, &id); // NOTE(Kyle): generate texture ID
		GLenum sides[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y };

		std::string backS = folder + "/" + back;
		std::string frontS = folder + "/" + front;
		std::string rightS = folder + "/" + right;
		std::string leftS = folder + "/" + left;
		std::string upS = folder + "/" + up;
		std::string downS = folder + "/" + down;

		const char *fname[6] =
		{
			backS.c_str(),
			frontS.c_str(),
			rightS.c_str(),
			leftS.c_str(),
			upS.c_str(),
			downS.c_str()
		};

		// NOTE(Kyle): bind texture and set parameters
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int i = 0; i<6; i++)
		{
			int width;
			int height;
			int componentsInImageFile;
			unsigned char* image = NULL;
			image = stbi_load(fname[i],
				&width,
				&height,
				&componentsInImageFile,
				3);

			glTexImage2D(sides[i], 0, GL_RGB, width, height, 0,
						GL_RGB, GL_UNSIGNED_BYTE, image);

			stbi_image_free(image);
		}

		return id;
	}

	Mesh loadMesh(const char *filename)
	{
		Mesh returnMesh;
		
		Assimp::Importer *importer = new Assimp::Importer();
		const aiScene *scene = importer->ReadFile(filename, 
			aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords | aiProcess_PreTransformVertices);

 		returnMesh.numMeshes = scene->mNumMeshes;
		returnMesh.numOfIndices = new GLuint[returnMesh.numMeshes];
		returnMesh.vaos = new GLuint[returnMesh.numMeshes];
		returnMesh.bodyAABB = AABB();
		returnMesh.roofAABB = AABB();

		// NOTE(Kyle): generate and set up a VAO for each mesh
		glGenVertexArrays(returnMesh.numMeshes, returnMesh.vaos);

		for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
		{
			std::vector<GLuint> indices;
			const aiMesh* currentMesh = scene->mMeshes[meshIndex];

			for (unsigned int faceIndex = 0; faceIndex < currentMesh->mNumFaces; faceIndex++)
			{
				aiFace face = currentMesh->mFaces[faceIndex];

				assert(face.mNumIndices == 3);

				for (int index = 0; index < 3; index++)
				{
					// NOTE(Kyle): Add indices
					indices.push_back(face.mIndices[index]);
				}
			}

			returnMesh.numOfIndices[meshIndex] = indices.size();

			glBindVertexArray(returnMesh.vaos[meshIndex]);

			GLuint VBO;

			if (currentMesh->HasPositions())
			{
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, currentMesh->mNumVertices * 3 * sizeof(GLfloat), currentMesh->mVertices, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(0);
			}

			// NOTE(Kyle): VBO for normal data
			if (currentMesh->HasNormals())
			{
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, currentMesh->mNumVertices * 3 * sizeof(GLfloat), currentMesh->mNormals, GL_STATIC_DRAW);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(1);
			}

			// NOTE(Kyle): VBO for tex-coord data
			if (currentMesh->HasTextureCoords(0))
			{
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, currentMesh->mNumVertices * 3 * sizeof(GLfloat), currentMesh->mTextureCoords[0], GL_STATIC_DRAW);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(2);
			}

			if (indices.size() > 0)
			{
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
			}

			// NOTE(Kyle): unbind vertex array
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		}

		return returnMesh;
	}

	void drawMesh(Mesh mesh)
	{
		for (uint32_t i = 0; i < mesh.numMeshes; i++)
		{
			glBindVertexArray(mesh.vaos[i]);
			glDrawElements(GL_TRIANGLES, mesh.numOfIndices[i], GL_UNSIGNED_INT, 0);
		}
	}


	// TODO(Jonny): Tidy this function up properly
	Vector2 generate2DMesh(GLuint VBOarray[], GLuint &mesh,
				           float textureWidth, float textureHeight,
				           float noOfFrames_x, float noOfFrames_y)
	{
		Vector2 result = {};


		glGenVertexArrays(1, &mesh);
		glBindVertexArray(mesh);

		float desiredSize_x = 2.0f / (float)(1980 / textureWidth);
		float desiredSize_y = 2.0f / (float)(1080 / textureHeight);

		float temp_x = desiredSize_x/4.0f;
		float temp_y = desiredSize_x/4.0f;

		desiredSize_x /= 2.0f;
		desiredSize_x += temp_x;

		desiredSize_y /= 2.0f;
		desiredSize_y += temp_y;

		result.x = desiredSize_x * 0.1f;
		result.y = desiredSize_y * 0.1f;


		GLfloat vertices[] =
		{
			-1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f
		};

		float texHeight = 1.0f / noOfFrames_y;
		float texWidth  = 1.0f / noOfFrames_x;


		GLfloat texCoords[] =
		{
			0.0f, 0.0f, 0.0f,
			0.0f, texHeight, 0.0f,
			texWidth, 0.0f, 0.0f,
			texWidth, texHeight, 0.0f
		};

		glGenBuffers(1, &VBOarray[OPENGL_VERTEX]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOarray[OPENGL_VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, 3*4*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)OPENGL_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(OPENGL_VERTEX);

		glGenBuffers(1, &VBOarray[OPENGL_TEXCOORD]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOarray[OPENGL_TEXCOORD]);
		glBufferData(GL_ARRAY_BUFFER, 3*4*sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)OPENGL_TEXCOORD, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(OPENGL_TEXCOORD);


		return result;
	}

#endif