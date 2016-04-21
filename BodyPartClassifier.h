#ifndef _BODY_PART_CLASSIFIER_
#define _BODY_PART_CLASSIFIER_
#include <cassert>
#include "NuiSinect.h"
#include "RandomForest.h"
#include "Filter.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

//#define _Vector4I_
#ifndef _Vector4I_
#define _Vector4I_
typedef struct _Vector4I
{
    _Vector4I(int ax = 0, int ay = 0, int az = 0)
        : x(ax), y(ay), z(az), w(0)
    {
    }
    int x;
    int y;
    int z;
    int w;
}     Vector4I;
#endif// _Vector4I_

#define _DEPRECATED_
//////////////////////////////////////////////////////////////////////////
#ifndef _SMOOTH_FILTER_STATE_
#define _SMOOTH_FILTER_STATE_
typedef
    enum _SMOOTH_FILTER_STATE
{
    SMOOTH_FILTER_INITIAL  = 0,
    SMOOTH_FILTER_NTRACKED = SMOOTH_FILTER_INITIAL + 1,
    SMOOTH_FILTER_TRACKED  = SMOOTH_FILTER_NTRACKED + 1
}   SMOOTH_FILTER_STATE;
#endif// _SMOOTH_FILTER_STATE_

#ifndef _Joints_Smoothing_State_
#define _Joints_Smoothing_State_
typedef 
struct _Joints_Smoothing_State
{
    _Joints_Smoothing_State()
    {
        memset(&m_Bn, 0, 20 * sizeof(Vector4));
        memset(&m_Xn, 0, 20 * sizeof(Vector4));
        memset(&m_X, 0, 20 * sizeof(Vector4));
        memset(&m_FilterState, 0, 20 * sizeof(SMOOTH_FILTER_STATE));
    }
    Vector4 m_Bn[20];
    Vector4 m_Xn[20];
    Vector4 m_X[20];
    SMOOTH_FILTER_STATE m_FilterState[20];
}   Joints_Smoothing_State;
#endif// _Joints_Smoothing_State_
//////////////////////////////////////////////////////////////////////////

#ifndef _SUPPROT_PERSON_NUMBER_
#define _SUPPROT_PERSON_NUMBER_ 6
#endif // !_SUPPROT_PERSON_NUMBER_

#ifndef _BODY_PART_NUMBER_
#define _BODY_PART_NUMBER_ 31
#endif // !_BODY_PART_NUMBER_

#define INFER_IMAGE_HEIGHT 120
#define INFER_IMAGE_WIDTH 160
#define NORMALIZED_DEPTH_VALUE_ (2048.0f)
#define DEPTH_UNUSUAL_VALUE 6000 // 6m
class  BodyPartClassifier
{
public:
    BodyPartClassifier();
	//BodyPartClassifier(cl_context a_context);

	void init();

	~BodyPartClassifier();

    // 加载随机森林分类模型
   // BOOL BuildForestFromFile(const  char * szFileName);
	bool BuildForestFromResource( );

	//private:
    RandomForest     m_forest;
    //unsigned short * m_pLabelRaw;
    //unsigned char  * m_pMask;
    // CentroidsStage_RunFrame用
    // 当前帧所有像素的世界坐标系坐标(x, y, z(depth))
   // Vector4 * m_pvCoordWorldSpace;   //[120][160]
    // 6个人31个部位的概率
   // float   * m_pfBodyPartProbility; // [6][120][160][31]
    int  m_iBodyPartProbilityStep;
    // smooth用
    NUI_TRANSFORM_SMOOTH_PARAMETERS m_defaultSmoothingParams;
    Joints_Smoothing_State          m_msSmoothingState[6];


	//cl_mem cl_m_pfBodyPartProbility;
	//cl_context m_context;

	//@xu-li:过滤器
	Filter *filter;
	Vector4 pretmp1[ 20*6 ];
	Vector4 pretmp2[ 20*6 ];
	Vector4 btmp1[ 20*6 ];
	Vector4 btmp2[ 20*6 ];
	unsigned char isfirstTime[ 20*6 ];

	bool WriteForest(char* pwirtefilename)
	{
		return m_forest.WriteForest(pwirtefilename);
	}

	bool BuildRandomForestFromUnzipfile(char* pfilename)
	{
		return m_forest.BuildRandomForestFromUnzipfile(pfilename);
	}

	bool cmpRandomForest(void)
	{
		return m_forest.cmpRandomForest();
	}

	cv::Mat m_DepthMat;
	cv::Mat m_MaskMat;
	float m_ScaleHeight;
	float m_ScaleWidth;
	int m_CurrentPersonNumber;
	cv::Mat m_PriorMat[_SUPPROT_PERSON_NUMBER_][_BODY_PART_NUMBER_];
	Vector4 m_CoordWorldSpace[INFER_IMAGE_HEIGHT][INFER_IMAGE_WIDTH];
	Vector4 m_PartCentroid[_SUPPROT_PERSON_NUMBER_][_BODY_PART_NUMBER_];
	int		m_PartCount[_SUPPROT_PERSON_NUMBER_][_BODY_PART_NUMBER_];

	bool LoadImage(const cv::Mat in_depthmat, const cv::Mat in_maskmat);
	bool PredictRawParallel(void);
	bool PredictOnePixel(int in_x, int in_y);
	void initCentroidpoint(void);
	void ImageToWorldSpace(const Vector4I & src, Vector4 & dst, int height = INFER_IMAGE_HEIGHT, int width = INFER_IMAGE_WIDTH);
	void WorldToImageSpace(const Vector4 & src, Vector4I & dst, int height = INFER_IMAGE_HEIGHT, int width = INFER_IMAGE_WIDTH);
};

#endif//
