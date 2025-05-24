#include "PhotoCarousel.h"

PhotoCarousel::PhotoCarousel(Control *parent, Uint64 frameMSDuration, SRect targetRect, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_targetRect(targetRect),
    m_fadeTracker(nullptr),
    m_frameMSDuration(frameMSDuration),
    m_currentIdx(0),
    m_nextIdx(INT_MAX)
{
    setRect(targetRect);
    m_fadeTracker = make_shared<FadeTracker>(nullptr, 1, FadeOperation::FadeIn);
}

PhotoCarousel::~PhotoCarousel() {
    m_photos.clear();
}

void PhotoCarousel::startFrom(int idx){
    if (idx >= m_photos.size()){
        SDL_Log("PhotoCarousel::startFrom: idx %d out of range(size=%zu)", idx, m_photos.size());
        return;
    }
    m_photos[m_currentIdx]->hide();
    m_photos[m_currentIdx]->pause();

    m_fadeTracker->setFadeOperation(FadeOperation::FadeIn);

    m_currentIdx = idx;
    m_photos[m_currentIdx]->show();
    m_photos[m_currentIdx]->resume();
}

PhotoCarousel& PhotoCarousel::addPhoto(fs::path filePath, bool matchParentRect){
    m_photos.push_back(Animation(this, m_frameMSDuration)
                        .addFrame(ActorGroup(this)
                                        .addActor({0, 0}, make_shared<Actor>(this, filePath, matchParentRect))
                                        .build())
                        .setTracker(m_fadeTracker)
                        .setFrameMSDuration(m_frameMSDuration)
                        .setInitialPos(0, 0)
                        .build());
    return *this;
}
PhotoCarousel& PhotoCarousel::addPhotosInPath(fs::path filePath, string extName, bool matchParentRect){
    unique_ptr<TinyFS> fsystem = make_unique<TinyFS>();
    auto flist = fsystem->getFileList(filePath, extName);
    for (auto &f : flist) {
        SDL_Log("addPhotosInPath: %s", f.string().c_str());
        addPhoto(f, matchParentRect);
    }
    return *this;
}

vector<shared_ptr<Animation>> PhotoCarousel::getPhotos(void) {
    return m_photos;
}
void PhotoCarousel::setPhotos(vector<shared_ptr<Animation>> photos) {
    // 将photos复制过来时，需要把中的所有元素中的parent都改为当前的PhotoCarousel，否则原来
    // photos归属的PhotoCarousel在销毁后，photos中根据parent映射drawRect会失败
    m_photos = photos;
    for (auto animation : m_photos){
        animation->setParent(this);
        for (auto frame: animation->getFrames()){
            frame->setParent(this);
            for(auto actor: frame->getActors()){
                actor->setParent(this);
            }
        }
    }
}
shared_ptr<FadeTracker> PhotoCarousel::getFadeTracker(void) {
    return m_fadeTracker;
}
void PhotoCarousel::setFadeTracker(shared_ptr<FadeTracker> fadeTracker) {
    m_fadeTracker = fadeTracker;
    for (auto animation : m_photos){
        animation->setTracker(m_fadeTracker);
    }
}


shared_ptr<PhotoCarousel> PhotoCarousel::build(void){
    auto newPhotoCarouse = make_shared<PhotoCarousel>(this->getParent(), m_frameMSDuration, m_rect, m_xScale, m_yScale);
    newPhotoCarouse->setPhotos(getPhotos());
    newPhotoCarouse->setFadeTracker(getFadeTracker());
    return newPhotoCarouse;
}

// void PhotoCarousel::setRect(SRect rect) {
//     for(auto animation : m_photos){
//         animation->setRect(rect);
//     }
// }

void PhotoCarousel::start(void){
    if (m_photos.empty()) return;

    m_photos[m_currentIdx]->resume();
}

void PhotoCarousel::stop(void){
    if (m_photos.empty()) return;

    m_photos[m_currentIdx]->pause();
}
void PhotoCarousel::show(void){
    if (m_photos.empty()) return;

    m_photos[m_currentIdx]->show();
}
void PhotoCarousel::hide(void){
    if (m_photos.empty()) return;

    m_photos[m_currentIdx]->hide();
}

void PhotoCarousel::update(void){
    if (m_photos.empty()) return;

    m_photos[m_currentIdx]->update();
    if (m_fadeTracker->isEnded()){
        if (m_nextIdx != INT_MAX){
            m_photos[m_currentIdx]->pause();
            m_photos[m_currentIdx]->hide();
            m_currentIdx = m_nextIdx;
            m_nextIdx = INT_MAX;
            m_photos[m_currentIdx]->resume();
            m_photos[m_currentIdx]->show();

            m_fadeTracker->setFadeOperation(FadeOperation::FadeIn);
        }
    }
}

void PhotoCarousel::draw(void){
    if (m_photos.empty()) return;

    inheritRenderer();
    m_photos[m_currentIdx]->draw();
}

void PhotoCarousel::next(void){
    m_fadeTracker->setFadeOperation(FadeOperation::FadeOut);
    m_nextIdx = (m_currentIdx + 1) % m_photos.size();
}

void PhotoCarousel::previous(void){
    m_fadeTracker->setFadeOperation(FadeOperation::FadeOut);
    m_nextIdx = (m_currentIdx - 1 + m_photos.size()) % m_photos.size();
}


PhotoCarouselBuilder::PhotoCarouselBuilder(Control *parent, Uint64 frameMSDuration, SRect targetRect, float xScale, float yScale):
    m_photoCarousel(nullptr)
{
    m_photoCarousel = make_shared<PhotoCarousel>(parent, frameMSDuration, targetRect, xScale, yScale);
}
PhotoCarouselBuilder& PhotoCarouselBuilder::addPhoto(fs::path filePath, bool matchParentRect){
    m_photoCarousel->addPhoto(filePath, matchParentRect);
    return *this;
}
PhotoCarouselBuilder& PhotoCarouselBuilder::addPhotosInPath(fs::path filePath, string extName, bool matchParentRect){
    m_photoCarousel->addPhotosInPath(filePath, extName, matchParentRect);
    return *this;
}
shared_ptr<PhotoCarousel> PhotoCarouselBuilder::build(void){
    return m_photoCarousel;
}
