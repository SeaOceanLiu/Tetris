#ifndef ScreenH
#define ScreenH

#include "Panel.h"
#include "Arena.h"
#include "Label.h"
#include "Dialog.h"

class Screen: public Panel, public TopControl
{
private:
    SRect m_defaultArenaRect;
    SRect m_defaultBGRect;

    float m_N; // multiple of Arena
    float m_M; // multiple of BG

    Uint64 m_nextTick;
    GameEvent m_lastAction;
    SDL_AppResult m_isExiting;

    shared_ptr<Arena> m_arena;

    shared_ptr<Label> m_gameTitle;
    shared_ptr<Label> m_gameVersion;
    shared_ptr<Label> m_gameAuthor;
    shared_ptr<Label> m_aboutLabel;
    shared_ptr<Button> m_startButton;

    shared_ptr<Dialog> m_aboutDialog;
public:
    Screen(Control *parent, SRect rect, SDL_Renderer *renderer, float xScale=1.0f, float yScale=1.0f);
    void inputControl(GameEvent &event);
    void update(void) override;
    void onClose(shared_ptr<Button> btn);
    void onStart(shared_ptr<Button> btn);
    void onAbout(shared_ptr<Label> label);
    SDL_AppResult isExiting(void);
};
#endif // ScreenH