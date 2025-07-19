#ifndef PanelH
#define PanelH

#include <functional>
#include <vector>
#include <unordered_map>
#include "Control.h"
#include "Animation.h"
// #include "FontSuite.h"
#include "PhotoCarousel.h"

using namespace std;

class Panel: public ControlImpl {
    friend class PanelBuilder;
    using OnClickHandler = std::function<void (shared_ptr<Control>)>;
private:
    SDL_Color m_bgColor;
    SDL_Color m_borderColor;
    bool m_isTransparent;
    bool m_isBorderVisible;


    shared_ptr<PhotoCarousel> m_background;

    unordered_map<shared_ptr<Animation>, SPoint> m_animations;
    unordered_map<shared_ptr<Actor>, SPoint> m_actors;
public:
    Panel(Control *parent, SRect rect, float xScale=1.0f, float yScale=1.0f);
    void nextBackground(void);
    void resetBackground(void);
    void update(void) override;
    void draw(void) override;
    bool handleEvent(shared_ptr<Event> event) override;

    Panel& setBackground(shared_ptr<PhotoCarousel> background, int startFrom=0);
    Panel& addAnimation(float x, float y, shared_ptr<Animation> animation);
    Panel& addActor(float x, float y, shared_ptr<Actor> actor);
    Panel& setBGColor(SDL_Color color);
    Panel& setBorderColor(SDL_Color color);
    Panel& setTransparent(bool isTransparent);
    Panel& addControl(shared_ptr<Control> control) override;;
    shared_ptr<Panel> build(void);
};
class PanelBuilder {
private:
    shared_ptr<Panel> m_panel;
public:
    PanelBuilder(Control *parent, SRect rect, float xScale=1.0f, float yScale=1.0f);
    PanelBuilder& setBackground(shared_ptr<PhotoCarousel> background, int startFrom=0);
    PanelBuilder& addAnimation(float x, float y, shared_ptr<Animation> animation);
    PanelBuilder& addActor(float x, float y, shared_ptr<Actor> actor);
    PanelBuilder& setBGColor(SDL_Color color);
    PanelBuilder& setBorderColor(SDL_Color color);
    PanelBuilder& setTransparent(bool isTransparent);
    PanelBuilder& setBorderVisible(bool isBorderVisible);
    PanelBuilder& addControl(shared_ptr<Control> control);
    shared_ptr<Panel> build(void);
};
#endif