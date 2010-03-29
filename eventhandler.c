/*
 *  eventhandler.c
 *  MirrorboardX
 *
 *  Created by Ivan Stojić on 3/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include <ApplicationServices/ApplicationServices.h>

#include "statemachine.h"

CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type,  CGEventRef event, void *refcon) {
    /*if (type == kCGEventKeyDown) {
		// The incoming keycode.
		CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(
																   event, kCGKeyboardEventKeycode);
		
		CGKeyCode autorepeat = (CGKeyCode)CGEventGetIntegerValueField(
																	  event, kCGKeyboardEventAutorepeat);		
		
		printf("Key code is %d autorepeat %d\n", keycode, autorepeat);
		
	}
		
	
    if ((type == kCGEventKeyDown) || (type == kCGEventKeyUp)) {
		// The incoming keycode.
		CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(
																   event, kCGKeyboardEventKeycode);
		
		CGKeyCode autorepeat = (CGKeyCode)CGEventGetIntegerValueField(
																	  event, kCGKeyboardEventAutorepeat);
		
		
		// Swap 'a' (keycode=0) and 'z' (keycode=6).
		if (keycode == (CGKeyCode) 0)
			keycode = (CGKeyCode) 6;
		else if (keycode == (CGKeyCode) 6)
			keycode = (CGKeyCode) 0;
		else if (keycode == (CGKeyCode) 4)
			return NULL;
		
		// Set the modified keycode field in the event.
		CGEventSetIntegerValueField(
									event, kCGKeyboardEventKeycode, (int64_t)keycode);
		
	} else if (type == kCGEventFlagsChanged) {
		// Not sure what to do about modifiers yet... hold on and see what happens...
		
		 CGEventFlags flags = CGEventGetFlags(event);
		
		printf("Flags changed %x...\n", flags);
		
		if (flags & kCGEventFlagMaskAlphaShift) {
			printf("Caps Lock\n");
		}
		
		if (flags & kCGEventFlagMaskControl) {
			printf("Control\n");
		} 
		
		if (flags & kCGEventFlagMaskCommand) {
			printf("Command\n");
		} 
		
		if (flags & kCGEventFlagMaskAlternate) {
			printf("Alternate/option\n");
		}
	}
	
	// We must return the event for it to be useful. */

	return processEvent(type, event);
} 

