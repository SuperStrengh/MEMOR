#include <iostream>
#include "Audio.hpp"

std::map<std::string, Mix_Music*> mMusic;
std::map<std::string, Mix_Chunk*> mSFX;

void InitAudio()
{
    SDL_Init(SDL_INIT_AUDIO);

    if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
    {
        printf("Mix Init Error", Mix_GetError());
    }
}

void EndAudio()
{
    Mix_Quit();
}

Mix_Music* GetMusic(std::string filename)
{
    std::string fullPath = SDL_GetBasePath();
    fullPath.append(filename);

    if(mMusic[fullPath] != nullptr)
    {

    }else
    {
        mMusic[fullPath] = Mix_LoadMUS(fullPath.c_str());

        if(mMusic[fullPath] == NULL)
            printf("Loading Music Error", filename.c_str(), Mix_GetError());
    }

    return mMusic[fullPath];
}

Mix_Chunk* GetSFX(std::string filename)
{
    std::string fullPath = SDL_GetBasePath();
    fullPath.append(filename);

    if(mSFX[fullPath] != nullptr)
    {

    }else
    {
        mSFX[fullPath] = Mix_LoadWAV(fullPath.c_str());

        if(mSFX[fullPath] == NULL)
            printf("Loading SFX Error", filename.c_str(), Mix_GetError());
    }

    return mSFX[fullPath];
}

void PlayMusic(std::string filename,int loops)
{
    Mix_PlayMusic(GetMusic(filename), loops);
}

void PauseMusic()
{
    Mix_PauseMusic();
}

void ResumeMusic()
{
    Mix_ResumeMusic();
}

void StopSFX(int channel)
{
    Mix_HaltChannel(channel);
}

void StopMusic()
{
    Mix_HaltMusic();
}

void PlaySFX(std::string filename, int loops, int channel)
{
    Mix_PlayChannel(channel, GetSFX(filename), loops);
}