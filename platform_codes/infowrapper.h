#ifndef INFOWRAPPER_H
#define INFOWRAPPER_H
#include "requestinfo.h"
#include "tinyVM.h"
#include <QString>
#include <vector>
#include <utility>
#include <string>
#include <QVector3D>

class InfoWrapper
{
public:
    using iteratorType = std::vector<std::pair<std::string, float>>::iterator;
    using const_iteratorType = std::vector<std::pair<std::string, float>>::const_iterator;
    InfoWrapper(const RequestInfo &req);
    void refresh();
    QString shaderCode() const;
    const_iteratorType cbegin() const;
    const_iteratorType cend() const;
    int xwidth() const;
    int ywidth() const;
    int zwidth() const;
    float width() const;
    ~InfoWrapper();

private:
    std::vector<std::pair<std::string, float>> coList;
    RequestInfo r;
    TinyVM *vm;
    QString tpl;
};

#endif // INFOWRAPPER_H
