#ifndef CAMERAS_H
#define CAMERAS_H

#include <base/Types/Point.h>
#include <base/Types/SPtr.h>
#include <base/Types/VecParament.h>

namespace pi {

class CameraImpl;

class Camera
{
public:
    Camera(const std::string& name="");
    Camera(SPtr<CameraImpl>& Impl);

    ///
    /// \brief create a camera from given config file or configure item
    /// \param name - configure file or configure item
    /// \return
    ///
    static Camera createFromName(const std::string& name);

    ///
    /// \brief estimate the ideal camera from a given camera
    /// \param cam - camera object
    /// \return
    ///
    Camera estimateIdealCamera(void);

    std::string CameraType();

    std::string info();

    bool applyScale(double scale=0.5);

    bool isValid();

    Point2d Project(const Point3d& p3d);
    Point2d Project(const double x, const double y, const double z) { return Project(Point3d(x, y, z)); }

    Point3d UnProject(const Point2d& p2d);
    Point3d UnProject(const double x, const double y) { return UnProject(Point2d(x, y)); }

    int width();
    int height();

    VecParament<double> getParameters();
    bool getParameters(double& fx,double& fy,double& cx,double& cy);

private:
    SPtr<CameraImpl> impl;
};

}

#endif // CAMERAS_H
