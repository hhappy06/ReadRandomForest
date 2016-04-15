#include "stdafx.h"
//#include <CL/cl.h>
#include "RandomForest.h"
//#include "clUtil.h"

typedef struct  
{
	UINT ind;
	UCHAR v[5];
} tmpNodeValueStorage;


RandomForest::RandomForest()
	: m_pValue(NULL)
	//m_cl_tree(NULL),
	//m_context(NULL)
{
}


RandomForest::~RandomForest()
{
	delete [] m_ppTree[0];
	if (m_pValue)
	{
		delete [] m_pValue;
		m_pValue = NULL;
	}
}


bool RandomForest::BuildForest(const  char * pszData, const int size)
{
	// check input
	if (!pszData || FOREST_DATA_SIZE != size)
	{
		return FALSE;
	}

	memset(m_ppTree, 0, TREE_COUNT * sizeof(TreeNode *));//@xu-li: sizeof(TreeNode *)节点大小，这里m_ppTree是个数组，其内容是指向三棵树的根节点的指针，故只有三个节点的大小

	 //for (int i = 0; i < TREE_COUNT; i ++)
	 //{
	TreeNode * t = new TreeNode[NODE_COUNT * 3];
	if (t == NULL)
	{
		return FALSE;
	}

	m_ppTree[0] = t;
	m_ppTree[1] = t + NODE_COUNT;
	m_ppTree[2] = t + NODE_COUNT * 2;

	//}
	//}

	if (m_pValue == NULL)
	{
		m_pValue = new NodeValue[VALUE_COUNT];
		if (m_pValue == NULL)
		{
			return FALSE;
		}
	}


	const char * pForestData = pszData;
	for (int i=0; i<TREE_COUNT; i++)
	{
		TreeNode * n = m_ppTree[i];
		int ind = -1;
		while (++ind < NODE_COUNT)
		{
			TreeNode * src = (TreeNode *)pForestData;//@xu-li:强制转换不改变指针类型，只对赋值对象有影响
			n[ind].left = src->left;
			n[ind].right = src->right;
			n[ind].ux = src->ux;
			n[ind].uy = src->uy;
			n[ind].vx = src->vx;
			n[ind].vy = src->vy;
			n[ind].c  = src->c;
			pForestData += 0x10;					//@xu-li:pForestData是const char类型指针，一次移动一个字节，上面的强制转换并不改变指针类型
		}											//@xu-li:节点数据结构体内容实际上只有14个字节，但pForestData每次加16个字节，说明模型数据存储时有空余
		ASSERT(ind == NODE_COUNT);
		if (ind != NODE_COUNT)
		{
			return FALSE;
		}
	}

	{
		NodeValue * v = m_pValue;
		int ind = -1;
		while (++ind < VALUE_COUNT)
		{
			tmpNodeValueStorage * src = (tmpNodeValueStorage *)pForestData;
			UINT value_indices = src->ind;
			v[ind].v[0].id  = (unsigned char)(value_indices & 0x0000001F);//@xu-li:5位存id
			v[ind].v[0].cnt = src->v[0];
			value_indices >>= 5;
			v[ind].v[1].id  = (unsigned char)(value_indices & 0x0000001F);
			v[ind].v[1].cnt = src->v[1];
			value_indices >>= 5;
			v[ind].v[2].id  = (unsigned char)(value_indices & 0x0000001F);
			v[ind].v[2].cnt = src->v[2];
			value_indices >>= 5;
			v[ind].v[3].id  = (unsigned char)(value_indices & 0x0000001F);
			v[ind].v[3].cnt = src->v[3];
			value_indices >>= 5;
			v[ind].v[4].id  = (unsigned char)(value_indices & 0x0000001F);
			v[ind].v[4].cnt = src->v[4];
			pForestData += 9;
		}
		ASSERT(ind == VALUE_COUNT);
		if (ind != VALUE_COUNT)
		{
			return FALSE;
		}
	}
	if (pForestData != pszData + FOREST_DATA_SIZE)
	{
		return FALSE;
	}



	return TRUE;
}


const TreeNode * RandomForest::Tree(int treeID) const
{
	if (treeID < 0 || treeID >= TREE_COUNT)
	{
		return NULL;
	}
	else
	{
		return m_ppTree[treeID];
	}
}


const TreeNode * RandomForest::Node(int treeID, int nodeID) const
{
	if (treeID < 0 || treeID >= TREE_COUNT ||
		nodeID < 0 || nodeID >= NODE_COUNT)
	{
		return NULL;
	}
	else
	{
		return &(m_ppTree[treeID][nodeID]);
	}
}


const NodeValue * RandomForest::Value(int valueID) const
{
	if (valueID < 0 || valueID >= VALUE_COUNT)
	{
		return NULL;
	}
	else
	{
		return &(m_pValue[valueID]);
	}
}




