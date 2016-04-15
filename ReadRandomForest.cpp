#include "stdafx.h"
#include "BodyPartClassifier.h"

using namespace std;

int main()
{
	BodyPartClassifier mbd;
	printf("building random forest...");
	mbd.BuildForestFromResource();
	printf("building random success!");
}