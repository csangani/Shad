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
	void PlayGameplayMusic();
	void StopGameplayMusic();
	void PlayMenuToggle();
	void PlayMenuSelection();
	void PlayTeleportSound();
	void PlayJumpSound();
	void PlayLightningSound();
	void PlayDeathSound();
	void PlayLifeSound();
	void PlayEndLevel();

private:
	FMOD::System *system;
	FMOD::Sound *MainMenuMusic;
	FMOD::Sound *MenuToggle;
	FMOD::Sound *MenuSelection;
	FMOD::Sound *GameplayMusic;
	FMOD::Sound *Teleport;
	FMOD::Sound *Jump;
	FMOD::Sound *Lightning;
	FMOD::Sound *Death1;
	FMOD::Sound *Death2;
	FMOD::Sound *Life;
	FMOD::Sound *EndLevel;
	
	FMOD::Channel *MainMusicChannel;
	FMOD::Channel *GameplayMusicChannel;

	void InitSound();
};

#endif