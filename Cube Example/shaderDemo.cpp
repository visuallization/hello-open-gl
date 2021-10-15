#include <math.h>
#include <string>

#include "shaderDemo.h"

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#include "VSMathlib.h"
#include "VSShaderlib.h"

VSMathLib* vsml;
VSShaderLib shader;
VSShaderLib shader2;

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = -43.0f, beta = 48.0f;
float r = 5.25f;

// Frame counting and FPS computation
long myTime, timebase = 0, frame = 0;
char s[32];

GLuint vao;
GLuint vao2;

float hour = 0.0;
float day = 0.0;
float inc = 0.1;



// ------------------------------------------------------------
//
// Reshape Callback Function
//

void changeSize(int w, int h) {

	float ratio;
	// Prevent a divide by zero, when window is too short
	if (h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;
	vsml->loadIdentity(VSMathLib::PROJECTION);
	vsml->perspective(53.13f, ratio, 0.1f, 1000.0f);
}

void resize(int width, int height)
{
	// prevent division by zero
	if (height == 0) { height = 1; }

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}


// ------------------------------------------------------------
//
// Render stuff
//



void renderScene(void) {
	hour += inc;
	day += inc / 24.0;
	hour = hour - ((int)(hour / 24)) * 24;
	day = day - ((int)(day / 365)) * 365;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// load identity matrices	
	vsml->loadIdentity(VSMathLib::VIEW);
	vsml->loadIdentity(VSMathLib::MODEL);
	// set the camera using a function similar to gluLookAt
	vsml->lookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
	//vsml->lookAt(-2.39596820, 3.90001798, 2.57132173, 0,0,0, 0,1,0);

	// use our shader
	glUseProgram(shader.getProgramIndex());

	//vsml->pushMatrix(VSMathLib::MODEL);

	//vsml->rotate(360 * day / 365.0, 0, 1, 0);
	//vsml->rotate(15.0, 1.0, 0.0, 0.0);

	vsml->matrixToGL(VSMathLib::PROJ_VIEW_MODEL);

	shader.setUniform("angle", 360.0f * day / 365.0f);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, 0);

	//vsml->matricesToGL();
	//vsml->popMatrix(VSMathLib::MODEL);

	glUseProgram(shader2.getProgramIndex());

	//float translate[4][4] = {
	//	{1, 0, 0, 0},
	//	{0, 1, 0, 0},
	//	{0, 0, 1, 0},
	//	{0, 0, -4, 1}
	//};

	//shader2.setUniform("translation", translate);
	shader2.setUniform("scale", 0.5f);
	shader2.setUniform("offset", 2.0f);
	shader2.setUniform("angle", (360.0f * day / 365.0f) * 2);


	glBindVertexArray(vao2);
	glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, 0);


	//glBindVertexArray(vao);
	//local1 = translate(local1, vec3(0.0, 1.0f, 0.0));
	//glUniformMatrix4fv(matrix_location, 1, GL_FALSE, local1.m);
	//glBindVertexArray(_vertexArray1);
	//local1 = translate(local1, vec3(0.0, -1.0f, 0.0));
	//glUniformMatrix4fv(matrix_location, 1, GL_FALSE, local1.m);

	// render VAO
	//glBindVertexArray(vao);

	//vsml->pushMatrix(VSMathLib::MODEL);
	//vsml->translate(i, 0, 100);
	//float scale[4] = { i, i, 1, 1 };
	//shader.setUniform("scale", scale);

	//glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, 0);
	//vsml->popMatrix(VSMathLib::MODEL);

	//swap buffers
	glutSwapBuffers();
}


// ------------------------------------------------------------
//
// Events from the Keyboard
//

void processKeys(unsigned char key, int xx, int yy)
{
	switch (key) {

	case 27:
		glutLeaveMainLoop();
		break;

	case 'c':
		printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
		break;

	}

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}


// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux * sin(betaAux * 3.14f / 180.0f);

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.1f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r * sin(beta * 3.14f / 180.0f);

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}




// --------------------------------------------------------
//
// Shader Stuff
//


GLuint setupShaders() {

	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/helloWorld.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/helloWorld.frag");

	// set semantics for the shader variables
	shader.setProgramOutput(0, "outputF");
	shader.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "position");

	shader.prepareProgram();

	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

	return(shader.isProgramValid());
}

GLuint setupShaders2() {

	// Shader for models
	shader2.init();
	shader2.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/helloWorld.vert");
	shader2.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/helloWorld.frag");

	// set semantics for the shader variables
	shader2.setProgramOutput(0, "outputF");
	shader2.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "position");

	shader2.prepareProgram();

	printf("InfoLog for Hello World Shader\n%s\n\n", shader2.getAllInfoLogs().c_str());

	return(shader2.isProgramValid());
}



// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//



void initOpenGL()
{
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r * sin(beta * 3.14f / 180.0f);

	// some GL settings
	// z-buffer
	glEnable(GL_DEPTH_TEST);
	// culling: etwas selektieren, welches face/ welche Seite vom Dreieck (Vorder oder Rückseite)
	// Umlaufsinn eines Dreiecks, alles was im uhrzeiger sinn läuft ist vorne
	// alles was ich gegene den uhrzeiger sinn zeichne ist hinten
	// GL_CULL_FACE ist default mäßig im uhrzeiger sinn definiert
	// Vorteil von Culling, es muss nur einmal gezeichnet werden
	glEnable(GL_CULL_FACE);
	// im subpixel bereich werden farben vergeben
	glEnable(GL_MULTISAMPLE);
	// mit welcher farbe lösche ich front-buffer, back-buffer
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// create the VAO
	// bei Gen wird etwas generiert
	// in open gl wird meistens ein integery array generiert mit handles
	// in dem fall wird ein 1 integer wert generiert und in die variable vao reingesch´rieben
	glGenVertexArrays(1, &vao);
	// beim binden wird der grafik karte mitgeteilt, wir möchten dieses array mit dieser nummer verwenden
	glBindVertexArray(vao);

	// create buffers for our vertex data
	GLuint buffers[4];
	glGenBuffers(4, buffers);

	//vertex coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	// GL_STATIC_DRAW heißt ich zeichne einmal und verändere es danach nicht mehr
	// grafik karte legt es sich auf einem speicher bereich auf der grafik karte und kann es optimieren (verhindert speicher fragmentierung)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VSShaderLib::VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(VSShaderLib::VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);

	//texture coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VSShaderLib::TEXTURE_COORD_ATTRIB);
	glVertexAttribPointer(VSShaderLib::TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);

	//normals buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VSShaderLib::NORMAL_ATTRIB);
	glVertexAttribPointer(VSShaderLib::NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, 0);

	//index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndex), faceIndex, GL_STATIC_DRAW);

	// unbind the VAO
	//glBindVertexArray(0);


	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);
	// create buffers for our vertex data
	GLuint buffers2[4];
	glGenBuffers(4, buffers2);

	// Object 2 vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, buffers2[0]);
	// GL_STATIC_DRAW heißt ich zeichne einmal und verändere es danach nicht mehr
	// grafik karte legt es sich auf einem speicher bereich auf der grafik karte und kann es optimieren (verhindert speicher fragmentierung)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VSShaderLib::VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(VSShaderLib::VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);

	//texture coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers2[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VSShaderLib::TEXTURE_COORD_ATTRIB);
	glVertexAttribPointer(VSShaderLib::TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);

	//normals buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers2[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VSShaderLib::NORMAL_ATTRIB);
	glVertexAttribPointer(VSShaderLib::NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, 0);

	//index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers2[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndex), faceIndex, GL_STATIC_DRAW);

	glBindVertexArray(0);
}


void initVSL() {
	vsml = VSMathLib::getInstance();
	// tell VSL the uniform block name
	vsml->setUniformBlockName("Matrices");
	// set semantics for the matrix 
	vsml->setUniformName(VSMathLib::PROJ_VIEW_MODEL, "pvm");
}

int main(int argc, char** argv) {

	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	// open gl version: 3 major, 3 minor
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 350);
	glutCreateWindow("Lighthouse3D - Simple Shader Demo");

	//  Callback Registration
	glutDisplayFunc(renderScene);
	// window gets resized
	glutReshapeFunc(changeSize);
	// idle nothing needs to be rendered, we could clear memory
	// don't render scene on idle because it can cause stuttering
	glutIdleFunc(renderScene);

	//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	glutMouseWheelFunc(mouseWheel);

	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (!setupShaders())
		return(1);

	if (!setupShaders2())
		return(1);

	initOpenGL();

	initVSL();

	//  GLUT main loop
	glutMainLoop();

	return 0;

}

