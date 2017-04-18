#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QMutex>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>

QString LOCAL_LOG_FILE_NAME;

void localLog(QtMsgType type,
                   const QMessageLogContext &context,
                   const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //本地日志
    QDir logDir("./log");
    if (!logDir.exists())
        logDir.mkdir(logDir.absolutePath());                            //创建log目录
    LOCAL_LOG_FILE_NAME = QString(logDir.absolutePath()).append("/")
            .append("log_")
            .append(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"))
            .append(".txt");
//    qInstallMessageHandler(localLog);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));

    w.resize(800, 400);
    w.show();

    return a.exec();
}

/**
 * @brief localLog  将qDebug的log重定向到本地文件
 * @param type
 * @param context
 * @param msg
 */
void localLog(QtMsgType type,
              const QMessageLogContext &context,
              const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type) {
        case QtDebugMsg:
            text = QString("Debug:");
            break;

        case QtWarningMsg:
            text = QString("Warning:");
            break;

        case QtCriticalMsg:
            text = QString("Critical:");
            break;

        case QtFatalMsg:
            text = QString("Fatal:");
            break;
    }

    QString contextInfo = QString("File:(%1) Line:(%2)")
            .arg(QString(context.file))
            .arg(context.line);
    QString currentDateTime = QDateTime::currentDateTime()
            .toString("yyyy-MM-dd hh:mm:ss ddd");
    QString currentDate = QString("(%1)").arg(currentDateTime);
    QString message = QString("%1 %2 %3 %4")
            .arg(text).arg(contextInfo).arg(currentDate).arg(msg);

    QFile file(LOCAL_LOG_FILE_NAME);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}
