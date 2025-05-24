#ifndef AudioSuiteH
#define AudioSuiteH

#include <SDL3/SDL.h>
// #include "DebugTrace.h"
#include <filesystem>
#include "EventQueue.h"
#include "Control.h"

namespace fs = std::filesystem;

class AudioSuite: ControlImpl
{
public:
    bool m_isLoaded;
    bool m_isPlaying;
    bool m_isLooping;

    SDL_AudioSpec m_audioSpec;
    Uint8 *m_audioBuffer;
    Uint32 m_audioLength;
    Uint32 m_nextPlayPos;

    SDL_AudioStream *m_audioStream;

public:
    AudioSuite(Control *parent, float xScale=1.0f, float yScale=1.0f);
    AudioSuite(Control *parent, fs::path wavFilePath, float xScale=1.0f, float yScale=1.0f);
    ~AudioSuite();
    void play(bool isLoop=false);
    void stop(void);
    void pause(void);
    void resume(void);

    void setFile(fs::path wavFilePath);
    // typedef void (SDLCALL *SDL_AudioStreamCallback)(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount);
    static void (SDLCALL sharedFeedTheAudioStream)(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);
    //void (SDLCALL FeedTheAudioStreamMore)(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);
};

#endif