#include "Player.hpp"
#include "Camera.hpp"



int PlayerOrientation = 0;
int ThePlayerOrientation = 0;
float thePlayerPos = 0;

bool LeftKey = false;
bool RightKey = false;
bool UpKey = false;
bool DownKey = false;

bool RightKeyDown = false;
bool LeftKeyDown = false;
bool UpKeyDown = false;
bool DownKeyDown = false;

int PlayerOrinetation = 1;
int frameInt = 0;
int TheFrameTime = 1;

bool GrassWalkSoundPlaying = false;

std::string playerSprite = "Human/FrontAnimation/penguin1.png";


void SetKeyStatus(bool RightKeyPressed, bool LeftKeyPressed, bool UpKeyPressed, bool DownKeyPressed)
{
    RightKeyDown = RightKeyPressed;
    DownKeyDown = DownKeyPressed;
    UpKeyDown = UpKeyPressed;
    LeftKeyDown = LeftKeyPressed;

    if (RightKeyPressed = false || LeftKeyPressed == false)
    {
        LeftKey = LeftKeyPressed;
        RightKey = RightKeyPressed;
    }else
    {
        LeftKey = false;
        RightKey = false;
    }


    if (UpKey == false || DownKey == false)
    {
        UpKey = UpKeyPressed;
        DownKey = DownKeyPressed;

    }else
    {
        UpKey = false;
        DownKey = false;
    }
}

std::string GetSprite()
{
    return playerSprite;
}

glm::vec3 PlayerInputs(float speed, glm::vec3 Position)
{
    glm::vec3 position = Position;
    
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::vec3 TemporaryPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    if (UpKeyDown)
    {
        TemporaryPosition += glm::vec3(0.0f,0.0f,-1.0f);

        PlayerOrientation = 0;
        
        UpKey = true;
    }else
    {
        UpKey = false;
    }

    if (LeftKeyDown)
    {
        TemporaryPosition += glm::vec3(1.0f,0.0f,0.0f);

        PlayerOrientation = 90;

        LeftKey = true;
    }else
    {
        LeftKey = false;
    }
    
    if (DownKeyDown)
    {
        TemporaryPosition += glm::vec3(0.0f,0.0f,1.0f);

        PlayerOrientation = 180;

        DownKey = true;
    }else
    {
        DownKey = false;
    }

    if (RightKeyDown)
    {
        TemporaryPosition += glm::vec3(-1.0f,0.0f,0.0f);

        PlayerOrientation = -90;

        RightKey = true;
    }else
    {
        RightKey = false;
    }

    if(TemporaryPosition != glm::vec3(0.0f, 0.0f, 0.0f))  
    {
        if(GrassWalkSoundPlaying == false)
        {
            PlaySFX("res/dev/GrassWalk.wav", -1, 1);
            
            GrassWalkSoundPlaying = true;
        }
        
        position += speed * glm::normalize(TemporaryPosition);
    }else{
        GrassWalkSoundPlaying = false;
        StopSFX(1);
    }

    return position;
}


void SetSprite(float fps, glm::vec3 InputsPosition, glm::vec3 Position)
{
    if (InputsPosition != Position)
    {	
        TheFrameTime++;

        if(fps / TheFrameTime < 6)
        {
            TheFrameTime = 1;

            if(frameInt >= 6)
            {
                frameInt = 0;
            }

            frameInt++;

            //Chnage Texture
            if(DownKey == true)
            {
                playerSprite = "Human/FrontAnimation/penguin" + std::to_string(frameInt) + ".png";
                PlayerOrinetation = 1;
            }

            if(UpKey == true)
            {
                playerSprite = "Human/BackAnimation/penguin" + std::to_string(frameInt) + ".png";
                PlayerOrinetation = 2;
            }

            if(LeftKey == true)
            {
                playerSprite = "Human/LeftAnimation/penguin" + std::to_string(frameInt) + ".png";
                PlayerOrinetation = 3;
            }

            if(RightKey == true)
            {
                playerSprite = "Human/RightAnimation/penguin" + std::to_string(frameInt) + ".png";
                PlayerOrinetation = 4;
            }
        }	
    }else
    {
        frameInt = 1;

        if(PlayerOrinetation == 1)
        {
            playerSprite = "Human/FrontAnimation/penguin1.png";
        }else if(PlayerOrinetation == 2)
        {
            playerSprite = "Human/BackAnimation/penguin1.png";
        }else if(PlayerOrinetation == 3)
        {
            playerSprite = "Human/LeftAnimation/penguin1.png";
        }else if(PlayerOrinetation == 4)
        {
            playerSprite = "Human/RightAnimation/penguin1.png";
        }
        
        TheFrameTime = 1;
    }
}