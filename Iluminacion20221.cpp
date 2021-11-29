/*
Semestre 2022-1
Práctica : Iluminación
Cambios en el shader, en lugar de enviar la textura en el shader de fragmentos, enviaremos el finalcolor
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"


//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

//conejo
float movConejo;
//float movCocheZ;
float movOffsetCon;
float rotCone;
bool avanzaCon;
float avanzafCon = 1.0f;
float vueltaCon;
float rotConeOffset;
//conejo
float movPerro;
//float movCocheZ;
float movOffsetPerro;
float rotPerro;
bool avanzaperro;
float avanzafPerro = 1.0f;
float vueltaPerro;
float rotPerroOffset;


//carrito
float movCoche;
float movCocheZ;
float movOffset;
float rotllanta;
float rotllantaOffset;
float rotCoche;
bool avanza;
float avanzaf = 1.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;


Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model Casa_T;
Model Cocina;
Model Sofa_A;
Model Tele_A;
Model Alfombra_A;
Model Lampara_N;
Model Regalos_dos;
Model Mesita_Ce;
Model Cartas;
Model Luces_P;
Model Piso_C;
Model Cuarto_Prof;
Model Baño_C;
Model Pavo_C;
Model Comida_C;
Model Pan_C;
Model Casita_G;
Model Bota_R;
Model Taza_C;
Model Adorno_A1;
Model Adorno_A2;
Model Carrito;
Model llanta_M;
Model Conejo;
Model Perro;
Model Bellota;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};


	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");

	Dado_M = Model();
	Dado_M.LoadModel("Models/dadoanimales.obj");
	Casa_T = Model();
	Casa_T.LoadModel("Models/casa_piso.obj");
	Cocina = Model();
	Cocina.LoadModel("Models/cocina_integral.obj");
	Sofa_A = Model();
	Sofa_A.LoadModel("Models/sofa_a.obj");
	Tele_A = Model();
	Tele_A.LoadModel("Models/tele.obj");
	Alfombra_A = Model();
	Alfombra_A.LoadModel("Models/alfombra.obj");
	Lampara_N = Model();
	Lampara_N.LoadModel("Models/lamparas_N.obj");
	Regalos_dos = Model();
	Regalos_dos.LoadModel("Models/2regalos.obj");
	Mesita_Ce = Model();
	Mesita_Ce.LoadModel("Models/mesita2.obj");
	Cartas = Model();
	Cartas.LoadModel("Models/cartas_arb.obj");
	Luces_P = Model();
	Luces_P.LoadModel("Models/lucesitas.obj");
	Piso_C = Model();
	Piso_C.LoadModel("Models/piso.obj");
	Cuarto_Prof = Model();
	Cuarto_Prof.LoadModel("Models/CuartoProf.obj");
	Baño_C = Model();
	Baño_C.LoadModel("Models/Baño.obj");
	Pavo_C = Model();
	Pavo_C.LoadModel("Models/pavo.obj");
	Comida_C = Model();
	Comida_C.LoadModel("Models/comida.obj");
	Pan_C = Model();
	Pan_C.LoadModel("Models/pan.obj");
	Casita_G = Model();
	Casita_G.LoadModel("Models/casita_ga.obj");
	Bota_R = Model();
	Bota_R.LoadModel("Models/bota.obj");
	Taza_C = Model();
	Taza_C.LoadModel("Models/taza.obj");
	Adorno_A1 = Model();
	Adorno_A1.LoadModel("Models/adorno_fuera.obj");
	Adorno_A2 = Model();
	Adorno_A2.LoadModel("Models/arbol_fuera.obj");
	Carrito = Model();
	Carrito.LoadModel("Models/Carrito.obj");
	llanta_M = Model();
	llanta_M.LoadModel("Models/llanta.obj");
	Conejo = Model();
	Conejo.LoadModel("Models/Conejo.obj");
	Perro = Model();
	Perro.LoadModel("Models/pug.obj");
	Bellota = Model();
	Bellota.LoadModel("Models/Bellota2.obj");


	std::vector<std::string> skyboxFaces;

	skyboxFaces.push_back("Textures/Skybox/dia6.tga");
	skyboxFaces.push_back("Textures/Skybox/dia3.tga");
	skyboxFaces.push_back("Textures/Skybox/dia4.tga");
	skyboxFaces.push_back("Textures/Skybox/dia4.tga");
	skyboxFaces.push_back("Textures/Skybox/dia5.tga");
	skyboxFaces.push_back("Textures/Skybox/dia5.tga");

	/*skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");*/

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//posición inicial del helicóptero
	glm::vec3 posblackhawk = glm::vec3(-20.0f, 6.0f, -1.0);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		45.0f, 45.0f,
		60.0f, 38.0f, 12.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		40.0f, 40.0f,
		45.0f, 38.0f, 115.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(0.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		-5.0f, 65.0f, 85.0f, //donde está
		-5.0f, 0.0f, 0.0f, //adonde apunta
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		-5.0f, 65.0f, 65.0f, //donde está
		-5.0f, 0.0f, 0.0f, //adonde apunta
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movCocheZ = 0.0f;
	movOffset = 0.5f;
	rotllanta = 0.0f;
	rotCoche = 0.0f;
	rotllantaOffset = 10.0f;

	movConejo = 0.0f;
	movOffsetCon = 0.5f;
	rotCone = 0.0f;
	rotConeOffset = 10.0f;
	vueltaCon = 0.0f;

	movPerro = 0.0f;
	movOffsetPerro = 0.5f;
	rotPerro = 0.0f;
	rotPerroOffset = 10.0f;
	vueltaPerro = 0.0f;
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		printf("avanza vale %f \n", avanzaf);

		//conejo
		if (avanzaCon > 0)
		{
			if (movConejo < -90.0f)
			{
				movConejo -= movOffsetCon * deltaTime;

			}
			else
			{
				avanzaCon = -1.0f;
				//vueltaCon = -1.0f;
			}
		}

		else
		{
			if (movConejo > -2.0f)
			{
				movConejo += movOffsetCon * deltaTime;

			}
			else {
				avanzaCon = 1.0f;
				vueltaCon = 1.0f;
			}
		}
		rotCone += rotConeOffset * deltaTime;


		//Perro
		if (avanzaCon > 0)
		{
			if (movPerro > -89.0f)
			{
				movPerro += movOffsetCon * deltaTime;

			}
			else
			{
				avanzafPerro = -1.0f;
				vueltaPerro = -1.0f;
			}
		}

		else
		{
			if (movPerro > -90.0f)
			{
				movPerro -= movOffsetPerro * deltaTime;

			}
			else {
				avanzafPerro = 1.0f;
				vueltaPerro = 1.0f;
			}
		}
		rotPerro += rotPerroOffset * deltaTime;

		//carrito 
		if (avanzaf > 0)
		{
			if (movCoche < 3.0f)
			{
				movCoche += movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
			}
			else {
				avanzaf = -1.0f;

			}
		}
		else
		{
			if (movCoche > -6.0f)
			{
				movCoche -= movOffset * deltaTime;
				rotllanta -= rotllantaOffset * deltaTime;

			}
			else {
				avanzaf = 1.0f;

			}
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//luz ligada a la cámara de tipo flash 
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		//agregar material al plano de piso
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Casa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -0.1f));
		model = glm::scale(model, glm::vec3(180.0f, 140.0f, 230.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Casa_T.RenderModel();

		//Cocina
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, 32.0f, 118.0f));
		model = glm::scale(model, glm::vec3(80.0f, 80.0f, 80.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cocina.RenderModel();

		//pavo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 20.5f, 94.5f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pavo_C.RenderModel();

		//comida
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, 21.0f, 106.5f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Comida_C.RenderModel();

		//Taza
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(49.0f, 20.9f, 106.5f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Taza_C.RenderModel();

		//Pan
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(59.0f, 20.0f, 98.5f));
		model = glm::scale(model, glm::vec3(18.0f, 18.0f, 18.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pan_C.RenderModel();

		//Casita de galleta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 22.0f, 109.0f));
		model = glm::scale(model, glm::vec3(19.0f, 19.0f, 19.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Casita_G.RenderModel();

		//Baño
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, 5.0f, 10.0f));
		model = glm::scale(model, glm::vec3(22.0f, 21.0f, 22.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Baño_C.RenderModel();

		//Sofa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 40.0f, 110.0f));
		model = glm::scale(model, glm::vec3(90.0f, 90.0f, 100.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sofa_A.RenderModel();

		// BOTAS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, 40.0f, 110.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bota_R.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, 40.0f, 90.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bota_R.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, 40.0f, 70.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bota_R.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, 40.0f, 50.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bota_R.RenderModel();

		//BOTAS

		//Tele
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 40.0f, 70.0f));
		model = glm::scale(model, glm::vec3(80.0f, 80.0f, 90.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tele_A.RenderModel();

		//Cartas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 23.5f, 40.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cartas.RenderModel();

		//Alfombra
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 22.0f, 95.0f));
		model = glm::scale(model, glm::vec3(45.0f, 45.0f, 55.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Alfombra_A.RenderModel();

		//Mesita centro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 3.0f, 100.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesita_Ce.RenderModel();

		//Lámparas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 13.5f, 96.5f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_N.RenderModel();

		//Cuarto Profesor Utonio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(51.0f, 75.0f, 116.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuarto_Prof.RenderModel();

		//Adornos de afuera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.0f, 5.0f, -60.0f));
		model = glm::scale(model, glm::vec3(110.0f, 110.0f, 120.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Adorno_A1.RenderModel();

		//árboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 7.0f, -60.0f));
		model = glm::scale(model, glm::vec3(500.0f, 500.0f, 500.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Adorno_A2.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 7.0f, -180.0f));
		model = glm::scale(model, glm::vec3(500.0f, 500.0f, 500.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Adorno_A2.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 7.0f, -140.0f));
		model = glm::scale(model, glm::vec3(500.0f, 500.0f, 500.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Adorno_A2.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, 7.0f, -58.0f));
		model = glm::scale(model, glm::vec3(500.0f, 500.0f, 500.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Adorno_A2.RenderModel();

		//carrito 
		modelaux = glm::mat4(1.0);
		model = glm::mat4(1.0);
		modelaux = model = glm::translate(model, glm::vec3(movCoche * 30, 7.0f, -320.0f + movCocheZ));
		//modelaux = model;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotCoche * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		//model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Carrito.RenderModel();
		//llantas
		model = modelaux;
		//spotLights[1].SetPos(glm::vec3(movCoche - 9.0, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(12.5, -1.5f, 5.5f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians * 15, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotCoche * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llanta_M.RenderModel();
		model = modelaux;
		//spotLights[1].SetPos(glm::vec3(movCoche - 9.0, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-12.5, -1.5f, 5.5f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians * 15, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotCoche * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llanta_M.RenderModel();
		model = modelaux;
		//spotLights[1].SetPos(glm::vec3(movCoche - 9.0, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-12.5, -1.5f, -5.5f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians * 15, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotCoche * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llanta_M.RenderModel();
		model = modelaux;
		//spotLights[1].SetPos(glm::vec3(movCoche - 9.0, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(12.5, -1.5f, -5.5f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians * 15, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotCoche * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llanta_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.0f, 100.2f * 0.1 * sin(rotCone * 0.5), -100.0f + movConejo));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Conejo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f + movPerro * 2.2, 8.0f, -100.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Perro.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevexChica(), 4.0f + mainWindow.getmueveyChica(), -70.1f + mainWindow.getmuevezChica()));
		model = glm::scale(model, glm::vec3(120.0f, 120.0f, 120.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bellota.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
