#include <functional>   // 使用 std::bind()、std::function<> 需要包含该文件
#include "AudioSuite.h"

AudioSuite::AudioSuite(Control *parent, float xScale, float yScale) :
    ControlImpl(parent, xScale, yScale),
    m_isLoaded(false),
    m_isPlaying(false),
    m_audioStream(nullptr),
    m_nextPlayPos(0),
    m_audioLength(0),
    m_audioBuffer(nullptr)
{
}

AudioSuite::AudioSuite(Control *parent, fs::path wavFilePath, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_isLoaded(false),
    m_isPlaying(false),
    m_audioStream(nullptr),
    m_nextPlayPos(0),
    m_audioLength(0),
    m_audioBuffer(nullptr)
{
    setFile(wavFilePath);
}
AudioSuite::~AudioSuite() {
    if (m_isPlaying) {
        SDL_PauseAudioStreamDevice(m_audioStream);
        m_isPlaying = false;
    }

    if (m_isLoaded) {
        if (m_audioStream) {
            SDL_DestroyAudioStream(m_audioStream);
            m_audioStream = nullptr;
        }
        if (m_audioBuffer) {
            SDL_free(m_audioBuffer);
            m_audioBuffer = nullptr;
        }
        m_isLoaded = false;
    }
}

void AudioSuite::play(bool isLoop) {
    if (m_isPlaying) {
        return;
    }

    if (m_isLoaded) {
        m_nextPlayPos = 0;
        /* Create our audio stream in the same format as the .wav file. It'll convert to what the audio hardware wants. */
        if (!m_audioStream) {
            m_audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_audioSpec, &AudioSuite::sharedFeedTheAudioStream, this);
            if (!m_audioStream) {
                SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
                return;
            }
        }
        if (!m_isPlaying) {
            /* SDL_OpenAudioDeviceStream starts the device paused. You have to tell it to start! */
            m_isPlaying = SDL_ResumeAudioStreamDevice(m_audioStream);
            if (!m_isPlaying) {
                SDL_Log("Couldn't start audio stream: %s", SDL_GetError());
                return;
            }
        }
    }
    m_isLooping = isLoop;
}
void AudioSuite::stop(void) {
    if (m_isPlaying) {
        if(!SDL_PauseAudioStreamDevice(m_audioStream)) {
            SDL_Log("Couldn't pause audio stream: %s", SDL_GetError());
            return;
        }
        m_isPlaying = false;
    }
}
void AudioSuite::pause(void) {
    if (m_isPlaying) {
        if(!SDL_PauseAudioStreamDevice(m_audioStream)) {
            SDL_Log("Couldn't pause audio stream: %s", SDL_GetError());
            return;
        }
    }
}
void AudioSuite::resume(void) {
    if (m_isPlaying) {
        if(!SDL_ResumeAudioStreamDevice(m_audioStream)) {
            SDL_Log("Couldn't resume audio stream: %s", SDL_GetError());
            return;
        }
    }
}
void AudioSuite::setFile(fs::path wavFilePath) {
    if (m_isLoaded) {
        if (m_audioBuffer) {
            SDL_free(m_audioBuffer);
            m_audioBuffer = nullptr;
        }
        m_isLoaded = false;
    }
    m_isLoaded = SDL_LoadWAV(wavFilePath.string().c_str(), &m_audioSpec, &m_audioBuffer, &m_audioLength);
    if (!m_isLoaded)
    {
        SDL_Log("Couldn't load %s: %s", wavFilePath.string().c_str(), SDL_GetError());
        return;
    }
    SDL_Log("Loaded wave file: %s, audio buffer address = %0x", wavFilePath.string().c_str(), m_audioBuffer);
    SDL_Log("audio length = %d", m_audioLength);
}

void SDLCALL AudioSuite::sharedFeedTheAudioStream(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount) {
    /*
    total_amount:       how much data the audio stream is eating right now
    additional_amount:  how much more it needs than what it currently has queued (which might be zero!). You can supply any amount of data here;
                        it will take what it needs and use the extra later.
                        If you don't give it enough, it will take everything and then feed silence to the hardware for the rest.
                        Ideally, though, we always give it what it needs and no extra, so we aren't buffering more than necessary.
    */
   AudioSuite *audioSuiteInst = (AudioSuite *)userdata;
    if (audioSuiteInst->m_nextPlayPos >= audioSuiteInst->m_audioLength) {
        if (!audioSuiteInst->m_isLooping) {
            /* we've reached the end of the audio data. Stop feeding the stream. */
            audioSuiteInst->m_isPlaying = !SDL_PauseAudioStreamDevice(astream);

            // GameEvent *event = new GameEvent(EventName::AudioEnded, audioSuiteInst);
            audioSuiteInst->triggerEvent({EventName::AudioEnded, audioSuiteInst});
            return;
        } else {
            audioSuiteInst->m_nextPlayPos = 0;
        }
    }
    if (audioSuiteInst->m_nextPlayPos + additional_amount >= audioSuiteInst->m_audioLength) {
        /* we've reached the end of the audio data. Stop feeding the stream. */
        SDL_PutAudioStreamData(astream, audioSuiteInst->m_audioBuffer + audioSuiteInst->m_nextPlayPos, audioSuiteInst->m_audioLength - audioSuiteInst->m_nextPlayPos);
        if (audioSuiteInst->m_isLooping) {
            audioSuiteInst->m_nextPlayPos = 0;
        } else {
            audioSuiteInst->m_nextPlayPos = audioSuiteInst->m_audioLength;
        }
    } else {
        /* we still have more audio data to feed. */
        SDL_PutAudioStreamData(astream, audioSuiteInst->m_audioBuffer + audioSuiteInst->m_nextPlayPos, additional_amount);
        audioSuiteInst->m_nextPlayPos += additional_amount;
    }
}

// void AudioSuite::SDLCALL FeedTheAudioStreamMore(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount) {
//     /*
//     total_amount:       how much data the audio stream is eating right now
//     additional_amount:  how much more it needs than what it currently has queued (which might be zero!). You can supply any amount of data here;
//                         it will take what it needs and use the extra later.
//                         If you don't give it enough, it will take everything and then feed silence to the hardware for the rest.
//                         Ideally, though, we always give it what it needs and no extra, so we aren't buffering more than necessary.
//     */
//     AudioSuite *audioSuiteInst = (AudioSuite *)userdata;
//     if (audioSuiteInst->m_nextPlayPos >= audioSuiteInst->m_audioLength) {
//         audioSuiteInst->m_isPlaying = SDL_PauseAudioStreamDevice(astream);
//     }
//     if (audioSuiteInst->m_nextPlayPos + additional_amount >= audioSuiteInst->m_audioLength) {
//         /* we've reached the end of the audio data. Stop feeding the stream. */
//         SDL_PutAudioStreamData(astream, audioSuiteInst->m_audioBuffer + audioSuiteInst->m_nextPlayPos, audioSuiteInst->m_audioLength - audioSuiteInst->m_nextPlayPos);
//         audioSuiteInst->m_nextPlayPos = audioSuiteInst->m_audioLength;
//     } else {
//         /* we still have more audio data to feed. */
//         SDL_PutAudioStreamData(astream, audioSuiteInst->m_audioBuffer + audioSuiteInst->m_nextPlayPos, additional_amount);
//         audioSuiteInst->m_nextPlayPos += additional_amount;
//     }
// }