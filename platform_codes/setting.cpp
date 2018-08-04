#include "setting.h"
#include "ui_setting.h"
#include <QShortcut>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include "glwidget.h"



Setting::Setting(QWidget *parent, GLWidget *gl) : QWidget(parent), m_url("111.231.81.39"), m_show(true), toGl(gl)
{
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(showOrHide()));
    connect(this, &Setting::newRequest, toGl, &GLWidget::updataRequest);
//    QTimer::singleShot(15000, this, SLOT(updateRequest());
    setupUi(this);
    connect(pushButton, &QPushButton::clicked, this, &Setting::connectClicked);
    connect(pushButton_2, &QPushButton::clicked, this, &Setting::testClicked);
//  for testing
//    connect(pushButton_2, &QPushButton::clicked, this, &Setting::updateRequest);
// end
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateRequest()));
    timer->start(10000);
    this->lineEdit->setText(m_url.c_str());
    show();
}

bool Setting::getInfoFromServer()
{
    socket.connectToHost(m_url.c_str(), 12139);
    socket.write(QByteArray("hello", 6));
    socket.waitForReadyRead();
    m_info = socket.readAll().toStdString();
    socket.close();
    if(m_info[0] == 'f') return false;
    m_info.erase(0,5);
    QByteArray jdata(m_info.c_str(), m_info.size());
    QJsonParseError err;
    QJsonObject jobj = QJsonDocument::fromJson(jdata, &err).object();
    qDebug() << err.errorString();
    m_xstart = jobj["xstart"].toInt();
    m_ystart = jobj["ystart"].toInt();
    m_zstart = jobj["zstart"].toInt();
    m_xend = jobj["xend"].toInt();
    m_yend = jobj["yend"].toInt();
    m_zend = jobj["zend"].toInt();
    m_precision = jobj["precision"].toDouble();
    m_script.clear();
    for(const QJsonValue &i: jobj["script"].toArray()){
        m_script.push_back(i.toInt());
    }
    m_uniformExp.clear();
    m_colist.clear();
    for(const QJsonValue &i: jobj["colist"].toArray()){
        m_colist.push_back(i.toString().toStdString());
        m_uniformExp += "uniform float " + i.toString().toStdString() + ";\n";

    }
    m_functions = jobj["function"].toString().toStdString();
    return true;
}

void Setting::showOrHide()
{
    if(m_show){
        m_show = false;
        hide();
    }else{
        m_show = true;
        show();
    }
}

void Setting::updateRequest()
{
    if(!getInfoFromServer()){
        showInfo();
        return;
    }else{
        RequestInfo r;
        r.xstart(m_xstart);
        r.ystart(m_ystart);
        r.zstart(m_zstart);
        r.xend(m_xend);
        r.yend(m_yend);
        r.zend(m_zend);
        r.precision(m_precision);
        r.setScript(m_script);
        r.setFunction(m_functions);
        r.setUniformExp(m_uniformExp);
        r.setColist(m_colist);
        showInfo();
        emit newRequest(r);
    }
}

void Setting::showInfo()
{
    this->textBrowser->setText((
                "--xstart--\n" + std::to_string(m_xstart) + "\n--ystart--\n" + std::to_string(m_ystart)
                + "\n--zstart--\n" + std::to_string(m_zstart) + "\n--xend--\n" + std::to_string(m_xend)
                + "\n--yend--\n" + std::to_string(m_yend) + "\n--zend--\n" + std::to_string(m_zend)
                + "\n--precision--\n" + std::to_string(m_precision) + "\n--function--\n" + m_functions
                + "\n--uniforms--\n" + m_uniformExp + "\n--script size--\n" + std::to_string(m_script.size())).c_str()
                );
}


