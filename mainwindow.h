#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ftpmanager.h"
#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSettings settings;
    QString path, local;
    FtpManager m_ftp;

private slots:
    void on_pushButton_upload_clicked();
    void on_pushButton_choose_upload_clicked();
    void on_pushButton_path_local_clicked();
    void on_pushButton_download_clicked();
    void error(QNetworkReply::NetworkError error);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

};

#endif // MAINWINDOW_H