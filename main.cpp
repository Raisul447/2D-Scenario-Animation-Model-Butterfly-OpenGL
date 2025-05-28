#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Window settings
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Scene object (formerly butterfly)
struct SceneObject {
    float x = 0.0f; // X position
    float y = 0.0f; // Y position
    float speed = 0.01f;
    bool movingUp = false;
    bool movingDown = false;
    bool movingLeft = false;
    bool movingRight = false;
} sceneObject;

float zoomFactor = 1.0f; // Zoom control

// Function to draw the object using many triangles
static void drawSceneObject(float wingAngle) {
    glPushMatrix();
    glTranslatef(sceneObject.x, sceneObject.y, 0.0f);

    // === Body (2 triangles - 6 vertices) ===
    glColor3f(0.0f, 0.0f, 0.0f); // Black body
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.05f, -0.1f);
    glVertex2f(0.0f, 0.2f);
    glVertex2f(0.05f, -0.1f);

    glVertex2f(-0.05f, -0.1f);
    glVertex2f(0.0f, -0.2f);
    glVertex2f(0.05f, -0.1f);
    glEnd();

    // === Right Wing (7 triangles - 21 vertices) ===
    glBegin(GL_TRIANGLES);
    // Shades of pink/purple
    glColor3f(1.0f, 0.4f, 0.7f); glVertex2f(0.0f, 0.0f); glVertex2f(0.3f, 0.1f); glVertex2f(0.3f, -0.1f);
    glColor3f(1.0f, 0.5f, 0.8f); glVertex2f(0.0f, 0.0f); glVertex2f(0.3f, 0.1f); glVertex2f(0.5f, 0.0f);
    glColor3f(1.0f, 0.6f, 0.9f); glVertex2f(0.0f, 0.0f); glVertex2f(0.5f, 0.0f); glVertex2f(0.3f, -0.1f);
    glColor3f(0.9f, 0.4f, 0.7f); glVertex2f(0.0f, 0.0f); glVertex2f(0.3f, 0.2f); glVertex2f(0.3f, 0.1f);
    glColor3f(0.9f, 0.5f, 0.8f); glVertex2f(0.0f, 0.0f); glVertex2f(0.3f, -0.2f); glVertex2f(0.3f, -0.1f);
    glColor3f(0.8f, 0.6f, 0.9f); glVertex2f(0.0f, 0.0f); glVertex2f(0.2f, 0.3f); glVertex2f(0.3f, 0.2f);
    glColor3f(0.8f, 0.4f, 0.7f); glVertex2f(0.0f, 0.0f); glVertex2f(0.2f, -0.3f); glVertex2f(0.3f, -0.2f);
    glEnd();

    // === Left Wing mirrored (same logic) ===
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.4f, 0.7f); glVertex2f(0.0f, 0.0f); glVertex2f(-0.3f, 0.1f); glVertex2f(-0.3f, -0.1f);
    glColor3f(1.0f, 0.5f, 0.8f); glVertex2f(0.0f, 0.0f); glVertex2f(-0.3f, 0.1f); glVertex2f(-0.5f, 0.0f);
    glColor3f(1.0f, 0.6f, 0.9f); glVertex2f(0.0f, 0.0f); glVertex2f(-0.5f, 0.0f); glVertex2f(-0.3f, -0.1f);
    glColor3f(0.9f, 0.4f, 0.7f); glVertex2f(0.0f, 0.0f); glVertex2f(-0.3f, 0.2f); glVertex2f(-0.3f, 0.1f);
    glColor3f(0.9f, 0.5f, 0.8f); glVertex2f(0.0f, 0.0f); glVertex2f(-0.3f, -0.2f); glVertex2f(-0.3f, -0.1f);
    glColor3f(0.8f, 0.6f, 0.9f); glVertex2f(0.0f, 0.0f); glVertex2f(-0.2f, 0.3f); glVertex2f(-0.3f, 0.2f);
    glColor3f(0.8f, 0.4f, 0.7f); glVertex2f(0.0f, 0.0f); glVertex2f(-0.2f, -0.3f); glVertex2f(-0.3f, -0.2f);
    glEnd();

    // === Antennae (Lines - 4 points) ===
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.2f); glVertex2f(0.1f, 0.4f);
    glVertex2f(0.0f, 0.2f); glVertex2f(-0.1f, 0.4f);
    glEnd();

    glPopMatrix();
}

// Keyboard callback (Arrow keys)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_UP:    sceneObject.movingUp = true; break;
        case GLFW_KEY_DOWN:  sceneObject.movingDown = true; break;
        case GLFW_KEY_LEFT:  sceneObject.movingLeft = true; break;
        case GLFW_KEY_RIGHT: sceneObject.movingRight = true; break;
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (key) {
        case GLFW_KEY_UP:    sceneObject.movingUp = false; break;
        case GLFW_KEY_DOWN:  sceneObject.movingDown = false; break;
        case GLFW_KEY_LEFT:  sceneObject.movingLeft = false; break;
        case GLFW_KEY_RIGHT: sceneObject.movingRight = false; break;
        }
    }
}

// Mouse click callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Convert screen coordinates to [-1,1]
        sceneObject.x = (float(xpos) / width) * 2 - 1;
        sceneObject.y = (float(height - ypos) / height) * 2 - 1;
    }
}

// Mouse scroll callback for zoom
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    zoomFactor += (float)yoffset * 0.05f;
    if (zoomFactor < 0.1f) zoomFactor = 0.1f;
    if (zoomFactor > 3.0f) zoomFactor = 3.0f;

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspect > 1.0f)
        glOrtho(-1.0f * aspect * zoomFactor, 1.0f * aspect * zoomFactor,
            -1.0f * zoomFactor, 1.0f * zoomFactor, -1.0f, 1.0f);
    else
        glOrtho(-1.0f * zoomFactor, 1.0f * zoomFactor,
            -1.0f / aspect * zoomFactor, 1.0f / aspect * zoomFactor, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Resize callback
void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspect > 1.0f)
        glOrtho(-1.0f * aspect * zoomFactor, 1.0f * aspect * zoomFactor,
            -1.0f * zoomFactor, 1.0f * zoomFactor, -1.0f, 1.0f);
    else
        glOrtho(-1.0f * zoomFactor, 1.0f * zoomFactor,
            -1.0f / aspect * zoomFactor, 1.0f / aspect * zoomFactor, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 3", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set up callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL setup
    glClearColor(0.87f, 0.94f, 0.87f, 1.0f); // Light green background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float time = 0.0f;

    // Initial projection
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspect > 1.0f)
        glOrtho(-1.0f * aspect * zoomFactor, 1.0f * aspect * zoomFactor,
            -1.0f * zoomFactor, 1.0f * zoomFactor, -1.0f, 1.0f);
    else
        glOrtho(-1.0f * zoomFactor, 1.0f * zoomFactor,
            -1.0f / aspect * zoomFactor, 1.0f / aspect * zoomFactor, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Update time
        time += 0.01f;

        // Animate wings
        float wingAngle = 25.0f * sin(time * 5.0f); // Smooth wing flapping

        // Handle movement
        if (sceneObject.movingUp) sceneObject.y += sceneObject.speed;
        if (sceneObject.movingDown) sceneObject.y -= sceneObject.speed;
        if (sceneObject.movingLeft) sceneObject.x -= sceneObject.speed;
        if (sceneObject.movingRight) sceneObject.x += sceneObject.speed;

        // Draw
        glLoadIdentity();
        drawSceneObject(wingAngle);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}