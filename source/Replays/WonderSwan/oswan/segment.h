////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SEGMENT_H__
#define __SEGMENT_H__
#define	SB_COLOR	0x00000000
#define	SF_COLOR	0x00FFFFFF

DWORD sleep[] = {
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR
};

DWORD vertical[] = {
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR
};

DWORD horizontal[] = {
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR
};

DWORD circle1[] = {
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR
};
DWORD circle2[] = {
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR
};
DWORD circle3[] = {
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SF_COLOR,SF_COLOR,SF_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,
	SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR,SB_COLOR
};

#endif
