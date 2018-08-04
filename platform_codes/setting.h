#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QTcpSocket>
#include "ui_setting.h"
#include "requestinfo.h"
#include <string>
#include <QString>
#include <QByteArray>
#include "glwidget.h"

class Setting : public QWidget, private Ui::Setting
{
    Q_OBJECT
public:
    explicit Setting(QWidget *parent = nullptr, GLWidget *gl = nullptr);

signals:
    void newRequest(RequestInfo req);

public slots:
    void connectClicked(){
        m_url = this->lineEdit->text().toStdString();
    }
    void testClicked(){
         showInfo();
    }
    void showOrHide();
    void updateRequest();
private:
    QTcpSocket socket;
    bool getInfoFromServer();
    void testServer();
    void showInfo();
    void prepareData();

    std::string m_url;
    std::string m_info;
    std::string m_shaderText;
    int m_xstart;
    int m_ystart;
    int m_zstart;
    int m_xend;
    int m_yend;
    int m_zend;
    float m_precision;
    bool m_show;
    std::vector<int> m_script;
    std::string m_uniformExp;
    std::string m_functions;
    GLWidget *toGl;
    std::vector<std::string> m_colist;
};

#endif // SETTING_H
