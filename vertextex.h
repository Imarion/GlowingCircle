#ifndef VERTEXTEX_H
#define VERTEXTEX_H

#include <QVector3D>
#include <QVector2D>

#include "vertex.h"

class VertexTex : public Vertex
{
public:
    VertexTex();
    VertexTex(QVector3D pos, QVector2D tex);

    QVector2D getTexCoord();
    QVector3D getTangent();

    void setTangent(QVector3D tangent);

private:
    QVector2D m_tex;
    QVector3D m_tangent; // tangent for the TBN
};

#endif // VERTEXTEX_H
