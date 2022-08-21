/*********************************************************************************
 *Copyright(C): Juntuan.Lu, 2020-2030, All rights reserved.
 *Author:  Juntuan.Lu
 *Version: 1.0
 *Date:  2021/10/23
 *Email: 931852884@qq.com
 *Description:
 *Others:
 *Function List:
 *History:
 **********************************************************************************/

#ifndef GOSGRENDERITEM_H
#define GOSGRENDERITEM_H

#include <QElapsedTimer>
#include <QtQuick/QQuickFramebufferObject>
#include <osgViewer/Viewer>

class GOsgControl;

class GOsgRenderItem : public QQuickFramebufferObject {
    Q_OBJECT
    Q_DISABLE_COPY(GOsgRenderItem)
    Q_PROPERTY(int targetFpsRate READ targetFpsRate WRITE setTargetFpsRate NOTIFY targetFpsRateChanged)
    Q_PROPERTY(int currentFpsRate READ currentFpsRate NOTIFY currentFpsRateChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(GOsgControl* osgControl READ osgControl WRITE setOsgControl NOTIFY osgControlChanged)
public:
    explicit GOsgRenderItem(QQuickItem* parent = 0);
    ~GOsgRenderItem();

public:
    inline int targetFpsRate() const { return m_targetFpsRate; }
    inline int currentFpsRate() const { return m_currentFpsRate; }
    inline QColor backgroundColor() { return m_backgroundColor; }
    inline GOsgControl* osgControl() { return m_osgControl; }
    void setTargetFpsRate(int targetFpsRate);
    void setBackgroundColor(const QColor& color);
    void setOsgControl(GOsgControl* osgControl);
    //
    osgViewer::Viewer* getViewer() const { return m_viewer.get(); }
    void doFrame();

protected:
    Renderer* createRenderer() const override;
    virtual void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void touchEvent(QTouchEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void timerEvent(QTimerEvent* event) override;

private:
    void init();
    void initOsg();
    void initWindow();
    void setKeyboardModifiers(QInputEvent* event);
    void updateOsgSize(const QSizeF& size);
    void computerFpsRate(bool enable);
    void configFrameTimer();

private:
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_gw;
    QElapsedTimer m_fpsTime;
    GOsgControl* m_osgControl = nullptr;
    QColor m_backgroundColor = Qt::black;
    int m_targetFpsRate = 0;
    int m_currentFpsRate = 0;
    int m_fpsCount = 0;
    int m_frameTimerId = -1;

signals:
    void targetFpsRateChanged();
    void currentFpsRateChanged();
    void backgroundColorChanged();
    void osgControlChanged();
};
#endif // GOSGRENDERITEM_H
