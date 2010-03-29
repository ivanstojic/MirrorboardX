/*
 *  mappings.c
 *  MirrorboardX
 *
 *  Created by Ivan Stojić on 3/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mappings.h"


int REMAPPED_CODES[][2] = {
// Number row
{18, 29},
{19, 25},
{20, 28},
{21, 26},
{23, 22},

// First letter row
{12, 35},
{13, 31},
{14, 34},
{15, 32},
{17, 16},
 
 // Second letter row
 {0, 41},
 {1, 37},
 {2, 40},
 {3, 38},
 {5, 4},
 
 // Third letter row
 {6, 44},
 {7, 47},
 {8, 43},
 {9, 46},
 {11, 45},
 
 // Special keys
 {48, 51} // tab, backspace
 //{KEY_CAPSLOCK, KEY_ENTER},
};


int findMapping(int original) {
	int idx;
	
	for (idx = 0; idx < (sizeof(REMAPPED_CODES)/sizeof(int)/2); idx++) {
		if (REMAPPED_CODES[idx][0] == original) {
			return REMAPPED_CODES[idx][1];
		} else if (REMAPPED_CODES[idx][1] == original) {
			return REMAPPED_CODES[idx][0];
		}
	}
	
	printf("Could not find mapping for the requested key code %d, returning unmodified...\n", original);
	return original;
}

