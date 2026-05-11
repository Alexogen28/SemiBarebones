#include "lab/lab01/lab01.h"

#include <vector>

using namespace std;
using namespace lab;

Lab01::Lab01()
{
    window->SetSize(1280, 720);
}


Lab01::~Lab01()
{
}

void Lab01::Initialize()
{
    Exercise1();
    Exercise2();
    Exercise3();
    Exercise4();
    Exercise5();
    Exercise6();
}

void Lab01::Exercise1()
{
    SetExercise();

    //TODO(student): Ex. 1
    {
        SetPanel("1", { -3, -2, 100 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(5, 2);
        glm::vec2 v2(2, 3);

        glm::vec2 v3 = v1 + v2;

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");
        RenderVector(v3, glm::vec3(0, 0, 1), "v12");
        RenderVector(v1, glm::vec3(1, 0, 0), "v1", v2);
        RenderVector(v2, glm::vec3(0, 1, 0), "v2", v1);
    }
}

void Lab01::Exercise2()
{
    SetExercise();

    //TODO(student): Ex. 2
    {
        SetPanel("2", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        {
            glm::vec2 p1(1, 1);
            glm::vec2 p2(3, -3);

            glm::vec2 v21(-2, 4);

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");

            RenderVector(v21, glm::vec3(1, 0, 1), "v21", p2);
        }

        {
            glm::vec2 p3(-1, -2);
            glm::vec2 p4(-5, -4);

            glm::vec2 v34(-4, -2);

            RenderPoint(p3, glm::vec3(1, 0, 0), "P3");
            RenderPoint(p4, glm::vec3(0, 0, 1), "P4");

            RenderVector(v34, glm::vec3(1, 0, 1), "v34", p3);
        }

        {
            glm::vec2 p5(6, 3);
            glm::vec2 p6(4, 1);

            glm::vec2 v56(-2, -2);

            RenderPoint(p5, glm::vec3(1, 0, 0), "P5");
            RenderPoint(p6, glm::vec3(0, 0, 1), "P6");

            RenderVector(v56, glm::vec3(1, 0, 1), "v56", p5);
        }

        {
            glm::vec2 p7(5, 4);
            glm::vec2 p8(-3, 4);

            RenderPoint(p7, glm::vec3(1, 0, 0), "P7");
            RenderPoint(p8, glm::vec3(0, 0, 1), "P8");

            glm::vec2 v87(8, 0);

            RenderVector(v87, glm::vec3(1, 0, 1), "v87", p8);
        }
    }
}

void Lab01::Exercise3()
{
    SetExercise();

    //TODO(student): Ex. 3
    {
        SetPanel("3", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(4, 3);
        glm::vec2 v2(-3, 3);
        glm::vec2 v3(-6, -2);
        glm::vec2 v4(-2, -4);
        glm::vec2 v5(3, -2);

        glm::vec2 vu1 = glm::normalize(v1);
        glm::vec2 vu2 = glm::normalize(v2);
        glm::vec2 vu3 = glm::normalize(v3);
        glm::vec2 vu4 = glm::normalize(v4);
        glm::vec2 vu5 = glm::normalize(v5);

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");
        RenderVector(v3, glm::vec3(0, 1, 1), "v3");
        RenderVector(v4, glm::vec3(0, 0, 1), "v4");
        RenderVector(v5, glm::vec3(1, 0, 1), "v5");

        RenderVector(vu1, glm::vec3(0, 0, 0));
        RenderVector(vu2, glm::vec3(0, 0, 0));
        RenderVector(vu3, glm::vec3(0, 0, 0));
        RenderVector(vu4, glm::vec3(0, 0, 0));
        RenderVector(vu5, glm::vec3(0, 0, 0));
    }
}

void Lab01::Exercise4()
{
    SetExercise ();

    //TODO(student): Ex. 4
    {
        SetPanel("4", { -3, -2, 100 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(4, 1.5);
        glm::vec2 v2(2, 3);
        glm::vec2 v0(1, 0);
        glm::vec2 v0_y(0, 1);




        glm::vec2 v1_norm = glm::normalize(v1);
        glm::vec2 v2_norm = glm::normalize(v2);
        glm::vec2 v0_norm = glm::normalize(v0);
        glm::vec2 v0_y_norm = glm::normalize(v0_y);

        float initial_angle = acos(glm::dot(v1_norm, v0_norm));
        float angle = acos(glm::dot(v1_norm, v2_norm));

        RenderArc(initial_angle, angle, glm::vec3(0, 0, 0));

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");

        //BONUS HERE

        const float pi = 3.14;

        const float angle_v2oy = acos(glm::dot(v2_norm, v0_norm));

        const float angle_2 = (pi * 2) - angle;
        RenderArc(angle_v2oy, angle_2, glm::vec3(1, 0, 0), 1);


    }
}

void Lab01::Exercise5()
{
    SetExercise();

    //TODO(student): Ex. 5
    {
        SetPanel("5", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        {
            glm::vec2 p1(1, 1);
            glm::vec2 p2(3, 1);
            glm::vec2 p3(2, 4);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            float distance1 = sqrt(pow(3 - 1, 2) + pow(1 - 1, 2));
            float distance2 = sqrt(pow(2 - 3, 2) + pow(4 - 1, 2));
            float distance3 = sqrt(pow(1 - 2, 2) + pow(1 - 4, 2));

            float semiperimeter = (distance1 + distance2 + distance3) / 2;

            float areaFloat = sqrt(semiperimeter * (semiperimeter - distance1) * (semiperimeter - distance2) * (semiperimeter - distance3));
            string area = to_string(areaFloat);

            float gx = (1 + 3 + 2) / 3;
            float gy = (1 + 1 + 4) / 3;
            glm::vec2 weight(gx, gy);

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");
            RenderText(weight, area, glm::vec3(0,0,0));
        }

        {
            glm::vec2 p1(-1.5, -3.7);
            glm::vec2 p2(-2.5, 3.2);
            glm::vec2 p3(-6.1, 2.9);

            float distance1 = sqrt(pow(-1.5 + 2.5, 2) + pow(-3.7 - 3.2, 2));
            float distance2 = sqrt(pow(-2.5 + 6.1, 2) + pow(3.2 - 2.9, 2));
            float distance3 = sqrt(pow(-6.1 + 1.5, 2) + pow(2.9 + 3.7, 2));

            float semiperimeter = (distance1 + distance2 + distance3) / 2;
            float areaFloat = sqrt(semiperimeter * (semiperimeter - distance1) * (semiperimeter - distance2) * (semiperimeter - distance3));

            string area = to_string(areaFloat);

            float gx = (-1.5 -2.5 -6.1) / 3;
            float gy = (-3.7 +3.2 +2.9) / 3;

            glm::vec2 weight(gx, gy);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");
            RenderText(weight, area, glm::vec3(0, 0, 0));
        }

        {
            glm::vec2 p1(2.3, -2.1);
            glm::vec2 p2(7.1, -3.5);
            glm::vec2 p3(7.5, 3.7);

            float distance1 = sqrt(pow(2.3 - 7.1, 2) + pow(-2.1 + 3.5, 2));
            float distance2 = sqrt(pow(7.1 - 7.5, 2) + pow(-3.5 - 3.7, 2));
            float distance3 = sqrt(pow(7.5 - 2.3, 2) + pow(3.7 + 2.1, 2));

            float semiperimeter = (distance1 + distance2 + distance3) / 2;

            float areaFloat = sqrt(semiperimeter * (semiperimeter - distance1) * (semiperimeter - distance2) * (semiperimeter - distance3));

            string area = to_string(areaFloat);

            float gx = (2.3 + 7.1 + 7.5) / 3;
            float gy = (-2.1 -3.5 + 3.7) / 3;

            glm::vec2 weight(gx, gy);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));
            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");
            RenderText(weight, area, glm::vec3(0, 0, 0));
        }
    }
}

void Lab01::Exercise6()
{
    SetExercise();

    //TODO(student): Ex. 6
    {
        SetPanel("6", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        glm::vec2 p1(-4, 3);
        glm::vec2 p2(6, 2);
        glm::vec2 p3(1, -4);

        const float EPSILON = 0.05f;

        float distance_v1v2 = sqrt(pow(-4 -6, 2) + pow(3 -2, 2));
        float distance_v2v3 = sqrt(pow(6 -1, 2) + pow(2 +4, 2));
        float distance_v1v3 = sqrt(pow(1 +4, 2) + pow(-4 -3, 2));

        float semiperimeter = (distance_v1v2 + distance_v2v3 + distance_v1v3) / 2;

        float area_v1v2v3 = sqrt(semiperimeter * (semiperimeter - distance_v1v2) * (semiperimeter - distance_v2v3) * (semiperimeter - distance_v1v3));

        RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

        RenderPoint(p1, glm::vec3(0, 0, 1), "P1");
        RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
        RenderPoint(p3, glm::vec3(0, 0, 1), "P3");

        std::vector<glm::vec2> check_points{
            glm::vec2(1, 1), glm::vec2(3.21, 3.2),
            glm::vec2(-2.16, 2.9), glm::vec2(2, -2),
            glm::vec2(-1.1, -0.9), glm::vec2(-1.9, 2.1),
            glm::vec2(-4, 0), glm::vec2(-4, -2.5),
            glm::vec2(4.5, 1.1), glm::vec2(5.9, -3.1),
            glm::vec2(7.1, 1.9), glm::vec2(-5.9, 1.5)
        };

        int ok = 0;

        for (const glm::vec2 &check_point : check_points) {

            float point_x = check_point.x;
            float point_y = check_point.y;

            float distance_pv1 = sqrt(pow(point_x +4, 2) + pow(point_y -3, 2));
            float distance_pv2 = sqrt(pow(point_x -6, 2) + pow(point_y -2, 2));
            float distance_pv3 = sqrt(pow(point_x -1, 2) + pow(point_y +4, 2));

            float sp_pv1v3 = (distance_pv1 + distance_pv3 + distance_v1v3) / 2;
            float area_pv1v3 = sqrt(sp_pv1v3 * (sp_pv1v3 - distance_v1v3) * (sp_pv1v3 - distance_pv1) * (sp_pv1v3 - distance_pv3));

            float sp_pv2v3 = (distance_pv2 + distance_pv3 + distance_v2v3) / 2;
            float area_pv2v3 = sqrt(sp_pv2v3 * (sp_pv2v3 - distance_pv2) * (sp_pv2v3 - distance_pv3) * (sp_pv2v3 - distance_v2v3));

            float sp_pv1v2 = (distance_pv1 + distance_pv2 + distance_v1v2) / 2;
            float area_pv1v2 = sqrt(sp_pv1v2 * (sp_pv1v2 - distance_v1v2) * (sp_pv1v2 - distance_pv1) * (sp_pv1v2 - distance_pv2));


            bool inside_triangle = abs(area_v1v2v3 - (area_pv1v3 + area_pv1v2 + area_pv2v3)) < EPSILON;

            //string area = to_string(area_v1v2v3);
            string area = to_string(area_pv2v3);


            if (ok == 0) {
                RenderText(glm::vec2(0.5,0.5), area, glm::vec3(0,0,0));
                ok++;
            }
            else {
                ok++;
            }


            if (inside_triangle) {
                RenderPoint(check_point, glm::vec3(0, 1, 0), "Yep");
            }
            else {
                RenderPoint(check_point, glm::vec3(1, 0, 0), "Nope");
            }

        }
    }
}
