#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World01::Initialize()
    {
        for (int i = 0; i < 12; i++) m_positions.push_back({ randomf(-0.2, 0.2), randomf(-0.1, 0.2) });

        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt)
    {
        m_angle += 90 * dt;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? dt : 0;

        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? dt : 0;
        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt : 0;


        m_rotate = ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_R) ? true : false;

        m_time += dt;

    }

    void World01::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render

        //multipule
        for (int i = 0; i < 12; i++)
        {

            glPushMatrix();
            glTranslatef(m_positions[i].x, m_positions[i].y, 0);
            glRotatef(m_angle, 0, 0, 1);
            glScalef(0.2f, 0.2f, 0.2f);//(sin(m_time * 5) + 1) * 

            glBegin(GL_TRIANGLES);

            glColor3f(1, 0, 0);
            glVertex2f(-0.5f, -0.5f);

            glColor3f(0, 1, 0);
            glVertex2f(0, 0.5f);

            glColor3f(0, 0, 1);
            glVertex2f(0.5f, -0.5f);

            glEnd();

            glPopMatrix();
        }

        //player/controlled
        glPushMatrix();
        glTranslatef(m_position.x, m_position.y, 0);
        if(m_rotate) glRotatef(m_angle, 1, 0, 0);

        glScalef((sin(m_time * 5) + 1) * 0.5f, 0.5f, 0.5f);

        glBegin(GL_TRIANGLE_STRIP);

        glColor3f(1, 0, 0);
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0, 1, 0);
        glVertex2f(0, 0.5f);

        glColor3f(0, 0, 1);
        glVertex2f(0.5f, -0.5f);

        glColor3f(1, 0, 0);
        glVertex2f(0.4, 0.4);

        glColor3f(1, 0, 0);
        glVertex2f(0.4, 0.1);

        glEnd();

        glPopMatrix();

        // post-render
        renderer.EndFrame();
    }
}
