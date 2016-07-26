#pragma once

#include <QSurfaceFormat>
#include <QWidget>

#include <osgViewer/GraphicsWindow>

class QInputEvent;
class QOpenGLContext;

namespace osgViewer {
    class ViewerBase;
}

namespace osgQt5
{

// forward declarations
class GraphicsWindowQt;

class OSGQtWidget : public QWidget
{
    typedef QWidget Inherited;
public:

    OSGQtWidget( const QSurfaceFormat& format, QWidget* parent = NULL, bool forwardKeyEvents = false );
    virtual ~OSGQtWidget();

    inline void setGraphicsWindow( GraphicsWindowQt* gw ) { _gw = gw; }
    inline GraphicsWindowQt* getGraphicsWindow() { return _gw.get(); }
    inline const GraphicsWindowQt* getGraphicsWindow() const { return _gw.get(); }

    inline bool getForwardKeyEvents() const { return _forwardKeyEvents; }
    virtual void setForwardKeyEvents( bool f ) { _forwardKeyEvents = f; }

    void setKeyboardModifiers( QInputEvent* event );

    bool underMouse() const;


protected:

    virtual void resizeEvent( QResizeEvent* event );
    virtual void moveEvent( QMoveEvent* event );
    virtual void keyPressEvent( QKeyEvent* event );
    virtual void keyReleaseEvent( QKeyEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseDoubleClickEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void wheelEvent( QWheelEvent* event );
    virtual QPaintEngine* paintEngine() const;

    friend class GraphicsWindowQt;
    osg::observer_ptr<GraphicsWindowQt> _gw;

    bool _forwardKeyEvents;
    qreal _devicePixelRatio;
    QSurfaceFormat _requestedFormat;
};

class GraphicsWindowQt : public osgViewer::GraphicsWindow
{
public:
    GraphicsWindowQt( osg::GraphicsContext::Traits* traits, QWidget* parent = NULL, Qt::WindowFlags f = 0 );
    virtual ~GraphicsWindowQt();

    OSGQtWidget* getQtWidget() { return _qtWindow.data(); }
    const OSGQtWidget* getQtWidget() const { return _qtWindow.data(); }

    QOpenGLContext* getQtContext() const { return _qtContext.data(); }

    bool init( QWidget* parent, Qt::WindowFlags f );

    static QSurfaceFormat traits2QSurfaceFormat( const osg::GraphicsContext::Traits* traits );

    virtual bool setWindowRectangleImplementation( int x, int y, int width, int height );
    virtual void getWindowRectangle( int& x, int& y, int& width, int& height );
    virtual bool setWindowDecorationImplementation( bool windowDecoration );
    virtual bool getWindowDecoration() const;
    virtual void grabFocus();
    virtual void grabFocusIfPointerInWindow();
    virtual void raiseWindow();
    virtual void setWindowName( const std::string& name );
    virtual std::string getWindowName();
    virtual void useCursor( bool cursorOn );
    virtual void setCursor( MouseCursor cursor );

    virtual bool valid() const;
    virtual bool realizeImplementation();
    virtual bool isRealizedImplementation() const;
    virtual void closeImplementation();
    virtual bool makeCurrentImplementation();
    virtual bool releaseContextImplementation();
    virtual void swapBuffersImplementation();
    virtual void runOperations();
    virtual void requestWarpPointer( float x, float y );


protected:

    friend class OSGQtWidget;

    QScopedPointer<OSGQtWidget> _qtWindow;
    QSharedPointer<QOpenGLContext> _qtContext;
    QSharedPointer<QOpenGLContext> _sharedContext;

    QCursor _currentCursor;
    bool _realized;
};

} // of namespace osgQt5
