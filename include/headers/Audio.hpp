#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>

void InitAudio();
void EndAudio();

Mix_Music* GetMusic(std::string filename);
Mix_Chunk* GetSFX(std::string filename);

void PlayMusic(std::string filename, int loops = -1);
void PauseMusic();
void StopMusic();
void ResumeMusic();

void PlaySFX(std::string filename, int loops = 0, int channel = -1);
void StopSFX(int channel = -1);