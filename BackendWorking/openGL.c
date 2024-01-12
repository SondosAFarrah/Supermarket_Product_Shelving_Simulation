#include <GL/glut.h>
#include <string.h>
#include <stdio.h>

#define NUM_CUSTOMERS 10
#define NUM_EMPLOYEES 12

float customerPositions[NUM_CUSTOMERS];
float employeePositions[NUM_EMPLOYEES];
int windowHeight = 300;

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 400.0, 0.0, (GLfloat)windowHeight);

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customerPositions[i] = 150 + i * 10; // Initial vertical positions for customers
    }

    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        employeePositions[i] = 110 + i * 10; // Initial vertical positions for employees
    }
}

void drawText(float x, float y, char* string) {
    glRasterPos2f(x, y);
    for (char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

void drawRectangle(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawProduct(float x, float y, float r, float g, float b, const char* name) {
    glColor3f(r, g, b);
    drawRectangle(x, y, x + 5, y + 5);
    drawText(x - 5, y - 10, (char*)name); // Product name
}

void drawCustomer(float x, float y, int customerNumber) {
    glColor3f(0.0, 0.0, 0.0); // Black color for customers
    drawRectangle(x, y, x + 5, y + 5);

    char label[20];
    sprintf(label, "Cust %d", customerNumber); // Shortened label to fit
    drawText(x + 8, y, label); // Position text next to the customer
}

void drawEmployee(float x, float y, int employeeNumber) {
    glColor3f(1.0, 1.0, 0.0); // Yellow color for employees
    drawRectangle(x, y, x + 5, y + 5);

    char label[20];
    sprintf(label, "Emp %d", employeeNumber); // Shortened label to fit
    drawText(x + 8, y, label); // Position text next to the employee
}
void drawManager(float x, float y, int employeeNumber) {
    glColor3f(0.0, 0.0, 1.0); // Yellow color for employees
    drawRectangle(x, y, x + 5, y + 5);

    char label[20];
    sprintf(label, "Manager %d", employeeNumber); // Shortened label to fit
    drawText(x + 8, y, label); // Position text next to the employee
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Shelf
    glColor3f(0.5, 0.35, 0.05); // Brown color for the shelf
    drawRectangle(10, 100, 190, 110); // Shelf
    drawText(15, 115, "Shelf");

    // Storage Area
    glColor3f(0.6, 0.6, 0.6); // Grey color for storage area
    drawRectangle(10, 10, 40, 50); // Storage area
    drawText(15, 55, "Storage");

    // Products on the shelf
    drawProduct(15, 105, 1.0, 0.0, 0.0, "Milk"); // Red Milk
    drawProduct(30, 105, 0.0, 1.0, 0.0, "Labneh"); // Green Labneh
    drawProduct(45, 105, 0.0, 0.0, 1.0, "Cheese"); // Blue Cheese

    // Products in the storage area
    drawProduct(15, 15, 1.0, 0.0, 0.0, "Milk"); // Red Milk
    drawProduct(25, 15, 0.0, 1.0, 0.0, "Labneh"); // Green Labneh
    drawProduct(35, 15, 0.0, 0.0, 1.0, "Cheese"); // Blue Cheese

    // Customers
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        drawCustomer(50 + i * 10, customerPositions[i], i + 1);
    }

    // Employees
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        if(i == 0 || i == 4 || i== 8){
            drawManager(200 + i * 10, employeePositions[i], i + 1);
        } else {
            drawEmployee(200 + i * 10, employeePositions[i], i + 1);
        }
    }

    glFlush();
}

void update(int value) {
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customerPositions[i] -= 1.0; // Move each customer downwards
        if (customerPositions[i] < 0) {
            customerPositions[i] = 150 + i * 10; // Reset position to the top
        }
    }

    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        employeePositions[i] -= 0.5; // Move each employee downwards at a slower pace
        if (employeePositions[i] < 0) {
            employeePositions[i] = 110 + i * 10; // Reset position to the top
        }
    }

    glutPostRedisplay(); // Redraw the scene
    glutTimerFunc(100, update, 0); // Set up next update in 100 ms
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, windowHeight);
    glutInitWindowPosition(50, 100);
    glutCreateWindow("OpenGL Supermarket with Moving Characters");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(100, update, 0); // Set up first timer call
    glutMainLoop();

    return 0;
}
