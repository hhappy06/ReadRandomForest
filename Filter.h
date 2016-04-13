#include <stdio.h>
#include <vector>
#include "NuiSinect.h"//@xu-li:使用其中vector的数据结构

class Filter
{
public:
	Filter();
	~Filter();

	bool bstart;
	bool isfirstTime;
	Vector4 vec_pres[20];

	
	Vector4 Exponential_sf( Vector4 *X, Vector4 *s_pre, float a = 0.5 );//常用值a=0.1/0.3/0.5/0.9
	Vector4 Exponential_sf( Vector4 *X,  Vector4 *s_pre, Vector4 *b1_pre,  float a=0.2f, float v=0.7f );
	Vector4 Exponential_sf( Vector4 X,  Vector4 s_pre, Vector4 b1_pre,  float a=0.2f, float v=0.7f );
	
	void DExponential_sf( Vector4 *X,  Vector4 *s1_pre, Vector4 *s2_pre, float a=0.6 );
	void DExponential_sf( Vector4 *X,  Vector4 *s1_pre, Vector4 *s2_pre, Vector4 *b1_pre, Vector4 *b2_pre, float a=0.2f, float v=0.7f );
	
	void JitterRemoval_sf( Vector4 *X, Vector4 *X_pre, Vector4 *b1_pre, float a, float v );

};