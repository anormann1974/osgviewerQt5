#include "GraphicsWindowQt5.h"

#include <QInputEvent>
#include <QOpenGLContext>
#include <QGuiApplication>
#include <QScreen>
#include <QThread>

#include <QDebug>
#include <QWindow>

using namespace osgQt5;

class QtKeyboardMap5
{

public:
    QtKeyboardMap5()
    {
        mKeyMap[Qt::Key_Escape     ] = osgGA::GUIEventAdapter::KEY_Escape;
        mKeyMap[Qt::Key_Delete   ] = osgGA::GUIEventAdapter::KEY_Delete;
        mKeyMap[Qt::Key_Home       ] = osgGA::GUIEventAdapter::KEY_Home;
        mKeyMap[Qt::Key_Enter      ] = osgGA::GUIEventAdapter::KEY_KP_Enter;
        mKeyMap[Qt::Key_End        ] = osgGA::GUIEventAdapter::KEY_End;
        mKeyMap[Qt::Key_Return     ] = osgGA::GUIEventAdapter::KEY_Return;
        mKeyMap[Qt::Key_PageUp     ] = osgGA::GUIEventAdapter::KEY_Page_Up;
        mKeyMap[Qt::Key_PageDown   ] = osgGA::GUIEventAdapter::KEY_Page_Down;
        mKeyMap[Qt::Key_Left       ] = osgGA::GUIEventAdapter::KEY_Left;
        mKeyMap[Qt::Key_Right      ] = osgGA::GUIEventAdapter::KEY_Right;
        mKeyMap[Qt::Key_Up         ] = osgGA::GUIEventAdapter::KEY_Up;
        mKeyMap[Qt::Key_Down       ] = osgGA::GUIEventAdapter::KEY_Down;
        mKeyMap[Qt::Key_Backspace  ] = osgGA::GUIEventAdapter::KEY_BackSpace;
        mKeyMap[Qt::Key_Tab        ] = osgGA::GUIEventAdapter::KEY_Tab;
        mKeyMap[Qt::Key_Space      ] = osgGA::GUIEventAdapter::KEY_Space;
        mKeyMap[Qt::Key_Delete     ] = osgGA::GUIEventAdapter::KEY_Delete;
        mKeyMap[Qt::Key_Alt      ] = osgGA::GUIEventAdapter::KEY_Alt_L;
        mKeyMap[Qt::Key_Shift    ] = osgGA::GUIEventAdapter::KEY_Shift_L;
        mKeyMap[Qt::Key_Control  ] = osgGA::GUIEventAdapter::KEY_Control_L;
        mKeyMap[Qt::Key_Meta     ] = osgGA::GUIEventAdapter::KEY_Meta_L;

        mKeyMap[Qt::Key_F1             ] = osgGA::GUIEventAdapter::KEY_F1;
        mKeyMap[Qt::Key_F2             ] = osgGA::GUIEventAdapter::KEY_F2;
        mKeyMap[Qt::Key_F3             ] = osgGA::GUIEventAdapter::KEY_F3;
        mKeyMap[Qt::Key_F4             ] = osgGA::GUIEventAdapter::KEY_F4;
        mKeyMap[Qt::Key_F5             ] = osgGA::GUIEventAdapter::KEY_F5;
        mKeyMap[Qt::Key_F6             ] = osgGA::GUIEventAdapter::KEY_F6;
        mKeyMap[Qt::Key_F7             ] = osgGA::GUIEventAdapter::KEY_F7;
        mKeyMap[Qt::Key_F8             ] = osgGA::GUIEventAdapter::KEY_F8;
        mKeyMap[Qt::Key_F9             ] = osgGA::GUIEventAdapter::KEY_F9;
        mKeyMap[Qt::Key_F10            ] = osgGA::GUIEventAdapter::KEY_F10;
        mKeyMap[Qt::Key_F11            ] = osgGA::GUIEventAdapter::KEY_F11;
        mKeyMap[Qt::Key_F12            ] = osgGA::GUIEventAdapter::KEY_F12;
        mKeyMap[Qt::Key_F13            ] = osgGA::GUIEventAdapter::KEY_F13;
        mKeyMap[Qt::Key_F14            ] = osgGA::GUIEventAdapter::KEY_F14;
        mKeyMap[Qt::Key_F15            ] = osgGA::GUIEventAdapter::KEY_F15;
        mKeyMap[Qt::Key_F16            ] = osgGA::GUIEventAdapter::KEY_F16;
        mKeyMap[Qt::Key_F17            ] = osgGA::GUIEventAdapter::KEY_F17;
        mKeyMap[Qt::Key_F18            ] = osgGA::GUIEventAdapter::KEY_F18;
        mKeyMap[Qt::Key_F19            ] = osgGA::GUIEventAdapter::KEY_F19;
        mKeyMap[Qt::Key_F20            ] = osgGA::GUIEventAdapter::KEY_F20;

        mKeyMap[Qt::Key_hyphen         ] = '-';
        mKeyMap[Qt::Key_Equal         ] = '=';

        mKeyMap[Qt::Key_division      ] = osgGA::GUIEventAdapter::KEY_KP_Divide;
        mKeyMap[Qt::Key_multiply      ] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
        mKeyMap[Qt::Key_Minus         ] = '-';
        mKeyMap[Qt::Key_Plus          ] = '+';
        //mKeyMap[Qt::Key_H              ] = osgGA::GUIEventAdapter::KEY_KP_Home;
        //mKeyMap[Qt::Key_                    ] = osgGA::GUIEventAdapter::KEY_KP_Up;
        //mKeyMap[92                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Up;
        //mKeyMap[86                    ] = osgGA::GUIEventAdapter::KEY_KP_Left;
        //mKeyMap[87                    ] = osgGA::GUIEventAdapter::KEY_KP_Begin;
        //mKeyMap[88                    ] = osgGA::GUIEventAdapter::KEY_KP_Right;
        //mKeyMap[83                    ] = osgGA::GUIEventAdapter::KEY_KP_End;
        //mKeyMap[84                    ] = osgGA::GUIEventAdapter::KEY_KP_Down;
        //mKeyMap[85                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Down;
        mKeyMap[Qt::Key_Insert        ] = osgGA::GUIEventAdapter::KEY_KP_Insert;
        //mKeyMap[Qt::Key_Delete        ] = osgGA::GUIEventAdapter::KEY_KP_Delete;
    }

    virtual ~QtKeyboardMap5()
    {
    }

    int remapKey(QKeyEvent* event)
    {
        KeyMap::iterator itr = mKeyMap.find(event->key());

        if (itr == mKeyMap.end())
            return int(*(event->text().toLocal8Bit().data()));
        else
            return itr.value();
    }

private:
    typedef QHash<unsigned int, int> KeyMap;
    KeyMap mKeyMap;
};

static QtKeyboardMap5 s_QtKeyboardMap;



OSGQtWidget::OSGQtWidget(const QSurfaceFormat &format, QWidget *parent, bool forwardKeyEvents)
    : Inherited(parent)
    , _gw(nullptr)
    , _forwardKeyEvents(forwardKeyEvents)
    , _requestedFormat(format)
{
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(true);

    _devicePixelRatio = devicePixelRatio();
}

OSGQtWidget::~OSGQtWidget()
{
    osg::ref_ptr<GraphicsWindowQt> gw;

    if (_gw.lock(gw))
    {
        _gw->close();
        _gw = nullptr;
    }
}

void OSGQtWidget::setKeyboardModifiers(QInputEvent *event)
{
    int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
    unsigned int mask = 0;
    if ( modkey & Qt::ShiftModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    if ( modkey & Qt::ControlModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    if ( modkey & Qt::AltModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
    _gw->getEventQueue()->getCurrentEventState()->setModKeyMask( mask );
}

bool OSGQtWidget::underMouse() const
{
    QPoint globalMouse = QCursor::pos();
    return frameGeometry().contains(globalMouse);
}

void OSGQtWidget::resizeEvent( QResizeEvent* event )
{
    const QSize& size = event->size();

    int scaled_width = static_cast<int>(size.width()*_devicePixelRatio);
    int scaled_height = static_cast<int>(size.height()*_devicePixelRatio);
    _gw->resized( x(), y(), scaled_width,  scaled_height);
    _gw->getEventQueue()->windowResize( x(), y(), scaled_width, scaled_height );
    _gw->requestRedraw();
}

void OSGQtWidget::moveEvent( QMoveEvent* event )
{
    const QPoint& pos = event->pos();
    int scaled_width = static_cast<int>(width()*_devicePixelRatio);
    int scaled_height = static_cast<int>(height()*_devicePixelRatio);
    _gw->resized( pos.x(), pos.y(), scaled_width,  scaled_height );
    _gw->getEventQueue()->windowResize( pos.x(), pos.y(), scaled_width,  scaled_height );
}

void OSGQtWidget::keyPressEvent( QKeyEvent* event )
{
    setKeyboardModifiers( event );
    int value = s_QtKeyboardMap.remapKey( event );
    _gw->getEventQueue()->keyPress( value );

    // this passes the event to the regular Qt key event processing,
    // among others, it closes popup windows on ESC and forwards the event to the parent widgets
    if( _forwardKeyEvents )
        Inherited::keyPressEvent( event );
}

void OSGQtWidget::keyReleaseEvent( QKeyEvent* event )
{
    if( event->isAutoRepeat() )
    {
        event->ignore();
    }
    else
    {
        setKeyboardModifiers( event );
        int value = s_QtKeyboardMap.remapKey( event );
        _gw->getEventQueue()->keyRelease( value );
    }

    // this passes the event to the regular Qt key event processing,
    // among others, it closes popup windows on ESC and forwards the event to the parent widgets
    if( _forwardKeyEvents )
        Inherited::keyReleaseEvent( event );
}

void OSGQtWidget::mousePressEvent( QMouseEvent* event )
{
    int button = 0;
    switch ( event->button() )
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::MidButton: button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton: button = 0; break;
        default: button = 0; break;
    }
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseButtonPress( event->x()*_devicePixelRatio, event->y()*_devicePixelRatio, button );
}

void OSGQtWidget::mouseReleaseEvent( QMouseEvent* event )
{
    int button = 0;
    switch ( event->button() )
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::MidButton: button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton: button = 0; break;
        default: button = 0; break;
    }
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseButtonRelease( event->x()*_devicePixelRatio, event->y()*_devicePixelRatio, button );
}

void OSGQtWidget::mouseDoubleClickEvent( QMouseEvent* event )
{
    int button = 0;
    switch ( event->button() )
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::MidButton: button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton: button = 0; break;
        default: button = 0; break;
    }
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseDoubleButtonPress( event->x()*_devicePixelRatio, event->y()*_devicePixelRatio, button );
}

void OSGQtWidget::mouseMoveEvent( QMouseEvent* event )
{
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseMotion( event->x()*_devicePixelRatio, event->y()*_devicePixelRatio );
}

void OSGQtWidget::wheelEvent( QWheelEvent* event )
{
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseScroll(
                event->orientation() == Qt::Vertical ?
                    (event->delta()>0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
                    (event->delta()>0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT) );
}

QPaintEngine* OSGQtWidget::paintEngine() const
{
    return nullptr;
}




GraphicsWindowQt::GraphicsWindowQt(osg::GraphicsContext::Traits *traits, QWidget *parent, Qt::WindowFlags f)
    : osgViewer::GraphicsWindow()
    , _qtWindow(nullptr)
    , _qtContext(nullptr)
    , _realized(false)
{
    _traits = traits;
    init( parent, f );
}

GraphicsWindowQt::~GraphicsWindowQt()
{
    // remove reference from Widget
    if ( _qtWindow )
        _qtWindow->_gw = nullptr;
}

bool GraphicsWindowQt::init( QWidget* parent, Qt::WindowFlags f )
{
    // create window if it does not exist
    //_qtWindow = nullptr;
    if ( _qtWindow.isNull() )
    {
        // WindowFlags
        Qt::WindowFlags flags = f | Qt::Window | Qt::CustomizeWindowHint;
        if ( _traits->windowDecoration )
            flags |= Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowSystemMenuHint;

        // create QWindow
        _qtWindow.reset(new OSGQtWidget( traits2QSurfaceFormat( _traits.get() ), parent ));
        _qtWindow->setWindowFlags(flags);
    }

    _qtWindow->setWindowTitle( _traits->windowName.c_str() );
    _qtWindow->setGeometry( _traits->x, _traits->y, _traits->width, _traits->height );

    // initialize widget properties
    _qtWindow->setMouseTracking( true );
    _qtWindow->setFocusPolicy( Qt::WheelFocus );
    _qtWindow->setGraphicsWindow( this );
    useCursor( _traits->useCursor );

    // initialize State
    setState( new osg::State );
    getState()->setGraphicsContext(this);

    // initialize contextID
    if ( _traits.valid() && _traits->sharedContext.valid() )
    {
        getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
        incrementContextIDUsageCount( getState()->getContextID() );
    }
    else
    {
        getState()->setContextID( osg::GraphicsContext::createNewContextID() );
    }

    // make sure the event queue has the correct window rectangle size and input range
    getEventQueue()->syncWindowRectangleWithGraphicsContext();

    return true;
}

QSurfaceFormat GraphicsWindowQt::traits2QSurfaceFormat( const osg::GraphicsContext::Traits* traits )
{
    QSurfaceFormat format;

    format.setAlphaBufferSize( traits->alpha );
    format.setRedBufferSize( traits->red );
    format.setGreenBufferSize( traits->green );
    format.setBlueBufferSize( traits->blue );
    format.setDepthBufferSize( traits->depth );
    format.setStencilBufferSize( traits->stencil );
    //format.setSampleBuffers( traits->sampleBuffers );
    format.setSamples( traits->samples );

    format.setSwapBehavior( traits->doubleBuffer ? QSurfaceFormat::DoubleBuffer : QSurfaceFormat::SingleBuffer);
    format.setSwapInterval( traits->vsync ? 1 : 0 );
    format.setStereo( traits->quadBufferStereo ? 1 : 0 );

    return format;
}

bool GraphicsWindowQt::setWindowRectangleImplementation( int x, int y, int width, int height )
{
    if ( _qtWindow == nullptr )
        return false;

    _qtWindow->setGeometry( x, y, width, height );
    return true;
}

void GraphicsWindowQt::getWindowRectangle( int& x, int& y, int& width, int& height )
{
    if ( _qtWindow )
    {
        const QRect& geom = _qtWindow->geometry();
        x = geom.x();
        y = geom.y();
        width = geom.width();
        height = geom.height();
    }
}

bool GraphicsWindowQt::setWindowDecorationImplementation( bool windowDecoration )
{
    Qt::WindowFlags flags = Qt::Window|Qt::CustomizeWindowHint;//|Qt::WindowStaysOnTopHint;
    if ( windowDecoration )
        flags |= Qt::WindowTitleHint|Qt::WindowMinMaxButtonsHint|Qt::WindowSystemMenuHint;
    _traits->windowDecoration = windowDecoration;

    if ( _qtWindow )
    {
        _qtWindow->setWindowFlags( flags );
        return true;
    }

    return false;
}

bool GraphicsWindowQt::getWindowDecoration() const
{
    return _traits->windowDecoration;
}

void GraphicsWindowQt::grabFocus()
{
    if ( _qtWindow )
        _qtWindow->setFocus();
}

void GraphicsWindowQt::grabFocusIfPointerInWindow()
{
    if ( _qtWindow && _qtWindow->underMouse() )
        _qtWindow->setFocus();
}

void GraphicsWindowQt::raiseWindow()
{
    if ( _qtWindow )
        _qtWindow->raise();
}

void GraphicsWindowQt::setWindowName( const std::string& name )
{
    if ( _qtWindow )
        _qtWindow->setWindowTitle( QString::fromLocal8Bit(name.c_str()) );
}

std::string GraphicsWindowQt::getWindowName()
{
    return _qtWindow ? _qtWindow->windowTitle().toStdString() : "";
}

void GraphicsWindowQt::useCursor( bool cursorOn )
{
    if ( _qtWindow )
    {
        _traits->useCursor = cursorOn;
        if ( !cursorOn ) _qtWindow->setCursor( Qt::BlankCursor );
        else _qtWindow->setCursor( _currentCursor );
    }
}

void GraphicsWindowQt::setCursor( MouseCursor cursor )
{
    if ( cursor==InheritCursor && _qtWindow )
    {
        _qtWindow->unsetCursor();
    }

    switch ( cursor )
    {
        case NoCursor: _currentCursor = Qt::BlankCursor; break;
        case RightArrowCursor: case LeftArrowCursor: _currentCursor = Qt::ArrowCursor; break;
        case InfoCursor: _currentCursor = Qt::SizeAllCursor; break;
        case DestroyCursor: _currentCursor = Qt::ForbiddenCursor; break;
        case HelpCursor: _currentCursor = Qt::WhatsThisCursor; break;
        case CycleCursor: _currentCursor = Qt::ForbiddenCursor; break;
        case SprayCursor: _currentCursor = Qt::SizeAllCursor; break;
        case WaitCursor: _currentCursor = Qt::WaitCursor; break;
        case TextCursor: _currentCursor = Qt::IBeamCursor; break;
        case CrosshairCursor: _currentCursor = Qt::CrossCursor; break;
        case HandCursor: _currentCursor = Qt::OpenHandCursor; break;
        case UpDownCursor: _currentCursor = Qt::SizeVerCursor; break;
        case LeftRightCursor: _currentCursor = Qt::SizeHorCursor; break;
        case TopSideCursor: case BottomSideCursor: _currentCursor = Qt::UpArrowCursor; break;
        case LeftSideCursor: case RightSideCursor: _currentCursor = Qt::SizeHorCursor; break;
        case TopLeftCorner: _currentCursor = Qt::SizeBDiagCursor; break;
        case TopRightCorner: _currentCursor = Qt::SizeFDiagCursor; break;
        case BottomRightCorner: _currentCursor = Qt::SizeBDiagCursor; break;
        case BottomLeftCorner: _currentCursor = Qt::SizeFDiagCursor; break;
        default: break;
    };
    if ( _qtWindow ) _qtWindow->setCursor( _currentCursor );
}

bool GraphicsWindowQt::valid() const
{
    return _qtWindow; // && _qtWindow->isValid();
}

bool GraphicsWindowQt::realizeImplementation()
{
    if (_realized)
    {
        return false;
    }

    if (!_qtWindow)
    {
        qWarning() << "no window created";
        return false;
    }

    _realized = true;
    // make sure the event queue has the correct window rectangle size and input range
    getEventQueue()->syncWindowRectangleWithGraphicsContext();

    return true;
}

bool GraphicsWindowQt::isRealizedImplementation() const
{
    return _realized;
}

void GraphicsWindowQt::closeImplementation()
{
    if ( _qtWindow )
        _qtWindow->close();
    _realized = false;
}

void GraphicsWindowQt::runOperations()
{
    osgViewer::GraphicsWindow::runOperations();
}

bool GraphicsWindowQt::makeCurrentImplementation()
{
#if 0
    if (_widget->getNumDeferredEvents() > 0)
        _widget->processDeferredEvents();
#endif

    if (_qtContext.isNull())
    {
        _qtContext.reset(new QOpenGLContext);
        _sharedContext = _qtContext;
        _qtContext->setFormat(_qtWindow->_requestedFormat);
        _qtContext->create();
    }
    else if (QThread::currentThread() != _qtContext->thread())
    {
        _qtContext = QSharedPointer<QOpenGLContext>(new QOpenGLContext);
        _qtContext->setShareContext(_sharedContext.data());
        _qtContext->setFormat(_qtWindow->_requestedFormat);
        _qtContext->create();
    }

    bool ok = _qtContext->makeCurrent(_qtWindow->windowHandle());
    if (!ok)
    {
        qWarning() << "Failed to make context current:" << _qtContext.data() << _qtWindow.data();
    }

    return ok;
}

bool GraphicsWindowQt::releaseContextImplementation()
{
    _qtContext->doneCurrent();
    _qtContext->moveToThread(_qtWindow->thread());
    return true;
}

void GraphicsWindowQt::swapBuffersImplementation()
{
    // QOpenGLContext complains if we swap on an non-exposed QWindow
    if (!_qtWindow || !_qtWindow->windowHandle()->isExposed()) {
        return;
    }

    // OSG calls this on the draw thread, hopefully that's okay for Qt
    _qtContext->makeCurrent(_qtWindow->windowHandle());
    _qtContext->swapBuffers(_qtWindow->windowHandle());
}

void GraphicsWindowQt::requestWarpPointer( float x, float y )
{
    if ( _qtWindow )
    {
        QPoint pt = QPointF(x,y).toPoint();
        QCursor::setPos( _qtWindow->mapToGlobal(pt));
    }
}
