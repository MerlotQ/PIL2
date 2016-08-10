#include <base/Utils/TestCase.h>
#include <cv/Camera/Camera.h>
#include <base/Svar/Svar.h>
#include <base/Types/Random.h>
#include <base/Time/Global_Timer.h>

using namespace pi;
using namespace std;

class CameraTest : public TestCase
{
public:
    CameraTest():TestCase("CameraTest"){}


    bool testCamera(pi::Camera camera)
    {
        pi_assert2(camera.isValid(),"Error while test camera"+camera.info());

        int times=svar.GetInt("Times",10000);
        cout<<"Testing camera "<<camera.info()<<",Times="<<times<<endl;

        vector<pi::Point2d> vec_re;
        vector<pi::Point2d> vec_p;
        vector<pi::Point3d> vec_cam;
        vec_cam.resize(times);
        vec_re.resize(times);
        vec_p.resize(times);
        for(int i=0;i<times;i++)
        {
            double rdx=pi::Random::RandomValue<double>()*camera.width();
            double rdy=pi::Random::RandomValue<double>()*camera.height();
            vec_p[i]=(pi::Point2d(rdx,rdy));
        }

//        pi::Point2d testP=vec_p[0];
//        pi::Point3d cam_P=camera.UnProject(testP);
//        pi::Point2d rep_P=camera.Project(cam_P);
//        cout<<"Image:"<<testP<<",Plane:"<<cam_P<<",ReProject:"<<rep_P<<endl;

        //UnProject
        string time_name=string(camera.CameraType()+"::H::UnProject");
        timer.enter(time_name.c_str());
        for(int i=0;i<times;i++)
            vec_cam[i]=camera.UnProject(vec_p[i]);
        timer.leave(time_name.c_str());

        //Project
        time_name=string(camera.CameraType()+"::H::Project");
        timer.enter(time_name.c_str());
        for(int i=0;i<times;i++)
            vec_re[i]=camera.Project(vec_cam[i]);
        timer.leave(time_name.c_str());

        double sum=0;
        for(int i=0;i<times;i++)
            sum+=(vec_p[i]-vec_re[i]).norm();
        cout<<"average error:"<<sum/times<<endl;

        pi_assert(sum<times);
        return true;
    }

    virtual void run()
    {
        svar.ParseLine("PinHoleCamera.CameraType=PinHole");
        svar.ParseLine("PinHoleCamera.Paraments=[1920 1080 1110 1110 960 540]");
        testCamera(pi::Camera("PinHoleCamera"));

        svar.ParseLine("ATANCamera.CameraType=ATAN");
        svar.ParseLine("ATANCamera.Paraments=[1920 1080 0.418092 0.759142 0.508446 0.493286 -0.838056]");
        testCamera(pi::Camera("ATANCamera"));

        svar.ParseLine("OpenCVCamera.CameraType= OpenCV");
        svar.ParseLine("OpenCVCamera.Paraments=[1920,1080,1123.37,1122.63,983.199,528.513,-0.274764,0.123371,0.000527734,-0.000986113,-0.0362192]");
        testCamera(pi::Camera("OpenCVCamera"));

//        svar.ParseLine("OCAMCamera.CameraType= OCAM");
//        svar.ParseLine("OCAMCamera.Paraments=[1920,1080,1123.37,1122.63,983.199,528.513,-0.274764,0.123371,0.000527734,-0.000986113,-0.0362192]");
//        testCamera(pi::Camera("OCAMCamera"));
    }
};
CameraTest cameraTest;
