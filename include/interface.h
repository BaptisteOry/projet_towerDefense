#ifndef INTERFACE_H__
#define INTERFACE_H__

typedef struct Interface{
	int wLogo, hLogo;
	int xLogo, yLogo;
	int wLogo2, hLogo2;
	int xLogo2, yLogo2;
	GLuint spriteLogo; // Sprite Logo

	int wQuit, hQuit;
	int xQuit, yQuit;

	int wInfosGame, hInfosGame;
	int xInfosGame, yInfosGame;

	int wInfosConstructions, hInfosConstructions;
    int xInfosConstructions, yInfosConstructions;
    char infosConstructions[510];

	int wFull, hFull;

	int xText, yText;
	void* font;
}Interface;

Interface* allocInterface(float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT);
void freeInterface(Interface* i);
void drawGameElements(Interface* f, Game* g);
void drawHelp(Interface* f);
void drawEnd(Interface* f);
void drawInfosConstructions(Interface* f);
int quitSelected(Interface* f, float x, float y);

#endif