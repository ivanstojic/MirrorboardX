/*
 *  statemachine.c
 *  MirrorboardX
 *
 *  Created by Ivan Stojić on 3/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mappings.h"
#include "statemachine.h"

unsigned char outsideMirror = 1;
unsigned char outside[256];
unsigned char inside[256];

unsigned long mirrorStartTime = -1;
unsigned long mirrorCount = 0;



void mark(unsigned char map[256], int keycode) {
#ifdef DEBUG
	printf(">> Marking %d in %p\n", keycode, map);
#endif
	
	if (!map[keycode]) {
		map[keycode] = 1;
		
	} else {
		printf("Warning: attempt to mark already marked value %d!\n", keycode);
	}
}


void unmark(unsigned char map[256], int keycode) {
#ifdef DEBUG
	printf(">> Unmarking %d in %p\n", keycode, map);
#endif
	
	if (map[keycode]) {
		map[keycode] = 0;
		
	} else {
		printf("Warning: attempt to unmark already unmarked value %d!\n", keycode);
	}
}



void goInside(CGEventRef event) {
#ifdef DEBUG
	printf(">> Going inside\n");
#endif
	
	if (outsideMirror) {
		outsideMirror = 0;
		//mirrorStartTime = evt.time.tv_sec * 1000000 + evt.time.tv_usec;
		mirrorCount = 0;
		
	} else {
		printf("Warning: already inside the mirror!\n");
	}
}


void goOutside() {
#ifdef DEBUG
	printf(">> Going outside\n");
#endif
	
	if (!outsideMirror) {
		outsideMirror = 1;
		
	} else {
		printf("Warning: already outside the mirror!\n");
	}
}


unsigned char burst(CGEventRef event) {
	unsigned long evttime = 123; //evt.time.tv_sec * 1000000 + evt.time.tv_usec;
	
	if (mirrorCount == 0 && (evttime - mirrorStartTime) < MIRROR_SPACE_BURST_LIMIT) {
		return 1;
		
#ifdef DEBUG
		printf(">> BURST!");
#endif
		
	} else {
		return 0;
	}
}


CGEventRef swallowEvent(CGEventRef event) {
	// Nothing. That's why it's called swallowing :-p
#ifdef DEBUG
	//printf(">> Swallowing event\n");
#endif
	
	return NULL;
}


CGEventRef passEvent(CGEventRef event) {
#ifdef DEBUG
	printf(">> Passing event\n");
#endif
	
	return event;
}


CGEventRef remapEvent(CGEventRef event) {
	CGKeyCode keycode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
	
	int newcode = findMapping(keycode);
	
	if (newcode != keycode) {
		CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, newcode);
		
	} else {
		printf("Debug: rewrite requested, but no rewrite for this event. Passing as-is!\n");
	}
	
	return event;
}



unsigned char marked(unsigned char map[256], int keycode) {
	return map[keycode];
}


CGEventRef emitSpace(CGEventRef event) {
	// TODO: make this actually work...
/*	struct input_event evt;
	
	// Space pressed
	memset(&evt, 0, sizeof(evt));
	gettimeofday(&evt.time, NULL);
	
	evt.type = EV_KEY;
	evt.code = KEY_SPACE;
	evt.value = 1;
	passEvent(evt);
	
	evt.type = EV_SYN;
	evt.code = SYN_REPORT;
	evt.value = 0;
	passEvent(evt);
	
	// Space released
	memset(&evt, 0, sizeof(evt));
	gettimeofday(&evt.time, NULL);
	
	evt.type = EV_KEY;
	evt.code = KEY_SPACE;
	evt.value = 0;
	passEvent(evt);
	
	evt.type = EV_SYN;
	evt.code = SYN_REPORT;
	evt.value = 0;
	passEvent(evt);  */
	
	return event;
}



CGEventRef processEvent(CGEventType type,  CGEventRef event) {
	CGKeyCode keycode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
	CGKeyCode autorepeat = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat);		
	
	if (outsideMirror) {
		// Outside mirror...
		
		if (type == kCGEventKeyDown) {
			if (autorepeat) {
				if (marked(outside, keycode)) {
					return passEvent(event);
				
				} else {
					return remapEvent(event);
				}
				
			} else {
				if (keycode == 49) { // space
					goInside(event);
					return swallowEvent(event);
					
				} else {
					mark(outside, keycode);
					return passEvent(event);
				}				
			}
			
			
		} else if (type == kCGEventKeyUp) {
			if (marked(outside, keycode)) {
				unmark(outside, keycode);
				return passEvent(event);
				
			} else {
				unmark(inside, keycode);
				return remapEvent(event);
			}
			
		}
				
	} else {
		// Inside mirror
		
		if (type == kCGEventKeyDown) {
			if (autorepeat) {
				if (keycode == 49) { // space
					return swallowEvent(event);
					
				} else {
					if (marked(inside, keycode)) {
						return remapEvent(event);
						
					} else {
						return passEvent(event);
					}
				}
				
			} else {
				mark(inside, keycode);
				mirrorCount++;
				return remapEvent(event);
			}
			
		} else if (type == kCGEventKeyUp) {
			if (keycode == 49) { // space
				goOutside();
				if (burst(event)) {
					return emitSpace(event);
					
				} else {
					return swallowEvent(event);
				}
				
			} else {
				if (marked(inside, keycode)) {
					unmark(inside, keycode);
					return remapEvent(event);
					
				} else {
					return passEvent(event);
				}
			}
			
		}
		
	}
	
	printf("Leaking events?!\n");
	return event;
}
