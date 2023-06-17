#ifndef BASEDEVICE_H
#define BASEDEVICE_H

#include <QWidget>

namespace Ui
{
class BaseDevice;
}

class BaseDevice : public QWidget
{
    Q_OBJECT

public:
    explicit BaseDevice(QWidget *parent = nullptr);
    ~BaseDevice();

public slots:
    void processData(const QByteArray &data);
    void readSettings();
protected:
    Ui::BaseDevice *ui;
    // max length can be 24, 29, 30 or 35
    // the default length is 24
    int m_maxNameLength = 24;

protected slots:
    void onBtnInNoiseGroupClicked();
    void onBtnInSoundEffectGroupClicked();
    void on_gameModeBox_clicked();
    void on_ASSlider_valueChanged(int value);
    void on_ASBox_valueChanged(int arg1);
    void on_ASSetButton_clicked();
    void on_PVSlider_valueChanged(int value);
    void on_PVBox_valueChanged(int arg1);
    void on_PVSetButton_clicked();
    void onCheckBoxInControlSettingsGroupClicked();
    void on_shutdownTimerGroup_clicked();
    void on_STSlider_valueChanged(int value);
    void on_STBox_valueChanged(int arg1);
    void on_STSetButton_clicked();
    void on_poweroffButton_clicked();
    void on_disconenctButton_clicked();
    void on_re_pairButton_clicked();
    void on_resetButton_clicked();
    void onBtnInLDACGroupClicked();
    void on_nameSetButton_clicked();
    void on_batteryGetButton_clicked();
    void on_MACGetButton_clicked();
    void on_firmwareGetButton_clicked();
    void on_cmdSentButton_clicked();
    void on_PCPlayButton_clicked();
    void on_PCPauseButton_clicked();
    void on_PCVolUpButton_clicked();
    void on_PCVolDownButton_clicked();
    void on_PCPrevButton_clicked();
    void on_PCNextButton_clicked();

signals:
    void sendCommand(const QByteArray& cmd, bool isRaw = false);
    void sendCommand(const char* hexCmd, bool isRaw = false);
    void showMessage(const QString& msg);
};

#endif // BASEDEVICE_H