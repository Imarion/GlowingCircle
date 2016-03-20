#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

class Vertex
{
public:
    Vertex();
    Vertex(QVector3D inPos, QVector3D inNormal);

    QVector3D getPos();
    QVector3D getNormal();

    void setNormal(QVector3D inNormal);
    void setPos(QVector3D inPos);

private:
    QVector3D m_pos;
    QVector3D m_normal;
};

#endif // VERTEX_H
