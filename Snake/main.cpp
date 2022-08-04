#include <SDL.h>
#include <SDL_ttf.h>
#include <random>
#include <list>
#include <string>

void DrawGrid(SDL_Renderer* renderer, int gridSizeX, int gridSizeY, int squareSize)
{
    SDL_SetRenderDrawColor(renderer, 22, 22, 22, 255);
    SDL_RenderClear(renderer);
    int startPos = 0;
    for (int y = 0; y < gridSizeY; y++) {
        for (int x = startPos; x < gridSizeX; x += 2) {
            SDL_Rect rect;
            rect.x = x * squareSize;
            rect.y = y * squareSize;
            rect.w = squareSize;
            rect.h = squareSize;
            SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        startPos = 1 - startPos;
    }
}

int GenerateApple(int gridSize, int grid[])
{
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<> distr(0, gridSize - 1);
     int result = distr(gen);
     return result;
}

void DrawRectangle(int pos, int xMax, SDL_Renderer *renderer, int squareSize, int r, int g, int b)
{
    int xPos = pos % xMax;
    int yPos = (pos - xPos) / xMax;
    SDL_Rect rect;
    rect.x = xPos * squareSize;
    rect.y = yPos * squareSize;
    rect.w = squareSize;
    rect.h = squareSize;
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);
}
void DrawText(std::string text, SDL_Renderer* renderer, SDL_Rect rect, TTF_Font* font)
{
    SDL_Surface* scoreMsg = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255 });
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, scoreMsg);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(scoreMsg);
}
int main(int argc, char *argv[])
{
    //Initialize SDL and TTF
	SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
	SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event e;
    TTF_Font* font = TTF_OpenFont("Fonts\\arial.ttf", 60);
    SDL_Texture* scoreTexture;
    SDL_Texture* highscoreTexture;
    SDL_Rect scoreRect;
    scoreRect.x = 20;
    scoreRect.y = 530;
    scoreRect.w = 120;
    scoreRect.h = 40;
    SDL_Rect highscoreRect;
    highscoreRect.x = 190;
    highscoreRect.y = 530;
    highscoreRect.w = 180;
    highscoreRect.h = 40;

    enum Directions
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    const int gridSizeX = 20;
    const int gridSizeY = 25;
    const int gridSize = gridSizeX * gridSizeY;
    int squareSize = 20;    
    int grid[gridSize] = {0};
    int startHeadPos = 205;
    int headPos = startHeadPos;
    std::list<int> snakeBody;
    snakeBody.push_front(startHeadPos);
    int direction = 0;
    int prevDirection = 0;
    bool running = true;
    int applePos = GenerateApple(gridSize, grid);
    if (applePos == startHeadPos)
        applePos += 1;
    bool checkApple;   
    int initialDelaySpeed = 300;
    int delaySpeed = initialDelaySpeed;
    int score = 0;
    int highscore = 0;
    std::string scoreTxt;
    std::string highscoreTxt;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_UP) 
                    direction = UP;
                if (e.key.keysym.sym == SDLK_DOWN) 
                    direction = DOWN;
                if (e.key.keysym.sym == SDLK_LEFT) 
                    direction = LEFT;
                if (e.key.keysym.sym == SDLK_RIGHT) 
                    direction = RIGHT;
            }
        }
        switch (direction)
        {
        case NONE:
            break;
        case UP:
        {
            if (prevDirection == DOWN)
            {
                grid[headPos] = 1;
                if ((headPos + 1) > gridSize - gridSizeX)
                //~Can go through borders:
                    //headPos -= (gridSizeX * (gridSizeY - 1));
                //~Die on border:
                {
                    snakeBody.clear();
                    snakeBody.push_front(startHeadPos);
                    headPos = startHeadPos;
                    applePos = GenerateApple(gridSize, grid);
                    direction = 0;
                    delaySpeed = initialDelaySpeed;
                    score = 0;
                    prevDirection = 0;
                    break;
                }
                else headPos += gridSizeX;
                prevDirection = DOWN;
                break;
            }
            grid[headPos] = 1;
            if ((headPos + 1) <= gridSizeX) 
            //~Can go through borders:
                //headPos += (gridSizeX * (gridSizeY - 1));
            //~Die on border:
            {
                snakeBody.clear();
                snakeBody.push_front(startHeadPos);
                headPos = startHeadPos;
                applePos = GenerateApple(gridSize, grid);
                direction = 0;
                delaySpeed = initialDelaySpeed;
                score = 0;
                prevDirection = 0;
                break;
            }
            else headPos -= gridSizeX;
            prevDirection = UP;
            break;
        } 
        case DOWN:
        {
            if (prevDirection == UP)
            {
                grid[headPos] = 1;
                if ((headPos + 1) <= gridSizeX)
                //~Can go through borders:
                    //headPos += (gridSizeX * (gridSizeY - 1));
                //~Die on border:
                {
                    snakeBody.clear();
                    snakeBody.push_front(startHeadPos);
                    headPos = startHeadPos;
                    applePos = GenerateApple(gridSize, grid);
                    direction = 0;
                    delaySpeed = initialDelaySpeed;
                    score = 0;
                    prevDirection = 0;
                    break;
                }
                else headPos -= gridSizeX;
                prevDirection = UP;
                break;
            }
            grid[headPos] = 1;
            if ((headPos + 1) > gridSize - gridSizeX) 
            //~Can go through borders:
                //headPos -= (gridSizeX * (gridSizeY - 1));
            //~Die on border:
            {
                snakeBody.clear();
                snakeBody.push_front(startHeadPos);
                headPos = startHeadPos;
                applePos = GenerateApple(gridSize, grid);
                direction = 0;
                delaySpeed = initialDelaySpeed;
                score = 0;
                prevDirection = 0;
                break;
            }
            else headPos += gridSizeX;
            prevDirection = DOWN;
            break;
        }
        case LEFT:
        {
            if (prevDirection == RIGHT)
            {
                grid[headPos] = 1;
                if ((headPos + 1) % gridSizeX == 0)
                //~Can go through borders:
                    //headPos -= (gridSizeX - 1);
                //~Die on border:
                {
                    snakeBody.clear();
                    snakeBody.push_front(startHeadPos);
                    headPos = startHeadPos;
                    applePos = GenerateApple(gridSize, grid);
                    direction = 0;
                    delaySpeed = initialDelaySpeed;
                    score = 0;
                    prevDirection = 0;
                    break;
                }
                else headPos += 1;
                prevDirection = RIGHT;
                break;
            }
            grid[headPos] = 1;
            if ((headPos + 1) % gridSizeX == 1) 
            //~Can go through borders:
                //headPos += (gridSizeX - 1);
            //~Die on border:
            {
                snakeBody.clear();
                snakeBody.push_front(startHeadPos);
                headPos = startHeadPos;
                applePos = GenerateApple(gridSize, grid);
                direction = 0;
                delaySpeed = initialDelaySpeed;
                score = 0;
                prevDirection = 0;
                break;
            }
            else headPos -= 1;
            prevDirection = LEFT;
            break;
        }
        case RIGHT:
        {
            if (prevDirection == LEFT)
            {
                grid[headPos] = 1;
                if ((headPos + 1) % gridSizeX == 1)
                //~Can go through borders:
                    //headPos += (gridSizeX - 1);
                //~Die on border:
                {
                    snakeBody.clear();
                    snakeBody.push_front(startHeadPos);
                    headPos = startHeadPos;
                    applePos = GenerateApple(gridSize, grid);
                    direction = 0;
                    delaySpeed = initialDelaySpeed;
                    score = 0;
                    prevDirection = 0;
                    break;
                }
                else headPos -= 1;
                prevDirection = LEFT;
                break;
            }
            grid[headPos] = 1;
            if ((headPos + 1) % gridSizeX == 0) 
            //~Can go through borders:
                //headPos -= (gridSizeX - 1);
            //~Die on border:
            {
                snakeBody.clear();
                snakeBody.push_front(startHeadPos);
                headPos = startHeadPos;
                applePos = GenerateApple(gridSize, grid);
                direction = 0;
                delaySpeed = initialDelaySpeed;
                score = 0;
                prevDirection = 0;
                break;
            }
            else headPos += 1;
            prevDirection = RIGHT;
            break;
        }
        }

        //Die upon collision with itself
        if (grid[headPos] == 1)
        {
            for (int x : snakeBody)
            {
                grid[x] = 0;
            }
            snakeBody.clear();
            snakeBody.push_front(startHeadPos);
            headPos = startHeadPos;
            applePos = GenerateApple(gridSize, grid);
            direction = 0;
            delaySpeed = initialDelaySpeed;
            score = 0;
            prevDirection = 0;
            continue;
        }
        grid[snakeBody.back()] = 0;
        grid[applePos] = 2;

        //Check if apple is eaten
        if (grid[headPos] == 2)
        {
            applePos = GenerateApple(gridSize, grid);
            score += 1;
            delaySpeed -= 5;
        }
        else snakeBody.pop_back();
        snakeBody.push_front(headPos);

        //Validate apple spawn
        do 
        {
            checkApple = true;
            for (int x : snakeBody)
            {
                if (applePos == x)
                {
                    applePos = GenerateApple(gridSize, grid);
                    checkApple = false;
                    break;
                }
            }
        } while (!checkApple);

        //Set scoreboard
        if (score > highscore)
            highscore = score;
        scoreTxt = "Score: " + std::to_string(score);
        highscoreTxt = "Highscore: " + std::to_string(highscore);  
        //Render items:
        DrawGrid(renderer, gridSizeX, gridSizeY, squareSize);
        //Body
        for (int x : snakeBody)
        {
            DrawRectangle(x, gridSizeX, renderer, squareSize, 0, 255, 0);
        }
        //Head
        DrawRectangle(snakeBody.front(), gridSizeX, renderer, squareSize, 0, 105, 0);
        //Apple
        DrawRectangle(applePos, gridSizeX, renderer, squareSize, 255, 0, 0);
        //Text:
        DrawText(scoreTxt, renderer, scoreRect, font);
        DrawText(highscoreTxt, renderer, highscoreRect, font);
        SDL_RenderPresent(renderer);
        //Set delay
        if (delaySpeed < 75)
            delaySpeed = 75;
        SDL_Delay(delaySpeed);       
    }
    //Destroy objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();    
	return 0;
}