#pragma once

#include "pxCommonDef.h"
#include <vector>
using namespace std;

class CPxMapManager
{
public:
	CPxMapManager(void);
	virtual ~CPxMapManager(void);

public:
	EPxMapState map(const SPxMapNode *in_kpsPxMapnode);
	bool upmap(const SPxMapNode *in_kpsPxMapnode);
	EPxMapState GetMapState(const SPxMapNode *in_kpsPxMapnode);
	void GetLocalDiskList(vector <CString> &out_vstrDiskNameList );
};

extern CPxMapManager g_oPxMapManager;

