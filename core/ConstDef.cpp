#include "ConstDef.h"
SSize ConstDef::DEFAULT_ARENA_SIZE = {200, 606};
SSize ConstDef::DEFAULT_BG_SIZE = {720, 1280};
#ifndef SDL_PLATFORM_WINDOWS
fs::path ConstDef::pathPrefix;
#else
fs::path ConstDef::pathPrefix = fs::path(std::string(SDL_GetBasePath()) + "assets");
#endif


SRect    ConstDef::DISPLAY_RECT = {0, 0, 720, 1280};    // 显示区域大小

// 以下是需要在修改缩放和偏移量时修改的常量
float    ConstDef::DEFAULT_SPACE_BETWEEN_PG_AND_HINT    = 10;    // 提示方块与游戏区域之间的间距
float    ConstDef::DEFAULT_FONT_MARGIN                  = 5.0f;    // 字体与边框之间的间距
SSize    ConstDef::DEFAULT_SINGLE_BLOCK_SIZE            = {20, 20}; // 单个方块的大小，在初始化时，可以根据从文件读入的大小做调整
SRect    ConstDef::DEFAULT_PG                           = {0, 0, 10, 25};   //宽10格，高25格，其中最上面5行作为提示方式区域
SRect    ConstDef::DEFAULT_HINT_PLACE                   = {ConstDef::DEFAULT_PG.left, ConstDef::DEFAULT_PG.top,
                                                            ConstDef::DEFAULT_PG.width, 5};    // 提示方块放置位置，高5格，与XML中的最大值保持一致

float    ConstDef::DEFAULT_VERSION_FONT_SIZE            = 60.0f;
float    ConstDef::DEFAULT_INFORMATION_FONT_SIZE        = 12.0f;
float    ConstDef::DEFAULT_PAUSED_TEXT_FONT_SIZE        = 20.0f;
float    ConstDef::DEFAULT_FAILED_TEXT_FONT_SIZE        = 20.0f;
float    ConstDef::DEFAULT_GAME_BAR_TEXT_FONT_SIZE      = 12.0f;
float    ConstDef::DEFAULT_DEBUG_INFO_FONT_SIZE         = 12.0f;
float    ConstDef::DEFAULT_COPYRIGHT_FONT_SIZE          = 30.0f;

float    ConstDef::DEFAULT_BUTTON_SIZE                  = 32.0f;//48.0f;
float    ConstDef::DEFAULT_BUTTON_CAPTION_SIZE          = 20.0f;
float    ConstDef::DEFAULT_PLAY_PAUSE_BUTTON_SIZE       = 20.0f;

SRect    ConstDef::DEFAULT_GAME_TITLE_POS               = {0, ConstDef::DEFAULT_PG.height * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.height + 35,
                                                            ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width,
                                                            ConstDef::DEFAULT_VERSION_FONT_SIZE + ConstDef::DEFAULT_FONT_MARGIN};
SRect    ConstDef::DEFAULT_VERSION_INFO_POS             = {ConstDef::DEFAULT_PG.left, ConstDef::DEFAULT_GAME_TITLE_POS.top + ConstDef::DEFAULT_GAME_TITLE_POS.height,
                                                            ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width,
                                                            ConstDef::DEFAULT_VERSION_FONT_SIZE + ConstDef::DEFAULT_FONT_MARGIN};
SRect    ConstDef::DEFAULT_AUTHOR_INFO_POS              = {ConstDef::DEFAULT_PG.left, ConstDef::DEFAULT_GAME_TITLE_POS.top + ConstDef::DEFAULT_GAME_TITLE_POS.height,
                                                            ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width,
                                                            ConstDef::DEFAULT_VERSION_FONT_SIZE + ConstDef::DEFAULT_FONT_MARGIN};
SRect    ConstDef::DEFAULT_FPS_TEXT_POS                 = {ConstDef::DEFAULT_PG.left, ConstDef::DEFAULT_PG.top,
                                                            ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width,
                                                            ConstDef::DEFAULT_GAME_BAR_TEXT_FONT_SIZE + ConstDef::DEFAULT_FONT_MARGIN};
SRect    ConstDef::DEFAULT_SPEED_TEXT_POS               = {ConstDef::DEFAULT_PG.left, ConstDef::DEFAULT_PG.top,
                                                            ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width,
                                                            ConstDef::DEFAULT_INFORMATION_FONT_SIZE + ConstDef::DEFAULT_FONT_MARGIN};  // 速度文本位置
SRect    ConstDef::DEFAULT_SCORE_TEXT_POS               = {ConstDef::DEFAULT_PG.left, ConstDef::DEFAULT_PG.top,
                                                            ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width,
                                                            ConstDef::DEFAULT_INFORMATION_FONT_SIZE + ConstDef::DEFAULT_FONT_MARGIN};  // 分数文本位置
SRect    ConstDef::DEFAULT_PAUSED_TEXT_POS              = {ConstDef::DEFAULT_PG.left, ConstDef::DEFAULT_PG.top,
                                                            ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width,
                                                            ConstDef::DEFAULT_PG.height * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.height + ConstDef::DEFAULT_FONT_MARGIN}; // 游戏暂停文本位置
SRect    ConstDef::DEFAULT_FAILED_TEXT_POS              = {ConstDef::DEFAULT_PG.left, ConstDef::DEFAULT_PG.top,
                                                            ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width,
                                                            ConstDef::DEFAULT_PG.height * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.height + ConstDef::DEFAULT_FONT_MARGIN}; // 游戏失败文本位置
SRect    ConstDef::DEFAULT_DEBUG_TEXT_POS                = {ConstDef::DEFAULT_PG.left, ConstDef::DEFAULT_PG.top + ConstDef::DEFAULT_PG.height * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.height - ConstDef::DEFAULT_DEBUG_INFO_FONT_SIZE - ConstDef::DEFAULT_FONT_MARGIN,
                                                            ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width,
                                                            ConstDef::DEFAULT_DEBUG_INFO_FONT_SIZE + ConstDef::DEFAULT_FONT_MARGIN}; // 调试信息位置

SRect    ConstDef::DEFAULT_LEFT_BUTTON_POS               = {ConstDef::DEFAULT_PG.left, ConstDef::DEFAULT_PG.height * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.height + ConstDef::DEFAULT_FONT_MARGIN,
                                                            ConstDef::DEFAULT_BUTTON_SIZE, ConstDef::DEFAULT_BUTTON_SIZE};
SRect    ConstDef::DEFAULT_RIGHT_BUTTON_POS              = {ConstDef::DEFAULT_LEFT_BUTTON_POS.left + ConstDef::DEFAULT_LEFT_BUTTON_POS.width + ConstDef::DEFAULT_FONT_MARGIN, ConstDef::DEFAULT_LEFT_BUTTON_POS.top,
                                                            ConstDef::DEFAULT_BUTTON_SIZE, ConstDef::DEFAULT_BUTTON_SIZE};
SRect    ConstDef::DEFAULT_DOWN_BUTTON_POS              = {ConstDef::DEFAULT_LEFT_BUTTON_POS.left + (ConstDef::DEFAULT_LEFT_BUTTON_POS.width + ConstDef::DEFAULT_FONT_MARGIN) / 2,
                                                            ConstDef::DEFAULT_LEFT_BUTTON_POS.top + ConstDef::DEFAULT_LEFT_BUTTON_POS.height + ConstDef::DEFAULT_FONT_MARGIN,
                                                            ConstDef::DEFAULT_BUTTON_SIZE, ConstDef::DEFAULT_BUTTON_SIZE};
SRect    ConstDef::DEFAULT_ROTATE_BUTTON_POS            = {ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width - ConstDef::DEFAULT_BUTTON_SIZE,
                                                            ConstDef::DEFAULT_LEFT_BUTTON_POS.top + ConstDef::DEFAULT_BUTTON_SIZE / 2,
                                                            ConstDef::DEFAULT_BUTTON_SIZE, ConstDef::DEFAULT_BUTTON_SIZE};
SRect    ConstDef::DEFAULT_PLAY_PAUSE_BUTTON_POS        = {(ConstDef::DEFAULT_PG.width * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.width) / 2,
                                                            ConstDef::DEFAULT_LEFT_BUTTON_POS.top + ConstDef::DEFAULT_BUTTON_SIZE / 2,//ConstDef::DEFAULT_PG.height * ConstDef::DEFAULT_SINGLE_BLOCK_SIZE.height + 3 * DEFAULT_PLAY_PAUSE_BUTTON_SIZE, //ConstDef::DEFAULT_BUTTON_SIZE,
                                                            ConstDef::DEFAULT_PLAY_PAUSE_BUTTON_SIZE, ConstDef::DEFAULT_PLAY_PAUSE_BUTTON_SIZE};
float    ConstDef::DEFAULT_ANIMATION_MOVE_STEP_X        = 1.0f;
float    ConstDef::DEFAULT_ANIMATION_MOVE_STEP_Y        = 1.0f;

float    ConstDef::SPACE_BETWEEN_PG_AND_HINT   = ConstDef::DEFAULT_SPACE_BETWEEN_PG_AND_HINT  ;
float    ConstDef::FONT_MARGIN                 = ConstDef::DEFAULT_FONT_MARGIN                ;
SSize    ConstDef::SINGLE_BLOCK_SIZE           = ConstDef::DEFAULT_SINGLE_BLOCK_SIZE          ;
SRect    ConstDef::PG                          = ConstDef::DEFAULT_PG                         ;
SRect    ConstDef::HINT_PLACE                  = ConstDef::DEFAULT_HINT_PLACE                 ;
float    ConstDef::VERSION_FONT_SIZE           = ConstDef::DEFAULT_VERSION_FONT_SIZE          ;
float    ConstDef::INFORMATION_FONT_SIZE       = ConstDef::DEFAULT_INFORMATION_FONT_SIZE      ;
float    ConstDef::PAUSED_TEXT_FONT_SIZE       = ConstDef::DEFAULT_PAUSED_TEXT_FONT_SIZE      ;
float    ConstDef::FAILED_TEXT_FONT_SIZE       = ConstDef::DEFAULT_FAILED_TEXT_FONT_SIZE      ;
float    ConstDef::GAME_BAR_TEXT_FONT_SIZE     = ConstDef::DEFAULT_GAME_BAR_TEXT_FONT_SIZE    ;
float    ConstDef::DEBUG_INFO_FONT_SIZE        = ConstDef::DEFAULT_DEBUG_INFO_FONT_SIZE       ;
float    ConstDef::COPYRIGHT_FONT_SIZE         = ConstDef::DEFAULT_COPYRIGHT_FONT_SIZE        ;
float    ConstDef::BUTTON_SIZE                 = ConstDef::DEFAULT_BUTTON_SIZE                ;
float    ConstDef::BUTTON_CAPTION_SIZE         = ConstDef::DEFAULT_BUTTON_CAPTION_SIZE        ;
SRect    ConstDef::GAME_TITLE_POS              = ConstDef::DEFAULT_GAME_TITLE_POS             ;
SRect    ConstDef::VERSION_INFO_POS            = ConstDef::DEFAULT_VERSION_INFO_POS           ;
SRect    ConstDef::AUTHOR_INFO_POS             = ConstDef::DEFAULT_AUTHOR_INFO_POS            ;
SRect    ConstDef::FPS_TEXT_POS                = ConstDef::DEFAULT_FPS_TEXT_POS               ;
SRect    ConstDef::SPEED_TEXT_POS              = ConstDef::DEFAULT_SPEED_TEXT_POS             ;
SRect    ConstDef::SCORE_TEXT_POS              = ConstDef::DEFAULT_SCORE_TEXT_POS             ;
SRect    ConstDef::PAUSED_TEXT_POS             = ConstDef::DEFAULT_PAUSED_TEXT_POS            ;
SRect    ConstDef::FAILED_TEXT_POS             = ConstDef::DEFAULT_FAILED_TEXT_POS            ;
SRect    ConstDef::DEBUG_TEXT_POS              = ConstDef::DEFAULT_DEBUG_TEXT_POS             ;
SRect    ConstDef::LEFT_BUTTON_POS             = ConstDef::DEFAULT_LEFT_BUTTON_POS            ;
SRect    ConstDef::RIGHT_BUTTON_POS            = ConstDef::DEFAULT_RIGHT_BUTTON_POS           ;
SRect    ConstDef::DOWN_BUTTON_POS             = ConstDef::DEFAULT_DOWN_BUTTON_POS            ;
SRect    ConstDef::PLAY_PAUSE_BUTTON_POS       = ConstDef::DEFAULT_PLAY_PAUSE_BUTTON_POS      ;
SRect    ConstDef::ROTATE_BUTTON_POS           = ConstDef::DEFAULT_ROTATE_BUTTON_POS          ;
float    ConstDef::ANIMATION_MOVE_STEP_X       = ConstDef::DEFAULT_ANIMATION_MOVE_STEP_X      ;
float    ConstDef::ANIMATION_MOVE_STEP_Y       = ConstDef::DEFAULT_ANIMATION_MOVE_STEP_Y      ;

SDL_Color ConstDef::PANEL_BG_COLOR      = {211, 211, 211, SDL_ALPHA_OPAQUE};    // 背景颜色:lightgray
SDL_Color ConstDef::PANEL_BORDER_COLOR  = {153,  50, 204, SDL_ALPHA_OPAQUE};    // 边框颜色:purple/darkorchid
SDL_Color ConstDef::BUTTON_NORMAL_COLOR = {169, 169, 169, SDL_ALPHA_OPAQUE};    // 按钮弹起填充颜色:darkgray
SDL_Color ConstDef::BUTTON_HOVER_COLOR  = {128, 128, 128, SDL_ALPHA_OPAQUE};    // 按钮悬停填充颜色:gray
SDL_Color ConstDef::BUTTON_DOWN_COLOR   = {211, 211, 211, SDL_ALPHA_OPAQUE};    // 按钮按下填充颜色:lightgray

int      ConstDef::SCREEN_EMPTY_COLOR          = -1;
int      ConstDef::SCREEN_CROSS_COLOR          = 7;
int      ConstDef::SCREEN_BOMB_COLOR           = 8;
int      ConstDef::MAX_SCORE_LEVEL             = 6;
int      ConstDef::MAX_BOMB_LEVEL              = 9;
int      ConstDef::GRADE_SCORE_LEVEL           = 1500;  // 每1500分提速一级
int      ConstDef::MAX_SLOW_SPEED              = 0;
int      ConstDef::MAX_FAST_SPEED              = 19;
int      ConstDef::PG_START_ROW                = 5;
int      ConstDef::HINT_PLACE_START_COL        = 3;
int      ConstDef::HINT_PLACE_END_COL          = 7;
Uint64   ConstDef::DEFAULT_UPDATE_MS_INTERVAL  =  200;  // 方块下落的最小时间片，单位毫秒
Uint64   ConstDef::DEFAULT_BTN_MS_INTERVAL     = 100;   // 按钮响应的时间间隔，单位毫秒
Uint64   ConstDef::CURRENT_MS_TIMESTAMP        = 0;     // 当前时间戳，单位毫秒
int      ConstDef::CMB_DRAW_BIT                = 0x01;
int      ConstDef::CMB_CHECK_DOWN              = 0x02;
int      ConstDef::CMB_CHECK_LEFT              = 0x04;
int      ConstDef::CMB_CHECK_RIGHT             = 0x08;
int      ConstDef::CMB_CHECK_PASS              = 0x10;
int      ConstDef::CMB_CHECK_BOMB              = 0x20;
float    ConstDef::WINDOW_TITLE_HEIGHT         = 14.0f;


SMultipleSize ConstDef::s_scale = {1.0f, 1.0f};
SPoint ConstDef::s_offset = {0.0f, 0.0f};
//Leve                                              00    01    02    03    04    05    06    07    08    09    10    11    12    13    14    15    16    17    18    19
std::vector<int> ConstDef::LEVEL_SPEED_MAPPING = {9999, 9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000, 1000,  900,  800,  700,  600,  500,  400,  300,  200,  100,   50};    //速度等级与方块下落时间片的映射关系，单位：毫秒
std::vector<int> ConstDef::LEVEL_SCORE_MAPPING = {0, 100, 300, 500, 700, 1000};
std::vector<int> ConstDef::BOMB_SCORE_MAPPING = {0, 10, 20, 30, 100, 150, 300, 450, 600};

ConstDef::ConstDef()
{
    restToDefault();
}

void ConstDef::setScale(float scaleX, float scaleY){
    ConstDef::s_scale = {scaleX, scaleY};
    recalculate();
}
SMultipleSize ConstDef::getScale(void){
    return ConstDef::s_scale;
}

void ConstDef::setOffset(float offsetX, float offsetY){
    ConstDef::s_offset = {offsetX, offsetY};
    recalculate();
}
SPoint ConstDef::getOffset(void){
    return ConstDef::s_offset;
}

void ConstDef::restToDefault(void){
    ConstDef::s_scale = {1.0f, 1.0f};
    ConstDef::s_offset = {0.0f, 0.0f};
    recalculate();
}

void ConstDef::setDisplayRect(SRect rect){
    ConstDef::DISPLAY_RECT = rect;
}
SRect ConstDef::getDisplayRect(void){
    return ConstDef::DISPLAY_RECT;
}


void ConstDef::recalculate(void){
    ConstDef::SPACE_BETWEEN_PG_AND_HINT   = ConstDef::DEFAULT_SPACE_BETWEEN_PG_AND_HINT * ConstDef::s_scale.scaleX;
    ConstDef::FONT_MARGIN                 = ConstDef::DEFAULT_FONT_MARGIN * ConstDef::s_scale.scaleY;
    ConstDef::SINGLE_BLOCK_SIZE           = ConstDef::DEFAULT_SINGLE_BLOCK_SIZE * ConstDef::s_scale;
    ConstDef::PG                          = ConstDef::DEFAULT_PG + ConstDef::s_offset;

    ConstDef::HINT_PLACE                   = {ConstDef::PG.left, ConstDef::PG.top, ConstDef::PG.width, 5};  // 提示方块放置位置，高5格，与XML中的最大值保持一致

    ConstDef::VERSION_FONT_SIZE           = ConstDef::DEFAULT_VERSION_FONT_SIZE       * ConstDef::s_scale.scaleX;
    ConstDef::INFORMATION_FONT_SIZE       = ConstDef::DEFAULT_INFORMATION_FONT_SIZE   * ConstDef::s_scale.scaleX;
    ConstDef::PAUSED_TEXT_FONT_SIZE       = ConstDef::DEFAULT_PAUSED_TEXT_FONT_SIZE   * ConstDef::s_scale.scaleX;
    ConstDef::FAILED_TEXT_FONT_SIZE       = ConstDef::DEFAULT_FAILED_TEXT_FONT_SIZE   * ConstDef::s_scale.scaleX;
    ConstDef::GAME_BAR_TEXT_FONT_SIZE     = ConstDef::DEFAULT_GAME_BAR_TEXT_FONT_SIZE * ConstDef::s_scale.scaleX;
    ConstDef::BUTTON_SIZE                 = ConstDef::DEFAULT_BUTTON_SIZE               * ConstDef::s_scale.scaleX;

    ConstDef::GAME_TITLE_POS              = {ConstDef::PG.left, ConstDef::PG.top + ConstDef::PG.height * ConstDef::SINGLE_BLOCK_SIZE.height + ConstDef::FONT_MARGIN,
                                            ConstDef::PG.width * ConstDef::SINGLE_BLOCK_SIZE.width,
                                            ConstDef::VERSION_FONT_SIZE + ConstDef::FONT_MARGIN};
    ConstDef::VERSION_INFO_POS            = {ConstDef::PG.left, ConstDef::GAME_TITLE_POS.top + ConstDef::GAME_TITLE_POS.height,
                                            ConstDef::PG.width * ConstDef::SINGLE_BLOCK_SIZE.width,
                                            ConstDef::VERSION_FONT_SIZE + ConstDef::FONT_MARGIN};
    ConstDef::FPS_TEXT_POS                = {ConstDef::PG.left, ConstDef::PG.top,
                                            ConstDef::PG.width * ConstDef::SINGLE_BLOCK_SIZE.width,
                                            ConstDef::GAME_BAR_TEXT_FONT_SIZE + ConstDef::FONT_MARGIN};
    ConstDef::SPEED_TEXT_POS              = {ConstDef::PG.left, ConstDef::PG.top,
                                            ConstDef::PG.width * ConstDef::SINGLE_BLOCK_SIZE.width,
                                            ConstDef::INFORMATION_FONT_SIZE + ConstDef::FONT_MARGIN};  // 速度文本位置
    ConstDef::SCORE_TEXT_POS              = {ConstDef::PG.left, ConstDef::PG.top,
                                            ConstDef::PG.width * ConstDef::SINGLE_BLOCK_SIZE.width,
                                            ConstDef::INFORMATION_FONT_SIZE + ConstDef::FONT_MARGIN};  // 分数文本位置
    ConstDef::PAUSED_TEXT_POS             = {ConstDef::PG.left, ConstDef::PG.top,
                                            ConstDef::PG.width * ConstDef::SINGLE_BLOCK_SIZE.width,
                                            ConstDef::PG.height * ConstDef::SINGLE_BLOCK_SIZE.height + ConstDef::FONT_MARGIN}; // 游戏暂停文本位置
    ConstDef::FAILED_TEXT_POS             = {ConstDef::PG.left, ConstDef::PG.top,
                                            ConstDef::PG.width * ConstDef::SINGLE_BLOCK_SIZE.width,
                                            ConstDef::PG.height * ConstDef::SINGLE_BLOCK_SIZE.height + ConstDef::FONT_MARGIN}; // 游戏失败文本位置
    ConstDef::DEBUG_TEXT_POS              = {ConstDef::PG.left, ConstDef::PG.top + ConstDef::PG.height * ConstDef::SINGLE_BLOCK_SIZE.height,
                                            ConstDef::PG.width * ConstDef::SINGLE_BLOCK_SIZE.width,
                                            ConstDef::DEBUG_INFO_FONT_SIZE + ConstDef::FONT_MARGIN}; // 调试信息位置

    ConstDef::LEFT_BUTTON_POS             = {ConstDef::PG.left, ConstDef::GAME_TITLE_POS.top,
                                            ConstDef::BUTTON_SIZE, BUTTON_SIZE};
    ConstDef::RIGHT_BUTTON_POS            = {ConstDef::LEFT_BUTTON_POS.left + ConstDef::LEFT_BUTTON_POS.width + ConstDef::FONT_MARGIN, ConstDef::LEFT_BUTTON_POS.top,
                                            ConstDef::BUTTON_SIZE, BUTTON_SIZE};
    ConstDef::DOWN_BUTTON_POS             = {ConstDef::LEFT_BUTTON_POS.left + (ConstDef::LEFT_BUTTON_POS.width + ConstDef::FONT_MARGIN) / 2,
                                            ConstDef::LEFT_BUTTON_POS.top + ConstDef::LEFT_BUTTON_POS.height + ConstDef::FONT_MARGIN,
                                            ConstDef::BUTTON_SIZE, BUTTON_SIZE};
    ConstDef::ROTATE_BUTTON_POS           = {ConstDef::PG.left + ConstDef::PG.width * ConstDef::SINGLE_BLOCK_SIZE.width - ConstDef::BUTTON_SIZE,
                                            ConstDef::LEFT_BUTTON_POS.top + ConstDef::BUTTON_SIZE / 2, ConstDef::BUTTON_SIZE, BUTTON_SIZE};
    ConstDef::ANIMATION_MOVE_STEP_X       = ConstDef::DEFAULT_ANIMATION_MOVE_STEP_X * ConstDef::s_scale.scaleX;
    ConstDef::ANIMATION_MOVE_STEP_Y       = ConstDef::DEFAULT_ANIMATION_MOVE_STEP_Y * ConstDef::s_scale.scaleX;

}

