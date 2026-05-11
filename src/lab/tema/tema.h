#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab/lab03/transform2D.h"
#include "lab/lab05/transform3D.h"


namespace lab
{
    class Tema : public gfxc::SimpleScene
    {
    public:
        Tema();
        ~Tema();

        void Init() override;
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);

        int depth_texture_height;
        int depth_texture_width;

        float texture_width;
        float texture_height;
        std::vector<float> height_map_samples;
        std::vector<glm::vec4> tree_information;
        std::unordered_map<std::string, Texture2D*> mapTextures;
        glm::vec3 heli_position;
        glm::vec3 heli_rotation_axis;
        glm::vec3 propeller_rotation_axis;
        glm::vec3 tail_distance_from_cabin;
        glm::vec3 tail_world_position;
        glm::vec3 translation_direction;

        glm::vec3 marker_position;

        float heli_rotation_angle;
        float body_propeller_rotation_angle;
        float tree_branch_distance_y;

        glm::mat4 propeller_matrix;
        glm::mat4 tree_matrix;

        glm::vec3 tree_position;

        float move_speed;
        glm::vec3 heli_destionation;



    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void CheckHelicopterMovement();
        void RenderHelicopter();


        void RenderMarker();

        void RenderHappyLittleTree(glm::mat4 parent_matrix, glm::vec3 treePosition, int level, float length, float selection_value);

        Texture2D* CreateTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,const glm::vec4& worldPositionAndScale, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, Texture2D* texture3 = NULL, Texture2D* texture4 = NULL);
        void RenderSimpleTreeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,const glm::vec4& worldPositionAndScale, const glm::vec3& heliPosition, 
            int level, float length, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, Texture2D* texture3 = NULL, Texture2D* texture4 = NULL);
        void RenderSimpleMarkerMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec4& worldPositionAndScale, const glm::vec3& heliPosition,
            Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, Texture2D* texture3 = NULL, Texture2D* texture4 = NULL);

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& model,
            const gfxc::Camera* camera, const transform2D::ViewportSpace& viewport_space);

        void CreateFramebuffer(int width, int height);
        void RenderToCustomFramebuffer();


        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        Texture2D* LoadTexture(const char* imagePath);
        void LoadShader(const std::string& name);

        unsigned int custom_framebuffer_object;
        unsigned int color_texture;
        unsigned int color_texture_coords;


    protected:
        transform2D::ViewportSpace viewport_space;
    };
}   // namespace lab
