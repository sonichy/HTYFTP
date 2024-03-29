#include <QFileInfo>
#include "ftpmanager.h"

FtpManager::FtpManager(QObject *parent)
    : QObject(parent)
{
    // 设置协议
    m_pUrl.setScheme("ftp");
}

// 设置地址和端口
void FtpManager::setHostPort(const QString &host, int port)
{
    m_pUrl.setHost(host);
    m_pUrl.setPort(port);
}

// 设置登录 FTP 服务器的用户名和密码
void FtpManager::setUserInfo(const QString &userName, const QString &password)
{
    m_pUrl.setUserName(userName);
    m_pUrl.setPassword(password);
}

// 上传文件
void FtpManager::put(const QString &fileName, const QString &path)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    m_pUrl.setPath(path);
    qDebug() << "put" << fileName << path;
    QNetworkReply *pReply = m_manager.put(QNetworkRequest(m_pUrl), data);
    connect(pReply, SIGNAL(uploadProgress(qint64, qint64)), this, SIGNAL(uploadProgress(qint64, qint64)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
}

// 下载文件
void FtpManager::get(const QString &path, const QString &fileName)
{
    m_pUrl.setPath(path);
    qDebug() << "get" << path << fileName;
    QNetworkReply *pReply = m_manager.get(QNetworkRequest(m_pUrl));
    connect(pReply, &QNetworkReply::finished, [=]() {
        qDebug() << pReply->errorString();
        switch (pReply->error()) {
        case QNetworkReply::NoError :{
            QFile m_file;
            m_file.setFileName(fileName);
            m_file.open(QIODevice::WriteOnly);
            m_file.write(pReply->readAll());
            m_file.flush();
            m_file.close();
            break;}
        default:
            break;
        }
        pReply->deleteLater();
    });
    connect(pReply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));

}