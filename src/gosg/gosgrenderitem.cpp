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

#include "gosgrenderitem.h"
#include "gosgcontrol.h"
#include "gosgkeymap.h"
#include <QKeyEvent>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>
#include <QQuickWindow>
#include <QTimer>

#define RENDER_SAMPLES 4

class GOsgRenderItemPrivate : public QQuickFramebufferObject::Renderer {
public:
    explicit GOsgRenderItemPrivate(const GOsgRenderItem* osgRender)
    {
        m_osgRender = const_cast<GOsgRenderItem*>(osgRender);
    }

private:
    void render()
    {
        if (m_osgRender) {
            m_osgRender->doFrame();
            if (m_osgRender->targetFpsRate() <= 0) {
                this->update();
            }
        }
    }
    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size)
    {
        QOpenGLFramebufferObjectFormat format;
        format.setSamples(RENDER_SAMPLES);
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        return new QOpenGLFramebufferObject(size, format);
    }
    void synchronize(QQuickFramebufferObject* item)
    {
        Q_UNUSED(item);
    }

private:
    GOsgRenderItem* m_osgRender;
};

GOsgRenderItem::GOsgRenderItem(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
    , m_viewer(new osgViewer::Viewer())
    , m_osgControl(nullptr)
{
    init();
    initOsg();
    connect(this, &GOsgRenderItem::windowChanged, this, &GOsgRenderItem::initWindow, Qt::DirectConnection);
}

GOsgRenderItem::~GOsgRenderItem()
{
    m_viewer->setDone(true);
    if (m_osgControl) {
        m_osgControl->requestDestroy();
    }
}

void GOsgRenderItem::setTargetFpsRate(int targetFpsRate)
{
    if (m_targetFpsRate != targetFpsRate) {
        m_targetFpsRate = targetFpsRate;
        configFrameTimer();
        emit targetFpsRateChanged();
    }
}

void GOsgRenderItem::setBackgroundColor(const QColor& color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        m_viewer->getCamera()->setClearColor(osg::Vec4(color.red(), color.green(), color.blue(), color.alpha()));
        emit backgroundColorChanged();
    }
}

void GOsgRenderItem::setOsgControl(GOsgControl* osgControl)
{
    if (m_osgControl != osgControl) {
        m_osgControl = osgControl;
        osgControl->init(m_viewer);
        emit osgControlChanged();
    }
}

QQuickFramebufferObject::Renderer* GOsgRenderItem::createRenderer() const
{
    return new GOsgRenderItemPrivate(this);
}

void GOsgRenderItem::doFrame()
{
    if (m_osgControl && m_osgControl->checkFrameAllowed()) {
        QOpenGLContext::currentContext()->functions()->glUseProgram(0);
        m_viewer->frame();
        computerFpsRate(true);
    } else {
        computerFpsRate(false);
    }
}

void GOsgRenderItem::init()
{
    this->setAcceptedMouseButtons(Qt::AllButtons);
    this->setFlag(ItemAcceptsInputMethod, true);
    this->setAcceptHoverEvents(true);
    this->setFocus(true);
    this->setMirrorVertically(true);
    m_fpsTime.restart();
}

void GOsgRenderItem::initOsg()
{
    osg::DisplaySettings* displaySettings = osg::DisplaySettings::instance();
    displaySettings->setGLContextVersion("2.0");
    if (displaySettings->getVertexBufferHint() == osg::DisplaySettings::NO_PREFERENCE) {
        displaySettings->setVertexBufferHint(osg::DisplaySettings::VERTEX_BUFFER_OBJECT);
    }
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits(displaySettings);
    traits->x = 0;
    traits->y = 0;
    traits->width = this->width();
    traits->height = this->height();
    traits->samples = RENDER_SAMPLES;
    traits->doubleBuffer = true;
    traits->windowDecoration = false;
    traits->useCursor = false;
    m_gw = new osgViewer::GraphicsWindowEmbedded(traits);
    m_viewer->getCamera()->setGraphicsContext(m_gw);
    m_viewer->getCamera()->setViewport(0, 0, this->width(), this->height());
    m_viewer->getCamera()->setClearColor(osg::Vec4(m_backgroundColor.red(), m_backgroundColor.green(), m_backgroundColor.blue(), m_backgroundColor.alpha()));
    m_viewer->getCamera()->setProjectionMatrixAsPerspective(30.0, (double)this->width() / (double)this->height(), 1.0f, 10000.0f);
    m_viewer->setRunFrameScheme(osgViewer::Viewer::ON_DEMAND);
    m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    m_viewer->setKeyEventSetsDone(0);
}

void GOsgRenderItem::initWindow()
{
    if (this->window()) {
        //        QSurfaceFormat format = this->window()->requestedFormat();
        //        format.setVersion(3, 2);
        //        format.setSamples(RENDER_SAMPLES);
        //        format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        //        this->window()->setFormat(format);
        configFrameTimer();
    }
}

void GOsgRenderItem::setKeyboardModifiers(QInputEvent* event)
{
    int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
    unsigned int mask = 0;
    if (modkey & Qt::ShiftModifier)
        mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    if (modkey & Qt::ControlModifier)
        mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    if (modkey & Qt::AltModifier)
        mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
    m_gw->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}

void GOsgRenderItem::updateOsgSize(const QSizeF& size)
{
    m_gw->getEventQueue()->windowResize(0, 0, size.width(), size.height());
    m_gw->resized(0, 0, size.width(), size.height());
    m_viewer->getCamera()->resize(size.width(), size.height());
    m_viewer->getCamera()->setProjectionMatrixAsPerspective(45, (double)size.width() / (double)size.height(), 1.0f, 10000.0f);
}

void GOsgRenderItem::computerFpsRate(bool enable)
{
    if (enable) {
        m_fpsCount++;
    }
    if (m_fpsTime.elapsed() >= 1000) {
        m_fpsTime.restart();
        if (m_currentFpsRate != m_fpsCount) {
            m_currentFpsRate = m_fpsCount;
            emit currentFpsRateChanged();
        }
        m_fpsCount = 0;
    }
}

void GOsgRenderItem::configFrameTimer()
{
    if (m_frameTimerId >= 0) {
        this->killTimer(m_frameTimerId);
        m_frameTimerId = -1;
    }
    if (window() && m_targetFpsRate > 0) {
        m_frameTimerId = this->startTimer(qRound(1000.0 / m_targetFpsRate), Qt::CoarseTimer);
    }
}

void GOsgRenderItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    updateOsgSize(newGeometry.size());
}

void GOsgRenderItem::mousePressEvent(QMouseEvent* event)
{
    this->setFocus(true);
    QQuickItem::mousePressEvent(event);
    event->accept();
    int button = 0;
    switch (event->button()) {
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MidButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    case Qt::NoButton:
        button = 0;
        break;
    default:
        button = 0;
        break;
    }
    setKeyboardModifiers(event);
    m_gw->getEventQueue()->mouseButtonPress(event->localPos().x(), event->localPos().y(), button);
}

void GOsgRenderItem::mouseReleaseEvent(QMouseEvent* event)
{
    QQuickItem::mouseReleaseEvent(event);
    event->accept();
    int button = 0;
    switch (event->button()) {
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MidButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    case Qt::NoButton:
        button = 0;
        break;
    default:
        button = 0;
        break;
    }
    setKeyboardModifiers(event);
    m_gw->getEventQueue()->mouseButtonRelease(event->localPos().x(), event->localPos().y(), button);
}

void GOsgRenderItem::mouseMoveEvent(QMouseEvent* event)
{
    QQuickItem::mouseMoveEvent(event);
    event->accept();
    setKeyboardModifiers(event);
    m_gw->getEventQueue()->mouseMotion(event->localPos().x(), event->localPos().y());
}

void GOsgRenderItem::mouseDoubleClickEvent(QMouseEvent* event)
{
    QQuickItem::mouseDoubleClickEvent(event);
    event->accept();
    int button = 0;
    switch (event->button()) {
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MidButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    case Qt::NoButton:
        button = 0;
        break;
    default:
        button = 0;
        break;
    }
    setKeyboardModifiers(event);
    m_gw->getEventQueue()->mouseDoubleButtonPress(event->localPos().x(), event->localPos().y(), button);
}

void GOsgRenderItem::touchEvent(QTouchEvent* event)
{
    QQuickItem::touchEvent(event);
    event->accept();
    const QList<QTouchEvent::TouchPoint>& touchPoints = static_cast<QTouchEvent*>(event)->touchPoints();
    if (touchPoints.count() >= 2) {
        osg::ref_ptr<osgGA::GUIEventAdapter> osg_event(NULL);
        for (const QTouchEvent::TouchPoint& touchPoint : touchPoints) {
            const QPointF& touchPos = touchPoint.pos();
            if (touchPoint.state() == Qt::TouchPointPressed) {
                if (!osg_event) {
                    osg_event = m_gw->getEventQueue()->touchBegan(touchPoint.id(), osgGA::GUIEventAdapter::TOUCH_BEGAN, touchPos.x(), touchPos.y());
                } else {
                    osg_event->addTouchPoint(touchPoint.id(), osgGA::GUIEventAdapter::TOUCH_BEGAN, touchPos.x(), touchPos.y());
                }
            } else if (touchPoint.state() == Qt::TouchPointMoved) {
                if (!osg_event) {
                    osg_event = m_gw->getEventQueue()->touchMoved(touchPoint.id(), osgGA::GUIEventAdapter::TOUCH_MOVED, touchPos.x(), touchPos.y());
                } else {
                    osg_event->addTouchPoint(touchPoint.id(), osgGA::GUIEventAdapter::TOUCH_MOVED, touchPos.x(), touchPos.y());
                }
            } else if (touchPoint.state() == Qt::TouchPointReleased) {
                if (!osg_event) {
                    osg_event = m_gw->getEventQueue()->touchEnded(touchPoint.id(), osgGA::GUIEventAdapter::TOUCH_ENDED, touchPos.x(), touchPos.y(), 1);
                } else {
                    osg_event->addTouchPoint(touchPoint.id(), osgGA::GUIEventAdapter::TOUCH_ENDED, touchPos.x(), touchPos.y());
                }
            }
        }
    }
}

void GOsgRenderItem::wheelEvent(QWheelEvent* event)
{
    QQuickItem::wheelEvent(event);
    event->accept();
    setKeyboardModifiers(event);
    if (event->angleDelta().x() < 0) {
        m_gw->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_LEFT);
    } else if (event->angleDelta().x() > 0) {
        m_gw->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_RIGHT);
    } else if (event->angleDelta().y() < 0) {
        m_gw->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
    } else if (event->angleDelta().y() > 0) {
        m_gw->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);
    }
}

void GOsgRenderItem::keyPressEvent(QKeyEvent* event)
{
    QQuickItem::keyPressEvent(event);
    event->accept();
    setKeyboardModifiers(event);
    m_gw->getEventQueue()->keyPress(GOsgKeyMap::transKey(event->key(), event->text().toLocal8Bit()));
}

void GOsgRenderItem::keyReleaseEvent(QKeyEvent* event)
{
    QQuickItem::keyReleaseEvent(event);
    event->accept();
    if (event->isAutoRepeat()) {
        event->ignore();
    } else {
        setKeyboardModifiers(event);
        m_gw->getEventQueue()->keyRelease(GOsgKeyMap::transKey(event->key(), event->text().toLocal8Bit()));
    }
}

void GOsgRenderItem::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_frameTimerId) {
        if (m_targetFpsRate > 0) {
            this->update();
        }
    }
}
