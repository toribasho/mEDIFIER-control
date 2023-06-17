#include "basedevice.h"
#include "ui_basedevice.h"

#include <QDebug>
#include <QTimer>

BaseDevice::BaseDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseDevice)
{
    ui->setupUi(this);

    ui->nameEdit->setMaxLength(m_maxNameLength);

    connect(ui->noiseNormalButton, &QRadioButton::clicked, this, &BaseDevice::onBtnInNoiseGroupClicked);
    connect(ui->noiseReductionButton, &QRadioButton::clicked, this, &BaseDevice::onBtnInNoiseGroupClicked);
    connect(ui->noiseAmbientSoundButton, &QRadioButton::clicked, this, &BaseDevice::onBtnInNoiseGroupClicked);

    connect(ui->SENormalButton, &QRadioButton::clicked, this, &BaseDevice::onBtnInSoundEffectGroupClicked);
    connect(ui->SEPopButton, &QRadioButton::clicked, this, &BaseDevice::onBtnInSoundEffectGroupClicked);
    connect(ui->SEClassicalButton, &QRadioButton::clicked, this, &BaseDevice::onBtnInSoundEffectGroupClicked);
    connect(ui->SERockButton, &QRadioButton::clicked, this, &BaseDevice::onBtnInSoundEffectGroupClicked);

    connect(ui->CSNormalBox, &QCheckBox::clicked, this, &BaseDevice::onCheckBoxInControlSettingsGroupClicked);
    connect(ui->CSNoiseReductionBox, &QCheckBox::clicked, this, &BaseDevice::onCheckBoxInControlSettingsGroupClicked);
    connect(ui->CSAmbientSoundBox, &QCheckBox::clicked, this, &BaseDevice::onCheckBoxInControlSettingsGroupClicked);

    connect(ui->LDACOFFButton, &QRadioButton::clicked, this, &BaseDevice::onBtnInLDACGroupClicked);
    connect(ui->LDAC48kButton, &QRadioButton::clicked, this, &BaseDevice::onBtnInLDACGroupClicked);
    connect(ui->LDAC96kButton, &QRadioButton::clicked, this, &BaseDevice::onBtnInLDACGroupClicked);
}

BaseDevice::~BaseDevice()
{
    delete ui;
}

void BaseDevice::onBtnInNoiseGroupClicked()
{
    if(ui->noiseNormalButton->isChecked())
        emit sendCommand("C101");
    else if(ui->noiseReductionButton->isChecked())
        emit sendCommand("C102");
    else if(ui->noiseAmbientSoundButton->isChecked())
        emit sendCommand("C103");
}



void BaseDevice::onBtnInSoundEffectGroupClicked()
{
    if(ui->SENormalButton->isChecked())
        emit sendCommand("C400");
    else if(ui->SEPopButton->isChecked())
        emit sendCommand("C401");
    else if(ui->SEClassicalButton->isChecked())
        emit sendCommand("C402");
    else if(ui->SERockButton->isChecked())
        emit sendCommand("C403");
}

void BaseDevice::onCheckBoxInControlSettingsGroupClicked()
{
    QByteArray cmd = "\xF1\x0A";
    char val = 0;
    if(ui->CSNormalBox->isChecked())
        val += 1;
    if(ui->CSNoiseReductionBox->isChecked())
        val += 2;
    if(ui->CSAmbientSoundBox->isChecked())
        val += 4;
    cmd += val;
    qDebug() << cmd;
    emit sendCommand(cmd);
}

void BaseDevice::onBtnInLDACGroupClicked()
{
    if(ui->LDACOFFButton->isChecked())
        emit sendCommand("4900");
    else if(ui->LDAC48kButton->isChecked())
        emit sendCommand("4901");
    else if(ui->LDAC96kButton->isChecked())
        emit sendCommand("4902");
}

void BaseDevice::on_gameModeBox_clicked()
{
    if(ui->gameModeBox->isChecked())
        emit sendCommand("0901");
    else
        emit sendCommand("0900");
}


void BaseDevice::on_ASSlider_valueChanged(int value)
{
    ui->ASBox->blockSignals(true);
    ui->ASBox->setValue(value);
    ui->ASBox->blockSignals(false);
}


void BaseDevice::on_ASBox_valueChanged(int arg1)
{
    ui->ASSlider->blockSignals(true);
    ui->ASSlider->setValue(arg1);
    ui->ASSlider->blockSignals(false);
}


void BaseDevice::on_ASSetButton_clicked()
{
    QByteArray cmd = "\xC1\x03";
    cmd += (char)(6 + ui->ASBox->value());
    emit sendCommand(cmd);
    // setting ambient sound volume triggers ambient sound mode
    ui->noiseAmbientSoundButton->setChecked(true);
}


void BaseDevice::on_PVSlider_valueChanged(int value)
{
    ui->PVBox->blockSignals(true);
    ui->PVBox->setValue(value);
    ui->PVBox->blockSignals(false);
}


void BaseDevice::on_PVBox_valueChanged(int arg1)
{
    ui->PVSlider->blockSignals(true);
    ui->PVSlider->setValue(arg1);
    ui->PVSlider->blockSignals(false);
}


void BaseDevice::on_PVSetButton_clicked()
{
    QByteArray cmd = "\x06";
    cmd += (char)(ui->PVBox->value());
    emit sendCommand(cmd);
}


void BaseDevice::on_shutdownTimerGroup_clicked()
{
    if(!ui->shutdownTimerGroup->isChecked())
        emit sendCommand("D2");
}


void BaseDevice::on_STSlider_valueChanged(int value)
{
    ui->STBox->blockSignals(true);
    ui->STBox->setValue(value);
    ui->STBox->blockSignals(false);
}


void BaseDevice::on_STBox_valueChanged(int arg1)
{
    ui->STSlider->blockSignals(true);
    ui->STSlider->setValue(arg1);
    ui->STSlider->blockSignals(false);
}


void BaseDevice::on_STSetButton_clicked()
{
    QByteArray cmd = "\xD1\x00";
    cmd += (char)(ui->STBox->value());
    emit sendCommand(cmd);
}


void BaseDevice::on_poweroffButton_clicked()
{
    emit sendCommand("CE");
}


void BaseDevice::on_disconenctButton_clicked()
{
    emit sendCommand("CD");
}


void BaseDevice::on_re_pairButton_clicked()
{
    emit sendCommand("CF");
}


void BaseDevice::on_resetButton_clicked()
{
    emit sendCommand("07");
}


void BaseDevice::on_nameSetButton_clicked()
{
    QString name = ui->nameEdit->text();
    if(name.isEmpty())
        return;
    QByteArray nameBytes = name.toUtf8();
    // max length can be 24, 29, 30 or 35
    // the default length is 24(defined in basedevice.h)
    if(nameBytes.length() > m_maxNameLength)
    {
        emit showMessage(tr("The name is too long"));
        return;
    }
    QByteArray cmd = "\xCA";
    cmd += name.toUtf8();
    emit sendCommand(cmd);
}

void BaseDevice::processData(const QByteArray& data)
{
    const char head = data[0];
    const int len = (int)data[1];
    if(head == '\xBB')
    {
        // cmd + single byte response
        if(len == 2)
        {
            const char cmd = data[2];
            const char ch = data[3];
            if(cmd == '\xD5')
            {
                ui->SENormalButton->setChecked(ch == '\x00');
                ui->SEPopButton->setChecked(ch == '\x01');
                ui->SEClassicalButton->setChecked(ch == '\x02');
                ui->SERockButton->setChecked(ch == '\x03');
            }
            else if(cmd == '\x08')
            {
                ui->gameModeBox->setChecked(ch == '\x01');
            }
            else if(cmd == '\xD0')
            {
                ui->batteryLabel->setText(QString::number((int)ch) + "%");
            }
            else if(cmd == '\x48')
            {
                ui->LDACOFFButton->setChecked(ch == '\x00');
                ui->LDAC48kButton->setChecked(ch == '\x01');
                ui->LDAC96kButton->setChecked(ch == '\x02');
            }
            else if(cmd == '\x05')
            {
                ui->PVBox->setValue((int)ch);
            }
            else if(cmd == '\xD3')
            {
                ui->shutdownTimerGroup->setChecked(ch != '\x00');
            }
        }
        else if(len > 2)
        {
            const char cmd = data[2];
            if(cmd == '\xC8' && len == 7)
            {
                ui->MACLabel->setText(data.right(6).toHex(':'));
            }
            else if(cmd == '\xC6' && len == 4)
            {
                ui->firmwareLabel->setText(data.right(3).toHex('.'));
            }
            else if(cmd == '\xCC' && len == 3)
            {
                char mode = data[3];
                int ASVolume = (int)data[4] - 6;
                ui->noiseNormalButton->setChecked(mode == '\x01');
                ui->noiseReductionButton->setChecked(mode == '\x02');
                ui->noiseAmbientSoundButton->setChecked(mode == '\x03');
                ui->ASBox->setValue(ASVolume);
            }
            else if(cmd == '\xC9')
            {
                QString deviceName = QString::fromUtf8(data.mid(3));
                ui->nameEdit->setText(deviceName);
            }
            else if(cmd == '\xF0' && len == 3 && data[3] == '\x0A')
            {
                quint8 mode = data[4];
                ui->CSNormalBox->setChecked(mode & 1u);
                ui->CSNoiseReductionBox->setChecked(mode & 2u);
                ui->CSAmbientSoundBox->setChecked(mode & 4u);
            }
            else if(cmd == '\xD3' && len == 3)
            {
                int shutdownTimeout = data[4];
                ui->shutdownTimerGroup->setChecked(false);
                ui->STBox->setValue(shutdownTimeout);
            }
        }
    }
    else if(head == '\xCC')
    {

    }
}

void BaseDevice::readSettings()
{
    const int interval = 150;
    int i = 0;

    QTimer::singleShot(i, [ = ] {on_batteryGetButton_clicked();});
    i += interval;
    QTimer::singleShot(i, [ = ] {on_MACGetButton_clicked();});
    i += interval;
    QTimer::singleShot(i, [ = ] {on_firmwareGetButton_clicked();});
    i += interval;
    QTimer::singleShot(i, [ = ] {emit sendCommand("CC");});
    i += interval;
    QTimer::singleShot(i, [ = ] {emit sendCommand("C9");});
    i += interval;
    QTimer::singleShot(i, [ = ] {emit sendCommand("D5");});
    i += interval;
    QTimer::singleShot(i, [ = ] {emit sendCommand("08");});
    i += interval;
    QTimer::singleShot(i, [ = ] {emit sendCommand("F00A");});
    i += interval;
    QTimer::singleShot(i, [ = ] {emit sendCommand("48");});
    i += interval;
    QTimer::singleShot(i, [ = ] {emit sendCommand("05");});
    i += interval;
    QTimer::singleShot(i, [ = ] {emit sendCommand("D3");});
}

void BaseDevice::on_batteryGetButton_clicked()
{
    emit sendCommand("D0");
}


void BaseDevice::on_MACGetButton_clicked()
{
    emit sendCommand("C8");
}


void BaseDevice::on_firmwareGetButton_clicked()
{
    emit sendCommand("C6");
}


void BaseDevice::on_cmdSentButton_clicked()
{
    emit sendCommand(QByteArray::fromHex(ui->cmdEdit->text().toLatin1()), ui->cmdRawBox->isChecked());
}


void BaseDevice::on_PCPlayButton_clicked()
{
    emit sendCommand("C200");
}


void BaseDevice::on_PCPauseButton_clicked()
{
    emit sendCommand("C201");
}


void BaseDevice::on_PCVolUpButton_clicked()
{
    emit sendCommand("C202");
}


void BaseDevice::on_PCVolDownButton_clicked()
{
    emit sendCommand("C203");
}


void BaseDevice::on_PCPrevButton_clicked()
{
    emit sendCommand("C205");
}


void BaseDevice::on_PCNextButton_clicked()
{
    emit sendCommand("C204");
}
