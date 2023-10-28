#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "Input.h"
#include "Time.h"
int main(void)
{
    GLFWwindow* window;
    int width = 1920;
    int height = 1080;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Set up depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set up viewport
    glViewport(0, 0, width, height);

    Input::SetWindow(window);
    Input::setPosition(width, height);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Game game(window);
    game.start();
    double lastTime = glfwGetTime();
    double currentTime;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        currentTime = glfwGetTime();

        Time::setDelta((currentTime - lastTime));
        lastTime = currentTime;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.update();

        game.render();
        //obj.draw(cam.GetViewMatrix());

        Input::Update();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        // Funkcja zwrotna do obs³ugi komunikatów debugowych
    }

    glfwTerminate();
    return 0;
}