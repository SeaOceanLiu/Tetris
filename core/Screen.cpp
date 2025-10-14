#include "Screen.h"

SDL_EnumerationResult callback(void *userdata, const char *dirname, const char *fname) {
    SDL_Log("  - %s (%s)", fname, dirname);
    return SDL_ENUM_CONTINUE;
}

void Screen::initial(void){
    addControl(ButtonBuilder(this, SRect(m_rect.width - 128, 0, 128, 128))
                .setBtnNormalStateActor(    make_shared<Actor>(this, ResourceLoader::RID_cross_up_png, true))
                .setBtnHoverStateActor(     make_shared<Actor>(this, ResourceLoader::RID_cross_over_png, true))
                .setBtnPressedStateActor(   make_shared<Actor>(this, ResourceLoader::RID_cross_down_png, true))
                .setOnClick(std::bind(&Screen::onClose, this, std::placeholders::_1))
                .setTransparent(false)
                .build());


    // 计算背景图缩放比例和偏移 Todo: 应该优化成正方形的背景图做裁剪，而不是直接使用16:9的背景图做固定比例缩放
    float Nw = m_rect.width / m_defaultBGRect.width;
    float Nh = m_rect.height / m_defaultBGRect.height;
    if (m_rect.width > m_rect.height){
        m_M = min(Nw, Nh);
    } else {
        m_M = max(Nw, Nh);
    }

    m_defaultBGRect.left = (m_rect.width - m_defaultBGRect.width * m_M) / 2;
    m_defaultBGRect.top = 0;

    SDL_Log("Starting to load background images...");
    setBackground(PhotoCarouselBuilder(this, 4, m_defaultBGRect, m_M, m_M)
                    // .addPhotosInPath(ConstDef::pathPrefix / "images" / "background", ".jpg") // 尚未研究明白跑在Android下怎么遍历assets下的文件，所以暂不使用这种方式
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE00_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE01_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE02_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE03_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE04_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE05_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE06_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE07_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE08_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE09_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE10_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE11_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE12_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE13_jpg, true)
                    .addPhoto(ResourceLoader::RID_BACKGROUND_IMAGE14_jpg, true)
                    .build());

    SDL_Log("Constructing game title string...");
    float labelHeight = ConstDef::VERSION_FONT_SIZE + ConstDef::FONT_MARGIN;
    float labelTop = (m_rect.height - labelHeight) / 2;
    addControl(m_gameTitle = LabelBuilder(this, {0, labelTop, m_rect. width, labelHeight})
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setFont(FontName::Muyao_Softbrush)
                    .setFontSize((int)ConstDef::VERSION_FONT_SIZE)
                    .setCaption(u8"俄罗斯方块(Tetris)")  // 使用u8字符串时，应保证cpp源码使用UTF-8 with BOM编码保存
                    .setAlignmentMode(AlignmentMode::AM_CENTER)
                    .setShadow(true)
                    .setShadowColor({255, 255, 255, SDL_ALPHA_OPAQUE})
                    .build());

    SDL_Log("Constructing game version information string...");
    labelTop = labelTop + labelHeight + ConstDef::FONT_MARGIN;
    addControl(m_gameVersion = LabelBuilder(this, {0, labelTop, m_rect. width, labelHeight})
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setFont(FontName::Muyao_Softbrush)
                    .setFontSize((int)ConstDef::VERSION_FONT_SIZE)
                    .setCaption(ConstDef::VERSION_TEXT)
                    .setAlignmentMode(AlignmentMode::AM_CENTER)
                    .setShadow(true)
                    .setShadowColor({255, 255, 255, SDL_ALPHA_OPAQUE})
                    .build());

    SDL_Log("Constructing game auth string...");
    labelTop = labelTop + labelHeight * 2 + ConstDef::FONT_MARGIN;
    addControl(m_gameAuthor = LabelBuilder(this, {0, labelTop, m_rect. width, labelHeight})
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setFont(FontName::HarmonyOS_Sans_SC_Regular)
                    .setFontSize((int)ConstDef::COPYRIGHT_FONT_SIZE)
                    .setCaption(u8"Copyright©2025 SeaOcean.\nAll rights reserved.")
                    .setAlignmentMode(AlignmentMode::AM_CENTER)
                    .setShadow(true)
                    .setShadowColor({255, 255, 255, SDL_ALPHA_OPAQUE})
                    .build());

    SDL_Log("Constructing about link label...");
    // 先添加贴近屏幕底部的“关于”链接，采用了AlignmentMode::AM_BOTTOM_CENTER，所以只需要确定高度（m_rect.height - 100）即可
    SRect aboutRect = {0, 0, m_rect.width, m_rect.height - 100};
    m_aboutLabel = LabelBuilder(this, {0, 0, m_rect.width, m_rect.height - 100})
                        .setFont(FontName::Asul_Bold)
                        .setAlignmentMode(AlignmentMode::AM_BOTTOM_CENTER)
                        .setFontSize(50)
                        .setCaption("About")
                        .SetFontStyle(TTF_STYLE_UNDERLINE)
                        .setNormalStateColor({0, 0, 139, SDL_ALPHA_OPAQUE})
                        .setHoverStateColor({0, 0, 255, SDL_ALPHA_OPAQUE})
                        .setPressedStateColor({0, 255, 255, SDL_ALPHA_OPAQUE})
                        .setShadowColor({178,34,34, SDL_ALPHA_OPAQUE})
                        .setShadowOffset({10, 10})
                        .setOnClick(std::bind(&Screen::onAbout, this, std::placeholders::_1))
                        .build();
    addControl(m_aboutLabel);

    SDL_Log("Constructing start button...");
    // 根据“关于”链接的位置，向上移动添加“New game”按钮，位置是水平居中，垂直方向的底部与“关于”链接顶部相差150像素
    SRect buttonRect = {(m_rect.width - 200) / 2, m_aboutLabel->getHotRect().top - 150, 200, 100};
    m_startButton = ButtonBuilder(this, buttonRect)
                    .setCaption("New game")
                    .setCaptionSize(35)
                    .setOnClick(std::bind(&Screen::onStart, this, std::placeholders::_1))
                    .build();
    addControl(m_startButton);

    SDL_Log("Constructing Continue button...");
    // 添加“Continue”按钮，位置是在“New game”按钮正上方
    SRect continueBtnRect = {(m_rect.width - 200) / 2, buttonRect.top - 150, 200, 100};
    m_continueButton = ButtonBuilder(this, continueBtnRect)
                    .setCaption("Continue")
                    .setCaptionSize(35)
                    .setOnClick(std::bind(&Screen::onContinue, this, std::placeholders::_1))
                    .build();
    addControl(m_continueButton);

    // 根据是否有临时存盘文件来决定是否显示“Continue”按钮
    SDL_IOStream * saveidFileIO = ResourceLoader::getInstance()->openTempSavedFile('r');
    if (!saveidFileIO) {
        m_continueButton->hide();
    }
    ResourceLoader::getInstance()->closeTempSavedFile(saveidFileIO);

    // 计算游戏区域缩放比例和偏移
    Nw = /*2.0f * */m_rect.width / m_defaultArenaRect.width;
    Nh = /*2.0f * */m_rect.height / m_defaultArenaRect.height;
    float m_N = min(Nw, Nh);

    SDL_Log("Constructing arena.....................................");
    // 下面这行的分子部分多了一个m_N，是因为Arena构造函数中会对位移量进行缩放，为保证背景图居中，需要先除以m_N
    m_defaultArenaRect.left = (m_rect.width - m_defaultArenaRect.width * m_N) / 2;
    m_arena = make_shared<Arena>(this, m_defaultArenaRect, m_N, m_N);
    addControl(m_arena);
    m_arena->hide();

    SDL_Log("Constructing about dialog...");
    SSize dialogSize = {200, 150};
    SRect aboutDialogRect = {(m_rect.width - dialogSize.width * m_N) / 2, (m_rect.height - dialogSize.height * m_N) / 2, dialogSize.width, dialogSize.height};
    addControl(m_aboutDialog = DialogBuilder(this, aboutDialogRect, m_N, m_N)   //和Arena的缩放比例保持一致
                        .setTitle(u8"本软件使用的第三方开源库和媒体资源")
                                  //----------------------------------------------------89
                        .addText(u8"第三方开源库：")
                        .addText(u8"1. SDL3-3.2.14")
                        .addText(u8"   <https://github.com/libsdl-org/SDL>")
                        .addText(u8"2. SDL_image-3.2.4")
                        .addText(u8"   <https://github.com/libsdl-org/SDL_image>")
                        .addText(u8"3. SDL_ttf-3.2.0")
                        .addText(u8"   <https://github.com/libsdl-org/SDL_ttf>")
                        .addText(u8"4. nlohmann-3.12.0")
                        .addText(u8"   <https://github.com/nlohmann/json>")
                        .addText(u8"")
                        .addText(u8"")
                        .addText(u8"")
                        .addText(u8"")
                        .addText(u8"字体资源：")
                        .addText(u8"1. Asul-Bold/Asul-Regular")
                        .addText(u8"   <Copyright (c) 2011 by")
                        .addText(u8"           Mariela Monsalve (marmonsalve@gmail.com)>")
                        .addText(u8"2. HarmonyOS Sans Fonts")
                        .addText(u8"   <Copyright 2021 Huawei Device Co., Ltd.>")
                        .addText(u8"3. Maple Mono V7.2")
                        .addText(u8"   <https://github.com/subframe7536/maple-font>")
                        .addText(u8"4. 沐瑶软笔手写体 Muyao-Softbrush Ver 1.0")
                        .addText(u8"   春颜秋色<https://www.zcool.com.cn/u/402511>")
                        .addText(u8"5. Quando Version 1.002")
                        .addText(u8"   <Copyright (c) 2011 by")
                        .addText(u8"            Sorkin Type Co (www.sorkintype.com)>")
                        .addText(u8"音效及背景音乐资源：")
                        .addText(u8"1.音效来自于近二十年前某几款游戏，现在已经不知道是哪")
                        .addText(u8"  几款游戏了，如有侵权，请联系我修改及删除。")
                        .addText(u8"")
                        .addText(u8"2.背景音乐来自于<https://freepd.com/>：")
                        .addText(u8"  3 am West End      <Written by statusq>")
                        .addText(u8"  Beat One           <Written by Kevin MacLeod>")
                        .addText(u8"  Fright Night Twist <Written by Bryan Teoh>")
                        .addText(u8"  Goodnightmare      <Written by Kevin MacLeod>")
                        .addText(u8"  Palm and Soul      <Written by Kevin MacLeod>")
                        .addText(u8"  Take the Ride      <Written by Bryan Teoh>")
                        .addText(u8"")
                        .addText(u8"")
                        .build());
    m_aboutDialog->hide();

    // 判断是否需要重写资源
    // Todo: 后面考虑改成使用线程来重写资源
    if (ResourceLoader::getInstance()->isRewriteNeeded()){
        SDL_Log("Rewriting resources.....................................");
        ResourceLoader::getInstance()->saveAllResourceToPrefPath();
    }

    m_isInitialed = true;
    SDL_Log("Loading finished, waiting user starting game................................");
}

Screen::Screen(Control *parent, SRect rect, SDL_Renderer *renderer, float xScale, float yScale):
    TopControl(),
    Panel(parent, rect, xScale, yScale),
    m_isLoading(true),
    m_isInitialed(false),
    m_defaultArenaRect(SRect(SPoint(0, 0), ConstDef::DEFAULT_ARENA_SIZE)),
    m_defaultBGRect(SRect(SPoint(0, 0), ConstDef::DEFAULT_BG_SIZE)),
    m_nextTick(0),
    m_nextRepeatTick(0),
    m_isExiting(SDL_APP_CONTINUE),
    m_arena(nullptr),
    m_gameTitle(nullptr),
    m_gameVersion(nullptr),
    m_gameAuthor(nullptr),
    m_aboutLabel(nullptr),
    m_startButton(nullptr),
    m_aboutDialog(nullptr)
{
    setRenderer(renderer);
    setTransparent(true);

    SDL_Log("Loading resources.....................................");
    // 将资源加载到内存中
    ResourceLoader::getInstance()->loadConfig();
}

void Screen::inputControl(shared_ptr<Event> event) {
    if (m_eventJitter.find(event->m_eventName) != m_eventJitter.end()){
        if (SDL_GetTicks() < m_eventJitter[event->m_eventName]){
            return;
        }
        m_eventJitter[event->m_eventName] = SDL_GetTicks() + ConstDef::DEFAULT_BTN_MS_INTERVAL;
    }

    if(EventQueue::isPositionEvent(event->m_eventName)){
        m_lastAction = event;
        m_nextTick = SDL_GetTicks() + ConstDef::DEFAULT_BTN_MS_INTERVAL;
    }

    triggerEvent(event);
}

void Screen::repeatTrigger(void){
    if (m_lastAction != nullptr){
        // if (SDL_GetTicks() < m_nextRepeatTick){
        //     return;
        // }
        Uint64 currentTick = SDL_GetTicks();
        if (currentTick < m_nextRepeatTick || currentTick < m_eventJitter[m_lastAction->m_eventName]){
            return;
        }

        switch(m_lastAction->m_eventName){
            case EventName::FINGER_DOWN:
            case EventName::FINGER_MOTION:
            case EventName::MOUSE_LBUTTON_DOWN:
            case EventName::MOUSE_MBUTTON_DOWN:
            case EventName::MOUSE_RBUTTON_DOWN:
                triggerEvent(m_lastAction);
                break;
            default:
                break;
        }
        m_nextRepeatTick = SDL_GetTicks() + ConstDef::DEFAULT_BTN_MS_REPEAT;
    }
}
void Screen::update() {
    if (m_isLoading){
        if(ResourceLoader::getInstance()->getLoadingProgress() == 1.0f){
            ResourceLoader::getInstance()->detachLoadingThread();
            m_isLoading = false;
            initial();
        }
    } else {
        if (m_lastAction != nullptr){
            repeatTrigger();
        }
        Panel::update();
    }
}
void Screen::draw(void){
    if(m_isLoading){
        SRect rect = {0, m_rect.height / 2 - 50, m_rect.width, 100};
        SRect percentRect = {0, m_rect.height / 2 - 50, m_rect.width * ResourceLoader::getInstance()->getLoadingProgress(), 100};

        // 使用橙色（orange）画进度
        if(!SDL_SetRenderDrawColor(getRenderer(), 255, 165, 0, SDL_ALPHA_OPAQUE)){
            SDL_Log("Failed to set grid render color: %s", SDL_GetError());
        }
        if (!SDL_RenderFillRect(getRenderer(), percentRect.toSDLFRect())){
            SDL_Log("Failed to fill render rect: %s", SDL_GetError());
        }
        // 使用灰色（gray）画进度条外框
        if(!SDL_SetRenderDrawColor(getRenderer(), 128, 128, 128, SDL_ALPHA_OPAQUE)){
            SDL_Log("Failed to set grid render color: %s", SDL_GetError());
        }
        if (!SDL_RenderRect(getRenderer(), rect.toSDLFRect())){
            SDL_Log("Failed to fill render rect: %s", SDL_GetError());
        }
    }
    // 绘制子控件
    Panel::draw();
}

void Screen::onClose(shared_ptr<Button> btn) {
    m_isExiting = SDL_APP_SUCCESS;
}

SDL_AppResult Screen::isExiting(void) {
    return m_isExiting;
}

void Screen::onStart(shared_ptr<Button> btn) {
    m_gameTitle->hide();
    m_gameVersion->hide();
    m_gameAuthor->hide();

    m_continueButton->hide();
    m_startButton->hide();
    m_aboutLabel->hide();
    m_arena->show();
    triggerEvent(make_shared<Event>(EventName::Begin, 0));
    SDL_Log("state = %d", m_arena->getState());
}
void Screen::onAbout(shared_ptr<Label> label){
    m_aboutDialog->show();
}

void Screen::loadAudioMusic(void){
    m_arena->loadAudioMusic();
}

void Screen::onContinue(shared_ptr<Button> btn) {
    SDL_Log("Continue button clicked");

    onStart(nullptr);
    m_arena->setState(State::PAUSED);

    SDL_IOStream *fileStream = ResourceLoader::getInstance()->openTempSavedFile('r');
    m_arena->loadFromStream(fileStream);
    m_arena->forceSetArenaAfterLoadedFromStream();
    ResourceLoader::getInstance()->closeTempSavedFile(fileStream);
}

void Screen::saveGame(void){
    SDL_IOStream *fileStream = ResourceLoader::getInstance()->openTempSavedFile('w');
    m_arena->saveToStream(fileStream);
    ResourceLoader::getInstance()->closeTempSavedFile(fileStream);
}