#ifndef PhotoCarouselH
#define PhotoCarouselH

#include <filesystem>
#include <vector>
#include <SDL3/SDL.h>
#include "Animation.h"
#include "FadeTracker.h"
#include "TinyFS.h"

namespace fs = std::filesystem;

class PhotoCarousel: public ControlImpl
{
friend class PhotoCarouselBuilder;
private:
    Uint64 m_frameMSDuration;
    SRect m_targetRect;
    std::vector<shared_ptr<Animation>> m_photos;
    shared_ptr<FadeTracker> m_fadeTracker;
    int m_currentIdx;
    int m_nextIdx;  // The index of the photo that will be displayed next
public:
    PhotoCarousel(Control *parent, Uint64 frameMSDuration, SRect targetRect, float xScale=1.0f, float yScale=1.0f);
    ~PhotoCarousel();

    PhotoCarousel& addPhoto(fs::path filePath, bool matchParentRect=false);
    PhotoCarousel& addPhotosInPath(fs::path filePath, string extName, bool matchParentRect=false);
    shared_ptr<PhotoCarousel> build(void);

    vector<shared_ptr<Animation>> getPhotos(void);
    void setPhotos(vector<shared_ptr<Animation>> photos);
    shared_ptr<FadeTracker> getFadeTracker(void);
    void setFadeTracker(shared_ptr<FadeTracker> fadeTracker);

    void startFrom(int idx);
    void update(void);
    void draw(void);
    void next(void);
    void previous(void);
    void start(void);
    void stop(void);
    void show(void);
    void hide(void);
    // void setRect(SRect rect) override;
};
class PhotoCarouselBuilder{
private:
    shared_ptr<PhotoCarousel> m_photoCarousel;
public:
    PhotoCarouselBuilder(Control *parent, Uint64 frameMSDuration, SRect targetRect, float xScale=1.0f, float yScale=1.0f);
    PhotoCarouselBuilder& addPhoto(fs::path filePath, bool matchParentRect=false);
    PhotoCarouselBuilder& addPhotosInPath(fs::path filePath, string extName, bool matchParentRect=false);
    shared_ptr<PhotoCarousel> build(void);
};
#endif