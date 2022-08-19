#include "pagedexlistmain.h"
#include "ui_pagedexlistmain.h"

#include "global.h"
#include "style.h"

PageDexListMain::PageDexListMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDexListMain) {
    ui->setupUi(this);

    thisQRectBack = geometry();

    setScale();
    setStyle();
}

PageDexListMain::PageDexListMain(QList<entry_t> entrys, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDexListMain) {
    ui->setupUi(this);

    thisQRectBack = geometry();

    entry_t entry;
    foreach(entry, entrys) {
        append(entry.Ticker, entry.SpotAmount, entry.DexAmount);
    }
    renumerotateEntrys();
    repaint();

    setScale();
    setStyle();
}

PageDexListMain::~PageDexListMain() {
    clear();
    delete ui;
}

void PageDexListMain::setScale() {
    setGeometry(Global::Layout::scaleRect(thisQRectBack));

    PageDexList *entry;
    foreach (entry, EntryList) {
        entry->setScale();
    }
    repaint();
}

void PageDexListMain::setStyle() {
    PageDexList *entry;
    foreach (entry, EntryList) {
        entry->setStyle();
    }
}

void PageDexListMain::append(QString ticker, double spotAmount, double dexAmount) {
    entry_t entryData;
    entryData.Ticker = ticker;
    entryData.SpotAmount = spotAmount;
    entryData.DexAmount = dexAmount;
    PageDexList *entry = new PageDexList(entryData, this);
    entry->setVisible(true);
    EntryList.append(entry);
    renumerotateEntrys();
    repaint();
    connect(entry, &PageDexList::setupPushButton, this, &PageDexListMain::setupPushButton);
    connect(entry, &PageDexList::spotToDexPushButton, this, &PageDexListMain::spotToDexPushButton);
    connect(entry, &PageDexList::dexToSpotPushButton, this, &PageDexListMain::dexToSpotPushButton);
    connect(entry, &PageDexList::depositPushButton, this, &PageDexListMain::depositPushButton);
    connect(entry, &PageDexList::withdrawPushButton, this, &PageDexListMain::withdrawPushButton);
}

void PageDexListMain::insert(int index, QString ticker, double spotAmount, double dexAmount) {
    entry_t entryData;
    entryData.Ticker = ticker;
    entryData.SpotAmount = spotAmount;
    entryData.DexAmount = dexAmount;
    PageDexList *entry = new PageDexList(entryData, this);
    entry->setVisible(true);
    EntryList.insert(index, entry);
    renumerotateEntrys();
    repaint();
    connect(entry, &PageDexList::setupPushButton, this, &PageDexListMain::setupPushButton);
    connect(entry, &PageDexList::spotToDexPushButton, this, &PageDexListMain::spotToDexPushButton);
    connect(entry, &PageDexList::dexToSpotPushButton, this, &PageDexListMain::dexToSpotPushButton);
    connect(entry, &PageDexList::depositPushButton, this, &PageDexListMain::depositPushButton);
    connect(entry, &PageDexList::withdrawPushButton, this, &PageDexListMain::withdrawPushButton);
}

void PageDexListMain::clear() {
    PageDexList *entry;
    foreach (entry, EntryList) {
        entry->setVisible(false);
        delete entry;
    }
}

void PageDexListMain::removeAt(int index) {
    EntryList[index]->setVisible(false);
    delete EntryList[index];
    EntryList.removeAt(index);
    renumerotateEntrys();
}

void PageDexListMain::repaint() {
    PageDexList *entry;
    int posY = 0;
    foreach (entry, EntryList) {
        QRect rect = entry->geometry();
        rect.moveTop(posY);
        posY += entry->geometry().height();
        entry->setGeometry(rect);
    }
    QRect thisRect = this->geometry();
    thisRect.setBottom(posY);
    this->setGeometry(thisRect);
}

void PageDexListMain::renumerotateEntrys() {
    PageDexList *entry;
    int count = 0;
    foreach (entry, EntryList) {
        entry->setIndex(count);
        count++;
    }
}
