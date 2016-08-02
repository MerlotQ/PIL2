
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <base/Svar/Svar.h>
#include <base/Debug/Assert.h>
#include <base/Utils/utils_str.h>
#include <cv/VideoReader/VideoReader.h>

using namespace std;
using namespace pi;

int Test_VideoReader(void)
{
    string videoFN = svar.GetString("videoFN", "0");
    double scale = svar.GetDouble("scale", 0.5);

    pi::VideoReader vr;
    pi::VideoFrame vf;

    pi_dbg_info("open video: %s", videoFN.c_str());

    if( 0 != vr.open(videoFN) ) {
        pi_dbg_error("Can not open video: %s", videoFN.c_str());
        return -1;
    }

    if( svar.exist("ImgWidth") ) {
        int imgW, imgH;

        imgW = svar.GetInt("ImgWidth", 1920);
        imgH = svar.GetInt("ImgHeight", 1080);

        vr.setOpt("ImgWidth", itos(imgW));
        vr.setOpt("ImgHeight", itos(imgH));
    }

    string opt;
    vr.getOpt("FrameCount", opt);   printf("FrameCount = %s\n", opt.c_str());
    vr.getOpt("ImgWidth", opt);     printf("ImgWidth = %s\n", opt.c_str());
    vr.getOpt("ImgHeight", opt);    printf("ImgHeight = %s\n", opt.c_str());
    vr.getOpt("FPS", opt);          printf("FPS = %s\n", opt.c_str());

    while( 1 ) {
        if( 0 != vr.grabImage(vf) ) break;

        cv::Mat imgS;
        cv::resize(vf.img, imgS, cv::Size(), scale, scale);
        cv::imshow("image", imgS);
        printf("timeStamp = %f\n", vf.timestamp);

        cv::waitKey(10);
    }

    return 0;
}


int main(int argc,char** argv)
{
    svar.ParseMain(argc, argv);
    svar.dumpAllVars();

    string act = trim(svar.GetString("Act", "Test_VideoReader"));
    printf("Act = %s\n", act.c_str());

    if( act == "Test_VideoReader" ) Test_VideoReader();

    return 0;
}

