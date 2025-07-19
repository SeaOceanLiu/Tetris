#ifndef DialogH
#define DialogH
#include <memory>
#include "Panel.h"
#include "Button.h"

class Dialog : public Panel, public std::enable_shared_from_this<Dialog>
{
friend class DialogBuilder;
private:
    string m_title;
    vector<string>m_texts;
    int m_currentPage;
    string m_onScreenText;
    SRect m_clientRect;

    shared_ptr<Panel> m_titleBar;
    shared_ptr<Button> m_okButton;
    shared_ptr<Button> m_cancelButton;
    vector<shared_ptr<Label>> m_screenText;
public:
    static const int TEXT_LINE_COUNT = 13;
    Dialog(Control* parent, SRect rect, float xScale=1.0f, float yScale=1.0f);

    bool handleEvent(shared_ptr<Event> event) override;

    void onClose(shared_ptr<Button> btn);
    void onOk(shared_ptr<Button> btn);
    void onCancel(shared_ptr<Button> btn);
    void draw(void) override;
    void show(void) override;
};

class DialogBuilder
{
private:
    shared_ptr<Dialog> m_dialog;
public:
    DialogBuilder(Control* parent, SRect rect, float xScale=1.0f, float yScale=1.0f);

    DialogBuilder& setOkBtnCaption(string okCaption);
    DialogBuilder& setCancelBtnCaption(string cancelCaption);
    DialogBuilder& setTitle(string title);
    DialogBuilder& addText(string text);
    shared_ptr<Dialog> build(void);
};
#endif // Dialog.hpp