//https://blog.csdn.net/u014746838/article/details/81809288

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaEnum>
#include <QMimeData>
#include <QMimeDatabase>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(QCoreApplication::organizationName(), QCoreApplication::applicationName())
{
    ui->setupUi(this);
    //setFixedSize(QSize(400,350));
    readSettings();
    path = settings.value("path").toString();
    path_download = settings.value("path_download").toString();
    if(path_download == "") path_download = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    ui->lineEdit_path_download->setText(path_download);
    QAction *action_browse = new QAction(this);
    action_browse->setIcon(QIcon::fromTheme("folder"));
    connect(action_browse, SIGNAL(triggered(bool)), this, SLOT(chooseDownloadPath()));
    ui->lineEdit_path_download->addAction(action_browse, QLineEdit::TrailingPosition);
    connect(ui->listWidget_upload, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenu(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseDownloadPath()
{
    path = QFileDialog::getExistingDirectory(this, "选择路径", path);
    if(path != ""){
        //ui->pushButton_path_local->setToolTip(path);
        ui->lineEdit_path_download->setText(path);
        settings.setValue("path_download", path);
    }
}

void MainWindow::on_pushButton_upload1_clicked()
{
    ui->label_error_upload->setText("");
    bool b;
    int port = ui->lineEdit_port->text().toInt(&b);
    if(b){
        m_ftp.setHostPort(ui->lineEdit_IP->text(), port);
        QString username = ui->lineEdit_username->text();
        if(username == "") username = "anonymous";
        m_ftp.setUserInfo(username, ui->lineEdit_password->text());

        for(int i=0; i<ui->listWidget_upload->count(); i++){
            QString fp_local = ui->listWidget_upload->item(i)->toolTip();
            QString fp_remote = ui->lineEdit_path_upload->text() + "/" + QFileInfo(fp_local).fileName();
            ui->statusBar->showMessage("上传：" + fp_local, 5000);
            m_ftp.put(fp_local, fp_remote);
            connect(&m_ftp, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
            connect(&m_ftp, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadProgress(qint64, qint64)));
        }
    }else{
        QMessageBox::critical(NULL, "错误", "端口不是数字");
    }
}

void MainWindow::on_pushButton_download1_clicked()
{
    ui->label_error_upload->setText("");
    bool b;
    int port = ui->lineEdit_port->text().toInt(&b);
    if(b){
        m_ftp.setHostPort(ui->lineEdit_IP->text(), port);
        QString username = ui->lineEdit_username->text();
        if(username == "") username = "anonymous";
        m_ftp.setUserInfo(username, ui->lineEdit_password->text());

        QStringList SL = ui->textEdit->toPlainText().split("\n");
        for(int i=0; i<SL.length(); i++){
            QString path_remote = SL.at(i);
            QString path_download = ui->lineEdit_path_download->text() + "/" + QFileInfo(path_remote).fileName();
            ui->statusBar->showMessage("下载：" + path_remote);
            m_ftp.get(path_remote, path_download);
            connect(&m_ftp, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
            connect(&m_ftp, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
        }
    }else{
        QMessageBox::critical(NULL, "错误", "端口不是数字");
    }
}

void MainWindow::error(QNetworkReply::NetworkError error)
{
    qDebug() << error;
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

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    //qDebug() << e->mimeData()->formats();
    //if(e->mimeData()->hasFormat("text/uri-list")) //只能打开文本文件
    e->acceptProposedAction(); //可以在这个窗口部件上拖放对象
}

void MainWindow::dropEvent(QDropEvent *e) //释放对方时，执行的操作
{
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return;

    foreach (QUrl url, urls) {
        //qDebug() << url.toString();
        //open(url.toLocalFile());
        QString fp = url.toLocalFile();
        QString MIME = QMimeDatabase().mimeTypeForFile(fp).name();
        QIcon icon = QIcon::fromTheme(MIME.replace("/","-"));
        QListWidgetItem *LWI = new QListWidgetItem(icon, QFileInfo(fp).fileName());
        LWI->setToolTip(fp);
        ui->listWidget_upload->addItem(LWI);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::readSettings()
{
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::customContextMenu(const QPoint &pos)
{
    QList<QAction*> actions;
    QAction *action_delete;
    action_delete = new QAction(this);
    action_delete->setText("删除");
    action_delete->setIcon(QIcon::fromTheme("edit-delete"));
    actions.append(action_delete);
    QAction *result_action = QMenu::exec(actions, ui->listWidget_upload->mapToGlobal(pos));
    if (result_action == action_delete) {
        QList<QListWidgetItem*> selectedItems = ui->listWidget_upload->selectedItems();
        for (int i=0; i<selectedItems.length(); i++) {
            QListWidgetItem *LWI = selectedItems.at(i);
            ui->listWidget_upload->removeItemWidget(LWI);
            delete LWI;
        }
        return;
    }
}