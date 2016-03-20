#include "vertexcol.h"

VertexCol::VertexCol()
{
    m_color  = QVector3D(1.0f, 1.0f, 1.0f);
}

VertexCol::VertexCol(QVector3D pos, QVector3D color)
{
    setPos(pos);
    m_color = color;
}

QVector3D VertexCol::getColor()
{
    return m_color;
}
