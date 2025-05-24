#include "Screen.h"

Screen::Screen(Control *parent, SRect rect, SDL_Renderer *renderer, float xScale, float yScale):
    TopControl(),
    Panel(parent, rect, xScale, yScale),
    m_defaultArenaRect(SRect(SPoint(0, 0), ConstDef::DEFAULT_ARENA_SIZE)),
    m_defaultBGRect(SRect(SPoint(0, 0), ConstDef::DEFAULT_BG_SIZE)),
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

    addControl(ButtonBuilder(this, SRect(rect.width - 128, 0, 128, 128))
                .setBtnNormalStateActor(    make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "cross_up.png", true))
                .setBtnHoverStateActor(     make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "cross_over.png", true))
                .setBtnPressedStateActor(   make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "cross_down.png", true))
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

    setBackground(PhotoCarouselBuilder(this, 4, m_defaultBGRect, m_M, m_M)
                    // .addPhotosInPath(ConstDef::pathPrefix / "images" / "background", ".jpg") // 尚未研究明白跑在Android下怎么遍历assets下的文件，所以暂不使用这种方式
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "2022_1204_15565500_mh1670141573251.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20200903_202237.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20200920_100257.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20200920_100819.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20201002_184914.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20201003_201713.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20201004_084644.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20201004_084801.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20201004_085622.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20201004_183444.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20201018_145828.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "IMG_20201025_180928.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "mmexport1599240022745.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "mmexport1599449092788.jpg", true)
                    .addPhoto(ConstDef::pathPrefix / "images" / "background" / "mmexport1599449096347.jpg", true)
                    .build());

    float labelHeight = ConstDef::VERSION_FONT_SIZE + ConstDef::FONT_MARGIN;
    float labelTop = (m_rect.height - labelHeight) / 2;
    addControl(m_gameTitle = LabelBuilder(this, {0, labelTop, m_rect. width, labelHeight})
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setFont(FontName::Muyao_Softbrush)
                    .setFontSize(ConstDef::VERSION_FONT_SIZE)
                    .setCaption(u8"俄罗斯方块(Tetris)")  // 使用u8字符串时，应保证cpp源码使用UTF-8 with BOM编码保存
                    .setAlignmentMode(AlignmentMode::AM_CENTER)
                    .setShadow(true)
                    .setShadowColor({255, 255, 255, SDL_ALPHA_OPAQUE})
                    .build());

    labelTop = labelTop + labelHeight + ConstDef::FONT_MARGIN;
    addControl(m_gameVersion = LabelBuilder(this, {0, labelTop, m_rect. width, labelHeight})
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setFont(FontName::Muyao_Softbrush)
                    .setFontSize(ConstDef::VERSION_FONT_SIZE)
                    .setCaption(u8"单机版 V1.0.0")
                    .setAlignmentMode(AlignmentMode::AM_CENTER)
                    .setShadow(true)
                    .setShadowColor({255, 255, 255, SDL_ALPHA_OPAQUE})
                    .build());

    labelTop = labelTop + labelHeight * 2 + ConstDef::FONT_MARGIN;
    addControl(m_gameAuthor = LabelBuilder(this, {0, labelTop, m_rect. width, labelHeight})
                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                    .setFont(FontName::HarmonyOS_Sans_SC_Regular)
                    .setFontSize(ConstDef::COPYRIGHT_FONT_SIZE)
                    .setCaption(u8"Copyright©2025 SeaOcean.\nAll rights reserved.")
                    .setAlignmentMode(AlignmentMode::AM_CENTER)
                    .setShadow(true)
                    .setShadowColor({255, 255, 255, SDL_ALPHA_OPAQUE})
                    .build());

    m_startButton = ButtonBuilder(this, {(m_rect.width - 150) / 2, m_rect.height - 250, 150, 50})
                    .setCaption("Start")
                    .setCaptionSize(40)
                    .setOnClick(std::bind(&Screen::onStart, this, std::placeholders::_1))
                    .build();
    addControl(m_startButton);

    // 添加“关于”链接
    m_aboutLabel = LabelBuilder(this, {0, 0, m_rect. width, m_rect.height - 100})
                        .setFont(FontName::Asul_Bold)
                        .setAlignmentMode(AlignmentMode::AM_BOTTOM_CENTER)
                        .setFontSize(40)
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

    // 计算游戏区域缩放比例和偏移
    Nw = /*2.0f * */m_rect.width / m_defaultArenaRect.width;
    Nh = /*2.0f * */m_rect.height / m_defaultArenaRect.height;
    float m_N = min(Nw, Nh);

    // 下面这行的分子部分多了一个m_N，是因为Arena构造函数中会对位移量进行缩放，为保证背景图居中，需要先除以m_N
    m_defaultArenaRect.left = (m_rect.width - m_defaultArenaRect.width * m_N) / 2;
    m_arena = make_shared<Arena>(this, m_defaultArenaRect, m_N, m_N);
    addControl(m_arena);
    m_arena->hide();

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
}

void Screen::inputControl(GameEvent &event) {
    if (event.m_eventName == m_lastAction.m_eventName){
        if (SDL_GetTicks() < m_nextTick){
            return; // Todo: 这里直接返回会有内存泄漏，因为对于触控和鼠标事件来说，事件中还会带有在堆上分配的坐标点数据
        }
    }
    if(event.isPositionEvent()){
        m_lastAction = event;
        m_nextTick = SDL_GetTicks() + ConstDef::DEFAULT_BTN_MS_INTERVAL;
    }

    triggerEvent(event);
}

void Screen::update() {
    switch(m_lastAction.m_eventName){
        case EventName::FINGER_DOWN:
        case EventName::FINGER_MOTION:
        case EventName::MOUSE_LBUTTON_DOWN:
        case EventName::MOUSE_MBUTTON_DOWN:
        case EventName::MOUSE_RBUTTON_DOWN:
            // SDL_Log("repeat input");
            inputControl(m_lastAction);
            break;
        default:
            break;
    }
    Panel::update();
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

    m_startButton->hide();
    m_aboutLabel->hide();
    m_arena->show();
    triggerEvent(GameEvent(EventName::Begin, 0));
}
void Screen::onAbout(shared_ptr<Label> label){
    m_aboutDialog->show();
}