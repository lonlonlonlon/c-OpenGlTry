#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <SOIL/SOIL.h>
#include <iostream>

int windowWidth = 500;
int windowHeight = 500;

void renderTexture(GLuint textureID, float x, float y, float height, float width, float angle)
{
    // Aktiviere Texturing
    glEnable(GL_TEXTURE_2D);

    // Binde die Textur, die gerendert werden soll
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Setze Textur-Modus auf ersetzen (überschreibt den aktuellen Farbwert)
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    // Setze Transformationen zurück
    glLoadIdentity();

    // Verschiebe die Textur an die gewünschten Koordinaten und drehe sie um den gewünschten Winkel
    glTranslatef(x, y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    // Zeichne ein Quadrat, das die Textur füllt
    glBegin(GL_QUADS);
    glColor3f(255,255,255);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-width / 2.0f, -height / 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(width / 2.0f, -height / 2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(width / 2.0f, height / 2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-width / 2.0f, height / 2.0f);
    glEnd();

    // Deaktiviere Texturing
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

GLuint loadTextureOld(const char* filename) // use:    GLuint texture = loadTexture("filename.jpg");
{
    // Lade das Bild
    int widthTmp, heightTmp, numComponents;
    unsigned char* image = stbi_load(filename, &widthTmp, &heightTmp, &numComponents, 0);

    // Erstelle die Textur
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTmp, heightTmp, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    // Setze die Texturparameter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Lösche das Bild und gib die Textur-ID zurück
    stbi_image_free(image);
    return texture;
}

GLuint loadTexture(const char* filename)
{
    // Lade die Textur aus der angegebenen Datei
    GLuint textureID = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    // Prüfe, ob das Laden erfolgreich war
    if (textureID == 0)
    {
        // Das Laden ist fehlgeschlagen, gebe eine Fehlermeldung aus
        std::cerr << "Fehler beim Laden der Textur: " << filename << std::endl;
    }

    return textureID;
}

// load Textures
GLuint blueStone = loadTextureOld("blueStone.png");


/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

    // Draw a Red 1x1 Square centered at origin
//    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
//    glColor3f(1.0f, 0.0f, 0.0f); // Red
//    glVertex2f(-0.5f, -0.5f);    // x, y
//    glVertex2f( 0.5f, -0.5f);
//    glVertex2f( 0.5f,  0.5f);
//    glVertex2f(-0.5f,  0.5f);
//    glEnd();
    renderTexture(blueStone, 0, 0, 5, 5, 0);

    glFlush();  // Render now
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                 // Initialize GLUT
    glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
    glutInitWindowSize(windowWidth, windowHeight);   // Set the window's initial windowWidth & windowHeight
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    const unsigned char* version = glGetString(GL_VERSION);
    const unsigned char* extensions = glGetString(GL_EXTENSIONS);
    printf("%s %s\n", version, extensions);
    glutDisplayFunc(display); // Register display callback handler for window re-paint
    glutMainLoop();           // Enter the infinitely event-processing loop
    return 0;
}