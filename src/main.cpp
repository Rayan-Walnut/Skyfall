#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Player.h"
#include "Shader.h"

static int WINDOW_W = 1280;
static int WINDOW_H = 720;

bool started = false;
Player player;
Camera camera;
static bool firstMouse = true;
static double lastX = 0.0, lastY = 0.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WINDOW_W = width; WINDOW_H = height;
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window, float dt)
{
    glm::vec3 move(0.0f);
    const float speed = 5.0f;
    // Accept both QWERTY (W/A/S/D) and AZERTY (Z/Q/S/D)
    bool fw = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS);
    bool bw = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool left = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS);
    bool right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

    // camera-relative movement: compute forward/right from yaw
    float yaw = camera.getYaw();
    float yawRad = glm::radians(yaw);
    glm::vec3 forward = glm::normalize(glm::vec3(sin(yawRad), 0.0f, cos(yawRad)));
    // right vector: try cross(forward, up) to match expected left/right on different layouts
    glm::vec3 rightVec = glm::normalize(glm::cross(forward, glm::vec3(0.0f,1.0f,0.0f)));

    if (fw) move += forward * speed * dt;
    if (bw) move -= forward * speed * dt;
    if (left) move -= rightVec * speed * dt;
    if (right) move += rightVec * speed * dt;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) player.jump();

    // world-relative move (no rotation applied yet)
    player.move(move);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // start the game (capture cursor) on any left click to avoid tiny hitbox
    if (!started && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        started = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse = true;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) return;
    if (firstMouse) {
        lastX = xpos; lastY = ypos; firstMouse = false;
        return;
    }
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // reversed: y ranges top->bottom
    lastX = xpos; lastY = ypos;

    const float sensitivity = 0.1f;
    // invert X so mouse-right produces positive yaw (fixes inverted horizontal look)
    camera.addYawPitch((float)(-xoffset * sensitivity), (float)(yoffset * sensitivity));
    // debug print to console so we can see the camera reacting
    std::cout << "camera yaw=" << camera.getYaw() << " pitch=" << camera.getPitch() << "\n";
}

// simple shaders
static const char* basicVert = R"glsl(#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 uMVP;
void main() { gl_Position = uMVP * vec4(aPos, 1.0); }
)glsl";

static const char* basicFrag = R"glsl(#version 330 core
out vec4 FragColor;
uniform vec3 uColor;
void main() { FragColor = vec4(uColor, 1.0); }
)glsl";

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
    glfwSetCursorPosCallback(window, cursor_position_callback);

    glEnable(GL_DEPTH_TEST);

    // create shader
    Shader basic(basicVert, basicFrag);

    // cube vertices (centered unit cube)
    float cubeVerts[] = {
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,-0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f, -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f
    };

    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // ground quad
    float groundVerts[] = {
        -50.0f, 0.0f, -50.0f,
         50.0f, 0.0f, -50.0f,
         50.0f, 0.0f,  50.0f,
         50.0f, 0.0f,  50.0f,
        -50.0f, 0.0f,  50.0f,
        -50.0f, 0.0f, -50.0f
    };
    GLuint groundVAO, groundVBO;
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVerts), groundVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glBindVertexArray(0);

    double last = glfwGetTime();
    while(!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        float dt = float(now - last); last = now;
        // allow releasing the cursor with ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstMouse = true;
        }

        processInput(window, dt);
        player.update(dt);
        camera.setTarget(player.getPosition());
        camera.update(dt);

        glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render scene
        basic.use();
        glm::mat4 proj = camera.getProjectionMatrix((float)WINDOW_W / (float)WINDOW_H);
        glm::mat4 view = camera.getViewMatrix();

        // ground
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(glGetUniformLocation(basic.id(), "uMVP"), 1, GL_FALSE, &mvp[0][0]);
        glUniform3f(glGetUniformLocation(basic.id(), "uColor"), 0.2f, 0.8f, 0.2f);
        glBindVertexArray(groundVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // player cube
        model = glm::translate(glm::mat4(1.0f), player.getPosition());
        model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
        mvp = proj * view * model;
        glUniformMatrix4fv(glGetUniformLocation(basic.id(), "uMVP"), 1, GL_FALSE, &mvp[0][0]);
        glUniform3f(glGetUniformLocation(basic.id(), "uColor"), 0.8f, 0.2f, 0.2f);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Simple start menu overlay: draw a dark rectangle with scissor
        if (!started) {
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_SCISSOR_TEST);
            glScissor(WINDOW_W - 220, 20, 200, 80);
            glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_SCISSOR_TEST);
            glEnable(GL_DEPTH_TEST);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &groundVAO);
    glDeleteBuffers(1, &groundVBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
