#ifndef PAGESETTINGS_H
#define PAGESETTINGS_H

#include <QWidget>

namespace Ui {
class PageSettings;
}

class PageSettings : public QWidget
{
    Q_OBJECT

public:
    explicit PageSettings(QWidget *parent = nullptr);
    ~PageSettings();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();

private slots:
    void on_backPushButton_clicked();
    void on_languageComboBox_currentTextChanged(const QString &arg1);
    void on_networkComboBox_currentIndexChanged(int index);
    void on_backupWalletPushButton_clicked();


private:
    Ui::PageSettings *ui;

    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;

    QRect languageComboBoxQRectBack;
    QSize languageComboBoxQSizeBack;
    QFont languageComboBoxQFontBack;
    QRect networkComboBoxQRectBack;
    QSize networkComboBoxQSizeBack;
    QFont networkComboBoxQFontBack;
    QRect backupWalletPushButtonQRectBack;
    QSize backupWalletPushButtonQSizeBack;
    QFont backupWalletPushButtonQFontBack;

    bool initialized = false;
};

#endif // PAGESETTINGS_H
