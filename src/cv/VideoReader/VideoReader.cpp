
#include <opencv2/highgui/highgui.hpp>

#include "base/Utils/utils_str.h"
#include "base/Time/Timestamp.h"

#include "VideoReader.h"

namespace pi {


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int VideoFrame::call(const std::string& command, void* arg)
{
    return -1;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class VideoReaderData
{
public:
    typedef std::map<std::string, int> PropertyMap;
    enum VideoType {
        FILE,
        LIVE,
    };

public:
    VideoReaderData() {
        createPropertyMap();
    }
    virtual ~VideoReaderData() {}

    void createPropertyMap(void) {
        propertyMap["Timestamp"]    = CV_CAP_PROP_POS_MSEC;
        propertyMap["FrameIdx"]     = CV_CAP_PROP_POS_FRAMES;
        propertyMap["FrameCount"]   = CV_CAP_PROP_FRAME_COUNT;
        propertyMap["RelPos"]       = CV_CAP_PROP_POS_AVI_RATIO;

        propertyMap["ImgWidth"]     = CV_CAP_PROP_FRAME_WIDTH;
        propertyMap["ImgHeight"]    = CV_CAP_PROP_FRAME_HEIGHT;
        propertyMap["FPS"]          = CV_CAP_PROP_FPS;

        propertyMap["Brightness"]   = CV_CAP_PROP_BRIGHTNESS;
        propertyMap["Contrast"]     = CV_CAP_PROP_CONTRAST;
        propertyMap["Saturation"]   = CV_CAP_PROP_SATURATION;
        propertyMap["Hue"]          = CV_CAP_PROP_HUE;
        propertyMap["Gain"]         = CV_CAP_PROP_GAIN;
        propertyMap["Exposure"]     = CV_CAP_PROP_EXPOSURE;
    }

    int getPropertyID(const std::string& pName) {
        PropertyMap::iterator it = propertyMap.find(pName);

        if( it == propertyMap.end() ) return -1;
        else return it->second;
    }

public:
    cv::VideoCapture    vc;
    VideoType           videoType;
    PropertyMap         propertyMap;
};

#define getVideoData ((VideoReaderData*) m_data)

VideoReader::VideoReader()
{
    VideoReaderData *vrData = new VideoReaderData;
    m_data = vrData;
}

VideoReader::~VideoReader()
{
    VideoReaderData *vrData = getVideoData;
    delete vrData;
    m_data = NULL;
}

int VideoReader::open(const std::string& name)
{
    if( getVideoData->vc.isOpened() ) getVideoData->vc.release();

    if( is_number(name) ) {
        getVideoData->videoType = VideoReaderData::LIVE;
        if( getVideoData->vc.open(str_to_int(name)) ) return 0;
    } else {
        getVideoData->videoType = VideoReaderData::LIVE;
        if( getVideoData->vc.open(name) ) return 0;
    }

    return -1;
}

int VideoReader::close(void)
{
    if( getVideoData->vc.isOpened() ) getVideoData->vc.release();

    return 0;
}

int VideoReader::call(const std::string& command, void* arg)
{
    return -1;
}

int VideoReader::grabImage(VideoFrame& videoFrame)
{
    bool r = getVideoData->vc.read(videoFrame.img);

    if( getVideoData->videoType == VideoReaderData::FILE )
        videoFrame.timestamp = getVideoData->vc.get(CV_CAP_PROP_POS_MSEC)/1000.0;
    else
        videoFrame.timestamp = Timestamp::getTimestampF();

    if( r ) return 0;
    else return -1;
}

int VideoReader::getOpt(const std::string& optName, std::string& opt)
{
    double p;
    int pID = getVideoData->getPropertyID(optName);

    if( pID == 0 ) return -1;
    p = getVideoData->vc.get(pID);
    opt = dtos(p);

    return 0;
}

int VideoReader::setOpt(const std::string& optName, std::string opt)
{
    double p;
    int pID = getVideoData->getPropertyID(optName);

    if( pID == -1 ) return -1;

    p = str_to_double(opt);
    getVideoData->vc.set(pID, p);

    return 0;
}

} // end of namespace pi

