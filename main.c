#include <ApplicationServices/ApplicationServices.h>

#include "eventhandler.h"


int main(void) {  
	CFMachPortRef eventTap;  
	CGEventMask eventMask;  
	CFRunLoopSourceRef runLoopSource;  
	
	// Create an event tap. We are interested in mouse movements.  
    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp) | (1 << kCGEventFlagsChanged));
	
	eventTap = CGEventTapCreate(  kCGSessionEventTap, kCGHeadInsertEventTap,  0, eventMask, myCGEventCallback, NULL);  
	
	if (!eventTap) {  
		fprintf(stderr, "failed to create event tap\n");  
		exit(1);  
	} 
	
	// Create a run loop source.
	runLoopSource = CFMachPortCreateRunLoopSource(  kCFAllocatorDefault, eventTap, 0);  
	
	// Add to the current run loop.  
	CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,  kCFRunLoopCommonModes);  
	
	// Enable the event tap.  
	CGEventTapEnable(eventTap, true);  
	
	// Set it all running.  
	CFRunLoopRun();  
	
	// In a real program, one would have arranged for cleaning up.  
	exit(0); 
} 
