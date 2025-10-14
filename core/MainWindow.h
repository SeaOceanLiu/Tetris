#ifndef MainWindowH
#define MainWindowH

#include <SDL3/SDL.h>
#include "ConstDef.h"

#define INITIAL_WIDTH  200 //横向像素个数
#define INITIAL_HEIGHT 606 //纵向像素个数

#define INITIAL_BG_WIDTH  720 //横向像素个数
#define INITIAL_BG_HEIGHT 1280 //纵向像素个数

// #define SCREEN_WIDTH  200 //横向像素个数
// #define SCREEN_HEIGHT 606 //纵向像素个数
#define SCREEN_WIDTH  720 //横向像素个数
#define SCREEN_HEIGHT 1280 //纵向像素个数
// #define SCREEN_WIDTH  1344 //横向像素个数
// #define SCREEN_HEIGHT 2772 //纵向像素个数
// #define SCREEN_WIDTH  896 //横向像素个数
// #define SCREEN_HEIGHT 1848 //纵向像素个数

class MainWindow {
private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_PixelFormat m_pixelFormat;

    SDL_DisplayID m_displayId;
    float m_displayWidth;
    float m_displayHeight;

    MainWindow(): m_window(nullptr), m_renderer(nullptr) {
        if (!SDL_CreateWindowAndRenderer("Tetris", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN, &m_window, &m_renderer)) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            throw ("Couldn't create window/renderer");
        }
        SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); //设置窗口位置
        // // 开启垂直同步
        // if(!SDL_SetRenderVSync(renderer, 1)){
        //     SDL_Log("Couldn't set vsync: %s", SDL_GetError());
        // }

        // 获取窗口的像素格式
        m_pixelFormat = SDL_GetWindowPixelFormat(m_window);
        if (m_pixelFormat == SDL_PIXELFORMAT_UNKNOWN) {
            SDL_Log("SDL_GetWindowPixelFormat Error: %s", SDL_GetError());
            throw ("SDL_GetWindowPixelFormat Error");
        }

        // 获取显示器信息
        SDL_DisplayID m_displayId = SDL_GetPrimaryDisplay();
        if (m_displayId == 0) {
            SDL_Log("SDL_GetPrimaryDisplay Error: %s\n", SDL_GetError());
            throw ("SDL_GetPrimaryDisplay Error");
        }

        const SDL_DisplayMode *displayMode = SDL_GetCurrentDisplayMode(m_displayId);
        if (displayMode == nullptr) {
            SDL_Log("SDL_GetCurrentDisplayMode Error: %s\n", SDL_GetError());
            throw ("SDL_GetCurrentDisplayMode Error");
        }
        m_displayWidth = (float)displayMode->w * displayMode->pixel_density;
        m_displayHeight = (float)displayMode->h * displayMode->pixel_density;
    }
public:
    static MainWindow* getInstance(void){
        static MainWindow instance; // 静态局部变量，程序运行期间只会被初始化一次
        return &instance;
    }

    SDL_Window* getWindow(void) { return m_window; };
    SDL_Renderer* getRenderer(void) { return m_renderer; };
    SDL_PixelFormat getPixelFormat(void) { return m_pixelFormat; };
    float getDisplayWidth(void) { return m_displayWidth; };
    float getDisplayHeight(void) { return m_displayHeight; };
    SSize getDisplaySize(void) { return SSize{m_displayWidth, m_displayHeight}; };
    // ~MainWindow(){
    //     if (m_renderer != nullptr) {
    //         SDL_DestroyRenderer(m_renderer);
    //         m_renderer = nullptr;
    //     }
    //     if (m_window != nullptr) {
    //         SDL_DestroyWindow(m_window);
    //         m_window = nullptr;
    //     }
    // }
};
#endif // MainWindowH
