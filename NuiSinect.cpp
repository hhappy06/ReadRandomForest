#include "NuiSinect.h"

void NuiSinectImageResolutionToSize( _In_ NUI_IMAGE_RESOLUTION res, _Out_ int & refWidth, _Out_ int & refHeight )
{
	switch( res )
	{
	case NUI_IMAGE_RESOLUTION_80x60:
		refWidth = 80;
		refHeight = 60;
		break;
	case NUI_IMAGE_RESOLUTION_160x120:
		refWidth = 160;
		refHeight = 120;
		break;
	case NUI_IMAGE_RESOLUTION_320x240:
		refWidth = 320;
		refHeight = 240;
		break;
	case NUI_IMAGE_RESOLUTION_640x480:
		refWidth = 640;
		refHeight = 480;
		break;
	case NUI_IMAGE_RESOLUTION_1280x960 :
		refWidth = 1280;
		refHeight = 960;
		break;
	default:
		refWidth = 0;
		refHeight = 0;
		break;
	}
}


void
	NuiSinectTransformSkeletonToDepthImage(
	_In_ Vector4 vPoint,
	_Out_ long *plDepthX,
	_Out_ long *plDepthY,
	_Out_ unsigned short *pusDepthValue,
	_In_ NUI_IMAGE_RESOLUTION eResolution
	)
{
	if((plDepthX == 0) || (plDepthY == 0) || (pusDepthValue == 0))
	{
		return;
	}

	//
	// Requires a valid depth value.
	//

	if(vPoint.z > FLT_EPSILON)
	{
		int width;
		int height;
		NuiSinectImageResolutionToSize( eResolution, width, height );

		//
		// Center of depth sensor is at (0,0,0) in skeleton space, and
		// and (width/2,height/2) in depth image coordinates.  Note that positive Y
		// is up in skeleton space and down in image coordinates.
		//
		// The 0.5f is to correct for casting to int truncating, not rounding

		*plDepthX = static_cast<int>( width / 2 + vPoint.x * (width/320.f) * NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 / vPoint.z + 0.5f);
		*plDepthY = static_cast<int>( height / 2 - vPoint.y * (height/240.f) * NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 / vPoint.z + 0.5f);

		//
		//  Depth is in meters in skeleton space.
		//  The depth image pixel format has depth in millimeters shifted left by 3.
		//

		*pusDepthValue = static_cast<unsigned short>(vPoint.z *1000) << 3;
	} else
	{
		*plDepthX = 0;
		*plDepthY = 0;
		*pusDepthValue = 0;
	}
}


Vector4
	NuiSinectTransformDepthImageToSkeleton1(
	_In_ long lDepthX,
	_In_ long lDepthY,
	_In_ unsigned short usDepthValue,
	_In_ int width,
	_In_ int height
	)
{
	//
	//  Depth is in meters in skeleton space.
	//  The depth image pixel format has depth in millimeters shifted left by 3.
	//

	float fSkeletonZ = static_cast<float>(usDepthValue)  / 1000.0f;

	//
	// Center of depth sensor is at (0,0,0) in skeleton space, and
	// and (width/2,height/2) in depth image coordinates.  Note that positive Y
	// is up in skeleton space and down in image coordinates.
	//

	float fSkeletonX = (lDepthX - width/2.0f) * (320.0f/width) * NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * fSkeletonZ;
	float fSkeletonY = -(lDepthY - height/2.0f) * (240.0f/height) * NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * fSkeletonZ;

	//
	// Return the result as a vector.
	//

	Vector4 v4;
	v4.x = fSkeletonX;
	v4.y = fSkeletonY;
	v4.z = fSkeletonZ;
	v4.w = 1.0f;
	return v4;
}