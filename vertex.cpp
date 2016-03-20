#include "vertex.h"

Vertex::Vertex()
{
    m_pos    = QVector3D(0.0f, 0.0f, 0.0f);
    m_normal = QVector3D(0.0f, 0.0f, 0.0f);    
}

Vertex::Vertex(QVector3D pos, QVector3D normal)
{
    m_pos    = pos;
    m_normal = normal;
}

QVector3D Vertex::getPos()
{
    return m_pos;
}

QVector3D Vertex::getNormal()
{
    return m_normal;
}

void Vertex::setNormal(QVector3D inNormal)
{
    m_normal = inNormal;
}

void Vertex::setPos(QVector3D inPos)
{
    m_pos = inPos;
}
