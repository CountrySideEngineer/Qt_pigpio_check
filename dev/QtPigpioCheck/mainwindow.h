#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_GpioPinNum_valueChanged(const QString& text);
    void on_StartBlinkingBtn_toggled(bool checked);
    void on_BlinkTimer_dispatch();

protected:
    QTimer  m_BlinkTimer;
    bool    m_isLightOn;

    uint8_t m_pinNum;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
