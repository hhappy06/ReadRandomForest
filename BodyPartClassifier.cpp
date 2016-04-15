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

	char* pfile = ":/model/res/XXX.z";
	ifstream ifile(pfile, ios::binary);
	ifile.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);
	
										//@xu-li:创建一个QDataStream类，QDataStream是QT的类，对二进制提供串行数据输入输出流
	//get file length
	istream::pos_type current_pos = ifile.tellg();
	ifile.seekg(0, ios_base::end);
	const int cbSize = ifile.tellg();
	ifile.seekg(current_pos);

	printf("the random forest length is %d\n", cbSize);

	char * s = new  char [32*1024*1024];
	const  unsigned char * pSource  = (const unsigned char *)s;
	assert(s != 0);
	ifile.read(s, 32 * 1024 * 1024);
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


