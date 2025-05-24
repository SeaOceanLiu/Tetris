#ifndef MaterialH
#define MaterialH
#include <filesystem>
#include <SDL3/SDL.h>

#include "ConstDef.h"

#include "Control.h"

namespace fs = std::filesystem;

enum class AnchorType: int{
    AT_SPECIAL,
    AT_TOP_LEFT,
    AT_MID_LEFT,
    AT_BOTTOM_LEFT,

    AT_TOP_RIGHT,
    AT_MID_RIGHT,
    AT_BOTTOM_RIGHT,

    AT_TOP_CENTER,
    AT_CENTER,
    AT_BOTTOM_CENTER
};

class Material: public ControlImpl{
protected:
    fs::path m_filePath;
    SPoint m_anchorPoint;
    AnchorType m_anchorType;
public:
    Material(Control *parent, float xScale=1.0f, float yScale=1.0f);
    Material(Control *parent, fs::path filePath, float xScale=1.0f, float yScale=1.0f);
    Material(const Material &material);
    Material &operator=(const Material &material);
    ~Material() override;

    virtual void loadFromFile(fs::path filePath) = 0;

    void draw(void) override;
    void draw(SPoint pos, Uint8 alpha=SDL_ALPHA_OPAQUE);
    void draw(float x, float y, Uint8 alpha=SDL_ALPHA_OPAQUE);

    void setAnchorPoint(float x, float y);
    void setAnchorPoint(SPoint &anchorPoint);
    void setAnchorPoint(AnchorType anchorType);

    void correctAnchorPoint(void);

    void setAnchorPointTopLeft(void);
    void setAnchorPointMidLeft(void);
    void setAnchorPointBottomLeft(void);
    void setAnchorPointTopRight(void);
    void setAnchorPointMidRight(void);
    void setAnchorPointBottomRight(void);
    void setAnchorPointTopCenter(void);
    void setAnchorPointCenter(void);
    void setAnchorPointBottomCenter(void);

    SPoint anchorPointTranslate(SPoint point);
};
#endif