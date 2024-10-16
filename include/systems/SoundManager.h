#pragma once

#include <string>
#include <map>
#include <SDL2/SDL_mixer.h>

/**
 * @brief Clase encargada de gestionar la reproducción de música y efectos de sonido en el juego.
 *
 * La clase `SoundManager` permite cargar, reproducir y detener música de fondo y efectos de sonido
 * utilizando la biblioteca SDL2_mixer. Los sonidos pueden ser asignados a canales para su control
 * y la música de fondo se puede reproducir en bucle o detenerse en cualquier momento.
 */
class SoundManager {
public:
    /**
     * @brief Constructor de la clase SoundManager.
     *
     * Inicializa el sistema de audio con los parámetros predeterminados de SDL_mixer.
     */
    SoundManager();

    /**
     * @brief Destructor de la clase SoundManager.
     *
     * Libera todos los recursos de audio cargados y cierra SDL_mixer.
     */
    ~SoundManager();

    /**
     * @brief Reproduce música de fondo desde un archivo.
     *
     * @param filename Ruta al archivo de música (por ejemplo, .mp3, .ogg).
     *
     * La música se reproducirá en bucle hasta que sea detenida.
     */
    void playBackgroundMusic(const std::string& filename);

    /**
     * @brief Reproduce un efecto de sonido.
     *
     * @param effectName Nombre del efecto de sonido que fue previamente cargado.
     * @param loops Número de veces que se repetirá el efecto de sonido. 0 indica una reproducción única.
     * @return El canal en el que se está reproduciendo el efecto de sonido, o -1 si no se pudo reproducir.
     *
     * Reproduce un efecto de sonido y devuelve el canal donde se reproduce para mayor control.
     */
    int playSoundEffect(const std::string &effectName, int loops = 0);

    /**
     * @brief Detiene la reproducción de un efecto de sonido.
     *
     * @param effectName Nombre del efecto de sonido que se desea detener.
     *
     * Detiene el efecto de sonido en el canal donde se estaba reproduciendo.
     */
    void stopSoundEffect(const std::string& effectName);

    /**
     * @brief Detiene la reproducción de la música de fondo.
     */
    void stopBackgroundMusic();

    /**
     * @brief Carga un efecto de sonido desde un archivo.
     *
     * @param effectName Nombre del efecto de sonido, que será utilizado para referenciarlo en otras funciones.
     * @param filename Ruta al archivo del efecto de sonido (por ejemplo, .wav).
     *
     * Carga un archivo de efecto de sonido en la memoria para que pueda ser reproducido posteriormente.
     */
    void loadSoundEffect(const std::string& effectName, const std::string& filename);

private:
    /// Mapa que almacena los efectos de sonido cargados, con una clave de nombre y el valor correspondiente a un puntero de `Mix_Chunk`.
    std::map<std::string, Mix_Chunk*> soundEffects;

    /// Mapa que almacena los canales donde se están reproduciendo los efectos de sonido, para permitir su control posterior.
    std::map<std::string, int> playingChannels;
};

