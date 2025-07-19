#include "MusicCarousel.h"

MusicCarousel::MusicCarousel(Control* parent) :
    ControlImpl(parent),
    m_currentMusicIndex(0),
    m_isPlaying(false),
    m_nextPlayPos(0),
    m_silenceBuffer(nullptr)
{
    m_silenceBuffer = (Uint8 *)SDL_malloc(1024);    // allocate a 1K buffer of silence
    if (!m_silenceBuffer) {
        SDL_Log("Failed to allocate silence buffer, error: %s", SDL_GetError());
        // throw std::runtime_error("Failed to allocate silence buffer, error: " + string(SDL_GetError()));
        return;
    }
    SDL_memset(m_silenceBuffer, 0, 1024);            // fill it with silence
}

MusicCarousel::~MusicCarousel(){
    stop();
    SDL_free(m_silenceBuffer);
}

bool MusicCarousel::handleEvent(shared_ptr<Event> event){
    if (event->m_eventName == EventName::NextMusic && event->m_eventParam.type() == typeid(MusicCarousel *)){
        if(any_cast<MusicCarousel *>(event->m_eventParam ) == this){
            return true;
        }
    }
    return false;
}

void SDLCALL MusicCarousel::musicFeedTheAudioStream(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount) {
    /*
    total_amount:       how much data the audio stream is eating right now
    additional_amount:  how much more it needs than what it currently has queued (which might be zero!). You can supply any amount of data here;
                        it will take what it needs and use the extra later.
                        If you don't give it enough, it will take everything and then feed silence to the hardware for the rest.
                        Ideally, though, we always give it what it needs and no extra, so we aren't buffering more than necessary.
    */
    MusicCarousel *musicCarouselInst = (MusicCarousel *)userdata;
    if (musicCarouselInst->m_nextPlayPos >= musicCarouselInst->m_musics[musicCarouselInst->m_currentMusicIndex]->m_audioLength) {

        /* we've reached the end of the audio data. Stop feeding the stream. */
        SDL_Log("Reached end of music data, stop feeding the stream, trigger NextMusic event.");
        musicCarouselInst->pause();
        musicCarouselInst->m_currentMusicIndex = (musicCarouselInst->m_currentMusicIndex + 1) % musicCarouselInst->m_musics.size();
        musicCarouselInst->m_nextPlayPos = 0;
        musicCarouselInst->play();
        return;
    }

    Uint32 feedDataLength = 0;
    if (musicCarouselInst->m_nextPlayPos + additional_amount >= musicCarouselInst->m_musics[musicCarouselInst->m_currentMusicIndex]->m_audioLength) {
        /* we've reached the end of the audio data. Stop feeding the stream. */
        feedDataLength = musicCarouselInst->m_musics[musicCarouselInst->m_currentMusicIndex]->m_audioLength - musicCarouselInst->m_nextPlayPos;

    } else {
        /* we still have more audio data to feed. */
        feedDataLength = additional_amount;
    }
    SDL_PutAudioStreamData(astream,
                            musicCarouselInst->m_musics[musicCarouselInst->m_currentMusicIndex]->m_audioBuffer + musicCarouselInst->m_nextPlayPos,
                            feedDataLength);
    musicCarouselInst->m_nextPlayPos += feedDataLength;
}

void MusicCarousel::addMusicResource(string resourceId){
    m_musics.push_back(make_shared<MusicData>(resourceId));
}

void MusicCarousel::play(void) {
    if (m_isPlaying) {
        return;
    }

    if (m_musics.size() == 0) {
        SDL_Log("No music files to play");
        return;
    }

    if (m_musics[m_currentMusicIndex]->m_audioStream == nullptr){
        // 未加载过该音乐文件
        shared_ptr<Resource> resource = ResourceLoader::getInstance()->getResource(m_musics[m_currentMusicIndex]->m_musicResourceId);
        if (resource == nullptr || resource->resourceType != ResourceLoader::RT_MUSIC
            || resource->pMem == nullptr) {

            SDL_Log("LoadFromResource Error: '%s' is not a music\n", m_musics[m_currentMusicIndex]->m_musicResourceId.c_str());
            return;
        }

        SDL_IOStream *resourceStream = SDL_IOFromConstMem(resource->pMem.get(), resource->resourceSize);
        if( resourceStream == nullptr){
            SDL_Log("Create IOStream Error: %s\n", SDL_GetError());
            return;
        }

        if (!SDL_LoadWAV_IO(resourceStream, true,
                        &m_musics[m_currentMusicIndex]->m_audioSpec,
                        &m_musics[m_currentMusicIndex]->m_audioBuffer,
                        &m_musics[m_currentMusicIndex]->m_audioLength))
        {
            SDL_Log("Couldn't load %s: %s", m_musics[m_currentMusicIndex]->m_musicResourceId.c_str(), SDL_GetError());
            return;
        }
        SDL_Log("Loaded wave resource: %s, music buffer address = %0x", m_musics[m_currentMusicIndex]->m_musicResourceId.c_str(), m_musics[m_currentMusicIndex]->m_audioBuffer);
        SDL_Log("audio length = %d", m_musics[m_currentMusicIndex]->m_audioLength);
    }

    if (m_musics[m_currentMusicIndex]->m_audioBuffer) {
        m_nextPlayPos = 0;
        /* Create our audio stream in the same format as the .wav file. It'll convert to what the audio hardware wants. */
        if (!m_musics[m_currentMusicIndex]->m_audioStream) {
            m_musics[m_currentMusicIndex]->m_audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                                                                                        &m_musics[m_currentMusicIndex]->m_audioSpec,
                                                                                        &MusicCarousel::musicFeedTheAudioStream,
                                                                                        this);
            if (!m_musics[m_currentMusicIndex]->m_audioStream) {
                SDL_Log("Couldn't create music stream: %s", SDL_GetError());
                return;
            }
        }
        /* SDL_OpenAudioDeviceStream starts the device paused. You have to tell it to start! */
        m_isPlaying = SDL_ResumeAudioStreamDevice(m_musics[m_currentMusicIndex]->m_audioStream);
        if (!m_isPlaying) {
            SDL_Log("Couldn't start music stream: %s", SDL_GetError());
            return;
        }
    }
}

void MusicCarousel::stop(void){
    if (m_isPlaying) {
        SDL_PauseAudioStreamDevice(m_musics[m_currentMusicIndex]->m_audioStream);
        m_isPlaying = false;
    }
    if (m_musics[m_currentMusicIndex]->m_audioStream) {
        SDL_DestroyAudioStream(m_musics[m_currentMusicIndex]->m_audioStream);
        m_musics[m_currentMusicIndex]->m_audioStream = nullptr;
    }
    if (m_musics[m_currentMusicIndex]->m_audioBuffer) {
        SDL_free(m_musics[m_currentMusicIndex]->m_audioBuffer);
        m_musics[m_currentMusicIndex]->m_audioBuffer = nullptr;
    }
}

void MusicCarousel::pause(void) {
    if (m_isPlaying) {
        if(!SDL_PauseAudioStreamDevice(m_musics[m_currentMusicIndex]->m_audioStream)) {
            SDL_Log("Couldn't pause music stream: %s", SDL_GetError());
            return;
        }
        m_isPlaying = false;
    }
}
void MusicCarousel::resume(void) {
    if (!m_isPlaying) {
        if(!SDL_ResumeAudioStreamDevice(m_musics[m_currentMusicIndex]->m_audioStream)) {
            SDL_Log("Couldn't resume music stream: %s", SDL_GetError());
            return;
        }
        m_isPlaying = true;
    }
}

void MusicCarousel::restart(void){
    pause();
    m_currentMusicIndex = 0;
    m_nextPlayPos = 0;
    play();
}
