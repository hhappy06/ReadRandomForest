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
	//if (m_ppTree)
	//{
	//	for (int i = 0; i < TREE_COUNT; i ++)
	//	{
	//		if (m_ppTree[i])
	//		{
	//			delete [] m_ppTree[i];
	//			m_ppTree[i] = NULL;
	//		}
	//	}
	//	delete [] m_ppTree;
	//	// m_ppTree = NULL;
	//}

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

bool RandomForest::WriteForest(char* pwirtefilename)
{
	ASSERT(pwirtefilename != '\0', );

	ofstream ofileout;
	ofileout.open(pwirtefilename, ios::binary | ios::out);
	
	ASSERT(ofileout.is_open() != false);

	printf("writing random forest structure...");
	ofileout.write((char*)(&TREE_COUNT),sizeof(TREE_COUNT));
	ofileout.write((char*)(&TREE_DEPTH), sizeof(TREE_DEPTH));

	const TreeNode* pnode;
	int idxnode;
	for (int idxtree = 0; idxtree < TREE_COUNT; idxtree++)
	{
		for (idxnode = 0; idxnode < NODE_COUNT; idxnode++)
		{
			pnode = this->Node(idxtree, idxnode);
			ASSERT(pnode != NULL);
			ofileout.write((char*)(pnode),sizeof(TreeNode));
		}

		ASSERT(idxnode == NODE_COUNT);
	}

	printf(" done!\n");

	printf("writing random forest node value...");
	ofileout.write((char*)(&VALUE_COUNT),sizeof(VALUE_COUNT));
	
	tmpNodeValueStorage nodevalue;
	for (idxnode = 0; idxnode < VALUE_COUNT; idxnode++)
	{
		
		nodevalue.v[0] = m_pValue[idxnode].v[0].cnt;
		nodevalue.v[1] = m_pValue[idxnode].v[1].cnt;
		nodevalue.v[2] = m_pValue[idxnode].v[2].cnt;
		nodevalue.v[3] = m_pValue[idxnode].v[3].cnt;
		nodevalue.v[4] = m_pValue[idxnode].v[4].cnt;

		nodevalue.ind = m_pValue[idxnode].v[4].id;
		nodevalue.ind <<= 5;
		nodevalue.ind += m_pValue[idxnode].v[3].id;

		nodevalue.ind <<= 5;
		nodevalue.ind += m_pValue[idxnode].v[2].id;
		
		nodevalue.ind <<= 5;
		nodevalue.ind += m_pValue[idxnode].v[1].id;
		
		nodevalue.ind <<= 5;
		nodevalue.ind += m_pValue[idxnode].v[0].id;

		ofileout.write((char*)(&nodevalue),sizeof(nodevalue));
	}

	ofileout.close();

	printf("done!\n");

	return true;
}

bool RandomForest::BuildRandomForestFromUnzipfile(char* pfilename)
{
	ASSERT(pfilename != '\0');
	if (pfilename == NULL || pfilename == '\0')
		return false;

	ifstream ifile;
	ifile.open(pfilename, ios::binary | ios::in);
	if (!ifile.is_open())
		printf("cannot open the file:%s\n", pfilename);
	ASSERT(ifile.is_open());

	ifile.read((char*)(&m_TreeNumber), sizeof(m_TreeNumber));
	ifile.read((char*)(&m_TreeDepth),sizeof(m_TreeDepth));
	
	ASSERT(m_TreeNumber > 0);
	ASSERT(m_TreeDepth > 0);

	m_NodeNumber = (2 << (m_TreeDepth)) - 1;

	m_ppTree2 = new TreeNode*[m_TreeNumber];
	m_ppTree2[0] = new TreeNode[m_NodeNumber*m_TreeNumber];

	for (int itree = 1; itree < m_TreeNumber; itree++)
	{
		m_ppTree2[itree] = m_ppTree2[0] + m_NodeNumber * itree;
	}
	

	int idxnode;
	TreeNode* temptree;
	for (int itree = 0; itree < m_TreeNumber; itree++)
	{
		temptree = m_ppTree2[itree];
		for (idxnode = 0; idxnode < m_NodeNumber; idxnode++)
		{
			ifile.read((char*)temptree, sizeof(*temptree));
			temptree++;
		}
	}
	printf("my random forest structure is built\n");

	ifile.read((char*)(&m_ValueNumber),sizeof(m_ValueNumber));
	
	ASSERT(m_ValueNumber > 0);

	m_pValue2 = new NodeValue[m_ValueNumber];
	ASSERT(m_pValue2 != NULL);

	tmpNodeValueStorage tmpvalue;
	for (int ivalue = 0; ivalue < m_ValueNumber; ivalue++)
	{
		ifile.read((char*)(&tmpvalue), sizeof(tmpvalue));

		m_pValue2[ivalue].v[0].id = (unsigned char)(tmpvalue.ind & 0x0000001F);
		m_pValue2[ivalue].v[0].cnt = tmpvalue.v[0];

		tmpvalue.ind >>= 5;
		m_pValue2[ivalue].v[1].id = (unsigned char)(tmpvalue.ind & 0x0000001F);
		m_pValue2[ivalue].v[1].cnt = tmpvalue.v[1];

		tmpvalue.ind >>= 5;
		m_pValue2[ivalue].v[2].id = (unsigned char)(tmpvalue.ind & 0x0000001F);
		m_pValue2[ivalue].v[2].cnt = tmpvalue.v[2];

		tmpvalue.ind >>= 5;
		m_pValue2[ivalue].v[3].id = (unsigned char)(tmpvalue.ind & 0x0000001F);
		m_pValue2[ivalue].v[3].cnt = tmpvalue.v[3];

		tmpvalue.ind >>= 5;
		m_pValue2[ivalue].v[4].id = (unsigned char)(tmpvalue.ind & 0x0000001F);
		m_pValue2[ivalue].v[4].cnt = tmpvalue.v[4];
	}

	if (ifile.is_open())
		ifile.close();

	printf("my random forest reading is done!\n");

	return true;
}

bool RandomForest::cmpRandomForest(void)
{
	ASSERT(m_ppTree != NULL && m_ppTree2 != NULL);
	if (m_ppTree == NULL && m_ppTree2 == NULL)
		return false;

	printf("comparing structure:");
	
	if (m_TreeNumber != TREE_COUNT)
	{
		printf("tree number is not same!\n");
		return false;
	}

	if (m_NodeNumber != NODE_COUNT)
	{
		printf("node number is not same!\n");
		return false;
	}

	if (m_TreeDepth != TREE_DEPTH)
	{
		printf("tree depth is not same!\n");
		return false;
	}

	if (m_ValueNumber != VALUE_COUNT)
	{
		printf("value number is not same!\n");
		return false;
	}

	// match structure of random forest
	for (int itree = 0; itree < m_TreeNumber; itree++)
	{
		TreeNode* p1 = m_ppTree[itree];
		TreeNode* p2 = m_ppTree2[itree];

		for (int inode = 0; inode < m_NodeNumber; inode++)
		{
			if (p1[inode].c != p2[inode].c
				|| p1[inode].left != p2[inode].left
				|| p1[inode].right != p2[inode].right
				|| p1[inode].ux != p2[inode].ux
				|| p1[inode].uy != p2[inode].uy
				|| p1[inode].vx != p2[inode].vx
				|| p1[inode].vy != p2[inode].vy)
			{
				printf("tree node is not same\n");
				return false;
			}
		}
	}

	printf("structure of random forest is same\n");
	printf("Compare node value of random forest\n");

	for (int ivalue = 0; ivalue < m_ValueNumber; ivalue++)
	{
		if (m_pValue[ivalue] != m_pValue2[ivalue])
		{
			printf("node value is not same%d\n",ivalue);
			return false;
		}
	}
	printf("same\n");
	return true;
}

const TreeNode  * RandomForest::GetTree(int treeID) const
{	
	if (treeID < 0 || treeID >= m_TreeNumber)
	{
		return NULL;
	}
	return m_ppTree2[treeID];
}
const TreeNode  * RandomForest::GetNode(int treeID, int nodeID) const
{
	if(treeID < 0 || treeID >= m_TreeNumber
		|| nodeID < 0 || nodeID >= m_NodeNumber)
	{
		return NULL;
	}

	return m_ppTree2[treeID] + nodeID;
}
const NodeValue * RandomForest::GetValue(int valueID) const
{
	if (valueID < 0 || valueID >= m_ValueNumber)
		return NULL;
	
	return m_pValue2 + valueID;
}