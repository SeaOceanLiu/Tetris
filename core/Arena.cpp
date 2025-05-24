#include "Arena.h"

// 初始化静态成员变量
RandomNumberGenerator* RandomNumberGenerator::instance = nullptr;
std::once_flag RandomNumberGenerator::initInstanceFlag;
std::mutex RandomNumberGenerator::mutex_;

// 获取单例实例的静态方法
RandomNumberGenerator* RandomNumberGenerator::getInstance() {
    std::call_once(initInstanceFlag, &RandomNumberGenerator::initInstance);
    return instance;
}

// 生成范围在 [min, max] 之间的随机整数
int RandomNumberGenerator::generate(int min, int max) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng_);
}

// 私有构造函数
RandomNumberGenerator::RandomNumberGenerator():
    rng_(std::random_device{}())
{
    // 初始化随机数生成器
}

// 初始化单例实例的静态方法
void RandomNumberGenerator::initInstance() {
    instance = new RandomNumberGenerator();
}

Arena::Arena(Control *parent, SRect bgRect, float xScale, float yScale):
    StateMachine(State::SHOW_COVER),
    Panel(parent, bgRect, xScale, yScale),
    m_isDrawGrid(true),
    m_tickCount(0),
    m_nextUpdateTick(0),
    m_speed(ConstDef::MAX_SLOW_SPEED),
    m_totalScore(0),
    m_gradeScore(0),
    m_failed(false),
    m_musicCarousel(nullptr),
    m_frameCount(0),
    m_frameCounter(nullptr),
    m_rotateBtnAnimation(nullptr),
    m_blockMoveTracker(nullptr),
    m_bgColor({192, 192,192, 128}), // 使用半透明背景色
    m_hintPlaceBgColor({168, 168, 168, 128}), // 使用半透明背景色
    m_gridColor({128, 128, 128, SDL_ALPHA_OPAQUE}),
    m_borderColor({255, 255, 255, SDL_ALPHA_OPAQUE}),
    m_screenBuffer((int)ConstDef::PG.height, vector<int>((int)ConstDef::PG.width, ConstDef::SCREEN_EMPTY_COLOR)),
    m_brick(nullptr),
    m_blockData(nullptr),
    m_currentBlockColor(-1),
    m_nextBlockColor(-1),
    m_currentBlockGroup(-1),
    m_nextBlockGroup(-1),
    m_nextBlockColOffset(-1),
    m_currentBlockAngle(RotateAngle::DEGREE0),
    m_nextBlockAngle(RotateAngle::DEGREE0),
    m_toPlaySoundId(SoundId::None),
    m_leftBtn(nullptr),
    m_rightBtn(nullptr),
    m_downBtn(nullptr),
    m_rotateBtn(nullptr),
    m_playBtn(nullptr),
    m_pauseBtn(nullptr),
    m_fpsLabel(nullptr),
    m_speedLabel(nullptr),
    m_scoreLabel(nullptr),
    m_pauseLabel(nullptr),
    m_failedLabel(nullptr),
    m_debugLabel(nullptr)
{
    SDL_Log("Start to Arena init......");
    setParent(parent);
    setTransparent(true);

    // 初始化随机数生成器
    m_rng = RandomNumberGenerator::getInstance();

    if (!SDL_SetRenderDrawBlendMode(getRenderer(), SDL_BLENDMODE_BLEND)) {
        SDL_Log("Failed to set blend mode: %s", SDL_GetError());
    }

    SDL_Log("Loading Brick actor......");
    m_brick = make_shared<BrickPool>(this, ConstDef::pathPrefix);

    SRect brickRect = m_brick->getBrick(0)->getRect();
    ConstDef::SINGLE_BLOCK_SIZE = {brickRect.width, brickRect.height};
    SDL_Log("Setting orig ConstDef::SINGLE_BLOCK_SIZE to (%f, %f)......", ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height);

    m_playground = mapToDrawRect({ConstDef::PG.left * m_xScale, ConstDef::PG.top * m_yScale,
        ConstDef::SINGLE_BLOCK_SIZE.width * ConstDef::PG.width,
        ConstDef::SINGLE_BLOCK_SIZE.height * ConstDef::PG.height});
    SDL_Log("ConstDef::PG.left = %f, ConstDef::PG.width = %f", ConstDef::PG.left, ConstDef::PG.width);
    SDL_Log("Setting playground to (%f, %f, %f, %f)......", m_playground.left, m_playground.top, m_playground.width, m_playground.height);

    m_hintPlace = mapToDrawRect({ConstDef::HINT_PLACE.left * m_xScale, ConstDef::HINT_PLACE.top * m_yScale,
            ConstDef::SINGLE_BLOCK_SIZE.width * ConstDef::HINT_PLACE.width,
            ConstDef::SINGLE_BLOCK_SIZE.height * ConstDef::HINT_PLACE.height});
    SDL_Log("Setting hintPlace to (%f, %f, %f, %f)......", m_hintPlace.left, m_hintPlace.top, m_hintPlace.width, m_hintPlace.height);

    SDL_Log("Loading BlockData......");
    m_blockData = make_shared<BlockGroup>(this, ConstDef::pathPrefix);

    /**************************************************** 添加界面文本 *****************************************************/
    addControl(m_fpsLabel = LabelBuilder(this, ConstDef::FPS_TEXT_POS)
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setFont(FontName::HarmonyOS_Sans_SC_Thin)
                    .setFontSize(ConstDef::GAME_BAR_TEXT_FONT_SIZE)
                    .setCaption("FPS: 0")
                    .setAlignmentMode(AlignmentMode::AM_MID_LEFT)
                    .build());
    addControl(m_speedLabel = LabelBuilder(this, ConstDef::SPEED_TEXT_POS)
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setFont(FontName::HarmonyOS_Sans_SC_Thin)
                    .setFontSize(ConstDef::GAME_BAR_TEXT_FONT_SIZE)
                    .setCaption("Speed: 1")
                    .setAlignmentMode(AlignmentMode::AM_BOTTOM_RIGHT)
                    .build());
    addControl(m_scoreLabel = LabelBuilder(this, ConstDef::SCORE_TEXT_POS)
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setFont(FontName::HarmonyOS_Sans_SC_Thin)
                    .setFontSize(ConstDef::GAME_BAR_TEXT_FONT_SIZE)
                    .setCaption("Score: 0")
                    .setAlignmentMode(AlignmentMode::AM_BOTTOM_CENTER)
                    .build());

    addControl(m_pauseLabel = LabelBuilder(this, ConstDef::PAUSED_TEXT_POS)
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setShadowColor({255, 255, 255, SDL_ALPHA_OPAQUE})
                    .setShadow(true)
                    .setFont(FontName::Quando_Regular)
                    .setFontSize(ConstDef::PAUSED_TEXT_FONT_SIZE)
                    .setCaption("Paused")
                    .setAlignmentMode(AlignmentMode::AM_CENTER)
                    .build());
    m_pauseLabel->hide();

    addControl(m_failedLabel = LabelBuilder(this, ConstDef::FAILED_TEXT_POS)
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setShadowColor({255, 255, 255, SDL_ALPHA_OPAQUE})
                    .setShadow(true)
                    .setFont(FontName::Quando_Regular)
                    .setFontSize(ConstDef::FAILED_TEXT_FONT_SIZE)
                    .setCaption("You failed!")
                    .setAlignmentMode(AlignmentMode::AM_CENTER)
                    .build());
    m_failedLabel->hide();

    addControl(m_debugLabel = LabelBuilder(this, ConstDef::DEBUG_TEXT_POS)
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setShadowColor({255, 255, 255, SDL_ALPHA_OPAQUE})
                    .setShadow(true)
                    .setShadowOffset({1, 1})
                    .setFont(FontName::HarmonyOS_Sans_SC_Thin)
                    .setFontSize(ConstDef::DEBUG_INFO_FONT_SIZE)
                    .setCaption(u8"下落速度(ms): " + std::to_string(ConstDef::LEVEL_SPEED_MAPPING[m_speed]))
                    .setAlignmentMode(AlignmentMode::AM_MID_LEFT)
                    .build());
    // m_debugLabel->hide();

    /**************************************************** 添加界面按钮 *****************************************************/
    addControl(m_leftBtn = ButtonBuilder(this, ConstDef::LEFT_BUTTON_POS)//SRect(0, 505, 48, 48))
                .setBtnNormalStateActor(    make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "left.png"))
                .setBtnHoverStateActor(     make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "left_hover.png"))
                .setBtnPressedStateActor(   make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "left_pressed.png"))
                .setOnClick(std::bind(&Arena::onBtnClick, this, std::placeholders::_1))
                .setTransparent(true)
                .setId(static_cast<int>(ButtonId::LeftBtn))
                .build());
    addControl(m_rightBtn = ButtonBuilder(this, ConstDef::RIGHT_BUTTON_POS)//SRect(48 + 5, 505, 48, 48))
                .setBtnNormalStateActor(    make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "right.png"))
                .setBtnHoverStateActor(     make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "right_hover.png"))
                .setBtnPressedStateActor(   make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "right_pressed.png"))
                .setOnClick(std::bind(&Arena::onBtnClick, this, std::placeholders::_1))
                .setTransparent(true)
                .setId(static_cast<int>(ButtonId::RightBtn))
                .build());
    addControl(m_downBtn = ButtonBuilder(this, ConstDef::DOWN_BUTTON_POS)//SRect((48 + 5)/2, 48 + 510, 48, 48))
                .setBtnNormalStateActor(    make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "down.png"))
                .setBtnHoverStateActor(     make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "down_hover.png"))
                .setBtnPressedStateActor(   make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "down_pressed.png"))
                .setOnClick(std::bind(&Arena::onBtnClick, this, std::placeholders::_1))
                .setTransparent(true)
                .setId(static_cast<int>(ButtonId::DownBtn))
                .build());
    addControl(m_rotateBtn = ButtonBuilder(this, ConstDef::ROTATE_BUTTON_POS)
                .setAnimation(AnimationBuilder(this, 33)
                                // .addPhotosInPathAsFrames(ConstDef::pathPrefix / "images" / "rotate", ".png", true)  // 尚未研究明白跑在Android下怎么遍历assets下的文件，所以暂不使用这种方式
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate000.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate005.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate010.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate015.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate020.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate025.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate030.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate035.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate040.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate045.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate050.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate055.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate060.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate065.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate070.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate075.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate080.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate085.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate090.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate095.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate100.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate105.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate110.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate115.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate120.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate125.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate130.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate135.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate140.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate145.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate150.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate155.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate160.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate165.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate170.png", true))
                                            .build())
                                .addFrame(ActorGroupBuilder(this)
                                            .addActor({0, 0}, make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "rotate" / "rotate175.png", true))
                                            .build())
                                .setLoop(true)
                                .setStartFrame(0)
                                .build())
                .setOnClick(std::bind(&Arena::onBtnClick, this, std::placeholders::_1))
                .setTransparent(true)
                .setId(static_cast<int>(ButtonId::RotateBtn))
                .build());
    addControl(m_playBtn = ButtonBuilder(this, ConstDef::PLAY_PAUSE_BUTTON_POS)
                .setBtnNormalStateActor(make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "play.png", true))
                .setOnClick(std::bind(&Arena::onBtnClick, this, std::placeholders::_1))
                .setTransparent(true)
                .setId(static_cast<int>(ButtonId::PlayBtn))
                .build());
    m_playBtn->hide();
    addControl(m_pauseBtn = ButtonBuilder(this, ConstDef::PLAY_PAUSE_BUTTON_POS)
                .setBtnNormalStateActor(make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "pause.png", true))
                .setOnClick(std::bind(&Arena::onBtnClick, this, std::placeholders::_1))
                .setTransparent(true)
                .setId(static_cast<int>(ButtonId::PauseBtn))
                .build());
    m_pauseBtn->show();

    /**************************************************** 初始化方块移动动画 *****************************************************/
    m_blockAnimation = make_shared<Animation>(this, 2);
    m_blockMoveTracker = make_shared<BlockMoveTracker>(nullptr, SPoint(ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height));
    m_blockAnimation->setTracker(m_blockMoveTracker);
    SDL_Log("Animation(%p)::setTracker %p", (void *)m_blockAnimation.get(), (void *)m_blockMoveTracker.get());

    /******************************************************* 添加游戏音效 ********************************************************/
    m_audioSuite[SoundId::Move   ]          = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "Move.wav");
    m_audioSuite[SoundId::Bomb   ]          = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "Bomb.wav");
    m_audioSuite[SoundId::Pierce ]          = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "Pierce.wav");
    m_audioSuite[SoundId::Save   ]          = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "Save.wav");
    m_audioSuite[SoundId::Speedup]          = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "Speedup.wav");
    m_audioSuite[SoundId::LevelComplete]    = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "LevelComplete.wav");
    m_audioSuite[SoundId::Excellent]        = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "Excellent.wav");
    m_audioSuite[SoundId::GameOver]         = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "GameOver.wav");
    m_audioSuite[SoundId::CantDo]           = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "CantDo.wav");
    m_audioSuite[SoundId::Go]               = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "Go.wav");
    m_audioSuite[SoundId::Good]             = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "Good.wav");
    m_audioSuite[SoundId::Warning]          = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "Warning.wav");
    m_audioSuite[SoundId::Bombexplode]      = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "BombExplode.wav");
    m_audioSuite[SoundId::Multishot]        = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "sounds" / "MultiShot.wav");

    // m_audioSuite[SoundId::BGM1] = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "music" / "mainmenu.wav");
    // m_audioSuite[SoundId::BGM2] = make_unique<AudioSuite>(this, ConstDef::pathPrefix / "music" / "the flowbots drum and bass.wav");

    /******************************************************* 添加游戏背景音乐 *******************************************************/
    m_musicCarousel = make_shared<MusicCarousel>(this);
    // m_musicCarousel->addMusicFile(ConstDef::pathPrefix / "music" / "mainmenu.wav");
    // m_musicCarousel->addMusicFile(ConstDef::pathPrefix / "music" / "the flowbots drum and bass.wav");

    m_musicCarousel->addMusicFile(ConstDef::pathPrefix / "music" / "3 am West End.wav");
    m_musicCarousel->addMusicFile(ConstDef::pathPrefix / "music" / "Beat One.wav");
    m_musicCarousel->addMusicFile(ConstDef::pathPrefix / "music" / "Fright Night Twist.wav");
    m_musicCarousel->addMusicFile(ConstDef::pathPrefix / "music" / "Goodnightmare.wav");
    m_musicCarousel->addMusicFile(ConstDef::pathPrefix / "music" / "Palm and Soul.wav");
    m_musicCarousel->addMusicFile(ConstDef::pathPrefix / "music" / "Take the Ride.wav");
    addControl(m_musicCarousel);

    /******************************************************** 创建帧率统计定时器 ********************************************************/
    m_frameCounter = new Timer();

    /**************************************************** 注册状态机及对应的事件处理 *****************************************************/
    registerEnterStateHandler(State::SHOW_COVER,    std::bind(&Arena::showCoverEnter, this, std::placeholders::_1));
    registerEnterStateHandler(State::RUNNING,       std::bind(&Arena::runningEnter, this, std::placeholders::_1));
    registerEnterStateHandler(State::ACTING,        std::bind(&Arena::actingEnter, this, std::placeholders::_1));
    registerEnterStateHandler(State::PAUSED,        std::bind(&Arena::pausedEnter, this, std::placeholders::_1));
    registerEnterStateHandler(State::FAILED,        std::bind(&Arena::failedEnter, this, std::placeholders::_1));

    registerLeaveStateHandler(State::SHOW_COVER,    std::bind(&Arena::showCoverExit, this, std::placeholders::_1));
    registerLeaveStateHandler(State::RUNNING,       std::bind(&Arena::runningExit, this, std::placeholders::_1));
    registerLeaveStateHandler(State::ACTING,        std::bind(&Arena::actingExit, this, std::placeholders::_1));
    registerLeaveStateHandler(State::PAUSED,        std::bind(&Arena::pausedExit, this, std::placeholders::_1));
    registerLeaveStateHandler(State::FAILED,        std::bind(&Arena::failedExit, this, std::placeholders::_1));

    registerStateEventHandler(State::SHOW_COVER,    std::bind(&Arena::eventHandleInShowCoverState, this, std::placeholders::_1));
    registerStateEventHandler(State::RUNNING,       std::bind(&Arena::eventHandleInRunningState, this, std::placeholders::_1));
    registerStateEventHandler(State::ACTING,        std::bind(&Arena::eventHandleInActingState, this, std::placeholders::_1));
    registerStateEventHandler(State::PAUSED,        std::bind(&Arena::eventHandleInPausedState, this, std::placeholders::_1));
    registerStateEventHandler(State::FAILED,        std::bind(&Arena::eventHandleInFailedState, this, std::placeholders::_1));

    SDL_Log("BrickGroup init success with total %d blocks", m_blockData->getBlockCount());
}
Arena::~Arena()
{
    if (m_frameCounter != nullptr){
        m_frameCounter->stop();
        delete m_frameCounter;
        m_frameCounter = nullptr;
    }

    // m_actors.clear();
    m_audioSuite.clear();
}

void Arena::showCoverEnter(State lastState){
    // do something
    SDL_Log("showCoverEnter %d", static_cast<State>(lastState));
}
void Arena::runningEnter(State lastState){
    // do something
    // SDL_Log("runningEnter %d", static_cast<State>(lastState));

    if (lastState == State::PAUSED || lastState == State::ACTING){
        return;
    }
    m_audioSuite[SoundId::Go]->play(false);

    clean();
    getNextBlock();
    // m_audioSuite[SoundId::BGM1]->play(false);
    m_musicCarousel->play();

    m_frameCounter->start(std::bind(&Arena::onFrameCounter, this, std::placeholders::_1), this, 1000, true);
    resetBackground();
}
void Arena::actingEnter(State lastState){
    // do something
    // SDL_Log("actingEnter %d", static_cast<State>(lastState));
}
void Arena::failedEnter(State lastState){
    // do something
    SDL_Log("failedEnter %d", static_cast<State>(lastState));
    m_audioSuite[SoundId::Warning]->pause();
    m_audioSuite[SoundId::GameOver]->play(false);
    m_failedLabel->show();
}
void Arena::pausedEnter(State lastState){
    // do something
    SDL_Log("pausedEnter %d", static_cast<State>(lastState));
    m_pauseLabel->show();
    m_playBtn->show();
    m_pauseBtn->hide();
}


void Arena::showCoverExit(State nextState){
    // do something
    SDL_Log("showCoverExit %d", static_cast<State>(nextState));
}
void Arena::runningExit(State nextState){
    // do something
    // SDL_Log("runningExit %d", static_cast<State>(nextState));
    // m_rotateBtnAnimation->pause();
}
void Arena::actingExit(State nextState){
    // do something
    // SDL_Log("actingExit %d", static_cast<State>(nextState));
}
void Arena::failedExit(State nextState){
    // do something
    SDL_Log("failedExit %d", static_cast<State>(nextState));
    m_failedLabel->hide();
}
void Arena::pausedExit(State nextState){
    // do something
    SDL_Log("pausedExit %d", static_cast<State>(nextState));
    m_pauseLabel->hide();
    m_playBtn->hide();
    m_pauseBtn->show();
}

bool Arena::eventHandleInShowCoverState(Event<void *> &event){
    // do something
    // SDL_Log("eventHandleInInitialState handle event:%d, param:%d", static_cast<int>(event.m_eventName), event.m_eventParam);
    switch(event.m_eventName){
        case EventName::Begin:
            setState(State::RUNNING);
            return true;
        default:
            break;
    }
    return Panel::handleEvent(static_cast<GameEvent &>(event));
}
bool Arena::eventHandleInRunningState(Event<void *> &event){
    // do something
    // SDL_Log("eventHandleInRunningState handle event:%d, param:%d", static_cast<int>(event.m_eventName), event.m_eventParam);
    switch(event.m_eventName){
        case EventName::GridOnOff:
            setDrawGrid(!m_isDrawGrid);
            return true;
        case EventName::Fillfull:
            setState(State::FAILED);
            return true;
        case EventName::MoveDown:
            moveDown();
            return true;
        case EventName::MoveLeft:
            moveLeft();
            return true;
        case EventName::MoveRight:
            moveRight();
            return true;
        case EventName::Rotate:
            rotate();
            return true;
        case EventName::Draw:
            draw();
            return true;
        case EventName::Paused:
            setState(State::PAUSED);
            return true;
        case EventName::Update:
            update();
            return true;
        case EventName::Timer:
            // m_fontSuite[FontId::Gamebar]->setStaticText("fps", u8"FPS: " + std::to_string(m_frameCount));
            m_fpsLabel->setCaption(u8"FPS: " + std::to_string(m_frameCount));
            m_frameCount = 0;
            return true;
        case EventName::AudioEnded:
            // if (event.m_eventParam == m_audioSuite[SoundId::BGM1].get()){
            //     m_audioSuite[SoundId::BGM2]->play(false);
            //     return true;
            // } else if (event.m_eventParam == m_audioSuite[SoundId::BGM2].get()){
            //     m_audioSuite[SoundId::BGM1]->play(false);
            //     return true;
            // }
            break;
        case EventName::SpeedUp:
            speedUp();
        default:
            break;
    }
    return Panel::handleEvent(static_cast<GameEvent &>(event));
}
bool Arena::eventHandleInActingState(Event<void *> &event){
    // do something
    // SDL_Log("eventHandleInActingState handle event:%d, param:%d", static_cast<int>(event.m_eventName), event.m_eventParam);
    switch(event.m_eventName){
        case EventName::Draw:
            draw();
            return true;
        case EventName::Update:
            update();
            return true;
        case EventName::AnimationEnded:
            if (event.m_eventParam == m_blockAnimation.get()) {
                setState(State::RUNNING);
                return true;
            }
            break;
        default:
            break;
    }
    return Panel::handleEvent(static_cast<GameEvent &>(event));
}
bool Arena::eventHandleInFailedState(Event<void *> &event){
    // do something
    // SDL_Log("eventHandleInFailedState handle event:%d, param:%d", static_cast<int>(event.m_eventName), event.m_eventParam);
    switch(event.m_eventName){
        case EventName::Draw:
            draw();
            // m_fontSuite[FontId::Failed]->draw();
            return true;
        case EventName::Begin:
            setState(State::RUNNING);
            return  true;
        default:
            break;
    }
    return Panel::handleEvent(static_cast<GameEvent &>(event));
}
bool Arena::eventHandleInPausedState(Event<void *> &event){
    // do something
    // SDL_Log("eventHandleInPausedState handle event:%d, param:%d", static_cast<int>(event.m_eventName), event.m_eventParam);
    switch(event.m_eventName){
        case EventName::Draw:
            draw();
            // m_fontSuite[FontId::Paused]->draw();
            return true;
        case EventName::Paused:
            setState(State::RUNNING);
            return true;
        case EventName::AudioEnded:
            // if (event.m_eventParam == m_audioSuite[SoundId::BGM1].get()){
            //     m_audioSuite[SoundId::BGM2]->play(false);
            //     return true;
            // } else if (event.m_eventParam == m_audioSuite[SoundId::BGM2].get()){
            //     m_audioSuite[SoundId::BGM1]->play(false);
            //     return true;
            // }
            break;
        default:
            break;
    }
    return Panel::handleEvent(static_cast<GameEvent &>(event));
}

void Arena::setDrawGrid(bool isDraw){
    m_isDrawGrid = isDraw;
}

void Arena::draw(void){
    m_frameCount++;
    if (notInDrawingState()){
        return;
    }

    //先画游戏区
    drawPlayGround();
    drawGrid();
    drawScreen();
    //在提示区绘制下一个方块
    drawNextBlocks();
    //在游戏区上绘制当前方块组
    drawBlockGroup();
    drawBorder();

    // 绘制子控件
    Panel::draw();
}

bool Arena:: notInDrawingState(void){
    return m_currentState == State::SHOW_COVER;
}

bool Arena::handleEvent(GameEvent &event){
    return stateEvent(event);
}

void Arena::drawBorder(void){
    setColor(m_borderColor);
    if(!SDL_RenderRect(getRenderer(), m_playground.toSDLFRect())) {
        SDL_Log("drawBorder failed: %s", SDL_GetError());
    }
}
// 画定位线
void Arena::drawGrid(void){
    if (m_isDrawGrid){
        SRect gridRect = m_playground;
        setColor(m_gridColor);
        for (int row = 0; row <= ConstDef::PG.height; row++){
            float x1 = gridRect.left;
            float x2 = gridRect.left + gridRect.width;
            float y = gridRect.top + ConstDef::SINGLE_BLOCK_SIZE.height * m_yyScale * row;
            drawLine(x1, y, x2, y);
        }
        for (int col = 0; col <= ConstDef::PG.width; col++){
            float x = gridRect.left + ConstDef::SINGLE_BLOCK_SIZE.width * m_xxScale * col;
            float y1 = gridRect.top;
            float y2 = gridRect.top + gridRect.height;
            drawLine(x, y1, x, y2);
        }
    }
}
void Arena::drawPlayGround(void){
    //画整个游戏区绘制背景
    setColor(m_bgColor);
    drawRect(m_playground);

    //在提示区绘制背景
    setColor(m_hintPlaceBgColor);
    drawRect(m_hintPlace);
}
void Arena::drawScreen(void){
    // 画已落到地面的方块
    for (int row = 0; row < ConstDef::PG.height; row++){
        for (int col = 0; col < ConstDef::PG.width; col++){
            if (m_screenBuffer[row][col] != ConstDef::SCREEN_EMPTY_COLOR){
                drawBrick(row, col, m_screenBuffer[row][col]);
            }
        }
    }
}

void Arena::refreshBlockMoveAnimationActorGroup(void){
    int RowCount = m_blockData->getRowCount();
    int colCount = m_blockData->getColCount();

    auto singleGroup = m_blockData->m_blockGroups[m_currentBlockGroup];

    // auto actorGroup = make_shared<ActorGroup>(this);
    auto actorGroup = ActorGroupBuilder(this).build();
    shared_ptr<Actor> newBrick;

    float offsetX = 0;
    float offsetY = 0;
    for (int row = 0; row < RowCount; row++){
        for (int col = 0; col < colCount; col++){
            auto blockBodyInfo = singleGroup->getBodyInfo(m_currentBlockAngle);
            int b = blockBodyInfo->body[row][col];
            if (b != 0){
                offsetX = col * ConstDef::SINGLE_BLOCK_SIZE.width;
                offsetY = row * ConstDef::SINGLE_BLOCK_SIZE.height;
                switch(singleGroup->getType()){
                    case BlockType::BOMB:
                        actorGroup->addActor({offsetX, offsetY}, m_brick->getBrick(ConstDef::SCREEN_BOMB_COLOR));
                        m_currentBlockColor = ConstDef::SCREEN_BOMB_COLOR;
                        m_toPlaySoundId = SoundId::Bomb;
                        break;
                    case BlockType::PIERCE:
                        actorGroup->addActor({offsetX, offsetY}, m_brick->getBrick(ConstDef::SCREEN_CROSS_COLOR));
                        m_currentBlockColor = ConstDef::SCREEN_CROSS_COLOR;
                        m_toPlaySoundId = SoundId::Pierce;
                        break;
                    case BlockType::NORMAL:
                    default:
                        actorGroup->addActor({offsetX, offsetY}, m_brick->getBrick(m_currentBlockColor));
                        m_toPlaySoundId = SoundId::Move;
                        break;
                }
            }
        }
    }
    float initialX = m_blockPos.currentCol * ConstDef::SINGLE_BLOCK_SIZE.width;
    float initialY = m_blockPos.currentRow * ConstDef::SINGLE_BLOCK_SIZE.height;
    m_blockAnimation->freeFrames(m_brick.get());
    m_blockAnimation->addFrame(actorGroup);
    m_blockAnimation->prepare(initialX, initialY);
    m_blockAnimation->show();
}


void Arena::getNextBlock(void){
    if (m_nextBlockGroup < 0){
        newBlock(&m_nextBlockGroup, &m_nextBlockAngle, &m_nextBlockColor);
    }
    auto nextSingleGroup = m_blockData->m_blockGroups[m_nextBlockGroup];
    auto nextBlockBodyInfo = nextSingleGroup->getBodyInfo(m_nextBlockAngle);
    // 计算居中偏移
    m_nextBlockColOffset = int((ConstDef::PG.width - (nextBlockBodyInfo->m_rightCol - nextBlockBodyInfo->m_leftCol + 1)) / 2) - nextBlockBodyInfo->m_leftCol;

    m_blockPos.currentCol = m_nextBlockColOffset; // ConstDef::HINT_PLACE_START_COL;
    m_blockPos.currentRow = 0;

    m_currentBlockGroup = m_nextBlockGroup;
    m_currentBlockColor = m_nextBlockColor;
    m_currentBlockAngle = m_nextBlockAngle;
    m_nextBlockGroup = -1;

    refreshBlockMoveAnimationActorGroup();
}

void Arena::setColor(SDL_Color color){
    if(!SDL_SetRenderDrawColor(getRenderer(), color.r, color.g, color.b, color.a)){
        SDL_Log("Failed to set grid render color: %s", SDL_GetError());
    }
}
void Arena::drawRect(SRect rect){
    if (!SDL_RenderFillRect(getRenderer(), rect.toSDLFRect())){
        SDL_Log("Failed to fill render rect: %s", SDL_GetError());
    }
}
void Arena::drawRect(float x, float y, float w, float h){
    SRect rect(x, y, w, h);
    drawRect(rect);
}
void Arena::drawLine(float x1, float y1, float x2, float y2){
    if (!SDL_RenderLine(getRenderer(), x1, y1, x2, y2)){
        SDL_Log("Failed to draw line: %s", SDL_GetError());
    }
}

// 在playground上以行列的形式画指定的一个方块
void Arena::drawBrick(int row, int col, int blockId){
    if(row < 0 || col < 0 || row >= ConstDef::PG.height || col >= ConstDef::PG.width){
        return;
    }
    float x = col * ConstDef::SINGLE_BLOCK_SIZE.width;
    float y = row * ConstDef::SINGLE_BLOCK_SIZE.height;
    if (y >= 0 && x >= 0){
        m_brick->draw(x, y, blockId);
    }
}

// 在playground上以行列的形式，用指定的颜色索引画指定的一组方块
void Arena::drawBlockGroup(void){
    if (m_blockAnimation == nullptr){
        return;
    }
    m_blockAnimation->draw();
}

bool Arena::checkFull(void){
    for (int row = ConstDef::PG_START_ROW - 1; row >0 ; row--){
        for (int col = 0; col < ConstDef::PG.width; col++){
            if(isBrickExistOnScreen(row, col)){
                return true;
            }
        }
    }
    return false;
}

// 判断方块是否位于creenBuffer上方
bool Arena::isBrickOverTheAir(int row, int col){
    if (row < ConstDef::PG_START_ROW && col >= 0 && col < ConstDef::PG.width){
        return true;
    }
    return false;
}

void Arena::rotate(void){
    bool roateFlag = true;

    if (m_currentBlockGroup < 0){
        return;
    }

    int rowCount = m_blockData->getRowCount();
    int colCount = m_blockData->getColCount();

    auto singleBlockGroup = m_blockData->m_blockGroups[m_currentBlockGroup];
    auto currentBlockBodyInfo = singleBlockGroup->getBodyInfo(m_currentBlockAngle);
    auto rotatedBlockBodyInfo = singleBlockGroup->getRotated(m_currentBlockAngle);

    for (int row = 0; row < rowCount; row++){
        for (int col = 0; col < colCount; col++){
            if (rotatedBlockBodyInfo->body[row][col] !=0){
                int screenRow = m_blockPos.currentRow + row;
                int screenCol = m_blockPos.currentCol + col;
                // 对方块旋转后位于ScreenBuffer上方的情况做特殊处理，这时应是允许旋转的
                if (isBrickOverTheAir(screenRow, screenCol)){
                    continue;
                }
                if(!isInScreen(screenRow, screenCol) || isBrickExistOnScreen(screenRow, screenCol)){
                    roateFlag = false;
                    break;
                }
            }
        }
        if (roateFlag == false){
            break;
        }
    }

    if (roateFlag == true){
        m_audioSuite[SoundId::Move]->play();
        m_currentBlockAngle = EnumHelper::rotate(m_currentBlockAngle);
        refreshBlockMoveAnimationActorGroup();
    } else {
        m_audioSuite[SoundId::CantDo]->play();
    }
}

void Arena::newBlock(int *newBlock, RotateAngle *newAngle, int *newColor){
    int newBlockRange = m_blockData->getBlockCount();
    int newColorRange = m_brick->getBrickCount();
    int newRotateRange = 4;

    *newBlock = m_rng->generate(0, m_blockData->getBlockCount() - 1);
    *newAngle = static_cast<RotateAngle>(m_rng->generate(0, newRotateRange - 1));
    *newColor = m_rng->generate(m_brick->getMinBrickIdx(), m_brick->getMaxBrickIdx());
    SDL_Log("newBlock id = %d, angle = %d, color = %d", *newBlock, static_cast<int>(*newAngle), *newColor);
}

void Arena::onFrameCounter(void *userdata){
    // 计算帧率
    triggerEvent({EventName::Timer, 0});
}

void Arena::saveToScreen(void){
    int rowCount = m_blockData->getRowCount();
    int colCount = m_blockData->getColCount();

    auto currentSingleBlock = m_blockData->m_blockGroups[m_currentBlockGroup];
    auto currentBlockBodyInfo = currentSingleBlock->getBodyInfo(m_currentBlockAngle);

    m_audioSuite[SoundId::Save]->play();
    m_blockAnimation->hide();

    for (int row = 0; row < rowCount; row++){
        for (int col = 0; col < colCount; col++){
            int blockMaskInfo = currentBlockBodyInfo->body[row][col];
            if (blockMaskInfo != 0){
                int screenCol = m_blockPos.currentCol + col;
                int screenRow = m_blockPos.currentRow + row;
                if(isInScreen(screenRow, screenCol)){
                    m_screenBuffer[screenRow][screenCol] = m_currentBlockColor;
                }
            }
        }
    }

    int topRow = INT_MAX; // 记录屏幕上最顶部的砖块行
    for (int row = 0; row < ConstDef::PG.height; row++){
        for (int col = 0; col < ConstDef::PG.width; col++){
            if (isBrickExistOnScreen(row, col)){
                topRow = min(row, topRow);
                break;
            }
        }
        if (topRow != INT_MAX){
            break;
        }
    }

    if (topRow <= 8){
        m_audioSuite[SoundId::Warning]->play(true);
    } else {
        m_audioSuite[SoundId::Warning]->stop();
    }
}

// 判断行列是否在屏幕内，通常用于访问m_screenBuffer的保护
bool Arena::isInScreen(int row, int col){
    if (row < 0 || row >= ConstDef::PG.height || col < 0 || col >= ConstDef::PG.width){
        return false;
    }
    return true;
}
// 检测屏幕上是否有砖块
bool Arena::isBrickExistOnScreen(int row, int col){
    if (isInScreen(row, col) && m_screenBuffer[row][col] != ConstDef::SCREEN_EMPTY_COLOR){
        return true;
    }
    return false;
}

// 检测指定行列下方是否有砖块堆积
bool Arena::isExistUnderfill(int row, int col){
    if (!isInScreen(row, col) || !isInScreen(row + 1, col)){
        return false;
    }

    for (int screenRow = row + 1; screenRow < ConstDef::PG.height; screenRow++){
        if (m_screenBuffer[screenRow][col] == ConstDef::SCREEN_EMPTY_COLOR){
            return true;
        }
    }
    return false;
}
// 检测方块是否可以下落
bool Arena::checkDown(void){
    int blockMaskInfo = 0;
    int screenRow = 0;
    int screenCol = 0;

    int rowCount = m_blockData->getRowCount();
    int colCount = m_blockData->getColCount();
    auto currentSingleBlock = m_blockData->m_blockGroups[m_currentBlockGroup];
    auto currentBlockBodyInfo = currentSingleBlock->getBodyInfo(m_currentBlockAngle);

    // 先检测方块的最后一行
    int lastRow = rowCount - 1;

    for (int col = 0; col < colCount; col++){
        blockMaskInfo = currentBlockBodyInfo->body[lastRow][col];
        if (blockMaskInfo == 0){
            continue;
        }

        screenRow = m_blockPos.currentRow + lastRow + 1;
        screenCol = m_blockPos.currentCol + col;
        // 下方是屏幕边缘时，不能下落
        if (screenRow >= ConstDef::PG.height){
            return false;
        }
        if (currentSingleBlock->getType() == BlockType::PIERCE){
            // 对穿透方块做特殊处理
            if(isExistUnderfill(m_blockPos.currentRow + lastRow, screenCol)){
                return true;
            }
        }
        // 下方有堆积到屏幕底部的方块时，不能下落
        if (isBrickExistOnScreen(screenRow - 1, screenCol)){
            return false;
        }
    }
    // 再从下往上检测方块的其它行
    for (int row = rowCount - 2; row >= 0; row--){
        for (int col = 0; col < colCount; col++){
            blockMaskInfo = currentBlockBodyInfo->body[row][col];
            if (blockMaskInfo == 0){
                continue;
            }
            // 本砖块下方有其它砖块时，不需要使用本砖块检测
            if (currentBlockBodyInfo->body[row + 1][col] != 0){
                continue;
            }

            screenRow = m_blockPos.currentRow + row + 1;    //取下方屏幕的行
            screenCol = m_blockPos.currentCol + col;
            if (currentSingleBlock->getType() == BlockType::PIERCE){
                // 对穿透方块做特殊处理
                if(isExistUnderfill(m_blockPos.currentRow + row, screenCol)){
                    return true;
                }
            }

            // 下方是屏幕边缘时，不能下落
            if (screenRow >= ConstDef::PG.height){
                return false;
            }
            // 下方有堆积到屏幕底部的方块时，不能下落
            if (isBrickExistOnScreen(screenRow, screenCol)){
                return false;
            }
        }
    }
    return true;
}
// 检测方块是否可以左移
bool Arena::checkDirection(CheckDirection direction){
    int blockMaskInfo = 0;
    int screenRow = 0;
    int screenCol = 0;

    int rowCount = m_blockData->getRowCount();
    int colCount = m_blockData->getColCount();
    auto currentSingleBlock = m_blockData->m_blockGroups[m_currentBlockGroup];
    auto currentBlockBodyInfo = currentSingleBlock->getBodyInfo(m_currentBlockAngle);

    int newCol = m_blockPos.currentCol + (direction == CheckDirection::LEFT ? -1 : (direction == CheckDirection::RIGHT ? 1 : 0));
    int newRow = m_blockPos.currentRow + (direction == CheckDirection::DOWN ? 1 : 0);
    // 检测移动后是否与屏幕上的砖块重叠
    for (int row = 0; row < rowCount; row++){
        for (int col = 0; col < colCount; col++){
            blockMaskInfo = currentBlockBodyInfo->body[row][col];
            if (blockMaskInfo == 0){
                continue;
            }

            screenRow = newRow + row;
            screenCol = newCol + col;
            if (!isInScreen(screenRow, screenCol)){
                return false;
            }
            if (isBrickExistOnScreen(screenRow, screenCol)){
                if (direction == CheckDirection::DOWN && currentSingleBlock->getType() == BlockType::PIERCE){
                    // 对穿透方块做特殊处理
                    if(isExistUnderfill(m_blockPos.currentRow + row, screenCol)){
                        return true;
                    }
                }
                return false;
            }
        }
    }

    return true;
}

int Arena::checkLine(void){
    SoundId soundId = SoundId::Multishot;
    bool isLineFull;
    int clearLineCount = 0; // 一次消掉几行
    int bombedCount = 0;    // 炸弹炸掉的砖块数

    int score = 0;
    int screenRow = (int)ConstDef::PG.height - 1;
    while (screenRow >= ConstDef::PG_START_ROW){
        isLineFull = true;
        for (int screenCol = 0; screenCol < ConstDef::PG.width; screenCol++){
            if (!isBrickExistOnScreen(screenRow, screenCol)){
                isLineFull = false;
            }
            // 对炸弹特殊处理
            if (m_screenBuffer[screenRow][screenCol] == ConstDef::SCREEN_BOMB_COLOR) {
                for (int bombCol = screenCol - 1; bombCol < screenCol + 2; bombCol++){
                    for (int bombRow = screenRow - 1; bombRow < screenRow + 2; bombRow++){
                        if(isBrickExistOnScreen(bombRow, bombCol)){
                            m_screenBuffer[bombRow][bombCol] = ConstDef::SCREEN_EMPTY_COLOR;
                            bombedCount++;
                        }
                    }
                }
                isLineFull = false;
                soundId = SoundId::Bombexplode;
                break;
            }
        }

        if (isLineFull){
            clearLineCount++;
            for (int row = screenRow; row > 0; row--){
                for (int col = 0; col < ConstDef::PG.width; col++){
                    m_screenBuffer[row][col] = m_screenBuffer[row - 1][col];
                }
            }
            for (int col = 0; col < ConstDef::PG.width; col++){
                m_screenBuffer[0][col] = ConstDef::SCREEN_EMPTY_COLOR;
            }
        } else {
            screenRow--;
        }
    }

    if (clearLineCount >= ConstDef::MAX_SCORE_LEVEL){
        clearLineCount = ConstDef::MAX_SCORE_LEVEL - 1;
    }
    if (bombedCount >= ConstDef::MAX_BOMB_LEVEL){
        bombedCount = ConstDef::MAX_BOMB_LEVEL - 1;
    }

    score += ConstDef::LEVEL_SCORE_MAPPING[clearLineCount] + ConstDef::BOMB_SCORE_MAPPING[bombedCount];
    if (score > 0){
        SDL_Log("clearLineCount: %d, bombedCount: %d, score: %d", clearLineCount, bombedCount, score);
        m_audioSuite[soundId]->play(false);
    }
    return score;
}

void Arena::TriggerBlockMoveAnimation(MoveDirection moveDirection){
    m_blockAnimation->prepare(m_blockPos.currentCol * ConstDef::SINGLE_BLOCK_SIZE.width,
                              m_blockPos.currentRow * ConstDef::SINGLE_BLOCK_SIZE.height);
    m_blockMoveTracker->setMoveDirection(moveDirection);
    m_blockAnimation->resume();
    setState(State::ACTING);
}

void Arena::moveLeft(void){
    if (m_currentBlockGroup < 0){
        return;
    }
    if (checkDirection(CheckDirection::LEFT)){
            TriggerBlockMoveAnimation(MoveDirection::LEFT);
        m_blockPos.currentCol -= 1;
        m_audioSuite[m_toPlaySoundId]->play();
    }
}

void Arena::moveRight(void){
    if( m_currentBlockGroup < 0){
        return;
    }
    if (checkDirection(CheckDirection::RIGHT)){
            TriggerBlockMoveAnimation(MoveDirection::RIGHT);

        m_blockPos.currentCol += 1;
        m_audioSuite[m_toPlaySoundId]->play();
    }
}

// 返回值为TRUE时指示向下已不可移动
bool Arena::moveDown(void){
    if (m_currentBlockGroup < 0){
        return false;
    }
    m_audioSuite[m_toPlaySoundId]->play();

    if(checkDirection(CheckDirection::DOWN)){
        TriggerBlockMoveAnimation(MoveDirection::DOWN);

        m_blockPos.currentRow += 1;

        // 判断是否要产生提示方块
        auto currentSingleBlock = m_blockData->m_blockGroups[m_currentBlockGroup];
        auto currentBlockBodyInfo = currentSingleBlock->getBodyInfo(m_currentBlockAngle);

        if (m_nextBlockGroup < 0 && m_blockPos.currentRow + currentBlockBodyInfo->m_topRow >= ConstDef::PG_START_ROW){
            newBlock(&m_nextBlockGroup, &m_nextBlockAngle, &m_nextBlockColor);
            auto nextSingleBlock = m_blockData->m_blockGroups[m_nextBlockGroup];
            auto nextBlockBodyInfo = nextSingleBlock->getBodyInfo(m_nextBlockAngle);

            // 计算居中偏移
            m_nextBlockColOffset = (int)((ConstDef::PG.width - (nextBlockBodyInfo->m_rightCol - nextBlockBodyInfo->m_leftCol + 1)) / 2) - nextBlockBodyInfo->m_leftCol;
        }
        return false;
    } else {
        saveToScreen();
        if (checkFull()){
            GameEvent arenaEvent = {EventName::Fillfull, 0};
            stateEvent(arenaEvent);
        } else {
            int score = checkLine();
            SoundId evaluateSoundId = SoundId::None;
            if (score >= ConstDef::LEVEL_SCORE_MAPPING[2]){
                evaluateSoundId = SoundId::Good;
            }
            if (score >= ConstDef::LEVEL_SCORE_MAPPING[4]){
                evaluateSoundId = SoundId::Excellent;
            }
            if (evaluateSoundId != SoundId::None) {
                m_audioSuite[evaluateSoundId]->play();
            }
            m_totalScore += score;
            m_gradeScore += score;
            std::string strScore = "Score: " + std::to_string(m_totalScore);
            m_scoreLabel->setCaption(strScore.c_str());

            if (m_gradeScore >= ConstDef::GRADE_SCORE_LEVEL){
                speedUp();
                m_gradeScore = 0;
            }

            getNextBlock();
        }
        return true;
    }
}

void Arena::speedUp(void){
    m_speed ++;
    if (m_speed > ConstDef::MAX_FAST_SPEED){
        m_speed = ConstDef::MAX_FAST_SPEED;
    }
    std::string strSpeed = "Speed: " + std::to_string(m_speed + 1);
    m_speedLabel->setCaption(strSpeed.c_str());
    m_debugLabel->setCaption(u8"下落速度(ms): " + std::to_string(ConstDef::LEVEL_SPEED_MAPPING[m_speed]));

    m_audioSuite[SoundId::LevelComplete]->play();
    m_audioSuite[SoundId::Speedup]->play();
    triggerEvent({EventName::NextBackground, 0});
}

void Arena::update(void){
    Panel::update();

    m_blockAnimation->update();


    if(SDL_GetTicks() < m_nextUpdateTick){
        return;
    }
    m_nextUpdateTick = SDL_GetTicks() + ConstDef::LEVEL_SPEED_MAPPING[m_speed ];    // Todo: 发生翻转时怎么处理？答：超过5.84亿年时间才会翻转，不用考虑

    GameEvent arenaEvent = {EventName::MoveDown, 0};
    stateEvent(arenaEvent);
}

void Arena::clean(void){
    m_blockPos.currentRow = -1;
    m_blockPos.currentCol = -1;

    m_speed = ConstDef::MAX_SLOW_SPEED;

    for(int row = 0; row < ConstDef::PG.height; row++){
        for(int col = 0; col < ConstDef::PG.width; col++){
            m_screenBuffer[row][col] = ConstDef::SCREEN_EMPTY_COLOR;
        }
    }

    m_totalScore = 0;
}

void Arena::drawNextBlocks(void){
    int rowCount = m_blockData->getRowCount();
    int colCount = m_blockData->getColCount();

    if (m_nextBlockGroup < 0){
        return;
    }
    auto nextSingleBlock = m_blockData->m_blockGroups[m_nextBlockGroup];
    auto nextBlockBodyInfo = nextSingleBlock->getBodyInfo(m_nextBlockAngle);

    int iBlockIdx = m_nextBlockColor;
    for (int row = 0; row < rowCount; row++){
        for (int col = 0; col < colCount; col++){
            int b = nextBlockBodyInfo->body[row][col];
            if (b != 0){
                switch(nextSingleBlock->getType()){
                    case BlockType::PIERCE:
                        iBlockIdx = ConstDef::SCREEN_CROSS_COLOR;
                        break;
                    case BlockType::BOMB:
                        iBlockIdx = ConstDef::SCREEN_BOMB_COLOR;
                        break;
                    case BlockType::NORMAL:
                    default:
                        iBlockIdx = m_nextBlockColor;
                        break;
                }

                m_brick->draw((col + m_nextBlockColOffset) * ConstDef::SINGLE_BLOCK_SIZE.width + ConstDef::HINT_PLACE.left,
                    row * ConstDef::SINGLE_BLOCK_SIZE.height + ConstDef::HINT_PLACE.top, iBlockIdx);

            }
        }
    }
}

void Arena::onBtnClick(shared_ptr<Button> button){
    switch(static_cast<ButtonId>(button->getId())){
        case ButtonId::LeftBtn:
            moveLeft();
            break;
        case ButtonId::RightBtn:
            moveRight();
            break;
        case ButtonId::DownBtn:
            moveDown();
            break;
        case ButtonId::RotateBtn:
            rotate();
            break;
        case ButtonId::PauseBtn:
            triggerEvent({EventName::Paused, 0});
            break;
        case ButtonId::PlayBtn:
            triggerEvent({EventName::Paused, 0});
            break;
        default:
            break;
    }
}
