#include <Shad\Sound.h>
#include <stdio.h>

FMOD::Channel *MainMusicChannel;
FMOD::Channel *MenuSoundEffectChannel;

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
}

Sound::Sound() {
	InitSound();
}

void Sound::InitSound() {
	FMOD_RESULT      result;
	unsigned int     version;

	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->getVersion(&version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
	}

	result = system->init(32, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);
}

void Sound::Update()
{
	ERRCHECK(system->update());
}

void Sound::PlayMenuMusic() {
	bool isPlaying;
	ERRCHECK(MainMusicChannel->isPlaying(&isPlaying));
	if (!isPlaying) {
		ERRCHECK(Sound::system->createSound("assets/mp3/menu.mp3", FMOD_HARDWARE, 0, &MainMenuMusic));
		ERRCHECK(MainMusicChannel->setMode(FMOD_LOOP_NORMAL));
		ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, MainMenuMusic, false, &MainMusicChannel));
		MainMusicChannel->setVolume(0.3);
	}
}

void Sound::StopMenuMusic() {
	bool isPlaying;
	ERRCHECK(MainMusicChannel->isPlaying(&isPlaying));
	if (isPlaying) {
		ERRCHECK(MainMusicChannel->stop());
	}
}

void Sound::PlayMenuToggle() {
	ERRCHECK(Sound::system->createSound("assets/mp3/menu_toggle.mp3", FMOD_HARDWARE, 0, &MenuSoundEffect));
	ERRCHECK(MainMenuMusic->setMode(FMOD_LOOP_NORMAL));
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, MenuSoundEffect, false, &MenuSoundEffectChannel));
	MenuSoundEffectChannel->setVolume(0.2);
}

void Sound::PlayMenuSelection() {
	ERRCHECK(Sound::system->createSound("assets/mp3/menu_selection.mp3", FMOD_HARDWARE, 0, &MenuSoundEffect));
	ERRCHECK(MainMenuMusic->setMode(FMOD_LOOP_NORMAL));
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, MenuSoundEffect, false, &MenuSoundEffectChannel));
	MenuSoundEffectChannel->setVolume(0.5);
}