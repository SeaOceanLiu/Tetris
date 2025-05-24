#ifndef ConstDefH
#define ConstDefH
#include <filesystem>
#include <vector>
#include <SDL3/SDL.h>

namespace fs = std::filesystem;

class SMultipleSize{
public:
    float scaleX;
    float scaleY;
    SMultipleSize():scaleX(1), scaleY(1){}
    SMultipleSize(float x, float y):scaleX(x), scaleY(y){}
    SMultipleSize(const SMultipleSize& m):scaleX(m.scaleX), scaleY(m.scaleY){}
    SMultipleSize(const SMultipleSize&& m):scaleX(m.scaleX), scaleY(m.scaleY){}

    SMultipleSize& operator=(const SMultipleSize& m){
        scaleX = m.scaleX;
        scaleY = m.scaleY;
        return *this;
    }
    SMultipleSize& operator=(const SMultipleSize&& m){
        scaleX = m.scaleX;
        scaleY = m.scaleY;
        return *this;
    }
    bool operator==(const SMultipleSize& m){
        return scaleX == m.scaleX && scaleY == m.scaleY;
    }
    SMultipleSize operator*(const SMultipleSize& m){
        return SMultipleSize(scaleX * m.scaleX, scaleY * m.scaleY);
    }
};
class SSize{
public:
    float width;
    float height;
    SSize():width(0), height(0){}
    SSize(float w, float h):width(w), height(h){}
    SSize(const SSize& s):width(s.width), height(s.height){}
    SSize(const SSize&& s):width(s.width), height(s.height){}
    SSize& operator=(const SSize& s){
        width = s.width;
        height = s.height;
        return *this;
    }
    SSize& operator=(const SSize&& s){
        width = s.width;
        height = s.height;
        return *this;
    }
    bool operator==(const SSize& s){
        return width == s.width && height == s.height;
    }
    SSize operator+(const SSize& s){
        return SSize(width + s.width, height + s.height);
    }
    SSize operator-(const SSize& s){
        return SSize(width - s.width, height - s.height);
    }
    SSize operator*(const SMultipleSize& m){
        return SSize(width * m.scaleX, height * m.scaleY);
    }
};
class SPoint{
public:
    float x;
    float y;
    SPoint():x(0), y(0){}
    SPoint(float x, float y):x(x), y(y){}
    SPoint(const SPoint& p):x(p.x), y(p.y){}
    SPoint(const SPoint&& p):x(p.x), y(p.y){}
    SPoint& operator=(const SPoint& p){
        x = p.x;
        y = p.y;
        return *this;
    }
    SPoint& operator=(const SPoint&& p){
        x = p.x;
        y = p.y;
        return *this;
    }
    bool operator==(const SPoint& p){
        return x == p.x && y == p.y;
    }
    SPoint operator+(const SPoint& p){
        return SPoint(x + p.x, y + p.y);
    }
    SPoint operator-(const SPoint& p){
        return SPoint(x - p.x, y - p.y);
    }
    SDL_FPoint toSDLFPoint(void){
        return SDL_FPoint{x, y};
    }
};

class SRect{
private:
    SDL_FRect m_sdlFRect;
public:
    float left;
    float top;
    float width;
    float height;
    SRect():left(0), top(0), width(0), height(0){}
    SRect(SPoint p, SSize s):left(p.x), top(p.y), width(s.width), height(s.height){}
    SRect(float l, float t, float w, float h):left(l), top(t), width(w), height(h){}
    SRect(const SRect& r):left(r.left), top(r.top), width(r.width), height(r.height){}
    SRect(const SRect&& r):left(r.left), top(r.top), width(r.width), height(r.height){}
    SRect& operator=(const SRect& r){
        left = r.left;
        top = r.top;
        width = r.width;
        height = r.height;
        return *this;
    }
    SRect& operator=(const SRect&& r){
        left = r.left;
        top = r.top;
        width = r.width;
        height = r.height;
        return *this;
    }
    bool operator==(const SRect& r){
        return left == r.left && top == r.top && width == r.width && height == r.height;
    }
    bool contains(float x, float y){
        return x >= left && x <= left + width && y >= top && y <= top + height;
    }
    bool contains(const SPoint& p){
        return contains(p.x, p.y);
    }
    SRect operator+(const SPoint& p){
        return SRect(left + p.x, top + p.y, width, height);
    }
    SRect operator-(const SPoint& p){
        return SRect(left - p.x, top - p.y, width, height);
    }
    SRect operator*(const SMultipleSize& m){
        return SRect(left, top, width * m.scaleX, height * m.scaleY);
    }
    SDL_FRect* toSDLFRect(void){
        m_sdlFRect = SDL_FRect{left, top, width, height};
        return &m_sdlFRect;
    }
};

class ConstDef{
public:
    static SSize  DEFAULT_ARENA_SIZE;
    static SSize  DEFAULT_BG_SIZE;
    static fs::path pathPrefix; // 资源文件路径前缀

    static float  DEFAULT_SPACE_BETWEEN_PG_AND_HINT ;   // 提示方块与游戏区域之间的间距
    static float  DEFAULT_FONT_MARGIN               ;   // 字体与边框之间的间距
    static SSize  DEFAULT_SINGLE_BLOCK_SIZE         ;
    static SRect  DEFAULT_PG                        ;   //宽10格，高20格
    static SRect  DEFAULT_HINT_PLACE                ;   // 提示方块放置位置，宽高各5格，与XML中的最大值保持一致
    static float  DEFAULT_VERSION_FONT_SIZE         ;
    static float  DEFAULT_INFORMATION_FONT_SIZE     ;
    static float  DEFAULT_PAUSED_TEXT_FONT_SIZE     ;
    static float  DEFAULT_FAILED_TEXT_FONT_SIZE     ;
    static float  DEFAULT_GAME_BAR_TEXT_FONT_SIZE   ;
    static float  DEFAULT_DEBUG_INFO_FONT_SIZE      ;
    static float  DEFAULT_COPYRIGHT_FONT_SIZE       ;
    static float  DEFAULT_BUTTON_SIZE               ;
    static float  DEFAULT_BUTTON_CAPTION_SIZE       ;
    static float  DEFAULT_PLAY_PAUSE_BUTTON_SIZE    ;
    static SRect  DEFAULT_GAME_TITLE_POS            ;
    static SRect  DEFAULT_VERSION_INFO_POS          ;
    static SRect  DEFAULT_AUTHOR_INFO_POS           ;
    static SRect  DEFAULT_FPS_TEXT_POS              ;
    static SRect  DEFAULT_SPEED_TEXT_POS            ;   // 速度文本位置
    static SRect  DEFAULT_SCORE_TEXT_POS            ;   // 分数文本位置
    static SRect  DEFAULT_PAUSED_TEXT_POS           ;   // 游戏暂停文本位置
    static SRect  DEFAULT_FAILED_TEXT_POS           ;   // 游戏失败文本位置
    static SRect  DEFAULT_DEBUG_TEXT_POS            ;   // 游戏调试文本位置
    static SRect  DEFAULT_LEFT_BUTTON_POS           ;   // 左按钮位置
    static SRect  DEFAULT_RIGHT_BUTTON_POS          ;   // 右按钮位置
    static SRect  DEFAULT_DOWN_BUTTON_POS           ;   // 下按钮位置
    static SRect  DEFAULT_ROTATE_BUTTON_POS         ;   // 旋转按钮位置
    static SRect  DEFAULT_PLAY_PAUSE_BUTTON_POS     ;
    static float  DEFAULT_ANIMATION_MOVE_STEP_X     ;   // 动画移动步长X
    static float  DEFAULT_ANIMATION_MOVE_STEP_Y     ;   // 动画移动步长Y

    static SDL_Color PANEL_BG_COLOR;
    static SDL_Color PANEL_BORDER_COLOR;
    static SDL_Color BUTTON_NORMAL_COLOR;
    static SDL_Color BUTTON_HOVER_COLOR;
    static SDL_Color BUTTON_DOWN_COLOR;

    static int    SCREEN_EMPTY_COLOR         ;
    static int    SCREEN_CROSS_COLOR         ;
    static int    SCREEN_BOMB_COLOR          ;
    static int    MAX_SCORE_LEVEL            ;
    static int    MAX_BOMB_LEVEL             ;
    static int    GRADE_SCORE_LEVEL          ;  // 每1500分提速一级
    static int    MAX_SLOW_SPEED             ;
    static int    MAX_FAST_SPEED             ;
    static int    PG_START_ROW               ;  // 实际游戏起始行
    static int    HINT_PLACE_START_COL       ;  // 提示方块起始列
    static int    HINT_PLACE_END_COL         ;  // 提示方块结束列

    static Uint64 DEFAULT_UPDATE_MS_INTERVAL ;   // update的时间间隔，单位毫秒
    static Uint64 DEFAULT_BTN_MS_INTERVAL    ;   // 按钮速度时间间隔，单位毫秒
    static Uint64 CURRENT_MS_TIMESTAMP       ;   // 当前时间戳，单位毫秒

    static int    CMB_DRAW_BIT               ;   // define CHECK_MASK_BIT
    static int    CMB_CHECK_DOWN             ;
    static int    CMB_CHECK_LEFT             ;
    static int    CMB_CHECK_RIGHT            ;
    static int    CMB_CHECK_PASS             ;
    static int    CMB_CHECK_BOMB             ;
    static float  WINDOW_TITLE_HEIGHT        ;

    static float  SPACE_BETWEEN_PG_AND_HINT ;   // 提示方块与游戏区域之间的间距
    static float  FONT_MARGIN               ;   // 字体与边框之间的间距
    static SSize  SINGLE_BLOCK_SIZE         ;
    static SRect  PG                        ;   //宽10格，高20格
    static SRect  HINT_PLACE                ;   // 提示方块放置位置，宽高各5格，与XML中的最大值保持一致
    static float  VERSION_FONT_SIZE         ;
    static float  INFORMATION_FONT_SIZE     ;
    static float  PAUSED_TEXT_FONT_SIZE     ;
    static float  FAILED_TEXT_FONT_SIZE     ;
    static float  GAME_BAR_TEXT_FONT_SIZE   ;
    static float  DEBUG_INFO_FONT_SIZE      ;
    static float  COPYRIGHT_FONT_SIZE       ;
    static float  BUTTON_SIZE               ;
    static float  BUTTON_CAPTION_SIZE       ;
    static SRect  GAME_TITLE_POS            ;
    static SRect  VERSION_INFO_POS          ;
    static SRect  AUTHOR_INFO_POS           ;
    static SRect  FPS_TEXT_POS              ;
    static SRect  SPEED_TEXT_POS            ;   // 速度文本位置
    static SRect  SCORE_TEXT_POS            ;   // 分数文本位置
    static SRect  PAUSED_TEXT_POS           ;   // 游戏暂停文本位置
    static SRect  FAILED_TEXT_POS           ;   // 游戏失败文本位置
    static SRect  DEBUG_TEXT_POS            ;   // 游戏调试文本位置
    static SRect  LEFT_BUTTON_POS           ;   // 左按钮位置
    static SRect  RIGHT_BUTTON_POS          ;   // 右按钮位置
    static SRect  DOWN_BUTTON_POS           ;   // 下按钮位置
    static SRect  ROTATE_BUTTON_POS         ;   // 旋转按钮位置
    static SRect  PLAY_PAUSE_BUTTON_POS     ;
    static float  ANIMATION_MOVE_STEP_X     ;   // 动画移动步长X
    static float  ANIMATION_MOVE_STEP_Y     ;   // 动画移动步长Y

    static std::vector<int> LEVEL_SPEED_MAPPING;
    static std::vector<int> LEVEL_SCORE_MAPPING;
    static std::vector<int> BOMB_SCORE_MAPPING;

private:
    ConstDef();
    static SRect  DISPLAY_RECT              ;   // 显示区域
    // ConstDef(float scaleX, float scaleY, float offsetX, float offsetY) = delete;
public:
    static ConstDef& getInstance(void){
        static ConstDef instance; // 静态局部变量，程序运行期间只会被初始化一次
        return instance;
    }
    void setScale(float scaleX, float scaleY);
    SMultipleSize getScale(void);
    void setOffset(float offsetX, float offsetY);
    SPoint getOffset(void);
    void recalculate(void);
    void restToDefault(void);
    void setDisplayRect(SRect rect);
    SRect getDisplayRect(void);

protected:
    static SMultipleSize s_scale;
    static SPoint s_offset;
};


#endif