#ifndef ButtonH
#define ButtonH
#include <functional>
#include "Control.h"
#include "Actor.h"
#include "Animation.h"
#include "Label.h"
#include "LuotiAni.h"

enum class ButtonState {
    Normal,
    Hover,
    Pressed
};

class Button: public ControlImpl {
    friend class ButtonBuilder;
    using OnClickHandler = std::function<void (shared_ptr<Button>)>;
private:
    shared_ptr<Actor> m_actor;
    shared_ptr<Actor> m_hoverActor;
    shared_ptr<Actor> m_pressedActor;
    shared_ptr<Label> m_caption;
    shared_ptr<Animation> m_animation;
    shared_ptr<LuotiAni>m_luotiAni;

    string m_captionText;
    float m_captionSize;

    OnClickHandler m_onClick;
    ButtonState m_state;
    bool m_isTransparent;
public:
    Button(Control *parent, SRect rect, float xScale=1.0f, float yScale=1.0f);
    void update(void) override;
    void draw(void) override;
    bool handleEvent(shared_ptr<Event> event) override;

    void setBtnNormalStateActor(shared_ptr<Actor> actor);
    void setBtnHoverStateActor(shared_ptr<Actor> actor);
    void setBtnPressedStateActor(shared_ptr<Actor> actor);
    void setCaption(string caption);
    void setCaptionSize(float size);
    void setAnimation(shared_ptr<Animation> animation);
    void setLuotiAni(shared_ptr<LuotiAni>luotiAni);
    void setOnClick(OnClickHandler onClick);
    void setTransparent(bool isTransparent);
};

class ButtonBuilder {
private:
    shared_ptr<Button> m_button;
public:
    ButtonBuilder(Control *parent, SRect rect, float xScale=1.0f, float yScale=1.0f);
    ButtonBuilder& setBtnNormalStateActor(shared_ptr<Actor> actor);
    ButtonBuilder& setBtnHoverStateActor(shared_ptr<Actor> actor);
    ButtonBuilder& setBtnPressedStateActor(shared_ptr<Actor> actor);
    ButtonBuilder& setCaption(string caption);
    ButtonBuilder& setCaptionSize(float size);
    ButtonBuilder& setAnimation(shared_ptr<Animation> animation);
    ButtonBuilder& setLuotiAni(shared_ptr<LuotiAni> luotiAni);
    ButtonBuilder& addControl(shared_ptr<Control> child);
    ButtonBuilder& setOnClick(Button::OnClickHandler onClick);
    ButtonBuilder& setTransparent(bool isTransparent);
    ButtonBuilder& setId(int id);
    shared_ptr<Button> build(void);
};
#endif