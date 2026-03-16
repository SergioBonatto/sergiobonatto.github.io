#ifndef TOOLS_PACKER_H
#define TOOLS_PACKER_H

#define MAX_POSTS	256
#define MAX_PATH	512
#define MAX_SLUG	128
#define MAX_TITLE	256
#define MAX_DATE	32
#define MAX_DESCRIPTION	512

struct post {
	char path[MAX_PATH];
	char slug[MAX_SLUG];
	char title[MAX_TITLE];
	char date[MAX_DATE];
	char description[MAX_DESCRIPTION];
};

#endif
