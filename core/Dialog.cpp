﻿#include "Dialog.h"

// 构造函数，用于创建一个Dialog对象
Dialog::Dialog(Control* parent, SRect rect, float xScale, float yScale):
    // 调用父类的构造函数，初始化Panel对象
    Panel(parent, rect, xScale, yScale),
    m_title("Dialog"),
    m_currentLine(0),
    m_titleBar(nullptr),
    m_okButton(nullptr),
    m_cancelButton(nullptr)
{
    if(m_rect.width < 200) m_rect.width = 200;  // 如果宽度小于200，则设置为200
    if(m_rect.height < 100) m_rect.height = 100;// 如果高度小于200，则设置为200

    // 添加右上角关闭按钮
    addControl(ButtonBuilder(this, SRect(m_rect.width - ConstDef::WINDOW_TITLE_HEIGHT, 0, ConstDef::WINDOW_TITLE_HEIGHT, ConstDef::WINDOW_TITLE_HEIGHT))
                .setBtnNormalStateActor(    make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "cross_up.png", true))
                .setBtnHoverStateActor(     make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "cross_over.png", true))
                .setBtnPressedStateActor(   make_shared<Actor>(this, ConstDef::pathPrefix / "images" / "cross_down.png", true))
                .setOnClick(std::bind(&Dialog::onClose, this, std::placeholders::_1))
                .setTransparent(false)
                .build());
    // 添加标题栏
    SRect titleRect = {0, 0, m_rect.width - ConstDef::WINDOW_TITLE_HEIGHT, ConstDef::WINDOW_TITLE_HEIGHT};
    addControl(m_titleBar = PanelBuilder(this, titleRect)
                .addControl(LabelBuilder(this, titleRect)
                                .setFontSize(30)
                                .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                                .setAlignmentMode(AlignmentMode::AM_CENTER)
                                .setFont(FontName::HarmonyOS_Sans_SC_Regular)
                                .setCaption(m_title)
                                .build())
                // .setTransparent(false)
                .build());
    SSize btnSize(50, 15);
    // 添加Cancel按钮
    SRect cancelRect(m_rect.width - btnSize.width - ConstDef::FONT_MARGIN, m_rect.height - btnSize.height - ConstDef::FONT_MARGIN, btnSize.width, btnSize.height);
    addControl(m_cancelButton = ButtonBuilder(this, cancelRect)
                .setCaption("Cancel")
                .setCaptionSize(8)
                .setOnClick(std::bind(&Dialog::onCancel, this, std::placeholders::_1))
                .build());
    // 添加Ok按钮
    SRect okRect(cancelRect.left - btnSize.width - ConstDef::FONT_MARGIN, cancelRect.top, btnSize.width, btnSize.height);
    addControl(m_okButton = ButtonBuilder(this, okRect)
                .setCaption("Next")
                .setCaptionSize(8)
                .setOnClick(std::bind(&Dialog::onOk, this, std::placeholders::_1))
                .build());
    SRect clientRect = {ConstDef::FONT_MARGIN,
                        titleRect.height + ConstDef::FONT_MARGIN,
                        m_rect.width - ConstDef::FONT_MARGIN * 2,
                        cancelRect.top - titleRect.height - ConstDef::FONT_MARGIN * 2};
    addControl(m_screenText = LabelBuilder(this, clientRect)
                .setFontSize(6)
                .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                .setAlignmentMode(AlignmentMode::AM_TOP_LEFT)
                .setFont(FontName::MapleMono_NF_CN_Regular)
                .setCaption("Text")
                .build());
}

void Dialog::draw(void){
    if (m_visible){
        Panel::draw();
    }
}

bool Dialog::handleEvent(GameEvent &event){
    if(!m_visible || !m_enable) return false;

    ControlImpl::handleEvent(event);

    // 作为对话框，应该拦截所有消息
    return true;
}

void Dialog::onClose(shared_ptr<Button> btn) {
    hide();
}
void Dialog::onOk(shared_ptr<Button> btn){
    if (m_currentLine + Dialog::TEXT_LINE_COUNT < m_texts.size()) {
        m_currentLine += Dialog::TEXT_LINE_COUNT;
        m_onScreenText = "";

        for (int i = m_currentLine; i < m_currentLine + Dialog::TEXT_LINE_COUNT && i < m_texts.size(); i++) {
            m_onScreenText += m_texts[i] + "\n";
        }
        m_screenText->setCaption(m_onScreenText);
    } else {
        hide();
    }
}
void Dialog::onCancel(shared_ptr<Button> btn){
    hide();
}

void Dialog::show(void){
    m_currentLine = 0;
    m_onScreenText = "";
    for (int i = m_currentLine; i < m_currentLine + Dialog::TEXT_LINE_COUNT && i < m_texts.size(); i++) {
        m_onScreenText += m_texts[i] + "\n";
    }
    m_screenText->setCaption(m_onScreenText);

    Panel::show();

}


DialogBuilder::DialogBuilder(Control* parent, SRect rect, float xScale, float yScale):
    m_dialog(nullptr)
{
    m_dialog = make_shared<Dialog>(parent, rect, xScale, yScale);
}
DialogBuilder& DialogBuilder::setOkBtnCaption(string okCaption)
{
    m_dialog->m_okButton->setCaption(okCaption);
    return *this;
}
DialogBuilder& DialogBuilder::setCancelBtnCaption(string cancelCaption){
    m_dialog->m_cancelButton->setCaption(cancelCaption);
    return *this;
}

DialogBuilder& DialogBuilder::setTitle(string title){
    m_dialog->m_title = title;

    if (m_dialog->m_titleBar){
        m_dialog->removeControl(m_dialog->m_titleBar);
    }
    m_dialog->addControl(m_dialog->m_titleBar = PanelBuilder(m_dialog.get(), SRect(0, 0, m_dialog->m_rect.width - ConstDef::WINDOW_TITLE_HEIGHT, ConstDef::WINDOW_TITLE_HEIGHT))
                                                    .addControl(LabelBuilder(m_dialog->m_titleBar.get(), SRect(0, 0, m_dialog->m_rect.width - ConstDef::WINDOW_TITLE_HEIGHT, ConstDef::WINDOW_TITLE_HEIGHT))
                                                                    .setFontSize(ConstDef::WINDOW_TITLE_HEIGHT - 4)
                                                                    .setNormalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE})
                                                                    .setAlignmentMode(AlignmentMode::AM_CENTER)
                                                                    .setFont(FontName::HarmonyOS_Sans_SC_Regular)
                                                                    .setCaption(m_dialog->m_title)
                                                                    .build())
                                                    .setBGColor({173, 216, 230, SDL_ALPHA_OPAQUE})  // 设置背景颜色:lightblue浅蓝色
                                                    .setBorderVisible(false)
                                                    .build());
    return *this;
}
DialogBuilder& DialogBuilder::addText(string text){
    m_dialog->m_texts.push_back(text);
    return *this;
}

shared_ptr<Dialog> DialogBuilder::build(void){
    return m_dialog;
}
