#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QSettings>

#include "deviceform.h"
#include "devform.h"
#include "comms/comm.h"
#include "devices/basedevice.h"

class QSystemTrayIcon;


QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static void devMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);
public slots:
    void showMessage(const QString &msg);
    void connectToAudio(const QString &address);
    void updateLastAudioDeviceAddress(const QString &address);
private:
    Ui::MainWindow *ui;

    DeviceForm* m_deviceForm = nullptr;
    DevForm* m_devForm = nullptr;
    Comm* m_comm = nullptr;
    bool m_connected = false;
    bool m_autoConnect = false;
    BaseDevice* m_device = nullptr;
    QJsonObject* m_deviceInfo = nullptr;
    QHash<QBluetoothUuid, QString> m_deviceServiceMap;
    int m_clickCounter = 0;
    bool m_isDevMode = false;
    QSettings* m_settings = nullptr;
    static MainWindow* m_ptr;
    static const char* m_translatedNames[];

    QSystemTrayIcon* m_sysTrayIcon;
    QIcon m_noDevice;
    QIcon m_deviceConnected_normal;
    QIcon m_deviceConnected_noiseCancelation;
    QIcon m_deviceConnected_ambientSound;

    void changeDevice(const QString &deviceName);
    void connectDevice2Comm();
    void loadDeviceInfo();

    void switchToDevice();
    void createTrayIcon();
private slots:
    void connectToDevice(const QString &address, bool isBLE);
    void disconnectDevice();
    void onCommStateChanged(bool state);
    void on_readSettingsButton_clicked();

    void on_deviceBox_currentIndexChanged(int index);
    void on_tabWidget_tabBarClicked(int index);

    void processDeviceFeature(const QString &feature, bool isBLE);
    void processDeviceStatus(const DeviceStatus &status);

signals:
    void commStateChanged(bool connected);
    void readSettings(bool force = false);
    void devMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg);
protected slots:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *e);
};
#endif // MAINWINDOW_H
