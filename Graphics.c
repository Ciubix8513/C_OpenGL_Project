#include "Graphics.h"

Material *materials;
size_t materialsSize;

Model *models;
size_t modelsSize;

GLFWwindow *wnd;
long long prevTime, startT;
double dT;
vec2 res;

void TestFunc(Material);

int InitOpenGL()
{
	glewInit();
	glClearColor(0.257f, .711f, .957f, 1.0f);

	// Set up viewport
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(wnd);

	int s = SetupMaterials();
	int s3 = SetupModels();

	startT = getTick();
	int w, h;
	glfwGetWindowSize(wnd, &w, &h);

	res = Vec2(w, h);

	glEnable(GL_MULTISAMPLE);
	return s && s3;
}

int SetupModels()
{
	modelsSize = 2;
	models = calloc(modelsSize, sizeof(Model));

	models[0].transform.position = Vec3(0,2,0);
	models[0].transform.scale = Vec3(1,1,1);
	models[0].transform.rotation = Vec3(0,0,0);
	models[0].material = materials;

	models[1].transform.position = Vec3(0,0,0);
	models[1].transform.scale = Vec3(1,1,1);
	models[1].transform.rotation = Vec3(0,0,0);
	models[1].material = materials + 1;
	//  TODO add model filename to the struct and make it similar to the shader setup
	return LoadModel("Res/blahaj1.mdl", &models[0]) && LoadModel("Res/Ground.mdl", &models[1]);
}
int SetupMaterials()
{
	// Manual setup (for now at least)
	materialsSize = 2;
	materials = calloc(materialsSize, sizeof(Material));	

	materials[0].vFileName = "Res/shader.vert";
	materials[1].vFileName = "Res/shader.vert";

	materials[0].fFileName = "Res/texture.frag\0";
	materials[1].fFileName = "Res/shader.frag";

	materials[0].LoadData = &LoadTextureShaderData;
	materials[1].LoadData = &LoadShaderData;

	materials[0].samplerArrSize = 1;
	materials[0].samplerArr = calloc(materials[0].samplerArrSize, sizeof(uint));
	printf("Setting up shaders\n");
	for (size_t i = 0; i < materialsSize; i++)
		if (!setupShader(&materials[i]))
			return 0;
	setupTexture("Res/blahaj.bmp", materials[0].samplerArr);
	return 1;
	
}

int LoadModel(const char *filename, Model *model)
{
	Mesh m;
	if (!LoadMDL(filename, &m))
	{
		printf("could not load model\n");
		return 0;
	}

	glGenBuffers(1, &model->VBO);
	glGenBuffers(1, &model->EBO);
	glGenVertexArrays(1, &model->VAO);

	glBindVertexArray(model->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, m.numVertices * sizeof(Vertex), m.vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.numIndices * sizeof(unsigned int), m.Indices, GL_STATIC_DRAW);
	model->ind_size = m.numIndices;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, UV));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

	glBindVertexArray(0);

	free(m.vertices);
	free(m.Indices);
	return 1;
}

int setupShader(Material *material)
{
	FILE *f = fopen(material->vFileName, "r");

	if (f == NULL)
	{
		printf("Was not able to open %s\n", material->vFileName);
		return 0;
	}	
	fseek(f, 0, SEEK_END);
	int l = ftell(f);
	rewind(f);
	
	char *vFile = malloc(l);
	memset(vFile, 0, l);
	fread(vFile, 1, l, f);
	
	fclose(f);
	vFile[l] = '\0';	
	uint vert = glCreateShader(GL_VERTEX_SHADER);	
	glShaderSource(vert, 1,&vFile, NULL);
	free(vFile);	
	
	f = fopen(material->fFileName, "r");
	if (f == NULL)
	{
		printf("Was not able to open %s\n", material->fFileName);
		return 0;
	}

	fseek(f, 0, SEEK_END);
	l = ftell(f);
	rewind(f);

	char *fFile = malloc(l);
	memset(fFile, 0, l);
	fread(fFile, 1, l, f);

	fclose(f);
	uint frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fFile, &l);
	free(fFile);
	glCompileShader(vert);
	int Comp = 0;
	glGetShaderiv(vert, GL_COMPILE_STATUS, &Comp);

	if (Comp == GL_FALSE)
	{
		int mL = 0;
		glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &mL);
		char *log = malloc(mL);

		glGetShaderInfoLog(vert, mL, &mL, log);
		printf("vert Log = %s", log);
		free(log);

		return 0;
	}
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &Comp);

	if (Comp == GL_FALSE)
	{
		int mL = 0;
		glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &mL);
		char *log = malloc(mL);

		glGetShaderInfoLog(frag, mL, &mL, log);
		printf("frag Log = %s", log);
		free(log);
		return 0;
	}

	material->ShaderProg = glCreateProgram();
	glAttachShader(material->ShaderProg, vert);
	glAttachShader(material->ShaderProg, frag);

	glLinkProgram(material->ShaderProg);

	glDetachShader(material->ShaderProg, vert);
	glDetachShader(material->ShaderProg, frag);

	glDeleteShader(vert);
	glDeleteShader(frag);

	return 1;
}

int setupTexture(const char *filename, uint *texture)
{

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int w, h;
	unsigned char bpp;
	unsigned char *data = LoadBMP("Res/blahaj.bmp", &h, &w, &bpp, 0);
	if (!data)
	{
		printf("Could not setup texture \n");
		return 0;
	}
	if (bpp == 32)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	else if (bpp == 24)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	free(data);

	return 1;
}

void Render()
{
	long long cTime = getTick();
	long long t = cTime - startT;

	glClearColor(0.257f, .711f, .957f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 proj = PerspectiveProj(FreeCam.FOV, res.x / res.y, FreeCam.ScreenNear, FreeCam.ScreenFar);
	mat4 camera = LookAtCam(FreeCam);

	//glUniform1f(3, t / 1000.0f);

	for (size_t i = 0; i < modelsSize; i++)
	{
		
		glUseProgram(models[i].material->ShaderProg);
		mat4 world = STransformMat(&models[i].transform);


		glUniformMatrix4fv(0, 1, 0, &proj);
		glUniformMatrix4fv(1, 1, 0, &camera);
		glUniformMatrix4fv(2, 1, 1, &world);

		models[i].material->LoadData(models[i].material);
		glBindVertexArray(models[i].VAO);

		glDrawElements(GL_TRIANGLES, models[i].ind_size, GL_UNSIGNED_INT, 0);
	}

	dT = (cTime - prevTime) / 1000;
	prevTime = cTime;
}

int InitWindow(int height, int width, char *title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_SAMPLES, 4);

	wnd = glfwCreateWindow(width, height, title, NULL, NULL);

	if (wnd == NULL)
	{
		printf("Failed to create glfw window\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(wnd);
	return 0;
}

uint32_t getTick()
{
	struct timespec ts;
	unsigned theTick = 0U;
	clock_gettime(CLOCK_REALTIME, &ts);
	theTick = ts.tv_nsec / 1000000;
	theTick += ts.tv_sec * 1000;
	return theTick;
}

void Cleanup()
{
	// Cleanup materials
	for (size_t i = 0; i < materialsSize; i++)
	{		
		glDeleteTextures(materials[i].samplerArrSize, materials[i].samplerArr);		
		glDeleteProgram(materials[i].ShaderProg);			
		free(materials[i].intArr);
		free(materials[i].floatArr);
		free(materials[i].float2Arr);
		free(materials[i].float3Arr);
		free(materials[i].float4Arr);
		free(materials[i].mat4Arr);
	}

	for (size_t i = 0; i < modelsSize; i++)
	{
		glDeleteBuffers(1, &models[i].VBO);
		glDeleteBuffers(1, &models[i].EBO);
		glDeleteVertexArrays(1, &models[i].VAO);
	}
	free(models);
	free(materials);

	glfwTerminate();
}

void LoadTextureShaderData(Material *self)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE0, self->samplerArr[0]);
}

void LoadShaderData(Material *self)
{
	return;
}