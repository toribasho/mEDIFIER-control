#ifndef W820NBPLUSFORM_H
#define W820NBPLUSFORM_H

#include <QWidget>

namespace Ui
{
class W820NBPlusForm;
}

class W820NBPlusForm : public QWidget
{
    Q_OBJECT

public:
    explicit W820NBPlusForm(QWidget *parent = nullptr);
    ~W820NBPlusForm();

private:
    Ui::W820NBPlusForm *ui;

signals:
    void sendCommand(const QByteArray& cmd, bool isRaw = false);
    void sendCommand(const char* hexCmd, bool isRaw = false);
private slots:
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
};

#endif // W820NBPLUSFORM_H
