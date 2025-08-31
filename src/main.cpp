#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Player.h"

static int WINDOW_W = 1280;
static int WINDOW_H = 720;

bool started = false;
Player player;
Camera camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WINDOW_W = width; WINDOW_H = height;
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window, float dt)
{
    glm::vec3 move(0.0f);
    const float speed = 5.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) move.z -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) move.z += speed * dt;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) move.x -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) move.x += speed * dt;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) player.jump();

    // rotate move by camera yaw for world-relative controls
    player.move(move);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (!started && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mx, my; glfwGetCursorPos(window, &mx, &my);
        // Start button area: bottom-right 200x80 pixels
        if (mx >= WINDOW_W - 220 && mx <= WINDOW_W - 20 && my >= WINDOW_H - 100 && my <= WINDOW_H - 20) {
            started = true;
        }
    }
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Skyfall - Prototype", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cerr << "Failed to init GLAD\n"; return -1; }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glEnable(GL_DEPTH_TEST);

    // simple loop
    double last = glfwGetTime();
    while(!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        float dt = float(now - last); last = now;

        processInput(window, dt);
        player.update(dt);
        camera.setTarget(player.getPosition());
        camera.update(dt);

        glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: draw ground and a simple cube for player
        // For now we won't create VAOs/shaders; this is a stub render to keep structure.

        // Simple start menu overlay
        if (!started) {
            // draw a simple colored rect for the start button using glClear scissor trick
            glEnable(GL_SCISSOR_TEST);
            glScissor(WINDOW_W - 220, 20, 200, 80); // bottom-right (note: origin is bottom-left)
            glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_SCISSOR_TEST);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
