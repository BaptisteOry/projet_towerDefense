#ifndef INTERFACE_H__
#define INTERFACE_H__

typedef struct Interface{
	int wLogo, hLogo;
	int xLogo, yLogo;
	int wLogo2, hLogo2;
	int xLogo2, yLogo2;
	GLuint spriteLogo; // Sprite Logo

	int xBegin, yBegin;
	int wBeginB, hBeginB;
	int xBeginB, yBeginB;

	int wInfosGame, hInfosGame;
	int xInfosGame, yInfosGame;

	int wInfosConstructions, hInfosConstructions;
    int xInfosConstructions, yInfosConstructions;
    char infosConstructions[510];

    int wQuitB, hQuitB;
	int xQuitB, yQuitB;

	int wFull, hFull;

	int xText, yText;
	void* font;

	int r, g, b, a; // Couleur 1
	int r2, g2, b2, a2; // Couleur 2
}Interface;

Interface* allocInterface(float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT);
void freeInterface(Interface* i);
void drawBeginning(Interface* f);
void drawGameElements(Interface* f, Game* g);
void drawHelp(Interface* f);
void drawEnd(Interface* f);
void drawInfosConstructions(Interface* f);
int beginSelected(Interface* f, float x, float y);
int quitSelected(Interface* f, float x, float y);

#endif