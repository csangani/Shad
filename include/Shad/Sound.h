#ifndef SOUND_H
#define SOUND_H

#include <windows.h>
#include <FMODex/fmod.hpp>
#include <FMODex/fmod_errors.h>

class Sound {
public:
	Sound();
	void Update();

	void PlayMenuMusic();
	void StopMenuMusic();
	void PlayMenuToggle();
	void PlayMenuSelection();

private:
	FMOD::System *system;
	FMOD::Sound *MainMenuMusic;
	FMOD::Sound *MenuSoundEffect;

	void InitSound();
};

#endif