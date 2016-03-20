#include "GlowingCircle.h"

#include <QtGlobal>

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QTime>

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include <cmath>

#include "vertex.h"

#define NUM_VERTICES    36
#define NUM_INDICES     36

#define NUM_COLS        3
#define NUM_ROWS        10
#define NUM_TREES       NUM_COLS * NUM_ROWS
//#define NUM_VERTICES    4
//#define NUM_INDICES     6


MyWindow::~MyWindow()
{
    if (mVertices != 0)  delete[] mVertices;
    if (mIndices  != 0)  delete[] mIndices;
    if (mProgram != 0)   delete   mProgram;
}

MyWindow::MyWindow() : currentTimeMs(0), currentTimeS(0)
{
    mVertices = 0;
    mIndices  = 0;
    mProgram = 0;

    setSurfaceType(QWindow::OpenGLSurface);
    setFlags(Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    create();

    resize(800, 600);

    mContext = new QOpenGLContext(this);
    mContext->setFormat(format);
    mContext->create();

    mContext->makeCurrent( this );

    mFuncs = mContext->versionFunctions<QOpenGLFunctions_3_3_Core>();
    if ( !mFuncs )
    {
        qWarning( "Could not obtain OpenGL versions object" );
        exit( 1 );
    }
    if (mFuncs->initializeOpenGLFunctions() == GL_FALSE)
    {
        qWarning( "Could not initialize core open GL functions" );
        exit( 1 );
    }

    initializeOpenGLFunctions();

    QTimer *repaintTimer = new QTimer(this);
    connect(repaintTimer, &QTimer::timeout, this, &MyWindow::render);
    repaintTimer->start(1000/60);

    QTimer *elapsedTimer = new QTimer(this);
    connect(elapsedTimer, &QTimer::timeout, this, &MyWindow::modCurTime);
    elapsedTimer->start(1);       
}

void MyWindow::modCurTime()
{
    currentTimeMs++;
    currentTimeS=currentTimeMs/1000.0f;
}

void MyWindow::initialize()
{
    mFuncs->glGenVertexArrays(1, &mVAO);
    mFuncs->glBindVertexArray(mVAO);

    CreateVertexBuffer();
    initShaders();

    gMVPLocation      = mProgram->uniformLocation("gMVP");

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void MyWindow::CreateVertexBuffer()
{
    // C++11 required
    mVertices = new VertexTex[4] {
        VertexTex(QVector3D( -0.5f,  -0.5f, 0.0f),  QVector2D(-1.0f, -1.0f)),
        VertexTex(QVector3D(  0.5f,  -0.5f, 0.0f),  QVector2D( 1.0f, -1.0f)),
        VertexTex(QVector3D(  0.5f,   0.5f, 0.0f),  QVector2D( 1.0f,  1.0f)),
        VertexTex(QVector3D( -0.5f,   0.5f, 0.0f),  QVector2D(-1.0f,  1.0f))
    };

    mIndices = new unsigned int[6] {
         0, 1, 2,
         2, 3, 0
    };

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertices[0])*4, mVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndices[0])*6, mIndices, GL_STATIC_DRAW);
}

void MyWindow::resizeEvent(QResizeEvent *)
{
    mUpdateSize = true;
}

void MyWindow::render()
{
    if(!isVisible() || !isExposed())
        return;

    if (!mContext->makeCurrent(this))
        return;

    static bool initialized = false;
    if (!initialized) {
        initialize();
        initialized = true;
    }

    if (mUpdateSize) {
        glViewport(0, 0, size().width(), size().height());
        mUpdateSize = false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Tree
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    static float Scale = 0.0f;
    Scale += 0.1f; // tut 12

    QMatrix4x4 WVP, World;    

    //World.translate(0.0f, 0.0f, 1.0f);
    //World.translate(0.0f, 0.0f, Scale/10.0f);
    World.rotate(Scale*2, 1.0f, 0.0f, 0.0f);
    //mPointLight.setPosition(mPointLight.getPosition()-QVector3D(0.0f, 0.0f, Scale/1000.0f));

    //WVP.perspective(60.0f, (float)this->width()/(float)this->height(), 1.0f, 100.0f);
    //WVP.lookAt(cam.position(), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    QMatrix4x4 CameraMat(WVP);

    //PrintCoordOglDevOrig(QVector3D(0.0f,  0.0f, 0.0f), cam.position());
    //PrintCoordMoiRightHanded(QVector3D(0.0f,  0.0f, 0.0f), cam.position());

    //WVP *= World;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTex), (const GLvoid *)((sizeof(mVertices[0].getPos()))+(sizeof(mVertices[0].getNormal()))));

    mProgram->bind();
    {
        //glUniformMatrix4fv(gMVPLocation,  1, GL_FALSE, cam.matrix().constData());

        //glDrawArrays(GL_TRIANGLES, 0, 4);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
    mProgram->release();


    mContext->swapBuffers(this);
}

void MyWindow::initShaders()
{
    QOpenGLShader vShader(QOpenGLShader::Vertex);
    QOpenGLShader fShader(QOpenGLShader::Fragment);    
    QFile         shaderFile;
    QByteArray    shaderSource;

    //mTreeProgram
    // Shader 1
    shaderFile.setFileName(":/vshader.txt");
    shaderFile.open(QIODevice::ReadOnly);
    shaderSource = shaderFile.readAll();
    shaderFile.close();
    qDebug() << "vertex tree compile: " << vShader.compileSourceCode(shaderSource);

    shaderFile.setFileName(":/fshader.txt");
    shaderFile.open(QIODevice::ReadOnly);
    shaderSource = shaderFile.readAll();
    shaderFile.close();
    qDebug() << "frag   tree compile: " << fShader.compileSourceCode(shaderSource);

    mProgram = new (QOpenGLShaderProgram);
    mProgram->addShader(&vShader);
    mProgram->addShader(&fShader);
    qDebug() << "shader link tree: " << mProgram->link();
}

void MyWindow::PrepareTexture(GLenum TextureTarget, const QString& FileName, GLuint& TexObject, bool flip)
{
    QImage TexImg;

    if (!TexImg.load(FileName)) qDebug() << "Erreur chargement texture";
    if (flip==true) TexImg=TexImg.mirrored();

    glGenTextures(1, &TexObject);
    glBindTexture(TextureTarget, TexObject);
    glTexImage2D(TextureTarget, 0, GL_RGB, TexImg.width(), TexImg.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, TexImg.bits());
    glTexParameterf(TextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(TextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MyWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_P:
            break;
        case Qt::Key_Up:
            break;
        case Qt::Key_Down:
            break;
        case Qt::Key_Left:
            break;
        case Qt::Key_Right:
            break;
        case Qt::Key_Delete:
            break;
        case Qt::Key_PageDown:
            break;
        case Qt::Key_Home:
            break;
        case Qt::Key_Z:
            break;
        case Qt::Key_Q:
            break;
        case Qt::Key_S:
            break;
        case Qt::Key_D:
            break;
        case Qt::Key_A:
            break;
        case Qt::Key_E:
            break;
        default:
            break;
    }
}


void MyWindow::printMatrix(const QMatrix4x4& mat)
{
    const float *locMat = mat.transposed().constData();

    for (int i=0; i<4; i++)
    {
        qDebug() << locMat[i*4] << " " << locMat[i*4+1] << " " << locMat[i*4+2] << " " << locMat[i*4+3];
    }
}

void MyWindow::PrintCoordOglDevOrig(QVector3D pos, QVector3D cameraPos)
{
    QVector3D toCamera = QVector3D(cameraPos - pos).normalized();
    QVector3D up(0.0, 1.0, 0.0);
    QVector3D right = QVector3D::crossProduct(toCamera, up);
    QVector3D Pos(pos);

    qDebug() << "tocam: " << toCamera << " right: " << right;

    Pos -= (right * 0.5);
    qDebug() << "pos1: " << Pos;

    Pos.setY(Pos.y()+1.0);
    qDebug() << "pos2: " << Pos;

    Pos.setY(Pos.y()-1.0);
    Pos += right;
    qDebug() << "pos3: " << Pos;

    Pos.setY(Pos.y()+1.0);
    qDebug() << "pos4: " << Pos;
}

void MyWindow::PrintCoordMoiRightHanded(QVector3D pos, QVector3D cameraPos)
{
    QVector3D toCamera = QVector3D(cameraPos - pos).normalized();
    QVector3D up(0.0, 1.0, 0.0);
    QVector3D right = QVector3D::crossProduct(up, toCamera);
    QVector3D Pos(pos);

    qDebug() << "tocam: " << toCamera << " right: " << right;

    Pos += (right * 0.5);
    qDebug() << "pos1: " << Pos;

    Pos.setY(Pos.y()+1.0);
    qDebug() << "pos2: " << Pos;

    Pos -= right;
    Pos.setY(Pos.y()-1.0);
    qDebug() << "pos3: " << Pos;

    Pos.setY(Pos.y()+1.0);
    qDebug() << "pos4: " << Pos;
}
