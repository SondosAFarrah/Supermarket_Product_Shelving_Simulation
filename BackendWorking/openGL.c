#include <GL/glut.h>

const int windowWidth = 800;
const int windowHeight = 600;
const float shelfWidth = 10.0f;
const float shelfHeight = 100.0f;
const int numShelves = 5;

float employeeX[numShelves];
float customerX = -50.0f;
bool shelvesEmpty[numShelves] = { true, true, true, true, true };

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
}

void drawShelf(float x, float y) {
    glColor3f(0.8f, 0.6f, 0.4f);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + shelfWidth, y);
    glVertex2f(x + shelfWidth, y + shelfHeight);
    glVertex2f(x, y + shelfHeight);
    glEnd();
}

void drawEmployee(float x) {
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x, 10);
    glVertex2f(x + 20, 10);
    glVertex2f(x + 20, 40);
    glVertex2f(x, 40);
    glEnd();
}

void drawCustomer() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(customerX, 10);
    glVertex2f(customerX + 20, 10);
    glVertex2f(customerX + 20, 40);
    glVertex2f(customerX, 40);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < numShelves; i++) {
        drawShelf(i * (shelfWidth + 10), 100);
    }

    for (int i = 0; i < numShelves; i++) {
        drawEmployee(employeeX[i]);
    }
    drawCustomer();

    glutSwapBuffers();
}

void update(int value) {
    for (int i = 0; i < numShelves; i++) {
        employeeX[i] += rand() % 5 - 2;
    }

    customerX += rand() % 5;

    for (int i = 0; i < numShelves; i++) {
        if (customerX > i * (shelfWidth + 10) && customerX < (i + 1) * (shelfWidth + 10) && shelvesEmpty[i]) {
            shelvesEmpty[i] = false;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(100, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Supermarket Simulation");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}
