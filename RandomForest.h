#ifndef _RANDOMFOREST_
#define _RANDOMFOREST_
#include "fstream"

using namespace std;



// 决策树节点
struct TreeNode			//14字节
{
    int  left;			//4字节
    int  right;			//4字节
    signed char ux, uy;
    signed char vx, vy;
    signed short c;
};


// 叶子节点Value: 部位类别的概率
struct ClassDist
{
    unsigned char id;
    unsigned char cnt;
};


// 叶子节点Value, 5个最大概率最大的类别
struct NodeValue
{
    ClassDist v[5];
};


// 随机森林
class RandomForest
{
public:
    static const int TREE_COUNT = 3;						//@xu-li:3棵树
    static const int TREE_DEPTH = 18;						//@xu-li:18层?
    static const int NODE_COUNT = (1<<(TREE_DEPTH+1)) - 1;	//@xu-li:（TREE_DEPTH+1）是否需要加1?
    static const int VALUE_COUNT = 835114;
    static const int FOREST_DATA_SIZE = 0x01f2af4a;

    RandomForest();
    ~RandomForest();
  //  BOOL BuildForest(const  char * pszData, const int size, cl_context & context, cl_command_queue &commandQueue);
	bool BuildForest(const char * pszData, const int size);
	const TreeNode  * Tree(int treeID) const;
    const TreeNode  * Node(int treeID, int nodeID) const;
    const NodeValue * Value(int valueID) const;
//private:
    TreeNode * m_ppTree[3]; //[TREE_COUNT][NODE_COUNT];
    NodeValue * m_pValue; //[VALUE_COUNT];
	//cl_mem m_cl_tree;
	//cl_mem m_cl_value;
	//cl_context  m_context;
	//cl_command_queue m_commandQueue;

	int m_TreeNumber;
	int m_TreeDepth;
	int m_NodeNumber;
	int m_ValueNumber;
	TreeNode** m_ppTree2;
	NodeValue* m_pValue2;
	bool WriteForest(char* pwirtefilename);
	bool BuildRandomForestFromUnzipfile(char* pfilename);
};




#endif// _RANDOMFOREST_
