#ifndef REQUESTINFO_H
#define REQUESTINFO_H
#include <QVector3D>
#include <string>
#include <vector>

class RequestInfo{
public:
    friend class FuncShaderGenerator;
    void xstart(int xs)
    {
        m_xs = xs;
    }
    void xend(int xe){
        m_xe = xe;
    }
    void ystart(int ys){
        m_ys = ys;
    }
    void yend(int ye){
        m_ye = ye;
    }
    void zstart(int zs){
        m_zs = zs;
    }
    void zend(int ze){
        m_ze = ze;
    }
    void precision(float pre){
        m_presicion = pre;
    }
    QVector3D center() const
    {
        return QVector3D(m_xs + float(xsize()) / 2, m_ys + float(ysize()) / 2, m_zs + float(zsize()) /2) * m_presicion;
    }
    unsigned int xsize() const
    {
        return m_xe - m_xs;
    }
    unsigned int ysize() const
    {
        return m_ye - m_ys;
    }
    unsigned int zsize() const
    {
        return m_ze - m_zs;
    }
    float width() const
    {
        return m_presicion / 8;
    }

    std::vector<int> getScript() const{
        return script;
        }
    void setScript(const std::vector<int> &value){
        script = value;
        }

    std::string getFunction() const{
        return function;
        }
    void setFunction(const std::string &value){
        function = value;
        }

    std::string getUniformExp() const{
        return uniformExp;
        }
    void setUniformExp(const std::string &value){
        uniformExp = value;
        }

    std::vector<std::string> getColist() const {
    return colist;
    }

    void setColist(const std::vector<std::string> &value){
    colist = value;
    }

private:
    int m_xs = 0;
    int m_ys = 0;
    int m_zs = 0;
    int m_xe = 0;
    int m_ye = 0;
    int m_ze = 0;
    float m_presicion = 0;
    std::vector<int> script;
    std::string function;
    std::string uniformExp;
    std::vector<std::string> colist;
};

#endif // REQUESTINFO_H


