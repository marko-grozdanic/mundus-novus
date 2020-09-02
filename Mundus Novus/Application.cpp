#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Classes/Shader.h"
#include "Classes/Texture.h"
#include "Classes/Light.h"
#include "Classes/Terrain.h"
#include "Classes/Camera.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void secondMouseCallback(GLFWwindow* window, int button, int action, int mods);

const unsigned int SCR_WIDTH = 1664;
const unsigned int SCR_HEIGHT = 936;

bool secondMousePressed = false;

// camera
Camera camera(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // Initialise GLFW window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL v4.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //OpenGL v4.6
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mundus Novus", NULL, NULL); //Use glfwGetPrimaryMonitor() as the fourth argument to match monitor fullscreen resolution
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* Register callback functions after window creation and  before initiating the render loop */
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, secondMouseCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    //Check GLAD initialisation
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); //Enable Antialiasing, sample amount set with glfwWindowHint

    //Configure shaders, textures, pass textures as uniforms
    Shader lightingShader("./Resources/Shaders/diffuseSpecularAmbientShader.vert", "./Resources/Shaders/diffuseSpecularAmbientShader.frag");
    Shader simpleShader("./Resources/Shaders/simpleShader.vert", "./Resources/Shaders/simpleShader.frag");

    //Texture grassDiffuse("pebbles_diffuse.png", true), grassSpecular("pebbles_specular.png", true);
    Texture grassDiffuse("grass.png", true), grassSpecular("grass_spec.png", true);

    lightingShader.use();
    lightingShader.setInt("material.diffuse", grassDiffuse.ID);
    lightingShader.setInt("material.specular", grassSpecular.ID);

    //Start with rendering a flat terrain
    int sizeN = 6;
    Terrain newTerrain(sizeN);
    newTerrain.generateTerrain();    
    newTerrain.setupTerrain();

    const char* glsl_version = "#version 460";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;    
    ImGui::StyleColorsDark(); // Setup Dear ImGui style
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::GetIO().WantCaptureMouse = false;    

    static bool wireframe = false;
    static bool topShader = false;

    float lightDirX = 4.13f, lightDirY = 5.34f, lightDirZ = 2.75;
    float lightAmbientX = 0.2f, lightAmbientY = 0.2f, lightAmbientZ = 0.2f;
    float lightDiffuseX = 0.5f, lightDiffuseY = 0.5f, lightDiffuseZ = 0.5f;
    float lightSpecX = 1.0f, lightSpecY = 1.0f, lightSpecZ = 1.0f;

    /*Render loop*/
    while (!glfwWindowShouldClose(window))
    {
        // Frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGUI Settings window
        {
            static int seed = 571281;
            static float fHeight = 16.0;
            static float fRoughness = 0.6;

            ImGui::Begin("Settings");

            ImGui::Checkbox("Wireframe", &wireframe);
            ImGui::Checkbox("Topological Shader", &topShader);
            ImGui::SliderInt("Size(2^n + 1)", &sizeN, 2, 10);
            ImGui::InputInt("Seed", &seed);
            ImGui::InputFloat("fHeight", &fHeight, 0.01f, 1.0f, "%.2f");
            ImGui::SliderFloat("fRoughness", &fRoughness, 0.0f, 1.0f, "ratio = %.3f");
            //ImGui::InputFloat("fRoughness", &fRoughness, 0.01f, 1.0f, "%.2f");
            if (ImGui::Button("Generate")) {          
                newTerrain.setSize(sizeN);
                newTerrain.genMidpointDisplacement(seed, fHeight, fRoughness);
                newTerrain.generateTerrain();
                newTerrain.setupTerrain();
                std::cout << "Generate!!" << std::endl;
            }

            ImGui::Text("(%.1f FPS)\n", ImGui::GetIO().Framerate);
            ImGui::Text("Controls (?)");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("WASD - Camera movement\nRight mouse button - Rotate camera\nESC - Close program");
            /*ImGui::SliderFloat("Light Dir X", &lightDirX, -20.0f, 20.0f, "ratio = %.3f");
            ImGui::SliderFloat("Light Dir Y", &lightDirY, -20.0f, 20.0f, "ratio = %.3f");
            ImGui::SliderFloat("Light Dir Z", &lightDirZ, -20.0f, 20.0f, "ratio = %.3f");
            ImGui::SliderFloat("Ambient Dir X", &lightAmbientX, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("Ambient Dir Y", &lightAmbientY, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("Ambient Dir Z", &lightAmbientZ, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("Diffuse Dir X", &lightDiffuseX, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("Diffuse Dir Y", &lightDiffuseY, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("Diffuse Dir Z", &lightDiffuseZ, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("Spec Dir X", &lightSpecX, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("Spec Dir Y", &lightSpecY, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("Spec Dir Z", &lightSpecZ, 0.0f, 1.0f, "ratio = %.3f");*/
            ImGui::End();
        }

        // be sure to activate shader when setting uniforms/drawing objects
        if (topShader) {
            simpleShader.use();
            simpleShader.setVec3("viewPos", camera.Position);
            simpleShader.setFloat("material.shininess", 32.0f);

            Light dirLight(LIGHT_DIRECTIONAL, &simpleShader);
            dirLight.setDirection(lightDirX, lightDirY, lightDirZ);
            dirLight.setAmbient(lightAmbientX, lightAmbientY, lightAmbientZ);
            dirLight.setDiffuse(lightDiffuseX, lightDiffuseY, lightDiffuseZ);
            dirLight.setSpecular(lightSpecX, lightSpecY, lightSpecZ);

            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            simpleShader.setMat4("projection", projection);
            simpleShader.setMat4("view", view);

            // world transformation
            glm::mat4 model = glm::mat4(1.0f);
            simpleShader.setMat4("model", model);
        }
        else {
            lightingShader.use();
            lightingShader.setVec3("viewPos", camera.Position);
            lightingShader.setFloat("material.shininess", 32.0f);

            Light dirLight(LIGHT_DIRECTIONAL, &lightingShader);
            dirLight.setDirection(lightDirX, lightDirY, lightDirZ);
            dirLight.setAmbient(lightAmbientX, lightAmbientY, lightAmbientZ);
            dirLight.setDiffuse(lightDiffuseX, lightDiffuseY, lightDiffuseZ);
            dirLight.setSpecular(lightSpecX, lightSpecY, lightSpecZ);

            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);

            // world transformation
            glm::mat4 model = glm::mat4(1.0f);
            lightingShader.setMat4("model", model);

            // bind diffuse map
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            // bind specular map
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 1);
        }

        // Rendering
        ImGui::Render();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        newTerrain.drawTerrain(wireframe);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window); //Double buffer
    }

    glfwTerminate();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions;
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if(secondMousePressed)
        camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the second mouse button is pressed, this callback is called
// ----------------------------------------------------------------------
void secondMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        secondMousePressed = true;
    }
    else {
        secondMousePressed = false;
    }
}
