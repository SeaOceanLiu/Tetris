#ifndef ArenaH
#define ArenaH
#include <filesystem>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <random>
#include <mutex>
#include <SDL3/SDL.h>

#include "ConstDef.h"
#include "EventQueue.h"
#include "AudioSuite.h"
#include "BrickPool.h"
#include "BlockGroup.h"
#include "Button.h"
// #include "FontSuite.h"
#include "Timer.h"
#include "StateMachine.h"
// #include "AnchorPoint.h"
#include "Animation.h"
#include "BlockMoveTracker.h"
#include "PhotoCarousel.h"
#include "Panel.h"
#include "MusicCarousel.h"

using namespace std;

enum class ButtonId: int{
    LeftBtn,
    RightBtn,
    DownBtn,
    RotateBtn,
    PlayBtn,
    PauseBtn,

    None
};

enum class SoundId: int{
    Move            = 0,
    Bomb            = 1,
    Pierce          = 2,
    Save            = 3,
    Speedup         = 4,
    LevelComplete   = 5,
    Excellent       = 6,
    GameOver        = 7,
    CantDo          = 8,
    Go,
    Good,
    Warning,
    Bombexplode,
    Multishot,

    BGM1     = 100,
    BGM2     = 101,
    None
};

enum class FontId: int{
    Version,
    Information,
    Paused,
    Failed,
    Gamebar,
    Debug
};

enum class CheckDirection: int{
    LEFT,
    RIGHT,
    DOWN
};

class RandomNumberGenerator {
    public:
        // 获取单例实例的静态方法
        static RandomNumberGenerator* getInstance();
        // 生成范围在 [min, max] 之间的随机整数
        int generate(int min, int max);

    private:
        // 私有构造函数
        RandomNumberGenerator();
        // 单例实例的静态方法
        static void initInstance();

        // 禁止拷贝构造函数和赋值操作符
        RandomNumberGenerator(const RandomNumberGenerator&) = delete;
        RandomNumberGenerator& operator=(const RandomNumberGenerator&) = delete;

        // 静态成员变量
        static RandomNumberGenerator* instance;
        static std::once_flag initInstanceFlag;
        static std::mutex mutex_;
        std::mt19937 rng_; // Mersenne Twister 随机数生成器
};

enum class State: int{
    SHOW_COVER,
    RUNNING,
    ACTING,  //动画执行中
    PAUSED,
    FAILED
};

class BlockPos{
public:
    // 当前位置
    int currentCol;
    int currentRow;
};

class Arena: public StateMachine<void *>, public Panel
{
private:
    bool m_isDrawGrid;

    int m_tickCount;
    Uint64 m_nextUpdateTick; // 下一次更新时间
    int m_speed;
    int m_totalScore;
    bool m_failed;
    int m_gradeScore;


    SRect m_playground;
    SRect m_hintPlace; // 提示方块位置
    SDL_Color m_bgColor;
    SDL_Color m_hintPlaceBgColor;
    SDL_Color m_gridColor;
    SDL_Color m_borderColor; // 边框颜色

    vector<vector<int>> m_screenBuffer;
    shared_ptr<BlockGroup>m_blockData;
    BlockPos m_blockPos;
    // 方块色号
    int m_currentBlockColor;
    int m_nextBlockColor;
    int m_nextBlockColOffset;
    // 方块组序号
    int m_currentBlockGroup;
    int m_nextBlockGroup;
    // 方块组旋转角度
    RotateAngle m_currentBlockAngle;
    RotateAngle m_nextBlockAngle;

    // 随机数生成器
    RandomNumberGenerator *m_rng;
    shared_ptr<BrickPool> m_brick;

    unordered_map<SoundId, unique_ptr<AudioSuite>> m_audioSuite;
    SoundId m_toPlaySoundId;
    shared_ptr<MusicCarousel> m_musicCarousel;
    Timer *m_frameCounter;
    int m_frameCount; // 帧计数器

    shared_ptr<Animation> m_rotateBtnAnimation;
    shared_ptr<Animation> m_blockAnimation;
    shared_ptr<BlockMoveTracker> m_blockMoveTracker;
    shared_ptr<Button> m_leftBtn;
    shared_ptr<Button> m_rightBtn;
    shared_ptr<Button> m_downBtn;
    shared_ptr<Button> m_rotateBtn;
    shared_ptr<Button> m_playBtn;
    shared_ptr<Button> m_pauseBtn;

    shared_ptr<Label> m_fpsLabel;
    shared_ptr<Label> m_speedLabel;
    shared_ptr<Label> m_scoreLabel;
    shared_ptr<Label> m_pauseLabel;
    shared_ptr<Label> m_failedLabel;
    shared_ptr<Label> m_debugLabel;
private:
    bool notInDrawingState(void);
public:
    Arena(Control *parent, SRect bgRect, float xScale=1.0f, float yScale=1.0f);
    ~Arena();
    // 状态迁入函数
    void showCoverEnter(State lastState);
    void runningEnter(State lastState);
    void actingEnter(State lastState);
    void failedEnter(State lastState);
    void pausedEnter(State lastState);
    // 状态迁出函数
    void showCoverExit(State nextState);
    void runningExit(State nextState);
    void actingExit(State nextState);
    void failedExit(State nextState);
    void pausedExit(State nextState);

    // 状态中的事件处理函数
    // 注意这里的入参，不能直接使用Event的子类，需要使用基类
    bool eventHandleInShowCoverState(Event<void*> &event);
    bool eventHandleInRunningState(Event<void*> &event);
    bool eventHandleInActingState(Event<void*> &event);
    bool eventHandleInFailedState(Event<void*> &event);
    bool eventHandleInPausedState(Event<void*> &event);

    // void eventLoopEntry(void);

    bool handleEvent(GameEvent &event) override;

    void setDrawGrid(bool isDraw);
    void draw(void) override;
    void drawBorder(void);
    void drawScreen(void);
    void drawPlayGround(void);
    void drawGrid(void);
    void getNextBlock(void);
    void drawBrick(int row, int col, int blockId);
    void drawNextBlocks(void);
    void setColor(SDL_Color color);
    void drawRect(float x, float y, float w, float h);
    void drawRect(SRect rect);
    void drawLine(float x1, float y1, float x2, float y2);

    void drawBlockGroup(void);
    bool checkFull(void);   // 检查是否满行
    void saveToScreen(void);
    bool isInScreen(int row, int col);
    bool isBrickExistOnScreen(int row, int col);
    bool isExistUnderfill(int row, int col);
    bool isBrickOverTheAir(int row, int col);
    bool checkDown(void);
    bool checkDirection(CheckDirection direction);
    int checkLine(void);
    void moveLeft(void);
    void moveRight(void);
    bool moveDown(void);
    void rotate(void);      // 旋转
    void newBlock(int *newBlock, RotateAngle *newAngle, int *newColor); // 获取新的方块
    void refreshBlockMoveAnimationActorGroup(void);
    void speedUp(void);
    void TriggerBlockMoveAnimation(MoveDirection moveDirection);
    void clean(void);

    void update(void) override;
    void onFrameCounter(void *userdata);
    void onBtnClick(shared_ptr<Button> button);
};

#endif