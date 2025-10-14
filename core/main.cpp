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
#include "MainWindow.h"
#include "AudioSuite.h"
#include "Actor.h"
#include "Screen.h"
// #include "TinyFS.h"

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


// float g_displayWidth = SCREEN_WIDTH;
// float g_displayHeight = SCREEN_HEIGHT;

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
// static SDL_Window *window = NULL;
// static SDL_Renderer *renderer = NULL;
static SDL_FPoint targetPos = {0, 0};
// static const SDL_DisplayMode *displayMode = nullptr;

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

    SDL_Log("SDL_App Initialed......");

    // 禁止触摸事件转换为鼠标事件，避免一次触摸同时产生一欠触摸和一欠鼠标事件
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    SDL_Log("SDL_TOUCH_MOUSE_EVENTS = %s", SDL_GetHint(SDL_HINT_TOUCH_MOUSE_EVENTS));

    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // MainWindow::getInstance()必须在SDL_Init之后，TTF_Init之前调用
    SSize displaySize = MainWindow::getInstance()->getDisplaySize();

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
#ifdef SDL_PLATFORM_ANDROID
    ConstDef::workforldPath = fs::path(std::string(SDL_GetPrefPath("SeaOcean.Ltd.", "Tetris")));
#endif

    g_screen = make_unique<Screen>(nullptr, SRect(0, 0, displaySize.width, displaySize.height), MainWindow::getInstance()->getRenderer());

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    shared_ptr<Event> gameEvent = nullptr;
    shared_ptr<SPoint> pos;
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
            g_screen->saveGame();
            gameEvent = make_shared<Event>(EventName::Paused, 0);
            g_screen->inputControl(gameEvent);
            break;
        case SDL_EVENT_DID_ENTER_FOREGROUND:
            SDL_Log("Event------SDL_EVENT_DID_ENTER_FOREGROUND");
            gameEvent = make_shared<Event>(EventName::Paused, 0);
            g_screen->inputControl(gameEvent);
            g_screen->loadAudioMusic();
            break;
        case SDL_EVENT_FINGER_DOWN:
        case SDL_EVENT_FINGER_UP:
        // case SDL_EVENT_FINGER_MOTION:
        // case SDL_EVENT_FINGER_CANCELED:
            targetPos.x = event->tfinger.x * MainWindow::getInstance()->getDisplayWidth();
            targetPos.y = event->tfinger.y * MainWindow::getInstance()->getDisplayHeight();

            // 转为FINGER_DOWN等位置事件
            switch(event->type)
            {
                case SDL_EVENT_FINGER_DOWN:
                    pos = make_shared<SPoint>(targetPos.x, targetPos.y);
                    gameEvent = make_shared<Event>(EventName::FINGER_DOWN, pos);
                    break;
                case SDL_EVENT_FINGER_UP:
                    pos = make_shared<SPoint>(targetPos.x, targetPos.y);
                    gameEvent = make_shared<Event>(EventName::FINGER_UP, pos);
                    break;
                case SDL_EVENT_FINGER_MOTION:
                    pos = make_shared<SPoint>(targetPos.x, targetPos.y);
                    gameEvent = make_shared<Event>(EventName::FINGER_MOTION, pos);
                    break;
            }
            if (gameEvent != nullptr) {
                g_screen->inputControl(gameEvent);
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:        /**< Mouse moved */
            pos = make_shared<SPoint>(event->motion.x, event->motion.y);
            gameEvent = make_shared<Event>(EventName::MOUSE_MOVING, pos);
            g_screen->inputControl(gameEvent);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:   /**< Mouse button pressed */
            pos = make_shared<SPoint>(event->button.x, event->button.y);
            SDL_Log("SDL_EVENT_MOUSE_BUTTON_DOWN @ {%f, %f}", event->button.x, event->button.y);
            switch(event->button.button){
                case SDL_BUTTON_LEFT:       /**< Left mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_LBUTTON_DOWN, pos);
                    break;
                case SDL_BUTTON_RIGHT:      /**< Right mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_RBUTTON_DOWN, pos);
                    break;
                case SDL_BUTTON_MIDDLE:     /**< Middle (wheel) mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_MBUTTON_DOWN, pos);
                    break;
                default:
                    break;
            }
            if (gameEvent != nullptr) {
                g_screen->inputControl(gameEvent);
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:     /**< Mouse button released */
            pos = make_shared<SPoint>(event->button.x, event->button.y);
            SDL_Log("SDL_EVENT_MOUSE_BUTTON_UP @ {%f, %f}", event->button.x, event->button.y);
            switch(event->button.button){
                case SDL_BUTTON_LEFT:       /**< Left mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_LBUTTON_UP, pos);
                    break;
                case SDL_BUTTON_RIGHT:      /**< Right mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_RBUTTON_UP, pos);
                    break;
                case SDL_BUTTON_MIDDLE:     /**< Middle (wheel) mouse button */
                    gameEvent = make_shared<Event>(EventName::MOUSE_MBUTTON_UP, pos);
                    break;
                default:
                    break;
            }
            if (gameEvent != nullptr) {
                g_screen->inputControl(gameEvent);
            }
            break;

        case SDL_EVENT_KEY_DOWN:
            switch(event->key.key){
                case SDLK_DELETE:
                    gameEvent = make_shared<Event>(EventName::GridOnOff, 0);
                    g_screen->inputControl(gameEvent);
                    break;
                case SDLK_S:
                    gameEvent = make_shared<Event>(EventName::Begin, 0);
                    g_screen->inputControl(gameEvent);
                    break;
                case SDLK_SPACE:
                    gameEvent = make_shared<Event>(EventName::Rotate, 0);
                    g_screen->inputControl(gameEvent);
                    break;
                case SDLK_P:
                    // g_screen->saveGame();
                    gameEvent = make_shared<Event>(EventName::Paused, 0);
                    g_screen->inputControl(gameEvent);
                    break;
                case SDLK_DOWN:
                    gameEvent = make_shared<Event>(EventName::MoveDown, 0);
                    g_screen->inputControl(gameEvent);
                    break;
                case SDLK_LEFT:
                    gameEvent = make_shared<Event>(EventName::MoveLeft, 0);
                    g_screen->inputControl(gameEvent);
                    break;
                case SDLK_RIGHT:
                    gameEvent = make_shared<Event>(EventName::MoveRight, 0);
                    g_screen->inputControl(gameEvent);
                    break;
                case SDLK_U:
                    gameEvent = make_shared<Event>(EventName::SpeedUp, 0);
                    g_screen->inputControl(gameEvent);
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
    shared_ptr<Event> arenaEvent;
    // ConstDef::CURRENT_MS_TIMESTAMP = SDL_GetTicks();
    g_screen->eventLoopEntry();
    g_screen->update();

    arenaEvent = make_shared<Event>(EventName::Update, 0);
    g_screen->handleEvent(arenaEvent);

    /* clear the window to the draw color. */
    SDL_SetRenderDrawColor(MainWindow::getInstance()->getRenderer(), 0, 0, 0, 255);
    SDL_RenderClear(MainWindow::getInstance()->getRenderer());

    g_screen->draw();

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(MainWindow::getInstance()->getRenderer());


    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

    // 这里要强制释放资源，因为要确保在后面调用TTF_Quit()之前，要把FontSuite打开的字体都关闭掉
    g_screen.reset();
    // 线程需要显式detach，否则Android下会报泄漏
    ResourceLoader::getInstance()->detachLoadingThread();

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
