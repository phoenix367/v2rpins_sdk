/* 
 * File:   PlatformModel.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 19, 2014, 4:10 PM
 */

#ifndef PLATFORMMODEL_HPP
#define	PLATFORMMODEL_HPP

#include <QGLWidget>

class PlatformModel : public QGLWidget 
{
    Q_OBJECT
    
public:
    PlatformModel(QWidget *parent = 0);
    virtual ~PlatformModel();
    
    void rotateModel(GLfloat angleX, GLfloat angleY, GLfloat angleZ);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    void draw();
    int faceAtPosition(const QPoint &pos);
    
private:
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    QColor faceColors[6];
    QPoint lastPos;
};

#endif	/* PLATFORMMODEL_HPP */
