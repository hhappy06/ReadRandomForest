
#ifndef _NUISINECT_H
#define  _NUISINECT_H



#define _IN_
#define _OUT_
#define _INOUT_
#ifndef _In_
#define _In_
#endif // !_In_

#ifndef _Out_
#define _Out_
#endif 


#ifndef FLT_EPSILON
  #define FLT_EPSILON     1.192092896e-07F        /* smallest such that 1.0+FLT_EPSILON != 1.0 */
#endif

#ifndef _Vector4_
#define _Vector4_
typedef struct _Vector4
    {
        _Vector4(float ax = 0, float ay = 0, float az = 0)
            : x(ax), y(ay), z(az), w(0.0f)
        {
        }
        _Vector4 operator + (const _Vector4 & src) const
        {
            _Vector4 dst;
            dst.x = this->x + src.x;
            dst.y = this->y + src.y;
            dst.z = this->z + src.z;
            dst.w = this->w + src.w;
            return dst;
        }
        /*_Vector4 & operator + (const _Vector4 & src)
        {
            this->x += src.x;
            this->y += src.y;
            this->z += src.z;
            this->w += src.w;
            return *this;
        }*/
        _Vector4 operator - (const _Vector4 & src) const
        {
            _Vector4 dst;
            dst.x = this->x - src.x;
            dst.y = this->y - src.y;
            dst.z = this->z - src.z;
            dst.w = this->w - src.w;
            return dst;
        }
        /*_Vector4 & operator - (const _Vector4 & src)
        {
            this->x -= src.x;
            this->y -= src.y;
            this->z -= src.z;
            this->w -= src.w;
            return *this;
        }*/
        _Vector4 operator * (const float k) const
        {
            _Vector4 dst;
            dst.x = this->x * k;
            dst.y = this->y * k;
            dst.z = this->z * k;
            dst.w = this->w * k;
            return dst;
        }
        /*_Vector4 & operator * (const float k)
        {
            this->x *= k;
            this->y *= k;
            this->z *= k;
            this->w *= k;
            return *this;
        }*/
        float x;
        float y;
        float z;
        float w;
    }     Vector4;

//?????
//_bstr_t operator+(const char* s1, const _bstr_t& s2) ;



#endif// _Vector4_

#ifndef _NUI_BODY_PART_INDEX_
#define _NUI_BODY_PART_INDEX_
typedef 
enum _NUI_BODY_PART_INDEX
{
    BODY_PART_LU_HEAD = 0,  //0
    BODY_PART_RU_HEAD,      //1
    BODY_PART_NECK,         //2
    BODY_PART_LU_ARM,       //3
    BODY_PART_LU_TORSO,     //4
    BODY_PART_RU_TORSO,     //5
    BODY_PART_RU_ARM,       //6
    BODY_PART_LW_ARM,       //7
    BODY_PART_LW_TORSO,     //8
    BODY_PART_RW_TORSO,     //9
    BODY_PART_RW_ARM,       //10
    BODY_PART_L_HAND,       //11
    BODY_PART_LU_LEG,       //12
    BODY_PART_RU_LEG,       //13
    BODY_PART_R_HAND,       //14
    BODY_PART_LW_LEG,       //15
    BODY_PART_RW_LEG,       //16
    BODY_PART_L_FOOT,       //17
    BODY_PART_R_FOOT,       //18
    BODY_PART_LW_HEAD,      //19
    BODY_PART_RW_HEAD,      //20
    BODY_PART_L_SHOULDER,   //21
    BODY_PART_R_SHOULDER,   //22
    BODY_PART_L_ELBOW,      //23
    BODY_PART_R_ELBOW,      //24
    BODY_PART_L_KNEE,       //25
    BODY_PART_R_KNEE,       //26
    BODY_PART_L_WRIST,      //27
    BODY_PART_R_WRIST,      //28
    BODY_PART_L_ANKLE,      //29
    BODY_PART_R_ANKLE,      //30
    BODY_PART_COUNT         //31
} NUI_BODY_PART_INDEX;
#endif// _NUI_BODY_PART_INDEX_


#ifndef _NUI_SKELETON_POSITION_INDEX_
#define _NUI_SKELETON_POSITION_INDEX_
typedef 
enum _NUI_SKELETON_POSITION_INDEX
{    NUI_SKELETON_POSITION_HIP_CENTER    = 0,
     NUI_SKELETON_POSITION_SPINE    = ( NUI_SKELETON_POSITION_HIP_CENTER + 1 ) ,
     NUI_SKELETON_POSITION_SHOULDER_CENTER    = ( NUI_SKELETON_POSITION_SPINE + 1 ) ,
     NUI_SKELETON_POSITION_HEAD    = ( NUI_SKELETON_POSITION_SHOULDER_CENTER + 1 ) ,
     NUI_SKELETON_POSITION_SHOULDER_LEFT    = ( NUI_SKELETON_POSITION_HEAD + 1 ) ,
     NUI_SKELETON_POSITION_ELBOW_LEFT    = ( NUI_SKELETON_POSITION_SHOULDER_LEFT + 1 ) ,
     NUI_SKELETON_POSITION_WRIST_LEFT    = ( NUI_SKELETON_POSITION_ELBOW_LEFT + 1 ) ,
     NUI_SKELETON_POSITION_HAND_LEFT    = ( NUI_SKELETON_POSITION_WRIST_LEFT + 1 ) ,
     NUI_SKELETON_POSITION_SHOULDER_RIGHT    = ( NUI_SKELETON_POSITION_HAND_LEFT + 1 ) ,
     NUI_SKELETON_POSITION_ELBOW_RIGHT    = ( NUI_SKELETON_POSITION_SHOULDER_RIGHT + 1 ) ,
     NUI_SKELETON_POSITION_WRIST_RIGHT    = ( NUI_SKELETON_POSITION_ELBOW_RIGHT + 1 ) ,
     NUI_SKELETON_POSITION_HAND_RIGHT    = ( NUI_SKELETON_POSITION_WRIST_RIGHT + 1 ) ,
     NUI_SKELETON_POSITION_HIP_LEFT    = ( NUI_SKELETON_POSITION_HAND_RIGHT + 1 ) ,
     NUI_SKELETON_POSITION_KNEE_LEFT    = ( NUI_SKELETON_POSITION_HIP_LEFT + 1 ) ,
     NUI_SKELETON_POSITION_ANKLE_LEFT    = ( NUI_SKELETON_POSITION_KNEE_LEFT + 1 ) ,
     NUI_SKELETON_POSITION_FOOT_LEFT    = ( NUI_SKELETON_POSITION_ANKLE_LEFT + 1 ) ,
     NUI_SKELETON_POSITION_HIP_RIGHT    = ( NUI_SKELETON_POSITION_FOOT_LEFT + 1 ) ,
     NUI_SKELETON_POSITION_KNEE_RIGHT    = ( NUI_SKELETON_POSITION_HIP_RIGHT + 1 ) ,
     NUI_SKELETON_POSITION_ANKLE_RIGHT    = ( NUI_SKELETON_POSITION_KNEE_RIGHT + 1 ) ,
     NUI_SKELETON_POSITION_FOOT_RIGHT    = ( NUI_SKELETON_POSITION_ANKLE_RIGHT + 1 ) ,
     NUI_SKELETON_POSITION_COUNT    = ( NUI_SKELETON_POSITION_FOOT_RIGHT + 1 )
}     NUI_SKELETON_POSITION_INDEX;

#endif// _NUI_SKELETON_POSITION_INDEX_

#ifndef _NUI_IMAGE_TYPE_
#define _NUI_IMAGE_TYPE_
typedef 
enum _NUI_IMAGE_TYPE
{    NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX    = 0,
     NUI_IMAGE_TYPE_COLOR    = ( NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX + 1 ) ,
     NUI_IMAGE_TYPE_COLOR_YUV    = ( NUI_IMAGE_TYPE_COLOR + 1 ) ,
     NUI_IMAGE_TYPE_COLOR_RAW_YUV    = ( NUI_IMAGE_TYPE_COLOR_YUV + 1 ) ,
     NUI_IMAGE_TYPE_DEPTH    = ( NUI_IMAGE_TYPE_COLOR_RAW_YUV + 1 ) ,
     NUI_IMAGE_TYPE_COLOR_INFRARED    = ( NUI_IMAGE_TYPE_DEPTH + 1 ) ,
     NUI_IMAGE_TYPE_COLOR_RAW_BAYER    = ( NUI_IMAGE_TYPE_COLOR_INFRARED + 1 )
}     NUI_IMAGE_TYPE;

#endif// _NUI_IMAGE_TYPE_
#ifndef _NUI_IMAGE_RESOLUTION_
#define _NUI_IMAGE_RESOLUTION_
typedef 
enum _NUI_IMAGE_RESOLUTION
{    NUI_IMAGE_RESOLUTION_INVALID    = -1,
     NUI_IMAGE_RESOLUTION_80x60    = 0,
	 NUI_IMAGE_RESOLUTION_160x120  =( NUI_IMAGE_RESOLUTION_80x60 + 1 ),
     NUI_IMAGE_RESOLUTION_320x240    = ( NUI_IMAGE_RESOLUTION_160x120 + 1 ) ,
     NUI_IMAGE_RESOLUTION_640x480    = ( NUI_IMAGE_RESOLUTION_320x240 + 1 ) ,
     NUI_IMAGE_RESOLUTION_1280x960    = ( NUI_IMAGE_RESOLUTION_640x480 + 1 )
}     NUI_IMAGE_RESOLUTION;

#endif// _NUI_IMAGE_RESOLUTION_


#ifndef _NUI_IMAGE_VIEW_AREA_
#define _NUI_IMAGE_VIEW_AREA_
typedef struct _NUI_IMAGE_VIEW_AREA
{
    int eDigitalZoom;
    long lCenterX;
    long lCenterY;
}     NUI_IMAGE_VIEW_AREA;

#endif// _NUI_IMAGE_VIEW_AREA_


#ifndef _NUI_TRANSFORM_SMOOTH_PARAMETERS_
#define _NUI_TRANSFORM_SMOOTH_PARAMETERS_
typedef struct _NUI_TRANSFORM_SMOOTH_PARAMETERS
{
    _NUI_TRANSFORM_SMOOTH_PARAMETERS()
    {
        fSmoothing          = 0.5f;
        fCorrection         = 0.5f;
        fPrediction         = 0.5f;
        fJitterRadius       = 0.05f;
        fMaxDeviationRadius = 0.04f;
    }
    float fSmoothing;
    float fCorrection;
    float fPrediction;
    float fJitterRadius;
    float fMaxDeviationRadius;
}     NUI_TRANSFORM_SMOOTH_PARAMETERS;
#endif// _NUI_TRANSFORM_SMOOTH_PARAMETERS_


#ifndef _NUI_SURFACE_DESC_
#define _NUI_SURFACE_DESC_
typedef struct _NUI_SURFACE_DESC
{
    unsigned int Width;
    unsigned int Height;
}     NUI_SURFACE_DESC;

#endif// _NUI_SURFACE_DESC_


#ifndef _NUI_SKELETON_POSITION_TRACKING_STATE_
#define _NUI_SKELETON_POSITION_TRACKING_STATE_
typedef 
enum _NUI_SKELETON_POSITION_TRACKING_STATE												//@xu-li:骨架节点的跟踪状态有三种，跟踪上、推断出、未跟踪上
{    NUI_SKELETON_POSITION_NOT_TRACKED    = 0,
     NUI_SKELETON_POSITION_INFERRED    = ( NUI_SKELETON_POSITION_NOT_TRACKED + 1 ) ,
     NUI_SKELETON_POSITION_TRACKED    = ( NUI_SKELETON_POSITION_INFERRED + 1 )
}     NUI_SKELETON_POSITION_TRACKING_STATE;
#endif// _NUI_SKELETON_POSITION_TRACKING_STATE_

#ifndef _NUI_BODY_PART_POSITION_TRACKING_STATE_
#define _NUI_BODY_PART_POSITION_TRACKING_STATE_
typedef
enum _NUI_BODY_PART_POSITION_TRACKING_STATE
{
    NUI_BODY_PART_POSITION_NOT_TRACKED = 0,
    NUI_BODY_PART_POSITION_INFERRED    = ( NUI_BODY_PART_POSITION_NOT_TRACKED + 1 ),
    NUI_BODY_PART_POSITION_TRACKED     = ( NUI_BODY_PART_POSITION_INFERRED + 1 ),
} NUI_BODY_PART_POSITION_TRACKING_STATE;
#endif// _NUI_BODY_PART_POSITION_TRACKING_STATE_

#ifndef _NUI_SKELETON_TRACKING_STATE_
#define _NUI_SKELETON_TRACKING_STATE_
typedef 
enum _NUI_SKELETON_TRACKING_STATE
{    NUI_SKELETON_NOT_TRACKED    = 0,
     NUI_SKELETON_POSITION_ONLY    = ( NUI_SKELETON_NOT_TRACKED + 1 ) ,
     NUI_SKELETON_TRACKED    = ( NUI_SKELETON_POSITION_ONLY + 1 )
}     NUI_SKELETON_TRACKING_STATE;

#endif// _NUI_SKELETON_TRACKING_STATE_

#ifndef _NUI_BODY_PART_TRACKING_STATE_
#define _NUI_BODY_PART_TRACKING_STATE_
typedef 
enum _NUI_BODY_PART_TRACKING_STATE
{
    NUI_BODY_PART_NOT_TRACKED = 0,
    NUI_BODY_PART_POSISION_ONLY,
    NUI_BODY_PART_TRACKED
} NUI_BODY_PART_TRACKING_STATE;
#endif// _NUI_BODY_PART_TRACKING_STATE_

#ifndef _NUI_SKELETON_DATA_
#define _NUI_SKELETON_DATA_
typedef struct _NUI_SKELETON_DATA
{
    NUI_SKELETON_TRACKING_STATE eTrackingState;
    int dwTrackingID;
    int dwEnrollmentIndex;
    int dwUserIndex;
    Vector4 Position;
    Vector4 SkeletonPositions[ 20 ];
    NUI_SKELETON_POSITION_TRACKING_STATE eSkeletonPositionTrackingState[ 20 ];
    int dwQualityFlags;
}     NUI_SKELETON_DATA;

#endif// _NUI_SKELETON_TRACKING_STATE_
#ifndef _NUI_SKELETON_FRAME_
#define _NUI_SKELETON_FRAME_

#pragma pack(push, 16)
typedef struct _NUI_SKELETON_FRAME
{
    double liTimeStamp;
    int dwFrameNumber;
    int dwFlags;
    Vector4 vFloorClipPlane;
    Vector4 vNormalToGravity;
    NUI_SKELETON_DATA SkeletonData[ 6 ];
}     NUI_SKELETON_FRAME;


#pragma pack(pop)
#endif// _NUI_SKELETON_FRAME_

#ifndef _NUI_BODY_PART_CENTROIDS_
#define _NUI_BODY_PART_CENTROIDS_
typedef struct _NUI_BODY_PART_CENTROIDS
{
    NUI_BODY_PART_TRACKING_STATE eBodyPartTrackingState;
    Vector4 Centroids[ 31 ];
    int PixelCount[ 31 ];
    NUI_BODY_PART_POSITION_TRACKING_STATE eBodyPartPositionTrackingState[ 31 ];
} NUI_BODY_PART_CENTROIDS;
#endif// _NUI_BODY_PART_CENTROIDS_

#ifndef _NUI_CENTROIDS_STAGE_DATA_
#define _NUI_CENTROIDS_STAGE_DATA_
typedef struct _NUI_CENTROIDS_STAGE_DATA
{
    NUI_BODY_PART_CENTROIDS BodyPartCentroids[ 6 ];
} NUI_CENTROIDS_STAGE_DATA;
#endif// _NUI_CENTROIDS_STAGE_DATA_


#ifndef _NUI_LOCKED_RECT_
#define _NUI_LOCKED_RECT_
#ifdef _MIDL_OUTPUT_
typedef struct _NUI_LOCKED_RECT
{
    int Pitch;
    int size;
    void *pBits;
}     NUI_LOCKED_RECT;

#else
typedef struct _NUI_LOCKED_RECT
{
    int                         Pitch;
    int                         size;
    unsigned char                        *pBits;
} NUI_LOCKED_RECT;
#endif// _MIDL_OUTPUT_
#endif// _NUI_LOCKED_RECT_

#ifdef __cplusplus
extern "C" {
#endif
/// <summary>
/// Converts the resolution to a size.
/// </summary>
/// <param name="res">The image resolution.</param>
/// <param name="refWidth">The width of the image.</param>
/// <param name="refHeight">The height of the image.</param>
void NuiSinectImageResolutionToSize( _In_ NUI_IMAGE_RESOLUTION res, _Out_ int & refWidth, _Out_ int & refHeight );


#define NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS         (285.63f)   // Based on 320x240 pixel size.
#define NUI_CAMERA_DEPTH_NOMINAL_INVERSE_FOCAL_LENGTH_IN_PIXELS (3.501e-3f) // (1/NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS)
#define NUI_CAMERA_DEPTH_NOMINAL_DIAGONAL_FOV                   (70.0f)
#define NUI_CAMERA_DEPTH_NOMINAL_HORIZONTAL_FOV                 (58.5f)
#define NUI_CAMERA_DEPTH_NOMINAL_VERTICAL_FOV                   (45.6f)

#define NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS         (531.15f)   // Based on 640x480 pixel size.
#define NUI_CAMERA_COLOR_NOMINAL_INVERSE_FOCAL_LENGTH_IN_PIXELS (1.83e-3f)  // (1/NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS)
#define NUI_CAMERA_COLOR_NOMINAL_DIAGONAL_FOV                   ( 73.9f)
#define NUI_CAMERA_COLOR_NOMINAL_HORIZONTAL_FOV                 ( 62.0f)
#define NUI_CAMERA_COLOR_NOMINAL_VERTICAL_FOV                   ( 48.6f)

// Assuming a pixel resolution of 320x240
// x_meters = (x_pixelcoord - 160) * NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * z_meters;
// y_meters = (y_pixelcoord - 120) * NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * z_meters;
#define NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 (NUI_CAMERA_DEPTH_NOMINAL_INVERSE_FOCAL_LENGTH_IN_PIXELS)

// Assuming a pixel resolution of 320x240
// x_pixelcoord = (x_meters) * NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 / z_meters + 160;
// y_pixelcoord = (y_meters) * NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 / z_meters + 120;
#define NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 (NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS)


#ifdef __cplusplus
}
#endif

#endif