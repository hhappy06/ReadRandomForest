#include "stdafx.h"
#include "BodyPartClassifier.h"

using namespace std;

int main()
{
	BodyPartClassifier mbd;
	printf("building random forest...\n");
	mbd.BuildForestFromResource();
	printf("building random success!");

	char* poutfile = "rf.bin";
	mbd.WriteForest(poutfile);
	char ch = getchar();
}