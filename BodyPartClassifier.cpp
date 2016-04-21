#include "stdafx.h"
//#include <cl/cl.h>
//#include <ppl.h>
#include <iostream>
#include "fstream"
#include "zlib/zlib.h"
//#include "../resource.h"
#include "Util.h"

#include "RandomForest.h"
#include "BodyPartClassifier.h"
#include <fstream>

#include <iostream>
#include <float.h>
#include "stdio.h"

#include <cassert>
#define NUI_IMAGE_PLAYER_INDEX_SHIFT          3
#define NUI_IMAGE_PLAYER_INDEX_MASK           ((1 << NUI_IMAGE_PLAYER_INDEX_SHIFT)-1) //*#define NUI_IMAGE_PLAYER_INDEX_MASK 7

//@xu-li:
#define THE_FIRST_TIME_FOR_FILTERING 1
#define NOT_THE_FIRST_TIME_FOR_FILTERING 0
//#define USE_PARALLEL
#ifdef USE_PARALLEL
#define PARALLEL_FOR_BEGIN(V, S, E) Concurrency::parallel_for(S, E, [&](int V)
#define PARALLEL_FOR_END            });
#else
#define PARALLEL_FOR_BEGIN(V, S, E) for (int V = S; V < E; y ++)
#define PARALLEL_FOR_END            }
#endif


#include <limits.h>

#define FILEOUTPUT(x)

double g_initCentroid = 0;
double g_calcCentroid = 0;
double g_initCentroidMem = 0;
double g_cpuPredictRawParallelTimeD = 0;
double g_cpuRawToLabelTimeD = 0;
bool g_doMeanshift = true;
Vector4 g_vInitedCentroid_copy[6][31];


//extern int parallelism_enabled;
bool g_NoHugeArray;
bool g_GrayGravityCentroid = false;

#ifdef WIN32
	
#endif // WIN32

//void getvCentroid(Vector4 Centroid[6][31])
//{
//	memcpy(Centroid, g_vInitedCentroid_copy, sizeof(g_vInitedCentroid_copy));
//}

BodyPartClassifier::BodyPartClassifier()
{
	//m_pLabelRaw = new unsigned short[160*120*31];
	//ASSERT(m_pLabelRaw);
	//m_pMask     = new unsigned char [160*120];
	//ASSERT(m_pMask);
	// CentroidsStage_RunFrame用
	init();

}

void BodyPartClassifier::init()
{
	//	m_context = NULL;
	//cl_m_pfBodyPartProbility = NULL;
	//m_pvCoordWorldSpace   = new Vector4[120*160];
	//ASSERT(m_pvCoordWorldSpace);
	//m_pfBodyPartProbility = new float[6*31*120*160];
	//ASSERT(m_pfBodyPartProbility);
	m_iBodyPartProbilityStep = 120 * 160 * 31;

	//平滑参数默认值
	m_defaultSmoothingParams.fSmoothing          = 0.5f;
	m_defaultSmoothingParams.fCorrection         = 0.5f;
	m_defaultSmoothingParams.fPrediction         = 0.5f;
	m_defaultSmoothingParams.fJitterRadius       = 0.05f;
	m_defaultSmoothingParams.fMaxDeviationRadius = 0.04f;

	//@xu-li:过滤器
	filter = new Filter();
	memset(isfirstTime, 1,  20*6);
	

}


BodyPartClassifier::~BodyPartClassifier()
{
}


#define CHUNK 16384
#define MAGIC ((unsigned char)0x96);
//int inf(FILE *source, FILE *dest)
int inf(const unsigned char * pSrc, int srcSize, unsigned char * pDst, int dstSize, int * length)
{
	int ret;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;

	/* decompress until deflate stream ends or end of file */
	int sz, remain, pos;
	remain = srcSize;
	pos = 0;
	*length = 0;
	do {
		// decrypt
		sz = remain > CHUNK ? CHUNK : remain;
		if (sz == 0)
			break;
		memcpy(in, pSrc+pos, sz);
		remain -= sz;
		pos += sz;
		for (int i=0; i<sz; i++)
		{
			in[i] ^= MAGIC;
		}
		strm.avail_in = sz;
		/*strm.avail_in = fread(in, 1, CHUNK, source);
		if (ferror(source)) {
		(void)inflateEnd(&strm);
		return Z_ERRNO;
		}
		if (strm.avail_in == 0)
		break;*/
		strm.next_in = in;
		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}
			have = CHUNK - strm.avail_out;
			memcpy(pDst + *length, out, have);
			*length += have;
			/*if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
			(void)inflateEnd(&strm);
			return Z_ERRNO;
			}*/
		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

bool BodyPartClassifier::BuildForestFromResource( )
{
	bool res = FALSE;

	char* pfile = "XXX.z";
	ifstream ifile(pfile, ios::binary|ios::in);
	//ifile.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);

	char * s = new  char [32*1024*1024];
	const  unsigned char * pSource  = (const unsigned char *)s;
	assert(s != 0);
	ifile.read(s, 32 * 1024 * 1024);
	const int cbSize = ifile.gcount();
	printf("the random forest length is %d\n", cbSize);
	assert (cbSize > 0);

	unsigned char * pDest = new unsigned char [32*1024*1024];

	int length;
	int ret = inf(pSource, cbSize, pDest, 32*1024*1024, &length);//@xu-li:功能？
	if (ret == Z_OK)
	{
		res = m_forest.BuildForest((const  char *)pDest, length);
	}

	delete [] pDest;
	delete [] s;
	return res;
}

bool BodyPartClassifier::LoadImage(const cv::Mat in_depthmat, const cv::Mat in_maskmat)
{
	if (!m_DepthMat.empty())
	{
		m_DepthMat.release();
		m_MaskMat.release();
	}
	if (in_depthmat.empty() || in_maskmat.empty())
	{
		return NULL;
	}
	m_CurrentPersonNumber = 0;
	m_ScaleHeight = 0.0f;
	m_ScaleWidth = 0.0f;

	in_depthmat.copyTo(m_DepthMat);
	in_maskmat.copyTo(m_MaskMat);

	if (m_PriorMat[0][0].empty()
		|| m_PriorMat[0][0].size != in_depthmat.size)
	{
		for (int ip = 0; ip < _SUPPROT_PERSON_NUMBER_; ip++)
		{
			for (int im = 0; im < _BODY_PART_NUMBER_; im++)
			{
				m_PriorMat[ip][im] = cv::Mat(INFER_IMAGE_WIDTH,INFER_IMAGE_HEIGHT,CV_32SC1);
			}
		}
	}

}
bool BodyPartClassifier::PredictRawParallel(void)
{
	if (m_DepthMat.empty())
		return false;

	for (int ip = 0; ip < _SUPPROT_PERSON_NUMBER_; ip++)
	{
		for (int im = 0; im < _BODY_PART_NUMBER_; im++)
		{
			m_PriorMat[ip][im].setTo(0);
		}
	}

	m_ScaleWidth = float (m_DepthMat.cols) / (float) INFER_IMAGE_WIDTH;
	m_ScaleHeight = float(m_DepthMat.rows) / (float) INFER_IMAGE_HEIGHT;

	int x, y;
#pragma omp parallel for private(x)
	for ( y = 0; y < INFER_IMAGE_HEIGHT; y++)
	{
		for ( x = 0; x < INFER_IMAGE_WIDTH; x++)
		{
			PredictOnePixel(x, y);
		}
	}
}

bool BodyPartClassifier::PredictOnePixel(int in_x, int in_y)
{
	if (in_x < 0 || in_x >= INFER_IMAGE_WIDTH
		|| in_y < 0 || in_y >= INFER_IMAGE_HEIGHT)
		return;

	int orix = (int)(m_ScaleWidth * in_x);
	int oriy = (int)(m_ScaleHeight * in_y);

	int pid = (int) m_MaskMat.ptr<uchar>(oriy)[orix];
	if ( pid == 0)
		return;

	int depthv = m_DepthMat.ptr<unsigned short>(oriy)[orix];
	float fscalex = m_ScaleWidth * NORMALIZED_DEPTH_VALUE_ / (float)depthv;
	float fscaley = m_ScaleHeight * NORMALIZED_DEPTH_VALUE_ / (float)depthv;

	uchar ux, uy, vx, vy;
	int node_id, p1x, p1y, p2x, p2y;
	int depthv1, depthv2;
	for (int itree = 0; itree < m_forest.TreeNumber(); itree++)
	{
		node_id = 0;
		while (node_id >= 0)
		{
			const TreeNode* pnode = m_forest.GetNode(itree, node_id);

			ux = pnode->ux;
			uy = pnode->uy;
			vx = pnode->vx;
			vy = pnode->vy;

			// calculate shift point
			// calc p1, p2
			p1x = (int)(((float)ux * fscalex) + (orix));//@xu-li:第一个偏移后像素点的x,x*2是图像缩小的缘故
			p1y = (int)(((float)uy * fscaley) + (oriy));//@xu-li:第一个偏移后像素点的y
			p2x = (int)(((float)vx * fscalex) + (orix));//@xu-li:第二个偏移后像素点的x
			p2y = (int)(((float)vy * fscaley) + (oriy));//@xu-li:第二个偏移后像素点的y

			depthv1 = DEPTH_UNUSUAL_VALUE;
			if (p1x >= 0 && p1x < m_DepthMat.cols && p1y >= 0 && p1y < m_DepthMat.rows)
			{
				if (m_MaskMat.ptr<uchar>(p1y)[p1x] == pid)
					depthv1 = m_DepthMat.ptr<ushort>(p1y)[p1x];
			}

			if (vx == 0 && vy == 0)
			{
				depthv2 = depthv;
			}
			else
			{
				depthv2 = DEPTH_UNUSUAL_VALUE;
				if (p2x >= 0 && p2x < m_DepthMat.cols && p2y >= 0 && p2y < m_DepthMat.rows)
				{
					if (m_MaskMat.ptr<uchar>(p2y)[p2x] == pid)
						depthv2 = m_DepthMat.ptr<ushort>(p2y)[p2x];
				}
			}

			if (depthv1 - depthv2 < (int)pnode->c)
				node_id = pnode->left;
			else
			{
				node_id = pnode->right;
			}
		}

		// get leaf node 
		int value_id = -(node_id + 1);
		const NodeValue* valuedst = m_forest.GetValue(value_id);
		assert(valuedst->v[0].cnt + valuedst->v[1].cnt + valuedst->v[2].cnt + valuedst->v[3].cnt + valuedst->v[4].cnt <= 255);
		for (int i = 0; i < 5; i++)
		{
			uchar part_id = valuedst->v[i].id;
			uchar part_cnt = valuedst->v[i].cnt;
			m_PriorMat[pid-1][part_id].ptr<int>(in_y)[in_x] += part_cnt;
		}
	}
}

void BodyPartClassifier::initCentroidpoint(void)
{
	// initialize memory
	memset(m_CoordWorldSpace,0, INFER_IMAGE_HEIGHT*INFER_IMAGE_WIDTH*sizeof(Vector4));
	memset(m_PartCentroid,0, _SUPPROT_PERSON_NUMBER_*_BODY_PART_NUMBER_*sizeof(Vector4));
	memset(m_PartCount,0, _SUPPROT_PERSON_NUMBER_*_BODY_PART_NUMBER_*sizeof(int));

	int x, y;
	int orix,oriy,pid,partid;
	ushort depthv;
	Vector4 worldcord;
	float probability;
#pragma omp parallel for private(x,orix,oriy,depthv,pid,worldcord,partid,probability)
	for (y = 0; y < INFER_IMAGE_HEIGHT; y++)
	{
		for (x = 0; x < INFER_IMAGE_WIDTH; x++)
		{
			orix = (int)(m_ScaleWidth * x);
			oriy = (int)(m_ScaleHeight * y);
			depthv = m_DepthMat.ptr<ushort>(oriy)[orix];
			pid = m_MaskMat.ptr<uchar>(oriy)[orix];

			if(pid == 0 || pid > _SUPPROT_PERSON_NUMBER_)
				continue;

			ImageToWorldSpace(Vector4I(x,y,depthv),worldcord);

			m_CoordWorldSpace[y][x] = worldcord;

			for (partid = 0; partid < _BODY_PART_NUMBER_; partid++)
			{
				probability = m_PriorMat[pid - 1][partid].ptr<float>(y)[x];
				probability /= (m_forest.TreeNumber()*255.0f);
				m_PriorMat[pid - 1][partid].ptr<float>(y)[x] = probability;

				if (probability > 0.14f)
				{
					m_PartCentroid[pid - 1][partid].x += worldcord.x;
					m_PartCentroid[pid - 1][partid].y += worldcord.y;
					m_PartCentroid[pid - 1][partid].z += worldcord.z;

					m_PartCount[pid - 1][partid] += 1;
				}

			}
		}
	}
}


void BodyPartClassifier::ImageToWorldSpace(const Vector4I & src, Vector4 & dst, int height/* = 120*/, int width/* = 160*/)
{
	//  static const float NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 = 3.501f/1000;
	assert(height >= 120);
	assert(width >= 160);

	dst.z = src.z * 0.001f;	//the metrics: m
	dst.x = (src.x - width / 2.0f) * (320.0f / width) * NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * dst.z;
	dst.y = -(src.y - height / 2.0f) * (240.0f / height) * NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * dst.z;
	dst.w = 0;
}


void BodyPartClassifier::WorldToImageSpace(const Vector4 & src, Vector4I & dst, int height/* = 120*/, int width/* = 160*/)
{
	//  static const float NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 = 285.63f;
	assert(height >= 120);
	assert(width >= 160);

	if (src.z > FLT_EPSILON)
	{
		dst.x = (int)(width / 2 + src.x * (width / 320.0) * NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 / src.z);
		dst.y = (int)(height / 2 - src.y * (height / 240.0) * NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 / src.z);
		dst.z = (int)(src.z * 1000);
		dst.w = 1;
	}
	else
	{
		dst.x = 0;
		dst.y = 0;
		dst.z = 0;
		dst.w = 0;
	}
	//assert(dst.y >= 0 && dst.y < height);
	//assert(dst.x >= 0 && dst.x < width);
}