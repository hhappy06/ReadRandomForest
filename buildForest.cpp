#include "stdafx.h"
#include <CL/cl.h>
#include <assert.h>
//#include "const.h"
#include <string>
#include "NuiSinect.h"
#include "skeletonObj.h"

#include "RandomForest.h"
#include "BodyPartClassifier.h"
#include "clUtil.h"

#include <iostream>
#include "buildForest.h"
using namespace std;
bool BuildForestFromResource(SkeletonObj * a_this )
{
	cl_int  ret;
	cl_int iErrNum;
	TreeNode * t = 	((BodyPartClassifier * )a_this->m_pBPC)->m_forest.m_ppTree[0];
	
	NodeValue * pValue = 	((BodyPartClassifier * )a_this->m_pBPC)->m_forest.m_pValue;
	a_this->m_cl_tree = clCreateBuffer(a_this->m_context, CL_MEM_READ_ONLY/*|CL_MEM_ALLOC_HOST_PTR*/ , 
		sizeof(TreeNode) * RandomForest::NODE_COUNT * 3,   NULL, &iErrNum);
	clCHECKERROR(iErrNum, CL_SUCCESS);

	a_this->m_cl_value = clCreateBuffer(a_this->m_context, CL_MEM_READ_ONLY/*|CL_MEM_ALLOC_HOST_PTR */,
		sizeof(NodeValue) * RandomForest::VALUE_COUNT,    NULL, &iErrNum);
	clCHECKERROR(iErrNum, CL_SUCCESS);


	iErrNum = clEnqueueWriteBuffer(a_this->m_commandQueue, a_this->m_cl_tree, CL_TRUE, 0, sizeof(TreeNode) * RandomForest::NODE_COUNT * 3, t, 0, NULL, NULL);
	clCHECKERROR(iErrNum, CL_SUCCESS);
	iErrNum = clEnqueueWriteBuffer(a_this->m_commandQueue, a_this->m_cl_value, CL_TRUE, 0, sizeof(NodeValue) * RandomForest::VALUE_COUNT, pValue, 0, NULL, NULL);
	clCHECKERROR(iErrNum, CL_SUCCESS);

	return true;
}
void cl_releaseForest(SkeletonObj * a_this )
{
	cl_int status;
	status = clReleaseMemObject(a_this->m_cl_tree);//Release mem object.
	clCHECKERROR(status, CL_SUCCESS);
	status = clReleaseMemObject(a_this->m_cl_value);//Release mem object.
	clCHECKERROR(status, CL_SUCCESS);
}
