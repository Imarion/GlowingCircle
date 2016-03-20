#include "vertextex.h"

VertexTex::VertexTex()
{
    m_tex    = QVector2D(1.0f, 1.0f);
}

VertexTex::VertexTex(QVector3D pos, QVector2D tex)
{
    setPos(pos);
    m_tex = tex;
}

QVector2D VertexTex::getTexCoord()
{
    return m_tex;
}

QVector3D VertexTex::getTangent()
{
    return m_tangent;
}

void VertexTex::setTangent(QVector3D tangent)
{
    m_tangent = tangent;
}
