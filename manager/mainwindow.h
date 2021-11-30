#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clearTable();
    void clearLog();
    void saveTable();
    void saveLog();
    void saveHidedLog();

private slots:
    void onSerial();
    void openSerial();
    void reload();

    void on_pushButton_control_play_clicked();

private:
    Ui::MainWindow *ui;

    void configSerial(void);
    void startToolbar(void);
    QStringList getPorts(void);

    QComboBox *cbListPort;
    bool connected = false;

    QAction *actionConnect;
    QAction *actionReload;
    QAction *actionClearLog;
    QAction *actionSaveLog;
    QAction *actionClearProgram;
    QAction *actionSaveProgram;
    QString line;
};
#endif // MAINWINDOW_H
