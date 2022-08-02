#include "Graphics.h"

GLFWwindow *wnd;
UINT VBO, VAO, EBO, texture, ind_size;
long long prevTime, startT;
double dT;
mat4 proj, camera;
vec2 res;
Camera cam;
UINT ShaderProg;
unsigned int *ind;

int InitOpenGL()
{
	glewInit();
	glClearColor(0.257f, .711f, .957f, 1.0f);
	// Set up viewport
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(wnd);

	setUpModel();

	cam.FOV = 90.0f;
	cam.ScreenNear = .1f;
	cam.ScreenFar = 100.0f;
	cam.position = Vec3(0, -3, -15);
	cam.rotation = Vec3(0, 0, 0);
	int s = setupShaders();
	int s1 = setupTexture();
	startT = getTick();
	int w, h;
	glfwGetWindowSize(wnd, &w, &h);

	res = Vec2(w, h);

	glEnable(GL_MULTISAMPLE);
	return s && s1;
}

void setUpModel()
{
	Mesh m;
	if (!LoadMDL("Res/blahaj.mdl", &m))
	{
		printf("could not load model\n");
		return;
	}

	// printf("Model:\natr = %i\nnumInd = %i\nnumVert = %i\n",m.atr,m.numIndices,m.numVertices);
	VAO = 0;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m.numVertices * sizeof(Vertex), m.vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.numIndices * sizeof(unsigned int), m.Indices, GL_STATIC_DRAW);
	ind_size = m.numIndices;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, UV));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

	glBindVertexArray(0);

	free(m.vertices);
	free(m.Indices);
}

int setupShaders()
{
	UINT frag = 0;
	UINT vert = 0;

	char *vPath = "Res/shader.vert";
	char *fPath = "Res/shader.frag";
	FILE *f;

	f = fopen(vPath, "r");
	// if (fopen_s(&f, vPath, "r") != 0)
	if (f == NULL)
		return 0;
	fseek(f, 0, SEEK_END);
	int l = ftell(f);
	rewind(f);

	char *vFile = malloc(l);
	// ZeroMemory(vFile, l);
	memset(vFile, 0, l);
	fread(vFile, 1, l, f);

	fclose(f);
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vFile, &l);
	free(vFile);
	// glShaderSource()
	// if (fopen_s(&f, fPath, "r") != 0)
	f = fopen(fPath, "r");
	if (f == NULL)
		return 0;

	fseek(f, 0, SEEK_END);
	l = ftell(f);
	rewind(f);

	char *fFile = malloc(l);
	// ZeroMemory(fFile, l);
	memset(fFile, 0, l);
	fread(fFile, 1, l, f);

	fclose(f);
	frag = glCreateShader(GL_FRAGMENT_SHADER);
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
		// glDeleteShader( vert);
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
		// glDeleteShader( frag);
	}

	ShaderProg = glCreateProgram();
	glAttachShader(ShaderProg, vert);
	glAttachShader(ShaderProg, frag);

	glLinkProgram(ShaderProg);

	glDetachShader(ShaderProg, vert);
	glDetachShader(ShaderProg, frag);

	glDeleteShader(vert);
	glDeleteShader(frag);

	return 1;
}

int setupTexture()
{

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

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
	glClearColor(0.257f, .711f, .957f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ShaderProg);

	proj = PerspectiveProj(cam.FOV, res.x / res.y, cam.ScreenNear, cam.ScreenFar);
	// proj = PerspectiveGLU(cam.FOV, res.x / res.y, cam.ScreenNear, cam.ScreenFar);
	//  camera = LookAt(Vec3(0,0,-5),Vec3(0,0,1),Vec3(0,1,0));//LookAtcam(FreeCam);//(Vec3(0, -.5f, 2.5f), Vec3(0, 0, 1), Vec3(0, 1, 0));
	//  camera = Mat4m(
	//  	1,0,0,0,
	//  	0, 1, 0, 0,
	//  	0,0,1,0,
	//  	0,0,5,1
	//  );
	camera = TransformMat(minusVec(FreeCam.position), minusVec(FreeCam.rotation), Vec3(1, 1, 1)); // LookAtcam(FreeCam);
	camera = LookAtCam(FreeCam);
	// camera = Mat4m(
	// 	0.573576, 0 , 0.81915, 0,
	// 	0, 1, 0, 0,
	// 	-0.8192, 0 , 0.5735, 5,
	// 	0,0,0,1
	// );

	glUniformMatrix4fv(0, 1, 0, &proj);
	glUniformMatrix4fv(1, 1, 0, &camera);

	long long cTime = getTick();
	long long t = cTime - startT;
	glUniform1f(3, t / 1000.0f);

	glUniform2fv(4, 1, &res);
	// mat4 i
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	dT = (cTime - prevTime) / 1000;
	prevTime = cTime;
	// printf("dT = %i\n", dT);
	glBindVertexArray(VAO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, ind_size, GL_UNSIGNED_INT, 0);

	//	SwapBuffers(dc);
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
	glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(ShaderProg);
	glfwTerminate();
}
