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
    void clearProgram();
    void clearLog();
    void saveProgram();
    void saveLog();
    void saveHidedLog();

private slots:
    void onSerial();
    void openSerial();
    void reload();

    void on_pushButton_control_play_clicked();

    void on_pushButton_home_clicked();

    void on_pushButton_move_1_clicked();

    void on_pushButton_move_2_clicked();

    void on_pushButton_move_3_clicked();

    void on_pushButton_move_4_clicked();

    void on_pushButton_move_5_clicked();

    void on_pushButton_move_6_clicked();

    void on_pushButton_move_7_clicked();

    void on_pushButton_move_8_clicked();

    void on_pushButton_move_9_clicked();

    void on_pushButton_move_10_clicked();

    void on_textEdit_program_textChanged();

private:
    Ui::MainWindow *ui;

    void configSerial(void);
    void startToolbar(void);

    QString jog_list(QString, QString, QString);

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
