#include <Shad\Sound.h>
#include <stdio.h>
#include <iostream>



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

	result = system->init(200, FMOD_INIT_NORMAL, 0);

	// Create sounds
	ERRCHECK(Sound::system->createSound("assets/mp3/menu.mp3", FMOD_HARDWARE, 0, &MainMenuMusic));
	ERRCHECK(Sound::system->createSound("assets/mp3/menu_toggle.mp3", FMOD_HARDWARE, 0, &MenuToggle));
	ERRCHECK(Sound::system->createSound("assets/mp3/menu_selection.mp3", FMOD_HARDWARE, 0, &MenuSelection));
	ERRCHECK(Sound::system->createSound("assets/mp3/gameplay_long.mp3", FMOD_HARDWARE, 0, &GameplayMusic));
	ERRCHECK(Sound::system->createSound("assets/mp3/teleport.mp3", FMOD_HARDWARE, 0, &Teleport));
	ERRCHECK(Sound::system->createSound("assets/mp3/jump.mp3", FMOD_HARDWARE, 0, &Jump));
	ERRCHECK(Sound::system->createSound("assets/mp3/lightning.mp3", FMOD_HARDWARE, 0, &Lightning));
	ERRCHECK(Sound::system->createSound("assets/mp3/death1.mp3", FMOD_HARDWARE, 0, &Death1));
	ERRCHECK(Sound::system->createSound("assets/mp3/death2.mp3", FMOD_HARDWARE, 0, &Death2));
	ERRCHECK(Sound::system->createSound("assets/mp3/new_life.mp3", FMOD_HARDWARE, 0, &Life));
	ERRCHECK(Sound::system->createSound("assets/mp3/level_end.mp3", FMOD_HARDWARE, 0, &EndLevel));
	
	// Create channels
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, MainMenuMusic, true, &MainMusicChannel));
	ERRCHECK(MainMusicChannel->setMode(FMOD_LOOP_NORMAL));
	MainMusicChannel->setVolume(0.3);

	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, GameplayMusic, true, &GameplayMusicChannel));
	ERRCHECK(GameplayMusicChannel->setMode(FMOD_LOOP_NORMAL));
	GameplayMusicChannel->setVolume(0.3);

	ERRCHECK(result);
}

void Sound::Update()
{
	ERRCHECK(system->update());
}

void Sound::PlayMenuMusic() {	
	ERRCHECK(MainMusicChannel->setPaused(false));
}

void Sound::StopMenuMusic() {
	ERRCHECK(MainMusicChannel->setPaused(true));
}

void Sound::PlayGameplayMusic() {	
	ERRCHECK(GameplayMusicChannel->setPaused(false));
}

void Sound::StopGameplayMusic() {
	ERRCHECK(GameplayMusicChannel->setPaused(true));
}

void Sound::PlayMenuToggle() {
	FMOD::Channel *MenuToggleChannel;
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, MenuToggle, false, &MenuToggleChannel));
	MenuToggleChannel->setVolume(0.2);
}

void Sound::PlayMenuSelection() {
	FMOD::Channel *MenuSelectionChannel;
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, MenuSelection, false, &MenuSelectionChannel));
	MenuSelectionChannel->setVolume(0.5);
}

void Sound::PlayTeleportSound()
{
	FMOD::Channel *TeleportChannel;
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, Teleport, false, &TeleportChannel));
	TeleportChannel->setVolume(0.5);
}

void Sound::PlayJumpSound()
{
	FMOD::Channel *JumpChannel;
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, Jump, false, &JumpChannel));
	JumpChannel->setVolume(0.3);
}

void Sound::PlayLightningSound()
{
	FMOD::Channel *LightningChannel;
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, Lightning, false, &LightningChannel));
	LightningChannel->setVolume(0.3);
}

void Sound::PlayDeathSound()
{	
	FMOD::Channel *DeathChannel;
	float whichSoundToPlay = (float)rand()/(float)RAND_MAX;
	FMOD::Sound *deathSound = (whichSoundToPlay > 0.5) ? Death1 : Death2;
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, deathSound, false, &DeathChannel));
	DeathChannel->setVolume(0.9);
}

void Sound::PlayLifeSound()
{
	FMOD::Channel *LifeChannel;
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, Life, false, &LifeChannel));
	LifeChannel->setVolume(0.5);
}

void Sound::PlayEndLevel()
{
	FMOD::Channel *EndLevelChannel;
	ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, EndLevel, false, &EndLevelChannel));
	EndLevelChannel->setVolume(0.5);
}
