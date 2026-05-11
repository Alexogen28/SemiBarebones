#include "lab/Tema/Tema.h"

#include <vector>
#include <string>
#include <iostream>
#include <stb/stb_image.h>

#include "components/transform.h"
#include "components/camera_input.h"
#include "components/scene_input.h"
#include "components/transform.h"


using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema::Tema()
{
}


Tema::~Tema()
{
}


void Tema::Init()
{   
    heli_position = glm::vec3(20,0,20);
    tail_distance_from_cabin = glm::vec3(0,0,-2.0f);
    heli_rotation_axis = glm::vec3(0, 1.0f, 0);
    heli_rotation_angle = 231;
    translation_direction = glm::vec3(0, 0, 1.0f);
    propeller_rotation_axis = glm::vec3(1.0f, 0, 0);
    body_propeller_rotation_angle = 0;

    tree_matrix = glm::mat4(1);
    tree_position = glm::vec3(20, 0, 23);
    tree_branch_distance_y = 2.0f;

    marker_position = glm::vec3(0, 0, 0);

    move_speed = 1.0f;

    
    GetSceneCamera()->SetPosition(glm::vec3(heli_position.x, heli_position.y + 8, heli_position.z - 10));;
    GetSceneCamera()->SetRotation(glm::lookAt(glm::vec3(heli_position.x, heli_position.y + 3, heli_position.z - 7), heli_position, glm::vec3(0,1,0)));

    
    {
        Texture2D* texture = LoadTexture("assets\\textures\\ground.jpg");
        mapTextures["ground"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\noise.png");
        mapTextures["noise"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\water.jpg");
        mapTextures["water"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\snow.jpg");
        mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\crate.jpg");
        mapTextures["bark"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\leaves.jpg");
        mapTextures["leaves"] = texture;
    }

    //world plane geometry

    //256, same as noise texture
    int grid_size = 256;

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    
    for (int i = 0; i < grid_size; i++) 
    {
        for (int j = 0; j < grid_size; j++) 
        {
            glm::vec3 coords = glm::vec3(i, 0, j);
            VertexFormat vertex_to_add = VertexFormat(coords, glm::vec3(0), glm::vec3(0), glm::vec2((float)i/grid_size, (float)j/grid_size));

            vertices.push_back(vertex_to_add);
        }
    }

    //2 triangles to form a square -- bottom left/right and top left; bottom right, top left/right for 2nd triangle
    
    for (int i = 0; i < grid_size - 1; i++) {
        for (int j = 0; j < grid_size - 1; j++) {
            unsigned int index_bottom_left = i * grid_size + j;
            unsigned int index_top_left = (i + 1) * grid_size + j;
            unsigned int index_bottom_right = index_bottom_left + 1;
            unsigned int index_top_right = index_top_left + 1;

            indices.push_back(index_bottom_left);
            indices.push_back(index_top_left);
            indices.push_back(index_bottom_right);

            indices.push_back(index_bottom_right);
            indices.push_back(index_top_left);
            indices.push_back(index_top_right);
        }
    }
    
    {
        Mesh* mesh = new Mesh("plane");
        mesh->InitFromData(vertices, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    //sample noise texture
    int channels;
    unsigned char* imageData = stbi_load("src\\lab\\tema\\images\\noise.png", &depth_texture_width, &depth_texture_height, &channels, 0);

    for (int i = 0; i < depth_texture_width; i++)
    {
        for (int j = 0; j < depth_texture_height; j++)
        {
            float image_data_converted = imageData[i* depth_texture_height + j] / 255.0f;
            height_map_samples.push_back(image_data_converted);
        }
    }

    CreateFramebuffer(window->GetResolution().x, window->GetResolution().y);

    for(int i = 0; i < 10; i++)
    {
        if (i == 0) 
        {
            float tree_x, tree_z;
            tree_x = tree_position.x;
            tree_z = tree_position.z;
            tree_information.push_back(glm::vec4(tree_x, 0, tree_z, 0.1f * (i + 1)));

        }
        else
        {
            float tree_x, tree_z;
            tree_x = rand() % 100 + 1;
            tree_z = rand() % 100 + 1;

            tree_information.push_back(glm::vec4(tree_x, 0, tree_z, 0.1f * (i + 1)));
        }


    }

    LoadShader("AsteroidShader");
    LoadShader("FramebufferAsteroidShader");
    LoadShader("HeliShader");
    LoadShader("PropellerShader");
    LoadShader("TailPropellerShader");
    LoadShader("MarkerShader");
    LoadShader("OscilatingMarkerShader");
    LoadShader("TreeBranchShader");
}


Texture2D* Tema::LoadTexture(const char* imagePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);

    return CreateTexture(width, height, channels, imageData);
}


Texture2D* Tema::CreateTexture(unsigned int width, unsigned int height,
    unsigned int channels, unsigned char* data)
{
    GLuint textureID = 0;
    unsigned int size = width * height * channels;

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER and MAG_FILTER) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();


    // TODO(student): Use the "glTexImage2D" directive to load the information
    // into the graphics processor's memory. Use the correct format based on
    // the number of components:
    //   - 1 color channel - GL_RED
    //   - 2 color channels - GL_RG
    //   - 3 color channels - GL_RGB
    //   - 4 color channels - GL_RGBA
    if (channels == 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    }
    else if (channels == 2) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
    }
    else if (channels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (channels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}



void Tema::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema::Update(float deltaTimeSeconds)
{
    GetSceneCamera()->SetPosition(glm::vec3(heli_position.x, heli_position.y + 10, heli_position.z - 10));;
    GetSceneCamera()->SetRotation(glm::lookAt(glm::vec3(heli_position.x, heli_position.y + 3, heli_position.z - 7), heli_position, glm::vec3(0,1,0)));


    RenderToCustomFramebuffer();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //World plane geometry
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec4 worldPositionAndScale = glm::vec4(heli_position.x, 10, heli_position.z, 1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
        RenderSimpleMesh(meshes["plane"], shaders["AsteroidShader"], modelMatrix, glm::vec4(heli_position, 1), mapTextures["ground"], mapTextures["noise"], mapTextures["water"], mapTextures["snow"]);
    }

    CheckHelicopterMovement();
    RenderHelicopter();
    RenderMarker();

    for (int i = 0; i < tree_information.size(); i++)
    {
        glm::mat4 tree_matrix = glm::mat4(1);
        float length = 2.0f;
        glm::vec3 pos = glm::vec3(tree_information[i].x, 0, tree_information[i].z);
        tree_matrix = glm::translate(tree_matrix, glm::vec3(tree_information[i].x, 0, tree_information[i].z));
        tree_matrix = glm::scale(tree_matrix, glm::vec3(1, length, 1));
        RenderHappyLittleTree(tree_matrix, pos, 0, length, tree_information[i].w);
    }


    //glm::mat4 tree_matrix = glm::mat4(1);
    //float length = 2.0f;
    //tree_matrix = glm::translate(tree_matrix, glm::vec3(tree_position.x, tree_position.y, tree_position.z));
    //tree_matrix = glm::scale(tree_matrix, glm::vec3(1, length, 1));
    //RenderHappyLittleTree(tree_matrix, tree_position, 0, length);
}



void Tema::FrameEnd()
{
    //DrawCoordinateSystem();
}

void Tema::RenderToCustomFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, custom_framebuffer_object);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window->GetResolution().x, window->GetResolution().y);

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec4 worldPositionAndScale = glm::vec4(heli_position.x, 10, heli_position.z, 1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
        RenderSimpleMesh(meshes["plane"], shaders["FramebufferAsteroidShader"], modelMatrix, glm::vec4(heli_position, 1), mapTextures["ground"], mapTextures["noise"], mapTextures["water"], mapTextures["snow"]);
    }

    for (int i = 0; i < tree_information.size(); i++)
    {
        glm::mat4 tree_matrix = glm::mat4(1);
        float length = 2.0f;
        glm::vec3 pos = glm::vec3(tree_information[i].x, 0, tree_information[i].z);
        tree_matrix = glm::translate(tree_matrix, glm::vec3(tree_information[i].x, 0, tree_information[i].z));
        tree_matrix = glm::scale(tree_matrix, glm::vec3(1, length, 1));
        RenderHappyLittleTree(tree_matrix, pos, 0, length, tree_information[i].w);
    }
}

void Tema::RenderHappyLittleTree(glm::mat4 parent_matrix, glm::vec3 treePosition, int level, float length, float selection_value)
{
    if (level == 6) return;

    float scaleFactor = 0.7f;
    glm::mat4 branch_matrix = glm::scale(parent_matrix, glm::vec3(0.5f, length, 0.5f));

    glm::vec4 worldPositionAndScale = glm::vec4(treePosition, selection_value);
    RenderSimpleTreeMesh(meshes["box"], shaders["TreeBranchShader"], branch_matrix, worldPositionAndScale, heli_position, level, length, mapTextures["noise"], mapTextures["bark"], mapTextures["leaves"]);


    for (int i = 0; i < 3; i++) {
        glm::mat4 fork_matrix = glm::translate(parent_matrix, glm::vec3(0.0f, length / 2, 0.0f));
        fork_matrix = glm::rotate(fork_matrix, glm::radians(120.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
        fork_matrix = glm::rotate(fork_matrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        fork_matrix = glm::translate(fork_matrix, glm::vec3(0, length/8, length/8));
        fork_matrix = glm::scale(fork_matrix, glm::vec3(scaleFactor));

        RenderHappyLittleTree(fork_matrix, treePosition, level + 1, length, selection_value);
    }
}

void Tema::RenderMarker()
{
    //spherical body
    {

        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec4 worldPositionAndScale = glm::vec4(marker_position.x, marker_position.y, marker_position.z, 5);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(worldPositionAndScale.x, marker_position.y, worldPositionAndScale.z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.1f, 0.5f));
        RenderSimpleMarkerMesh(meshes["sphere"], shaders["MarkerShader"], modelMatrix, worldPositionAndScale, heli_position, mapTextures["noise"]);
    }

    //fluctuating part
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec4 worldPositionAndScale = glm::vec4(marker_position.x, 0, marker_position.z, 5);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(worldPositionAndScale.x, marker_position.y + 0.6f, worldPositionAndScale.z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.4f, 0.1f));
        RenderSimpleMarkerMesh(meshes["box"], shaders["OscilatingMarkerShader"], modelMatrix, worldPositionAndScale, heli_position, mapTextures["noise"]);
    }
}

void Tema::CheckHelicopterMovement()
{
    if (marker_position.x != 0 || marker_position.z != 0)
    {

        if (glm::length(heli_position - marker_position) < 0.05f)
        {
            marker_position = glm::vec3(0, 0, 0);
            return;
        }
        glm::vec3 direction = glm::normalize(heli_position - marker_position);
        heli_rotation_angle = DEGREES(atan2(direction.x, direction.z)) + 180;
        heli_position -= direction * 0.1f;
    }
}

void Tema::RenderHelicopter() 
{
    body_propeller_rotation_angle += 2;

    //Cabin
    {
        glm::mat4 cabinModelMatrix = glm::mat4(1);
        glm::vec4 worldPositionAndScale = glm::vec4(heli_position.x, heli_position.y, heli_position.z, 5);
        cabinModelMatrix = glm::translate(cabinModelMatrix, glm::vec3(worldPositionAndScale.x, worldPositionAndScale.y, worldPositionAndScale.z));
        cabinModelMatrix = glm::rotate(cabinModelMatrix, RADIANS(heli_rotation_angle), heli_rotation_axis);
        cabinModelMatrix = glm::scale(cabinModelMatrix, glm::vec3(0.6f, 0.75f, 1.0f));
        RenderSimpleMesh(meshes["box"], shaders["HeliShader"], cabinModelMatrix, glm::vec4(heli_position, 1), mapTextures["noise"]);
    }

    //Body propeller
    //Blade 1
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec4 worldPositionAndScale = glm::vec4(heli_position.x, heli_position.y, heli_position.z, 5 * 10);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(worldPositionAndScale.x, worldPositionAndScale.y + 0.4f, worldPositionAndScale.z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(heli_rotation_angle), heli_rotation_axis);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(body_propeller_rotation_angle), heli_rotation_axis);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.75f / 10, -2.5f));
        RenderSimpleMesh(meshes["box"], shaders["PropellerShader"], modelMatrix, worldPositionAndScale, mapTextures["noise"]);
    }

    //Blade 2
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec4 worldPositionAndScale = glm::vec4(heli_position.x, heli_position.y, heli_position.z, 5 * 10);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(worldPositionAndScale.x, worldPositionAndScale.y + 0.4f, worldPositionAndScale.z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(heli_rotation_angle), heli_rotation_axis);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(body_propeller_rotation_angle + 90), heli_rotation_axis);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.75f / 10, -2.5f));
        RenderSimpleMesh(meshes["box"], shaders["PropellerShader"], modelMatrix, worldPositionAndScale, mapTextures["noise"]);
    }


    //Tail
    {
        glm::mat4 tailModelMatrix = glm::mat4(1);
        glm::vec4 worldPositionAndScale = glm::vec4(heli_position.x, heli_position.y, heli_position.z, 5 * 2);
        tailModelMatrix = glm::translate(tailModelMatrix, glm::vec3(-sin(RADIANS(heli_rotation_angle)), 0, -cos(RADIANS(heli_rotation_angle))));
        tailModelMatrix = glm::translate(tailModelMatrix, glm::vec3(worldPositionAndScale.x, worldPositionAndScale.y+0.1f, worldPositionAndScale.z));
        tailModelMatrix = glm::rotate(tailModelMatrix, RADIANS(heli_rotation_angle), heli_rotation_axis);
        tailModelMatrix = glm::scale(tailModelMatrix, glm::vec3(0.3f, 0.75f / 2, -1.5f));
        RenderSimpleMesh(meshes["box"], shaders["HeliShader"], tailModelMatrix, worldPositionAndScale, mapTextures["noise"]);
    }

    //Tail propeller
    //Blade 1
    {
        glm::mat4 tailModelMatrix = glm::mat4(1);
        glm::vec4 worldPositionAndScale = glm::vec4(heli_position.x, heli_position.y, heli_position.z, 5 * 10);
        tailModelMatrix = glm::translate(tailModelMatrix, glm::vec3(-sin(RADIANS(heli_rotation_angle)), 0, -cos(RADIANS(heli_rotation_angle))));
        tailModelMatrix = glm::translate(tailModelMatrix, glm::vec3(worldPositionAndScale.x, worldPositionAndScale.y+0.1f, worldPositionAndScale.z));
        tailModelMatrix = glm::rotate(tailModelMatrix, RADIANS(heli_rotation_angle), heli_rotation_axis);
        tailModelMatrix = glm::translate(tailModelMatrix, glm::vec3(0, 0, -0.7f));
        tailModelMatrix = glm::rotate(tailModelMatrix, RADIANS(body_propeller_rotation_angle), glm::vec3(1, 0, 0));
        tailModelMatrix = glm::scale(tailModelMatrix, glm::vec3(0.1f, 0.75f / 10, 1));
        RenderSimpleMesh(meshes["box"], shaders["TailPropellerShader"], tailModelMatrix, worldPositionAndScale, mapTextures["noise"]);
    }

    //Blade 2
    {
        glm::mat4 tailModelMatrix = glm::mat4(1);
        glm::vec4 worldPositionAndScale = glm::vec4(heli_position.x, heli_position.y, heli_position.z, 5 * 10);
        tailModelMatrix = glm::translate(tailModelMatrix, glm::vec3(-sin(RADIANS(heli_rotation_angle)), 0, -cos(RADIANS(heli_rotation_angle))));
        tailModelMatrix = glm::translate(tailModelMatrix, glm::vec3(worldPositionAndScale.x, worldPositionAndScale.y+0.1f, worldPositionAndScale.z));
        tailModelMatrix = glm::rotate(tailModelMatrix, RADIANS(heli_rotation_angle), heli_rotation_axis);
        tailModelMatrix = glm::translate(tailModelMatrix, glm::vec3(0, 0, -0.7f));
        tailModelMatrix = glm::rotate(tailModelMatrix, RADIANS(body_propeller_rotation_angle + 90), glm::vec3(1, 0, 0));
        tailModelMatrix = glm::scale(tailModelMatrix, glm::vec3(0.1f, 0.75f / 10, 1));
        RenderSimpleMesh(meshes["box"], shaders["TailPropellerShader"], tailModelMatrix, worldPositionAndScale, mapTextures["noise"]);
    }
}

Mesh* Tema::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

void Tema::CreateFramebuffer(int width, int height)
{
    glGenFramebuffers(1, &custom_framebuffer_object);
    glBindFramebuffer(GL_FRAMEBUFFER, custom_framebuffer_object);

    //First color texture
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);

    //Second color texture (coords)
    glGenTextures(1, &color_texture_coords);
    glBindTexture(GL_TEXTURE_2D, color_texture_coords);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, color_texture_coords, 0);

    std::vector<GLenum> draw_textures;

    draw_textures.push_back(GL_COLOR_ATTACHMENT0);
    draw_textures.push_back(GL_COLOR_ATTACHMENT1);
    glDrawBuffers(draw_textures.size(), &draw_textures[0]);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "error";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tema::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec4& worldPositionAndScale, Texture2D* texture1, Texture2D* texture2, Texture2D* texture3, Texture2D* texture4)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need
    int loc_time = glGetUniformLocation(shader->program, "Time");
    glUniform1f(loc_time, Engine::GetElapsedTime());

    //binds the world space matrix
    int loc_world_position = glGetUniformLocation(shader->program, "WorldPositionAndScale");
    glUniform4fv(loc_world_position, 1, glm::value_ptr(worldPositionAndScale));

    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_1"), 0);
    }

    if (texture2)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_2"), 1);

    }

    //Bonus 
    if (texture3)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_3"), 2);
    }

    if (texture4)
    {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture4->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_4"), 3);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema::RenderSimpleTreeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec4& worldPositionAndScale, const glm::vec3& heliPosition, int level, float length, Texture2D* texture1, Texture2D* texture2, Texture2D* texture3, Texture2D* texture4)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need
    int loc_time = glGetUniformLocation(shader->program, "Time");
    glUniform1f(loc_time, Engine::GetElapsedTime());

    //binds the world space matrix
    int loc_world_position = glGetUniformLocation(shader->program, "WorldPositionAndScale");
    glUniform4fv(loc_world_position, 1, glm::value_ptr(worldPositionAndScale));

    int loc_level = glGetUniformLocation(shader->program, "Level");
    glUniform1i(loc_level, level);

    int loc_length = glGetUniformLocation(shader->program, "Length");
    glUniform1f(loc_level, length);

    int heli_position = glGetUniformLocation(shader->program, "HeliPosition");
    glUniform3fv(heli_position, 1, glm::value_ptr(heliPosition));
    

    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_1"), 0);
    }

    if (texture2)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_2"), 1);

    }

    //Bonus 
    if (texture3)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_3"), 2);
    }

    if (texture4)
    {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture4->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_4"), 3);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema::RenderSimpleMarkerMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec4& worldPositionAndScale, const glm::vec3& heliPosition,
    Texture2D* texture1, Texture2D* texture2, Texture2D* texture3, Texture2D* texture4)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need
    int loc_time = glGetUniformLocation(shader->program, "Time");
    glUniform1f(loc_time, Engine::GetElapsedTime());

    //binds the world space matrix
    int loc_world_position = glGetUniformLocation(shader->program, "WorldPositionAndScale");
    glUniform4fv(loc_world_position, 1, glm::value_ptr(worldPositionAndScale));

    int heli_position = glGetUniformLocation(shader->program, "HeliPosition");
    glUniform3fv(heli_position, 1, glm::value_ptr(heliPosition));


    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_1"), 0);
    }

    if (texture2)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_2"), 1);

    }

    //Bonus 
    if (texture3)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_3"), 2);
    }

    if (texture4)
    {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture4->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_unit_4"), 3);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema::LoadShader(const std::string& name)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, "src/lab", "Tema", "shaders");

    // Create a shader program
    {
        Shader* shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".FS.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema::OnInputUpdate(float deltaTime, int mods)
{

}


void Tema::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float data[4];

        int x = window->GetCursorPosition().x;
        int y = window->GetCursorPosition().y;

        y = window->props.resolution.y - y;

        glBindFramebuffer(GL_READ_FRAMEBUFFER, custom_framebuffer_object);
        glReadBuffer(GL_COLOR_ATTACHMENT1);

        glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, data);

        if (data[3] == 0)
        {
            marker_position.x = data[0];
            marker_position.z = data[2];
        }

        if (data[3] != 0)
        {
            tree_information[data[3] * 10 - 1].x = rand() % 100 + 1;
            tree_information[data[3] * 10 - 1].z = rand() % 100 + 1;
        }

        std::cout << "World Coordinates Read: ("
            << data[0] << ", " << data[1] << ", " << data[2] << ", " << data[3] << ")" << std::endl;

        std::cout << "Read at cursor position: ("
            << x << ", " << y << ")" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}


void Tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema::OnWindowResize(int width, int height)
{
}