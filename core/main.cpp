#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#ifdef SDL_PLATFORM_ANDROID
    #include <android/log.h>
#endif
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "ConstDef.h"
#ifdef SDL_PLATFORM_WINDOWS
    #include "DebugTrace.h"
#endif
#include "AudioSuite.h"
#include "Actor.h"
#include "Screen.h"
#include "TinyFS.h"

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*                                                       */
/* Remove this source, and replace with your SDL sources */
/*                                                       */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

//int main(int argc, char *argv[]) {
//    (void)argc;
//    (void)argv;
//    if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed (%s)", SDL_GetError());
//        return 1;
//    }
//
//    if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello World",
//                                 "!! Your SDL project successfully runs on Android !!", NULL)) {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ShowSimpleMessageBox failed (%s)", SDL_GetError());
//        return 1;
//    }
//
//    SDL_Quit();
//    return 0;
//}
#ifdef SDL_PLATFORM_ANDROID
    #define LOG_TAG "SDL_APP"
    #define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , LOG_TAG, __VA_ARGS__)
    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO , LOG_TAG, __VA_ARGS__)
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN , LOG_TAG, __VA_ARGS__)
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , LOG_TAG, __VA_ARGS__)
#endif

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

float g_displayWidth = SCREEN_WIDTH;
float g_displayHeight = SCREEN_HEIGHT;

#ifdef SDL_PLATFORM_WINDOWS
#ifdef _DEBUG
    DebugTrace *g_pDebugTrace = nullptr;
#endif
#endif
void debugTraceOutput(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
#ifdef SDL_PLATFORM_WINDOWS
#ifdef _DEBUG
    if (g_pDebugTrace == nullptr) {
        return;
    }
    g_pDebugTrace->m_pfDebugPrint("Category[%02d], priority[%02d]: %s", category, priority, message);
#endif
#endif
#ifdef SDL_PLATFORM_ANDROID
    LOGV("Category[%02d], priority[%02d]: %s", category, priority, message);
#endif
}
/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_FPoint targetPos = {0, 0};
static const SDL_DisplayMode *displayMode = nullptr;

EventQueue *g_eventQueue = nullptr;
unique_ptr<Screen> g_screen = nullptr;
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
#ifdef SDL_PLATFORM_WINDOWS
#ifdef _DEBUG
    g_pDebugTrace = new DebugTrace();
    g_pDebugTrace->Initial("Debug frame for Tetris");
    g_pDebugTrace->m_pfShowDebugForm();
#endif
#endif
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE); //设置日志级别
    SDL_SetLogOutputFunction(debugTraceOutput, nullptr);

    SDL_Log("SDL_AppInited......");

    // 禁止触摸事件转换为鼠标事件，避免一次触摸同时产生一欠触摸和一欠鼠标事件
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    SDL_Log("SDL_TOUCH_MOUSE_EVENTS = %s", SDL_GetHint(SDL_HINT_TOUCH_MOUSE_EVENTS));

    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); //设置窗口位置
    // // 开启垂直同步
    // if(!SDL_SetRenderVSync(renderer, 1)){
    //     SDL_Log("Couldn't set vsync: %s", SDL_GetError());
    // }

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    char* currentDirector = SDL_GetCurrentDirectory();
    SDL_Log("SDL_GetCurrentDirectory = %s", currentDirector);
    const char* baseDirector = SDL_GetBasePath();
    SDL_Log("SDL_GetBasePath = %s", baseDirector);
#ifdef SDL_PLATFORM_ANDROID
    const char* interalDirector = SDL_GetAndroidInternalStoragePath();
    SDL_Log("SDL_GetAndroidInternalStoragePath = %s", interalDirector);
    const char* cacheDirector = SDL_GetAndroidCachePath();
    SDL_Log("SDL_GetAndroidCachePath = %s", cacheDirector);

    unique_ptr<TinyFS> fsystem = make_unique<TinyFS>();
    auto flist = fsystem->getFileList(interalDirector, "");
    SDL_Log("interalDirector file list total:%zu", flist.size());
    for (auto &f : flist) {
        SDL_Log("file name = %s", f.string().c_str());
    }
#endif

    SDL_DisplayID displayId = SDL_GetPrimaryDisplay();
    if (displayId == 0) {
        SDL_Log("SDL_GetPrimaryDisplay Error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    displayMode = SDL_GetCurrentDisplayMode(displayId);
    if (displayMode == nullptr) {
        SDL_Log("SDL_GetCurrentDisplayMode Error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    g_displayWidth = (float)displayMode->w * displayMode->pixel_density;
    g_displayHeight = (float)displayMode->h * displayMode->pixel_density;

    g_screen = make_unique<Screen>(nullptr, SRect(0, 0, g_displayWidth, g_displayHeight), renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    // GameEvent arenaEvent;
    GameEvent *gameEvent = nullptr;
    SPoint *pos;
    int dx = 0;
    int dy = 0;
    switch(event->type) {
        case SDL_EVENT_TERMINATING:
            /*  Terminate the app.
                Shut everything down before returning from this function.
            */
            SDL_Log("Event------SDL_EVENT_TERMINATING");
            return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
        case SDL_EVENT_QUIT:
            SDL_Log("Event------SDL_EVENT_QUIT");
            return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
        case SDL_EVENT_WILL_ENTER_BACKGROUND:   //
            SDL_Log("Event------SDL_EVENT_WILL_ENTER_BACKGROUND");
            /* Prepare your app to go into the background.  Stop loops, etc.
            This gets called when the user hits the home button, or gets a call.

            You should not make any OpenGL graphics calls or use the rendering API,
            in addition, you should set the render target to NULL, if you're using
            it, e.g. call SDL_SetRenderTarget(renderer, NULL).
            */
            gameEvent = new GameEvent(EventName::Paused, 0);
            g_screen->inputControl(*gameEvent);
            // g_eventQueue->pushEventIntoQueue(gameEvent);
            break;
        case SDL_EVENT_DID_ENTER_FOREGROUND:
            SDL_Log("Event------SDL_EVENT_DID_ENTER_FOREGROUND");
            gameEvent = new GameEvent(EventName::Paused, 0);
            g_screen->inputControl(*gameEvent);
            break;
        case SDL_EVENT_FINGER_DOWN:
        case SDL_EVENT_FINGER_UP:
        // case SDL_EVENT_FINGER_MOTION:
        // case SDL_EVENT_FINGER_CANCELED:
            targetPos.x = event->tfinger.x * g_displayWidth * displayMode->pixel_density;
            targetPos.y = event->tfinger.y * g_displayHeight * displayMode->pixel_density;

            // 转为FINGER_DOWN等位置事件
            switch(event->type)
            {
                case SDL_EVENT_FINGER_DOWN:
                    pos = new SPoint(targetPos.x, targetPos.y);
                    gameEvent = new GameEvent(EventName::FINGER_DOWN, pos);
                    break;
                case SDL_EVENT_FINGER_UP:
                    pos = new SPoint(targetPos.x, targetPos.y);
                    gameEvent = new GameEvent(EventName::FINGER_UP, pos);
                    break;
                case SDL_EVENT_FINGER_MOTION:
                    pos = new SPoint(targetPos.x, targetPos.y);
                    gameEvent = new GameEvent(EventName::FINGER_MOTION, pos);
                    break;
            }
            if (gameEvent != nullptr) {
                g_screen->inputControl(*gameEvent);
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:        /**< Mouse moved */
            pos = new SPoint(event->motion.x, event->motion.y);
            gameEvent = new GameEvent(EventName::MOUSE_MOVING, pos);
            g_screen->inputControl(*gameEvent);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:   /**< Mouse button pressed */
            pos = new SPoint(event->button.x, event->button.y);
            SDL_Log("SDL_EVENT_MOUSE_BUTTON_DOWN @ {%f, %f}", event->button.x, event->button.y);
            switch(event->button.button){
                case SDL_BUTTON_LEFT:       /**< Left mouse button */
                    gameEvent = new GameEvent(EventName::MOUSE_LBUTTON_DOWN, pos);
                    break;
                case SDL_BUTTON_RIGHT:      /**< Right mouse button */
                    gameEvent = new GameEvent(EventName::MOUSE_RBUTTON_DOWN, pos);
                    break;
                case SDL_BUTTON_MIDDLE:     /**< Middle (wheel) mouse button */
                    gameEvent = new GameEvent(EventName::MOUSE_MBUTTON_DOWN, pos);
                    break;
                default:
                    delete pos;
                    break;
            }
            if (gameEvent != nullptr) {
                g_screen->inputControl(*gameEvent);
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:     /**< Mouse button released */
            pos = new SPoint(event->button.x, event->button.y);
            SDL_Log("SDL_EVENT_MOUSE_BUTTON_UP @ {%f, %f}", event->button.x, event->button.y);
            switch(event->button.button){
                case SDL_BUTTON_LEFT:       /**< Left mouse button */
                    gameEvent = new GameEvent(EventName::MOUSE_LBUTTON_UP, pos);
                    break;
                case SDL_BUTTON_RIGHT:      /**< Right mouse button */
                    gameEvent = new GameEvent(EventName::MOUSE_RBUTTON_UP, pos);
                    break;
                case SDL_BUTTON_MIDDLE:     /**< Middle (wheel) mouse button */
                    gameEvent = new GameEvent(EventName::MOUSE_MBUTTON_UP, pos);
                    break;
                default:
                    delete pos;
                    break;
            }
            if (gameEvent != nullptr) {
                g_screen->inputControl(*gameEvent);
            }
            break;

        case SDL_EVENT_KEY_DOWN:
            switch(event->key.key){
                case SDLK_DELETE:
                    gameEvent = new GameEvent(EventName::GridOnOff, 0);
                    g_screen->inputControl(*gameEvent);
                    break;
                case SDLK_S:
                    gameEvent = new GameEvent(EventName::Begin, 0);
                    g_screen->inputControl(*gameEvent);
                    break;
                case SDLK_SPACE:
                    gameEvent = new GameEvent(EventName::Rotate, 0);
                    g_screen->inputControl(*gameEvent);
                    break;
                case SDLK_P:
                    gameEvent = new GameEvent(EventName::Paused, 0);
                    g_screen->inputControl(*gameEvent);
                    break;
                case SDLK_DOWN:
                    gameEvent = new GameEvent(EventName::MoveDown, 0);
                    g_screen->inputControl(*gameEvent);
                    break;
                case SDLK_LEFT:
                    gameEvent = new GameEvent(EventName::MoveLeft, 0);
                    g_screen->inputControl(*gameEvent);
                    break;
                case SDLK_RIGHT:
                    gameEvent = new GameEvent(EventName::MoveRight, 0);
                    g_screen->inputControl(*gameEvent);
                    break;
                case SDLK_U:
                    gameEvent = new GameEvent(EventName::SpeedUp, 0);
                    g_screen->inputControl(*gameEvent);
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return g_screen->isExiting();
    // return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    GameEvent arenaEvent;
    // ConstDef::CURRENT_MS_TIMESTAMP = SDL_GetTicks();
    g_screen->eventLoopEntry();
    g_screen->update();

    arenaEvent = {EventName::Update, 0};
    g_screen->handleEvent(arenaEvent);

    /* clear the window to the draw color. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    g_screen->draw();

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);


    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

    // 这里要强制释放资源，因为要确保在后面调用TTF_Quit()之前，要把FontSuite打开的字体都关闭掉
    g_screen.reset();

#ifdef SDL_PLATFORM_WINDOWS
#ifdef _DEBUG
    if (g_pDebugTrace != nullptr){
        delete g_pDebugTrace;
        g_pDebugTrace = nullptr;
    }
#endif
#endif
    TTF_Quit();
    /* SDL will clean up the window/renderer for us. */
}
