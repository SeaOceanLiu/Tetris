#ifndef LabelH
#define LabelH

#include <SDL3_ttf/SDL_ttf.h>
#include <unordered_map>
#include <string>
#include "ConstDef.h"
#include "Control.h"
#include "ResourceLoader.h"

// #include "FontSuite.h"
enum class LabelState {
    Normal,
    Hover,
    Pressed
};

enum class AlignmentMode: int{
    AM_TOP_LEFT,
    AM_MID_LEFT,
    AM_BOTTOM_LEFT,

    AM_TOP_RIGHT,
    AM_MID_RIGHT,
    AM_BOTTOM_RIGHT,

    AM_TOP_CENTER,
    AM_CENTER,
    AM_BOTTOM_CENTER
};
class Label: public ControlImpl {
    friend class LabelBuilder;
    using OnClickHandler = std::function<void (shared_ptr<Label>)>;
private:
    TTF_Text *m_ttfText;
    TTF_Font *m_font;
    TTF_TextEngine *m_textEngin;
    SPoint m_translatedPos;
    SSize m_textSize;
    SDL_Cursor *m_hoverCursor;
    SDL_Cursor *m_defaultCursor;


    SDL_Color m_normalStateColor;
    SDL_Color m_hoverStateColor;
    SDL_Color m_pressedStateColor;
    SDL_Color m_shadowColor;
    SPoint m_shadowOffset;
    AlignmentMode m_AlignmentMode;
    int m_fontSize;
    string m_caption;
    bool m_shadowEnabled;
    FontName m_fontName;    // 由于初始化列表的执行顺序信赖际这里的声明顺序，所以m_fontName要放在m_fontFile之前
    fs::path m_fontFile;
    TTF_FontStyleFlags m_fontStyle;
    SRect m_hotRect;

    OnClickHandler m_onClick;
    LabelState m_state;

private:
    void loadFromFile(void);
    void loadFromResource(string resourceId);
    void createTextEngine(void);

public:
    Label(Control *parent, SRect rect, float xScale=1.0f, float yScale=1.0f);
    ~Label(void);
    void update(void);
    void draw(void);
    bool handleEvent(shared_ptr<Event> event);
    SRect getHotRect(void) const;

    Label& setNormalStateColor(SDL_Color color);
    Label& setHoverStateColor(SDL_Color color);
    Label& setPressedStateColor(SDL_Color color);
    Label& setCaption(string caption);
    Label& setFont(FontName fontName);
    Label& setAlignmentMode(AlignmentMode Alignment);
    Label& setFontSize(int fontSize);
    Label& setShadow(bool enabled);
    Label& setShadowColor(SDL_Color color);
    Label& setShadowOffset(SPoint offset);
    Label& setOnClick(OnClickHandler handler);

    Label& SetFontStyle(TTF_FontStyleFlags fontStyle);

    shared_ptr<Label> build(void);
};

class LabelBuilder {
private:
    shared_ptr<Label> m_label;
public:
    LabelBuilder(Control *parent, SRect rect, float xScale=1.0f, float yScale=1.0f);
    LabelBuilder& setNormalStateColor(SDL_Color color);
    LabelBuilder& setHoverStateColor(SDL_Color color);
    LabelBuilder& setPressedStateColor(SDL_Color color);
    LabelBuilder& setCaption(string caption);
    LabelBuilder& setFont(FontName fontName);
    LabelBuilder& setAlignmentMode(AlignmentMode Alignment);
    LabelBuilder& setFontSize(int fontSize);
    LabelBuilder& setShadow(bool enabled);
    LabelBuilder& setShadowColor(SDL_Color color);
    LabelBuilder& setShadowOffset(SPoint offset);
    LabelBuilder& setOnClick(Label::OnClickHandler handler);
    LabelBuilder& setId(int id);

    LabelBuilder& SetFontStyle(TTF_FontStyleFlags fontStyle);

    shared_ptr<Label> build(void);
};
#endif  // LabelH