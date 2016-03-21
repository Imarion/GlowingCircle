#include <QWindow>
#include <QTimer>
#include <QString>
#include <QKeyEvent>

#include <QVector3D>
#include <QMatrix4x4>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include <QOpenGLShaderProgram>

#include "vertextex.h"

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

//class MyWindow : public QWindow, protected QOpenGLFunctions_3_3_Core
class MyWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MyWindow();
    ~MyWindow();
    virtual void keyPressEvent( QKeyEvent *keyEvent );    

private slots:
    void render();

private:    
    void initialize();
    void modCurTime();

    void initShaders();
    void CreateVertexBuffer();
    void initBlobSettings();

    void PrepareTexture(GLenum TextureTarget, const QString& FileName, GLuint& TexObject, bool flip);

    void PrintCoordOglDevOrig(QVector3D pos, QVector3D camera);
    void PrintCoordMoiRightHanded(QVector3D pos, QVector3D camera);

protected:
    void resizeEvent(QResizeEvent *);

private:
    QOpenGLContext *mContext;
    QOpenGLFunctions_3_3_Core *mFuncs;

    QOpenGLShaderProgram *mProgram;

    QTimer mRepaintTimer;
    double currentTimeMs;
    double currentTimeS;
    bool   mUpdateSize;

    GLuint mVAO, mVBO, mIBO, mUBO;

    VertexTex     *mVertices;
    unsigned int  *mIndices;

    //debug
    void printMatrix(const QMatrix4x4& mat);
};
