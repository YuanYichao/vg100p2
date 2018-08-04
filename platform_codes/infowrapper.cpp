#include "infowrapper.h"
#include "QFile"
#include <exception>
#include <QRegularExpression>
#include <QRegExp>
#include <string>

InfoWrapper::InfoWrapper(const RequestInfo &req):
    r(req), vm(new TinyVM)
{
    QFile f("C:/Users/yyc19/Documents/Qt/OpenGlFunc/marchingcube.gtpl");
    if(!f.open(QFile::ReadOnly)) throw std::runtime_error("no tpl file");
    tpl = f.readAll();
    vm->addSource(r.getScript().data(), r.getScript().size() * 4);
    QRegExp unif("@<uniform>");
    QRegExp func("@<func>");
    QRegExp cen("@<center>");
    tpl.replace(unif, r.getUniformExp().c_str());
    tpl.replace(func, r.getFunction().c_str());
    QVector3D c = r.center();
    tpl.replace(cen, (
                    "vec3(" + std::to_string(c.x()) + ","
                    + std::to_string(c.y()) + "," + std::to_string(c.z())
                    + ")"
                    ).c_str()
                );
    auto t = r.getColist();
    for(auto const &i : t){
        coList.push_back(std::pair<std::string, float>{i, 1.0f});
    }
}

void InfoWrapper::refresh()
{
    vm->run();
    int c = 0;
    for(auto &i : coList){
        i.second = vm->varValue("var" + std::to_string(c));
        c++;
    }
    return;
}

QString InfoWrapper::shaderCode() const
{
    return tpl;
}

InfoWrapper::const_iteratorType InfoWrapper::cbegin() const
{
    return coList.cbegin();
}

InfoWrapper::const_iteratorType InfoWrapper::cend() const
{
    return coList.cend();
}

int InfoWrapper::xwidth() const
{
    return r.xsize();
}

int InfoWrapper::ywidth() const
{
    return r.ysize();
}

int InfoWrapper::zwidth() const
{
    return r.zsize();
}

float InfoWrapper::width() const
{
    return r.width();
}

InfoWrapper::~InfoWrapper()
{
    delete vm;
}
