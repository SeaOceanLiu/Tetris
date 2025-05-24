#ifndef MusicCarouselH
#define MusicCarouselH
#include <filesystem>
#include <SDL3/SDL.h>

#include "Control.h"

namespace fs = std::filesystem;

class MusicData{
public:
    fs::path m_musicFile;
    SDL_AudioStream *m_audioStream;
    SDL_AudioSpec m_audioSpec;
    Uint8 *m_audioBuffer;
    Uint32 m_audioLength;

    MusicData(fs::path path) :
        m_musicFile(path),
        m_audioStream(nullptr),
        m_audioBuffer(nullptr),
        m_audioLength(0)
    {
    }
    ~MusicData(){
        if(m_audioStream) {
            SDL_DestroyAudioStream(m_audioStream);
            m_audioStream = nullptr;
        }
        if(m_audioBuffer) {
            SDL_free(m_audioBuffer);
            m_audioBuffer = nullptr;
        }
    }
};

class MusicCarousel: public ControlImpl
{
private:
    vector<shared_ptr<MusicData>>m_musics;

    int m_currentMusicIndex;
    bool m_isPlaying;

    Uint32 m_nextPlayPos;

    Uint8 *m_silenceBuffer;

public:
    MusicCarousel(Control *parent);
    ~MusicCarousel();
    bool handleEvent(GameEvent &event) override;
    static void (SDLCALL musicFeedTheAudioStream)(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);

    void addMusicFile(fs::path path);

    void play(void);
    void stop(void);
    void pause(void);
    void resume(void);
    void restart(void);
};
#endif // MusicCarousel.h