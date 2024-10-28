#include "systems/SoundManager.h"
#include <iostream>

SoundManager::SoundManager() {
    // Inicializar SDL Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "No se pudo inicializar SDL Mixer: " << Mix_GetError() << std::endl;
    }
}

SoundManager::~SoundManager() {
    // Liberar todos los Mix_Chunk cargados
    for (int i = 0; i < static_cast<int>(SoundEffectType::COUNT); ++i) {
        if (soundEffects[i]) {
            Mix_FreeChunk(soundEffects[i]);
            soundEffects[i] = nullptr;
        }
    }
    Mix_CloseAudio();
    Mix_Quit();
}

void SoundManager::loadSoundEffect(SoundEffectType effect, const std::string& filename) {
    int index = static_cast<int>(effect);
    if (index >= static_cast<int>(SoundEffectType::COUNT)) {
        std::cerr << "Tipo de efecto de sonido inválido: " << index << std::endl;
        return;
    }

    Mix_Chunk* effectChunk = Mix_LoadWAV(filename.c_str());
    if (effectChunk == nullptr) {
        std::cerr << "No se pudo cargar el efecto de sonido (" << filename << "): " << Mix_GetError() << std::endl;
        return;
    }

    soundEffects[index] = effectChunk;
}

void SoundManager::playBackgroundMusic(const std::string& filename) {
    Mix_Music* music = Mix_LoadMUS(filename.c_str());
    if (music == nullptr) {
        std::cerr << "No se pudo cargar la música de fondo (" << filename << "): " << Mix_GetError() << std::endl;
        return;
    }
    Mix_VolumeMusic(35); // Puedes ajustar este valor entre 0 y 128
    if (Mix_PlayMusic(music, -1) == -1) {
        std::cerr << "No se pudo reproducir la música de fondo: " << Mix_GetError() << std::endl;
    }
}

void SoundManager::stopBackgroundMusic() {
    Mix_HaltMusic();
}

int SoundManager::playSoundEffect(SoundEffectType effect, int loops) {
    int index = static_cast<int>(effect);
    if (index >= static_cast<int>(SoundEffectType::COUNT)) {
        std::cerr << "Tipo de efecto de sonido inválido: " << index << std::endl;
        return -1;
    }

    if (soundEffects[index] == nullptr) {
        std::cerr << "Efecto de sonido no cargado para: " << index << std::endl;
        return -1;
    }

    int channel = Mix_PlayChannel(-1, soundEffects[index], loops);
    if (channel == -1) {
        std::cerr << "No hay canales disponibles para reproducir el efecto: " << index << std::endl;
        return -1;
    }

    playingChannels[index] = channel;
    return channel;
}

void SoundManager::stopSoundEffect(SoundEffectType effect) {
    int index = static_cast<int>(effect);
    if (index >= static_cast<int>(SoundEffectType::COUNT)) {
        std::cerr << "Tipo de efecto de sonido inválido: " << index << std::endl;
        return;
    }

    if (playingChannels[index] != -1) {
        Mix_HaltChannel(playingChannels[index]);
        playingChannels[index] = -1;
    }
}