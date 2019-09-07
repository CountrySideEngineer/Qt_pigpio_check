#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "pigpio/pigpio.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->on_GpioPinNum_valueChanged(this->ui->GpioPinNum->text());

    this->m_BlinkTimer.setSingleShot(false);
    this->m_BlinkTimer.setInterval(1000);   //Every 1 second disptach event.

    connect(&(this->m_BlinkTimer), SIGNAL(timeout()), this, SLOT(on_BlinkTimer_dispatch()));

    if (PI_INIT_FAILED == gpioInitialise()) {
        qDebug() << "gpioInitialise() failed";
    }
}

MainWindow::~MainWindow()
{
    gpioTerminate();

    delete ui;
}

/**
 * @brief MainWindow::on_GpioPinNum_valueChanged    Event handler of spin box.
 * @param text  Text data.
 */
void MainWindow::on_GpioPinNum_valueChanged(const QString& text)
{
    qDebug() << text;

    this->m_pinNum =  static_cast<uint8_t>(text.toInt());
}

/**
 * @brief MainWindow::on_StartBlinkingBtn_toggled   Event handler of start blinking button.
 * @param checked   State of button, true means checked, false means not checked.
 */
void MainWindow::on_StartBlinkingBtn_toggled(bool checked)
{
    if (checked) {
        qDebug() << "gpioSetMode() - " << m_pinNum;
        int setModeResult = gpioSetMode(this->m_pinNum, PI_OUTPUT);
        if (0 == setModeResult) {
            qDebug() << "gpioSetMode() - OK";
        } else {
            if (PI_BAD_GPIO == setModeResult) {
                qDebug() << "gpioSetMode() - FAILED : PI_BAD_GPIO";
            } else if (PI_BAD_MODE == setModeResult) {
                qDebug() << "gpioSetMode() - FAILED : PI_BAD_MODE";
            } else {
                qDebug() << "gpioSetMode() - FAILED : UNKNOWN";
            }

            this->ui->StartBlinkingBtn->setChecked(false);  //Cancel the check.

            return;
        }
        this->ui->GpioPinNum->setEnabled(false);
        qDebug() << "Checked - Start timer.";

        this->m_BlinkTimer.start(1000);
    } else {
        qDebug() << "not Checked - Stop timer.";

        gpioWrite(this->m_pinNum, PI_LOW);

        this->ui->GpioPinNum->setEnabled(true);

        this->m_BlinkTimer.stop();
    }
}

/**
 * @brief MainWindow::on_BlinkTimer_dispatch    Timer dispatch event handler.
 */
void MainWindow::on_BlinkTimer_dispatch()
{
    this->m_isLightOn = !this->m_isLightOn;
    if (this->m_isLightOn) {
        qDebug() << "Turn on Light";

        gpioWrite(this->m_pinNum, PI_HIGH);
    } else {
        qDebug() << "Turn off Light";

        gpioWrite(this->m_pinNum, PI_LOW);
    }
}
