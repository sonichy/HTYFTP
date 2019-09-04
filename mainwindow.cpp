//https://blog.csdn.net/u014746838/article/details/81809288

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaEnum>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(QCoreApplication::organizationName(), QCoreApplication::applicationName())
{
    ui->setupUi(this);
    setFixedSize(QSize(400,350));
    path = settings.value("path").toString();
    local = settings.value("local").toString();
    ui->pushButton_choose_upload->setText(QFileInfo(path).fileName());
    ui->pushButton_choose_upload->setToolTip(path);
    ui->pushButton_path_local->setToolTip(local);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_choose_upload_clicked()
{
    path = QFileDialog::getOpenFileName(this, "选择文件", path, "文件(*)");
    if(path != ""){
        ui->pushButton_choose_upload->setText(QFileInfo(path).fileName());
        ui->pushButton_choose_upload->setToolTip(path);
        settings.setValue("path", path);
    }
}

void MainWindow::on_pushButton_path_local_clicked()
{
    path = QFileDialog::getExistingDirectory(this, "选择路径", path);
    if(path != ""){
        ui->pushButton_path_local->setToolTip(path);
        settings.setValue("local", path);
    }
}

void MainWindow::on_pushButton_upload_clicked()
{
    ui->label_error_upload->setText("");
    bool b;
    int port = ui->lineEdit_port->text().toInt(&b);
    if(b){
        m_ftp.setHostPort(ui->lineEdit_IP->text(), port);
        QString username = ui->lineEdit_username->text();
        if(username == "") username = "anonymous";
        m_ftp.setUserInfo(username, ui->lineEdit_password->text());

        QString filepath = ui->lineEdit_path_upload->text() + "/" + QFileInfo(ui->pushButton_choose_upload->text()).fileName();
        ui->statusBar->showMessage("上传：" + filepath, 5000);
        m_ftp.put(ui->pushButton_choose_upload->toolTip(), filepath);
        connect(&m_ftp, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
        /*
        connect(&m_ftp, &QNetworkReply::error, [=](QNetworkReply::NetworkError error){
            qDebug() << error;
            QNetworkReply *pReply = qobject_cast<QNetworkReply*>(sender());
            QMessageBox::critical(NULL, "错误", pReply->errorString());
        });
        */
        connect(&m_ftp, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadProgress(qint64, qint64)));
        /*
        connect(&m_ftp, &QNetworkReply::uploadProgress, [=](qint64 bytesSent, qint64 bytesTotal){
            ui->progressBar_upload->setMaximum(bytesTotal);
            ui->progressBar_upload->setValue(bytesSent);
        });
        */
    }else{
        QMessageBox::critical(NULL, "错误", "端口不是数字");
    }
}

void MainWindow::on_pushButton_download_clicked()
{
    ui->label_error_upload->setText("");
    bool b;
    int port = ui->lineEdit_port->text().toInt(&b);
    if(b){
        m_ftp.setHostPort(ui->lineEdit_IP->text(), port);
        QString username = ui->lineEdit_username->text();
        if(username == "") username = "anonymous";
        m_ftp.setUserInfo(username, ui->lineEdit_password->text());
        QString path_remote = ui->lineEdit_path_download->text();
        QString path_local = ui->pushButton_path_local->toolTip() + "/" + QFileInfo(path_remote).fileName();
        ui->statusBar->showMessage("下载：" + path_remote, 5000);
        m_ftp.get(path_remote, path_local);
        connect(&m_ftp, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
        connect(&m_ftp, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
    }else{
        QMessageBox::critical(NULL, "错误", "端口不是数字");
    }
}

void MainWindow::error(QNetworkReply::NetworkError error)
{
    //qDebug() << error;
    QMetaEnum metaEnum = QMetaEnum::fromType<QNetworkReply::NetworkError>();
    ui->label_error_upload->setText("错误：" + QString(metaEnum.valueToKey(error)));
    /*
    QNetworkReply *NReply = qobject_cast<QNetworkReply*>(sender());
    qDebug() << NReply->operation() << error;   // Crash NReply 0x0
    if(NReply->operation() == QNetworkAccessManager::PutOperation){
        ui->label_error_upload->setText("错误：上传 " + QString(metaEnum.valueToKey(error)));
    }else{
        ui->label_error_download->setText("错误：下载 " + QString(metaEnum.valueToKey(error)));
    }
    */
}

void MainWindow::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    ui->progressBar_upload->setMaximum(bytesTotal);
    ui->progressBar_upload->setValue(bytesSent);
}

void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->progressBar_download->setMaximum(bytesTotal);
    ui->progressBar_download->setValue(bytesReceived);
}