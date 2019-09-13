#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ftpmanager.h"
#include <QMainWindow>
#include <QSettings>
#include <QDragEnterEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QSettings settings;
    QString path, path_download;
    FtpManager m_ftp;
    void readSettings();

private slots:
    void on_pushButton_upload1_clicked();
    void on_pushButton_download1_clicked();
    void error(QNetworkReply::NetworkError error);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void chooseDownloadPath();
    void customContextMenu(const QPoint &pos);

};

#endif // MAINWINDOW_H