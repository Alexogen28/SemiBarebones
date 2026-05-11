#include "lab/lab02/triangle_rasterizer.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace lab;

void TriangleRasterizer::Rasterize(
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3,
    Image *image,
    DepthImage *depthImage)
{
    float xMin = min(v1.position.x, min(v2.position.x, v3.position.x));
    float yMin = min(v1.position.y, min(v2.position.y, v3.position.y));
    float xMax = max(v1.position.x, max(v2.position.x, v3.position.x));
    float yMax = max(v1.position.y, max(v2.position.y, v3.position.y));

    for (int row = yMin; row < yMax; row++) {
        for (int column = xMin; column < xMax; column++) {

            if (row < 0 || row >= image->GetHeight() ||
                column < 0 || column >= image->GetWidth()) {
                continue;
            }

            glm::vec2 p(column + 0.5f, row + 0.5f);

            if (CheckPointInsideTriangle(p, v1, v2, v3)) {

                float depth = ComputePixelDepth(p, v1, v2, v3);
                float pixelDepth = depthImage->Get(row, column).x;

                if (pixelDepth > depth) {
                    image->Set(row, column, ComputePixelColor (p, v1, v2, v3));
                    depthImage->Set(row, column, glm::vec3(depth, 0, 0));
                }
            }
        }
    }
}

float TriangleRasterizer::ComputeTriangleArea(
    const glm::vec2 &v1,
    const glm::vec2 &v2,
    const glm::vec2 &v3)
{
    // TODO(student): Ex. 1

    float length_v1v2 = sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y,2));
    float length_v1v3 = sqrt(pow(v1.x - v3.x, 2) + pow(v1.y - v3.y,2));
    float length_v2v3 = sqrt(pow(v2.x - v3.x, 2) + pow(v2.y - v3.y,2));


    float semiperimeter = (length_v1v2 + length_v1v3 + length_v2v3) / 2;

    float triangleArea = sqrt(semiperimeter * (semiperimeter - length_v1v2) * (semiperimeter - length_v1v3) * (semiperimeter - length_v2v3));

    return triangleArea;
}

bool TriangleRasterizer::CheckPointInsideTriangle(
    const glm::vec2 &p,
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3)
{
    const float EPSILON = 5.0f;

    float triangle_area = ComputeTriangleArea(v1.position, v2.position, v3.position);
    float triangle_pv1v2_area = ComputeTriangleArea(p, v1.position, v2.position);
    float triangle_pv1v3_area = ComputeTriangleArea(p, v1.position, v3.position);
    float triangle_pv2v3_area = ComputeTriangleArea(p, v2.position, v3.position);

    bool inside_triangle = abs(triangle_area - (triangle_pv1v3_area + triangle_pv2v3_area + triangle_pv1v2_area)) < EPSILON;

    // TODO(student): Ex. 1

    return inside_triangle;
}

glm::vec3 TriangleRasterizer::ComputePixelColor(
    const glm::vec2 &p,
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3)
{
    // TODO(student): Ex. 2

    float triangle_area = ComputeTriangleArea(v1.position, v2.position, v3.position);
    float triangle_pv1v2_area = ComputeTriangleArea(p, v1.position, v2.position);
    float triangle_pv1v3_area = ComputeTriangleArea(p, v1.position, v3.position);
    float triangle_pv2v3_area = ComputeTriangleArea(p, v2.position, v3.position);

    float u = triangle_pv1v2_area / triangle_area;
    float v = triangle_pv1v3_area / triangle_area;
    float w = triangle_pv2v3_area / triangle_area;

    glm::vec3 color_ratio_v1(v1.color.x * w, v1.color.y * w, v1.color.z * w);
    glm::vec3 color_ratio_v2(v2.color.x * v, v2.color.y * v, v2.color.z * v);
    glm::vec3 color_ratio_v3(v3.color.x * u, v3.color.y * u, v3.color.z * u);

    glm::vec3 point_color = color_ratio_v1 + color_ratio_v2 + color_ratio_v3;


    return point_color;
}

float TriangleRasterizer::ComputePixelDepth(
    const glm::vec2 &p,
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3)
{
    // TODO(student): Ex. 3

    float triangle_area = ComputeTriangleArea(v1.position, v2.position, v3.position);
    float triangle_pv1v2_area = ComputeTriangleArea(p, v1.position, v2.position);
    float triangle_pv1v3_area = ComputeTriangleArea(p, v1.position, v3.position);
    float triangle_pv2v3_area = ComputeTriangleArea(p, v2.position, v3.position);

    float u = triangle_pv1v2_area / triangle_area;
    float v = triangle_pv1v3_area / triangle_area;
    float w = triangle_pv2v3_area / triangle_area;

    float depth_v1 = v1.position.z * w;
    float depth_v2 = v2.position.z * v;
    float depth_v3 = v3.position.z * u;

    float point_depth = depth_v1 + depth_v2 + depth_v3;

    return point_depth;
}
