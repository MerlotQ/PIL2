 

#include <string>
#include <vector>

#include "opencv2/core/core.hpp"

#include "base/Types/SPtr.h"


#ifndef __VIDEOREADER_H__
#define __VIDEORWADER_H__


namespace pi {


class VideoFrame
{
public:
    VideoFrame() {}

    virtual std::string type() { return "DefaultVideoFrame"; }
    virtual int call(const std::string& command, void* arg = NULL);

public:
    cv::Mat     img;                        ///< image
    double      timestamp;                  ///< timestamp (seconds sinece 1970/1/1, UTC time)
};

class VideoReader
{
public:
    VideoReader();
    virtual ~VideoReader();


    /*
        Video device:
        OpenCV: 1
        OpenCV: /home/test.mp4
        GigE: 192.168.0.10
    */
    virtual int open(const std::string& name);
    virtual int close(void);

    virtual std::string type() { return "DefaultVideoReader"; }
    virtual int call(const std::string& command, void* arg = NULL);

    virtual int grabImage(VideoFrame& videoFrame);


    /*
        optNames:
            ImgWidth        - image width
            ImgHeight       - image height
            FPS             - frame per second
            ColorMode       - color mode
                                MONO8, RGB24, BGR24, YUV444, ....

            Exposure        - Exposure
                .shuttur    - shuttur speed (us)
            Gain            - Gain (db)
    */
    virtual int getOpt(const std::string& optName, std::string& opt);
    virtual int setOpt(const std::string& optName, std::string opt);

protected:
    void*       m_data;
};

} // end of namespace pi

#endif

