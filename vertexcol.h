#ifndef VERTEXCOL_H
#define VERTEXCOL_H

#include <QVector3D>

#include "vertex.h"

class VertexCol : public Vertex
{
public:
    VertexCol();
    VertexCol(QVector3D pos, QVector3D color);

    QVector3D getColor();

private:
    QVector3D m_color;

};

#endif // VERTEXCOL_H
