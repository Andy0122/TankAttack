#include "systems/SoundManager.h"
#include <iostream>
#include <View.h>

SoundManager::SoundManager() {
    // Inicializar SDL Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "No se pudo inicializar SDL Mixer: " << Mix_GetError() << std::endl;
    }
}

SoundManager::~SoundManager() {
    for (auto& pair : soundEffects) {
        Mix_FreeChunk(pair.second);
    }
    soundEffects.clear();
    Mix_CloseAudio();
    Mix_Quit();
}

void SoundManager::playBackgroundMusic(const std::string& filename) {
    Mix_Music* music = Mix_LoadMUS(filename.c_str());
    if (music == nullptr) {
        std::cerr << "No se pudo cargar la música de fondo: " << Mix_GetError() << std::endl;
        return;
    }
    Mix_VolumeMusic(64); // Puedes ajustar este valor entre 0 y 128
    Mix_PlayMusic(music, -1);
}

void SoundManager::stopBackgroundMusic() {
    Mix_HaltMusic();
}

void SoundManager::loadSoundEffect(const std::string& effectName, const std::string& filename) {
    Mix_Chunk* effect = Mix_LoadWAV(filename.c_str());
    if (effect == nullptr) {
        std::cerr << "No se pudo cargar el efecto de sonido: " << Mix_GetError() << std::endl;
        return;
    }
    soundEffects[effectName] = effect;
}

int SoundManager::playSoundEffect(const std::string &effectName, int loops) {
    auto it = soundEffects.find(effectName);
    if (it != soundEffects.end()) {
        int channel = Mix_PlayChannel(-1, it->second, loops);
        if (channel == -1) {
            std::cerr << "No hay canales disponibles para reproducir el efecto: " << effectName << std::endl;
            return -1;
        } else {
            playingChannels[effectName] = channel;
            return channel;
        }
    } else {
        std::cerr << "Efecto de sonido no encontrado: " << effectName << std::endl;
        return -1;
    }
}

void SoundManager::stopSoundEffect(const std::string& effectName) {
    auto it = playingChannels.find(effectName);
    if (it != playingChannels.end()) {
        Mix_HaltChannel(it->second);
        playingChannels.erase(it);
    }
}
