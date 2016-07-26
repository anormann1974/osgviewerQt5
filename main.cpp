#include "GraphicsWindowQt5.h"

#include <QApplication>
#include <QPaintEvent>
#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>

// OpenSceneGraph
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>


static osg::Node* createScene()
{
    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<osg::MatrixTransform> xform1 = new osg::MatrixTransform;
    osg::ref_ptr<osg::MatrixTransform> xform2 = new osg::MatrixTransform(osg::Matrix::translate(0,0,-10.0));
    osg::ref_ptr<osg::MatrixTransform> xform3 = new osg::MatrixTransform(osg::Matrix::translate(0,0,10.0));
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Shape> sphere = new osg::Sphere(osg::Vec3(0,0,0), 2.5f);
    osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(-10.0f, 0, 0), 2.0f, 5.0f);
    osg::ref_ptr<osg::Cone> cone = new osg::Cone(osg::Vec3(10.0f, 0, 0), 2.0f, 5.0f);

    geode->addDrawable(new osg::ShapeDrawable(sphere));
    geode->addDrawable(new osg::ShapeDrawable(cylinder));
    geode->addDrawable(new osg::ShapeDrawable(cone));

    xform1->addChild(geode);
    xform2->addChild(geode);
    xform3->addChild(geode);
    root->addChild(xform1);
    root->addChild(xform2);
    root->addChild(xform3);
    return root.release();
}

class ViewerWidget
        : public QWidget
        , public osgViewer::Viewer
{
public:

    ViewerWidget(QWidget *parent = Q_NULLPTR)
        : QWidget(parent)
        , osgViewer::Viewer()
    {
        setKeyEventSetsDone(0);
        setQuitEventSetsDone(false);
    }

    virtual void paintEvent(QPaintEvent *) override
    {
        frame();
    }
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    ViewerWidget viewerWidget;
    viewerWidget.setGeometry(100, 100, 1280, 720);
    viewerWidget.addEventHandler(new osgViewer::StatsHandler);
    viewerWidget.addEventHandler(new osgViewer::ThreadingHandler);
    viewerWidget.setCameraManipulator(new osgGA::TrackballManipulator);
    viewerWidget.setSceneData(createScene());

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->width = viewerWidget.width();
    traits->height = viewerWidget.height();
    traits->doubleBuffer = true;
    traits->samples = 8;

    osg::ref_ptr<osgQt5::GraphicsWindowQt> gw = new osgQt5::GraphicsWindowQt(traits);
    osg::ref_ptr<osg::Camera> camera = viewerWidget.getCamera();
    camera->setGraphicsContext(gw);
    camera->setViewport(0, 0, 1280, 720);
    camera->setClearColor(osg::Vec4(0.3, 0.3, 0.5, 1.0));

    QVBoxLayout layout(&viewerWidget);
    layout.setSpacing(0);
    layout.setContentsMargins(1,1,1,1);
    layout.addWidget(gw->getQtWidget());

    viewerWidget.setLayout(&layout);
    viewerWidget.show();

    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &viewerWidget, SLOT(update()));
    timer.start(8);

    return a.exec();
}
