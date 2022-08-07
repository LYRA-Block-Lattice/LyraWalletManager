#ifndef STYLE_H
#define STYLE_H

#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

#include "color.h"
#include "global.h"

class Style {
public:
    static void setMainTabStyle(QTabWidget *tabWidget ) {
        tabWidget->setStyleSheet(
                    "QTabWidget::pane { /* The tab widget frame */"
                    "   border-top: " + QString::number(Global::Layout::scaleValue(2)) + "px solid " + Color::getMainLighten() +";"
                    "}"
                    "QTabWidget::tab-bar {"
                    "   left: 0px; /* move to the right by 5px */"
                    "}"
                    "/* Style the tab using the tab sub-control. Note that it reads QTabBar _not_ QTabWidget */"
                    "QTabBar::tab {"
                    "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + Color::getMainLighten() +", stop: 0.4 " + Color::getMain() +", stop: 0.5 " + Color::getMain() +", stop: 1.0 " + Color::getMainLighten() +");"
                    "   border: " + QString::number(Global::Layout::scaleValue(0)) + "px solid " + Color::getMainLighten() +";"
                    "   border-bottom-color: " + Color::getMainLighten() +"; /* same as the pane color */"
                    "   color: " + Color::getText2() +";"
                    "   border-top-left-radius: " + QString::number(Global::Layout::scaleValue(6)) + "px;"
                    "   border-top-right-radius: " + QString::number(Global::Layout::scaleValue(6)) + "px;"
                    "   min-width: " + QString::number(Global::Layout::scaleValue(PAGE_WIDTH / 5)) + "px;"
                    "   min-height: " + QString::number(Global::Layout::scaleValue(MENU_BAR_HEIGHT)) + "px;"
                    "   padding: 0px;"
                    "}"
                    "QTabBar::tab:selected, QTabBar::tab:hover {"
                    "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + Color::getMainLighten() +", stop: 0.4 " + Color::getMainLighten() +", stop: 0.5 " + Color::getMainLighten() +", stop: 1.0 " + Color::getMainLighten() +");"
                    "}"
                    "QTabBar::tab:selected {"
                    "   border-color: " + Color::getMain() + ";"
                    "   border-bottom-color: " + Color::getMainLighten() +"; /* same as pane color */"
                    "}"
                    "QTabBar::tab:!selected {"
                    "   margin-top: " + QString::number(Global::Layout::scaleValue(4)) + "px; /* make non-selected tabs look smaller */"
                    "}"
                    );
    }
#define ON_MOBILE 0
#if defined(__APPLE__) || defined(Q_OS_ANDROID)
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || defined(Q_OS_ANDROID)
#undef ON_MOBILE
#define ON_MOBILE 1
#endif
#endif
    static void setButtonStyle(QPushButton *button ) {
        button->setStyleSheet(
                    "QPushButton {"
                    "   background-color: " + Color::getMainLighten() + ";"
                    "   color: " + Color::getText2() + ";"
                    "   border-style: outset;"
                    "   border-width: " + QString::number(Global::Layout::scaleValue(1)) + "px;"
                    "   border-radius: " + QString::number(Global::Layout::scaleValue(6)) + "px;"
                    "   border-color: beige;"
                    // "   font: bold " + (char)Global::scaleValue(14 * FONT_OFFSET) + "px;"
                    // "   min-width: " + QString::number(Global::scaleValue(10)) + "em;"
                    "   padding: " + QString::number(Global::Layout::scaleValue(6)) + "px;"
                    "}"
                    "QPushButton::hover {"
                    "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + Color::getMain() +", stop: 0.4 " + Color::getMain() +", stop: 0.5 " + Color::getMain() +", stop: 1.0 " + Color::getMain() +");"
                    "}"
                    "QPushButton::pressed {"
#if ON_MOBILE
                   "   border-color: " + Color::getMainLighten() + ";"
                   // "   border-bottom-color: " + Color::getMainLighten() +"; /* same as pane color */"
                   "   background-color: " + Color::getMain() + ";"
#else
                    "   border-color: " + Color::getMain() + ";"
                    // "   border-bottom-color: " + Color::getMainLighten() +"; /* same as pane color */"
                    "   background-color: " + Color::getMainLighten() + ";"
#endif
                    "}");
    }
    static void setButtonRoundStyle(QPushButton *button ) {
        button->setStyleSheet(
                    "QPushButton {"
                    "   background-color: " + Color::getMainLighten() + ";"
                    "   color: " + Color::getText2() + ";"
                    "   border-style: solid;"
                    "   border-width: " + QString::number(Global::Layout::scaleValue(4)) + "px;"
                    "   border-radius: " + QString::number(Global::Layout::scaleValue(29)) + "px;"
                    "   border-color: " + Color::getBackgroundAcc() + ";"
                    "}"
                    "QPushButton::hover {"
                    "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + Color::getMain() +", stop: 0.4 " + Color::getMain() +", stop: 0.5 " + Color::getMain() +", stop: 1.0 " + Color::getMain() +");"
                    "}"
                    "QPushButton::pressed {"
#if ON_MOBILE
                   "   border-color: " + Color::getMainLighten() + ";"
                   // "   border-bottom-color: " + Color::getMainLighten() +"; /* same as pane color */"
                   "   background-color: " + Color::getMain() + ";"
#else
                    "   border-color: " + Color::getMain() + ";"
                    // "   border-bottom-color: " + Color::getMainLighten() +"; /* same as pane color */"
                    "   background-color: " + Color::getMainLighten() + ";"
#endif
                    "   border-style: solid;"
                    "   border-width: " + QString::number(Global::Layout::scaleValue(4)) + "px;"
                    "   border-color: " + Color::getBackgroundAcc() + ";"
                    "}");
    }
    static void setButtonTransparentStyle(QPushButton */*button*/ ) {
        /*button->setStyleSheet(
                    "QPushButton {"
                    "   background-color: transparent;"
                    "   color: " + Color::getText2() + ";"
                    "}"
                    );*/
    }
    static void setLabelStyle(QLabel *label) {
        label->setStyleSheet(
                    "QLabel {"
                    "   background-color: transparent;"
                    "   color: " + Color::getText2() + ";"
                    "}"
                    );
    }
    static void setLineEditlStyle(QLineEdit *label) {
        label->setStyleSheet(
            "QLineEdit {"
            "   background-color: " + Color::getMain() + ";"
            "   color: " + Color::getText2() + ";"
            "   border-style: outset;"
            "   border-width: " + QString::number(Global::Layout::scaleValue(1)) + "px;"
            "   border-radius: " + QString::number(Global::Layout::scaleValue(6)) + "px;"
            "   border-color: beige;"
            // "   font: bold " + (char)Global::scaleValue(14 * FONT_OFFSET) + "px;"
            // "   min-width: " + QString::number(Global::scaleValue(10)) + "em;"
            "   padding: " + QString::number(Global::Layout::scaleValue(6)) + "px;"
            "}"
            "QLineEdit[text=\"\"]{"
            "color: grey;"
            "}"
        );
    }
    static void setLineEditlWhiteStyle(QLineEdit *lineEdit) {
        setLineEditlWhiteStyle(lineEdit, "black");
    }
    static void setLineEditlWhiteStyle(QLineEdit *lineEdit, QString textColot) {
        lineEdit->setStyleSheet(
            "QLineEdit[text=\"\"]{"
            "    color:gray;"
            "}"
            "QLineEdit{ "
            "    color:" + textColot + "; "
            "    border-style: solid;"
            "    border-radius: " + QString::number(Global::Layout::scaleValue(5)) + "px;"
            "    border-width: 1px;"
            "    border-color: rgb(235,235,235);"
            "    padding-left: " + QString::number(Global::Layout::scaleValue(4)) + "px;"
            "    padding-right: " + QString::number(Global::Layout::scaleValue(4)) + "px;"
            "}"
        );
    }
    static void setComboBoxStyle(QComboBox *combobox) {
        combobox->setStyleSheet(
            "QComboBox {"
            "    border: 1px solid lightgray;"
            "    border-radius: " + QString::number(Global::Layout::scaleValue(5)) + "px;"
            "    padding: 1px " + QString::number(Global::Layout::scaleValue(18)) + "px 1px " + QString::number(Global::Layout::scaleValue(3)) + "px;"
            "    min-width: 6em;"
            "}"
            "QComboBox:on { /* shift the text when the popup opens */"
            "    padding-top: " + QString::number(Global::Layout::scaleValue(3)) + "px;"
            "    padding-left: " + QString::number(Global::Layout::scaleValue(4)) + "px;"
            "}"
            "QComboBox::drop-down {"
            "    subcontrol-origin: padding;"
            "    subcontrol-position: top right;"
            "    width: " + QString::number(Global::Layout::scaleValue(15)) + "px;"
            "    border-left-width: 1px;"
            "    border-left-color: darkgray;"
            "    border-left-style: solid; /* just a single line */"
            "    border-top-right-radius: " + QString::number(Global::Layout::scaleValue(5)) + "px; /* same radius as the QComboBox */"
            "    border-bottom-right-radius: " + QString::number(Global::Layout::scaleValue(5)) + "px;"
            "}"
            "QComboBox::drop-down::hover {"
            "    background-color: rgba(0, 0, 0, 16);"
            "}"
            "QComboBox::drop-down::pressed {"
            "    background-color: rgba(0, 0, 0, 64);"
            "}"
            "QComboBox::down-arrow {"
            "    image: url(:/res/ic/res/ic/dropDownArrow.png);"
            "    width: " + QString::number(Global::Layout::scaleValue(12)) + "px;"
            "    height: " + QString::number(Global::Layout::scaleValue(12)) + "px;"
            "}"
            "QComboBox::down-arrow:on { /* shift the arrow when popup is open */"
            "    top: 1px;"
            "    left: 1px;"
            "}"
            "QListView::item {"
            "     height: 31px;"
            "    background-color: white;"
            "    color: black;"
            "}"
            "QListView::item:selected:!active, QComboBox::item:selected:active {"
            "    background-color: lightgrey;"
            "    color: black;"
            "}"
            "QListView::item::hover {"
            "    background-color: grey;"
            "    /*font: url(:/Resources/Fonts/GameJam.ttf);*/"
            "    color: black;"
            "}"
     );
    }
};

#endif // STYLE_H
