#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mandlebrot/main.h"
#include "mandlebrot/Vector2.hpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";\


//  const char *fragmentShaderSource = readFile("shaders/test.frag");

 //"#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "in vec4 gl_FragCoord;\n"
//     "void main()\n"
//     "{\n"
//     // "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"

//  "   vec3 color = mix(vec3(1.0, 0.0, 0.0), \n" // Red
// "                     vec3(0.0, 0.0, 1.0),  // Blue\n"
// "                     gl_FragCoord.x/1000); "       // Mix based on x position\n

//     "FragColor = vec4(color, 1.0);\n"
//     "}\n\0";



// "out vec4 FragColor;\n"
// "in vec2 TexCoords; // or use gl_FragCoord depending on how your vertex shader is set up\n"

// "void main() {\n"
//  "   // Assuming TexCoords range from 0.0 to 1.0 across the screen\n"
//  "   vec3 color = mix(vec3(1.0, 0.0, 0.0), \n" // Red
// "                     vec3(0.0, 0.0, 1.0),  // Blue\n"
// "                     TexCoords.x); "       // Mix based on x position\n

//     "FragColor = vec4(color, 1.0);\n"
// "}\n\0";

/*
GLOBALS
 */

double zoom = 2.0;

Vector2 center = Vector2(-0.5, 0.0);
Mouse mouse = Mouse();

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mouse.is_holding = true;
    } else {
        mouse.is_holding = false;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    mouse.update_delta(Vector2(xpos, ypos));

}

void cursor_enter_callback(GLFWwindow* window, int entered) {
    if (entered) {
        mouse.is_in_window = true;
    }
    else {
        mouse.is_in_window = false;
        mouse.is_holding = false;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    float zoom_speed = 0.97;
    if(yoffset > 0){
        zoom *= zoom_speed;
    } else if(yoffset < 0){
        zoom /= zoom_speed;
    }
}



int main() {
    // std::string fragmentShaderSource_nonpointer = readFile("shaders/gradient.frag").c_str();
    std::string fragmentShaderSource_nonpointer = readFile("shaders/mandlebrot.frag").c_str();
    const char *fragmentShaderSource = fragmentShaderSource_nonpointer.c_str();

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mandlebrot", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, // bottom left  
         1.0f, -1.0f, 0.0f, // bottom right 
        -1.0f,  1.0f, 0.0f,  // top left
        1.0f,  1.0f, 0.0f  // top right
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    int width, height;
    // const int MaxIteratons = 32;

    const int logFrequency = 20;
    double lastTime;
    double FPSSum = 0.0;
    int increment = 0;


    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);
        glfwSetScrollCallback(window, scroll_callback);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


        glfwGetFramebufferSize(window, &width, &height); 

        GLint resolutionLoc = glGetUniformLocation(shaderProgram, "resolution");//resolution variable
        glUseProgram(shaderProgram);
        glUniform2f(resolutionLoc, (float)width, (float)height);

        GLint aspectLoc = glGetUniformLocation(shaderProgram, "aspect"); // precomputed aspect ratio
        glUniform1f(aspectLoc, (float)width/(float)height);

        GLint centerLoc = glGetUniformLocation(shaderProgram, "center");
        // glUniform2f(centerLoc, -0.5, 0.0);
        glUniform2f(centerLoc, center.x, center.y);

        GLint zoomLoc = glGetUniformLocation(shaderProgram, "zoom");
        glUniform1f(zoomLoc, zoom);

        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();



        //Loging the average previous frames FPS

        FPSSum += glfwGetTime() - lastTime;

        lastTime = glfwGetTime();

        if (increment > logFrequency){
            increment = 0;
            std::cout << "FPS: " << 1/(FPSSum / logFrequency) << "\n";
            FPSSum = 0;
        }


        increment++;
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
//     float zoom_speed = 0.9;
//     if(yoffset > 0){
//         zoom *= zoom_speed;
//     } else if(yoffset < 0){
//         zoom /= zoom_speed;
//     }

// }
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}