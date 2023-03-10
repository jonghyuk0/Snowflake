#include <iostream>
#include <vector>
#include <random>

#include <Screen.hpp>
#include <Snow.hpp>
#include <Vector2f.hpp>

int main(int argc, char** argv)
{

    if(SDL_Init(SDL_INIT_VIDEO))
        std::cout << "SDL init failed. Error : " << SDL_GetError() << std::endl;
    if(!IMG_Init(IMG_INIT_PNG))
        std::cout << "IMG init failed. Error : " << IMG_GetError() << std::endl;

    Screen screen("SnowFlake", 1600, 800);

    SDL_GL_SetSwapInterval(1);

    SDL_Event event;

    SDL_Texture* snow = screen.loadTexture("res/gfx/snow.png");

    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(0, 1);

    std::vector<Snow> entities;

    for(int i = 0; i < 100; i++)
    {
        Snow tempentity(
            Vector2f(dis(gen) * 1600, -1 * dis(gen) * 800 - 4),
            snow, 8, 8, (unsigned char)(dis(gen) * 200 + 55),
            dis(gen) * 20 + 40,
            dis(gen) * 5 + 10,
            dis(gen) * 5 + 10,
            dis(gen) < 0.5 ? -1 : 1,
            dis(gen) < 0.5 ? -10 : 10
        );
        entities.push_back(tempentity);
    }

    bool gamerunning = true;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    float deltaTime = 0;

    // int fps = 0;
    // float time = 0;

    while(gamerunning)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (float)(NOW - LAST) / SDL_GetPerformanceFrequency();


        // time += deltaTime;
        // fps++;

        // if(time >= 1)
        // {
        //     std::cout << fps << std::endl;
        //     time -= 1;
        //     fps = 0;
        // }

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                gamerunning = false;
                break;
            }
        }

        screen.clear();

        for(int i = 0; i < (int)entities.size(); i++)
        {
            screen.render(entities[i]);
            entities[i].position().x += entities[i].swingspeed * entities[i].swingdir * deltaTime;
            entities[i].position().y += entities[i].speed * deltaTime;

            
            entities[i].swingspeed += entities[i].swinginde * deltaTime;

            if(entities[i].swingspeed >= entities[i].swing)
            {
                entities[i].swingspeed = entities[i].swing;
                entities[i].swinginde *= -1;
            }
            else if(entities[i].swingspeed <= 0)
            {
                entities[i].swingspeed = 0;
                entities[i].swinginde *= -1;
                entities[i].swingdir *= -1;
            }

            if(entities[i].position().y > 800)
            {
                entities.erase(entities.begin() + i);
                i--;

                Snow tempentity(
                    Vector2f(dis(gen) * 1600, -4),
                    snow, 8, 8, (unsigned char)(dis(gen) * 200 + 55),
                    dis(gen) * 20 + 40,
                    dis(gen) * 5 + 10,
                    dis(gen) * 5 + 10,
                    dis(gen) < 0.5 ? -1 : 1,
                    dis(gen) < 0.5 ? -10 : 10
                );
                entities.push_back(tempentity);
            }
        }

        screen.display();
    }

    screen.clean();
    SDL_Quit();

    return 0;
}