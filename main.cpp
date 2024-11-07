// template based on material from learnopengl.com
#define GLEW_STATIC
#define GLEW_BUILD

#include <GL/glew.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "importObj.h"

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


/*
string GetFile(const char* filename)   //reads shader files
{
    ifstream file(filename, ios::in);
    string content;
    if (file.good())
    {
        stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
    }
    return content;
}
*/
std::string ReadShader(const std::string& filename) {
    std::ifstream file(filename.c_str());
    std::string code = "";


    if (!file.is_open()) {
        std::cout << "File not open" << std::endl;
        std::cout << filename << std::endl;
    }
    else {
        std::stringstream shaderStream;
        shaderStream << file.rdbuf();
        file.close();
        code = shaderStream.str();
    }
    /*while (std::getline(file, buffer)) {

        code += buffer + '\n';
    }
    */

    // return shaderStream;
    return code;

}
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


float FoV = 55.0f;
glm::mat4 transform = glm::mat4(1.0f); // transformation matrix
glm::vec3 lightPos = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, 0.0f);

bool zprime_bufferKeyPressed = false;
bool zprime_buffer = false;
bool z_bufferKeyPressed = false;
bool z_buffer = false;

int main()
{
    string  filepath = "data/eagle.obj";

    vector<Vertex> vertices;
    vector<Face> faces;
    vector<Normal> normals;
    const char* filepathPointer = &filepath[0];
    readObjFile(filepathPointer, vertices, normals, faces);

    vector<float> vertexData;
    int count = 0;
    glm::vec3 colorTemp;
    for (const Face& face : faces) {        //generates the vextexArray and adds colors
        const Vertex& vertex = vertices[face.vertexIndex];
        vertexData.push_back(vertex.x);
        vertexData.push_back(vertex.y);
        vertexData.push_back(vertex.z);
        if (count % 3 == 0)
        {
            colorTemp = glm::vec3(1.0f, 0.0f, 0.0f);   //red color
        }
        else if (count % 3 == 1)
        {
            colorTemp = glm::vec3(1.0f, 0.0f, 0.0f);   //green color
        }
        else
        {
            colorTemp = glm::vec3(1.0f, 0.0f, 0.0f);   //blue color
        }
        vertexData.push_back(colorTemp.x);
        vertexData.push_back(colorTemp.y);
        vertexData.push_back(colorTemp.z);
        count += 1;
    }

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    // build and compile our shader program - change so both shaders can be read
    // ------------------------------------
    // vertex shader
    cout << " " << endl;
    cout << "Enter Shader Number to render: 1.Phong 2. Gouraud" << endl;
    cout << "Your Choice (1 or 2):";
    string shaderNum, vertexShaderPath, fragmentShaderPath;
    cin >> shaderNum;
    if (shaderNum.compare("1")) {
        vertexShaderPath = "phong_vertShader.glsl";
        fragmentShaderPath = "phong_fragShader.glsl";
    }
    else if (shaderNum.compare("2")) {
        vertexShaderPath = "gouraud_vertShader.glsl";
        fragmentShaderPath = "gouraud_fragShader.glsl";
    }
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string temp1 = ReadShader(vertexShaderPath);
    const char* vertexShaderSource = temp1.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string temp2 = ReadShader(fragmentShaderPath);
    const char* fragmentShaderSource = temp2.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    /*
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
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        }
        // fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
    */
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
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
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    unsigned int VBO, VAO, lightVAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);


    //light VAO
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // GLuint query;
    // glGenQueries(1, &query);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // Start GPU timing query
        // glBeginQuery(GL_TIME_ELAPSED, query);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //  glDepthMask(GL_TRUE); //may not be correct

          //shading
        unsigned int lightColor = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3f(lightColor, 1.0f, 1.0f, 1.0f); //black light
        unsigned int lightLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightLoc, 1, &lightPos[0]);
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(viewLoc, 1, &viewPos[0]);


        //z-buffer toggle
        unsigned int zprimebufferLoc = glGetUniformLocation(shaderProgram, "z_prime_buffer");
        glUniform1i(zprimebufferLoc, zprime_buffer);
        unsigned int zbufferLoc = glGetUniformLocation(shaderProgram, "z_buffer");
        glUniform1i(zbufferLoc, z_buffer);

        // get matrix's uniform location and set matrix 
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1.0f);
        //glm::mat4 MP = projection * transform;


        if (shaderNum.compare("1")) {
            unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        }
        else if(shaderNum.compare("2")) {
             unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));  
        }



        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 3);

        /*
        // End GPU timing query
        // glEndQuery(GL_TIME_ELAPSED);
        // Check if the query result is available
        // GLuint available = 0;
        // while (!available) {
        // glGetQueryObjectuiv(query, GL_QUERY_RESULT_AVAILABLE, &available);
        // }

        //  // Get the elapsed time
        // GLuint64 elapsed_time = 0;
        // glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);
        // if(elapsed_time > 0)
        // std::cout << "GPU Transformation Time: " << elapsed_time << " nanoseconds" << std::endl;
        */

        // glBindVertexArray(0); // unbind our VA no need to unbind it every time 
       //  glDepthMask(GL_FALSE); //may not be correct
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    // glDeleteProgram(lightShaderProgram);

     // glfw: terminate, clearing all previously allocated GLFW resources.
     // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)    //right
        transform = glm::translate(transform, glm::vec3(0.001f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)    //left
        transform = glm::translate(transform, glm::vec3(-0.001f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)   //up
        transform = glm::translate(transform, glm::vec3(0.0f, 0.001f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)    //down
        transform = glm::translate(transform, glm::vec3(0.0f, -0.001f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //rotate left
        transform = glm::rotate(transform, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)    //rotate right
        transform = glm::rotate(transform, glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)   //rotate up
        transform = glm::rotate(transform, glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //rotate down
        transform = glm::rotate(transform, glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //scale
        transform = glm::scale(transform, glm::vec3(1.0002f, 1.0002f, 1.0002f));
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) //scale down
        transform = glm::scale(transform, glm::vec3(0.9999f, 0.9999f, 0.9999f));
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !z_bufferKeyPressed) {
        z_bufferKeyPressed = !z_bufferKeyPressed;
        z_buffer = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        z_bufferKeyPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !zprime_bufferKeyPressed) {
        zprime_bufferKeyPressed = !zprime_bufferKeyPressed;
        zprime_buffer = true;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
    {
        zprime_bufferKeyPressed = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}