/*
          _____                              
     _ __|___ /_      ___ __ ___             
    | '_ \ |_ \ \ /\ / / '_ ` _ \   Powerfull
    | |_) |__) \ V  V /| | | | | |  Perfection 
    | .__/____/ \_/\_/ |_| |_| |_|  Pretty   
    |_| easily customizable windowmanager    

    Copyright (C) 2022  Pr0G4m1ngXD

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unordered_map>

#define XK_MISCELLANY 1
#define XK_LATIN1 2
#include <X11/keysymdef.h>

#include "printables/printables.h"
#include "define.h"
#include "readconf.h"
#include "logging.h"

void announcedOpen(std::string name);
void closeWindow(Window window);
void toggleFullscreen(Window window);
void createNotify(XCreateWindowEvent &event);
void reconfigureRequest(XConfigureRequestEvent& event);
void mapRequest(Window window);
void mapWindow(Window window, bool beforeInit);
void unmapWindow(XUnmapEvent& event);

int onXError(Display *display, XErrorEvent *event);
int returnHigher(int a, int b);

Display* pointerToDisplay;
Window rootWindow;
XWindowAttributes windowAttributes;
XButtonEvent xEventButton;
XEvent xEvent;
std::unordered_map<Window, Window> windowMap;
char config[57];

int main(int argc, char *argv[]) {

    std::cout << logo << std::endl; // Print the logo
    std::cout << version << std::endl; // Print the version
    std::cout << disclaimer << std::endl; // Print the disclaimer

    log("Starting p3wm", 0); // Print a test message

    if (!(pointerToDisplay = XOpenDisplay(0x0))) {
        log("Could not open display (is X running?)", 3);
        exit(1);
    }

    // Get home directory
    const char* home = getenv("HOME");
    // Make a new const char* for the path to the config files and set it to the home directory + .p3wm/config.conf
    const char* confighome = "/.p3wm/config.conf";
    // Make a new char* that is the home (const char*) + the config (const char*)
    strcpy(config, home);
    strcat(config, confighome);


    // Test if config can be read .p3wm/config.conf in home directory
    if (readVarStr(config, "type", "") == "") {
        log("Could not read config file, creating new one", 1);
        system("cp /usr/share/p3wm/config.conf ~/.p3wm/config.conf");
    }

    rootWindow = DefaultRootWindow(pointerToDisplay); // Get the root window of the display

    // Check if nitrogen is installed
    if (system("which nitrogen > /dev/null") != 0) {
        log("Could not find nitrogen, so no wallpaper will be set", 1); // Nitrogen is not installed so wallpaper will be blank... just like your soul hehe
    } else {
        system("nitrogen --restore &"); // Restore the wallpaper
    }

    // Grab the keyboard and mouse inputs
    XGrabKey(pointerToDisplay, XKeysymToKeycode(pointerToDisplay, XStringToKeysym("F1")), Mod1Mask, rootWindow, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(pointerToDisplay, XKeysymToKeycode(pointerToDisplay, XStringToKeysym("F4")), Mod1Mask, rootWindow, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(pointerToDisplay, XKeysymToKeycode(pointerToDisplay, XStringToKeysym("F11")), Mod1Mask, rootWindow, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(pointerToDisplay, XKeysymToKeycode(pointerToDisplay, XStringToKeysym("x")), Mod1Mask, rootWindow, True, GrabModeAsync, GrabModeAsync);

    XGrabButton(pointerToDisplay, LEFTMOUSEBUTTON, Mod1Mask, rootWindow, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None); // Left mouse button
    XGrabButton(pointerToDisplay, RIGHTMOUSEBUTTON, Mod1Mask, rootWindow, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None); // Right mouse button

    // Select Inputs to get all events
    XSelectInput(pointerToDisplay, rootWindow, SubstructureRedirectMask | SubstructureNotifyMask);
    XSync(pointerToDisplay, False);

    XSetErrorHandler(onXError);
    // Frame exsisting windows
    XGrabServer(pointerToDisplay);
    Window returnedRoot, returnedParent;
    Window* topLevelWindows;
    unsigned int numberOfWindows;
    XQueryTree(pointerToDisplay, rootWindow, &returnedRoot, &returnedParent, &topLevelWindows, &numberOfWindows);
    for (int i = 0; i < numberOfWindows; i++) {
        mapWindow(topLevelWindows[i], true);
    }
    XFree(topLevelWindows);
    XUngrabServer(pointerToDisplay);
    

    // Loop through the events
    for (;;) {

        XNextEvent(pointerToDisplay, &xEvent); // Get the next event

        // Check the event type
        switch (xEvent.type) {
            case KeyPress: //note to self: return in a switch statement during a for loop is may and will exit the app
                break;
            case KeyRelease:
                if (xEvent.xkey.subwindow != None) {
                    log("Key: " + std::to_string(xEvent.xkey.keycode), 7);
                    switch (XKeycodeToKeysym(pointerToDisplay, xEvent.xkey.keycode, 0)) {
                        case XK_F1:  std::cout << help << std::endl;
                            break;
                        case XK_F4:  closeWindow(xEvent.xkey.subwindow);
                            break;
                        case XK_F11: toggleFullscreen(xEvent.xkey.subwindow);
                            break;
                        case XK_x:   announcedOpen("xterm");
                            break;
                        default:     log("Key grabbed but not recognized", 1);
                            break;
                    }
                }
                break;
            case ButtonPress:
                if (xEvent.xbutton.subwindow != None) {
                    XGrabPointer(pointerToDisplay, xEvent.xbutton.subwindow, True, PointerMotionMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
                    XGetWindowAttributes(pointerToDisplay, xEvent.xbutton.subwindow, &windowAttributes);
                    XRaiseWindow(pointerToDisplay, xEvent.xbutton.subwindow);
                    xEventButton = xEvent.xbutton;
                }
                break;
            case MotionNotify:
                int xdiff, ydiff;
                int innerPadding;
                innerPadding = readVarInt(config, "innerPadding", 0);
                while (XCheckTypedEvent(pointerToDisplay, MotionNotify, &xEvent)); // make pc go brrrr (idk why this works)
                xdiff = xEvent.xbutton.x_root - xEventButton.x_root; // difference in x coordinates from start of drag
                ydiff = xEvent.xbutton.y_root - xEventButton.y_root; // difference in y coordinates from start of drag
                XMoveResizeWindow(pointerToDisplay, xEvent.xmotion.window, // move the window
                    windowAttributes.x + (xEventButton.button == 1 ? xdiff : 0), //New X position
                    windowAttributes.y + (xEventButton.button == 1 ? ydiff : 0), //New Y position
                    returnHigher(1, windowAttributes.width + (xEventButton.button == 3 ? xdiff : 0)),   //New width
                    returnHigher(1, windowAttributes.height + (xEventButton.button == 3 ? ydiff : 0))); //New height
                XMoveResizeWindow(pointerToDisplay, xEvent.xmotion.subwindow, // move the window
                    // we dont want to move the inner window, so we move the frame but we need to rezise the inner window
                    innerPadding,
                    innerPadding,
                    returnHigher(1, windowAttributes.width + (xEventButton.button == 3 ? xdiff : 0) - (innerPadding * 2)),   //New width
                    returnHigher(1, windowAttributes.height + (xEventButton.button == 3 ? ydiff : 0) - (innerPadding * 2))); //New height
                break;
            case ButtonRelease:
                XUngrabPointer(pointerToDisplay, CurrentTime);
                break;
            case CreateNotify:
                createNotify(xEvent.xcreatewindow);
                break;
            case ConfigureRequest:
                reconfigureRequest(xEvent.xconfigurerequest);
                break;  
            case MapRequest:
                mapRequest(xEvent.xmaprequest.window);
                break;
            case UnmapNotify:
                unmapWindow(xEvent.xunmap);
                break;
            case DestroyNotify:
                break;
            default:
                break;
                
        }
    }
}

void announcedOpen(std::string name) {
    log("Opening " + name, 0);
    name += " &";
    system(name.c_str());
}

void closeWindow(Window window) {
    log("Closing window: " + std::to_string(window), 7);
    xEvent.type = ClientMessage;
    xEvent.xclient.window = window;
    xEvent.xclient.message_type = XInternAtom(pointerToDisplay, "WM_PROTOCOLS", False);
    xEvent.xclient.format = 32;
    xEvent.xclient.data.l[0] = XInternAtom(pointerToDisplay, "WM_DELETE_WINDOW", False);
    xEvent.xclient.data.l[1] = CurrentTime;
    XSendEvent(pointerToDisplay, xEvent.xclient.window, False, NoEventMask, &xEvent);
}

void toggleFullscreen(Window window) {
    log("Toggling fullscreen for window: " + std::to_string(window), 7);
    XEvent xEvent;
    // Doesnt Work Yet, I probably need to manually move and resize and map the window again
}

void createNotify(XCreateWindowEvent &event) {
    log("Creating window: " + std::to_string(event.window), 7);
}

void reconfigureRequest(XConfigureRequestEvent& event) {
    XWindowChanges newValues;

    // For now we dont need to modify the window so we can just copy the values
    newValues.x = event.x;
    newValues.y = event.y;
    newValues.width = event.width;
    newValues.height = event.height;
    newValues.border_width = event.border_width;
    newValues.sibling = event.above;
    newValues.stack_mode = event.detail;

    if (windowMap.count(event.window)) {
        const Window frame = windowMap[event.window];
        XConfigureWindow(pointerToDisplay, frame, event.value_mask, &newValues);
    }

    XConfigureWindow(pointerToDisplay, event.window, event.value_mask, &newValues); // Grant the request
    //LOG(INFO) << "Resize " << event.window << " to " << Size<int>(event.width, event.height);
    log("Resize " + std::to_string(event.window) + " to " + std::to_string(event.width) + "x" + std::to_string(event.height), 7);
}

void mapRequest(Window window) {
    log("Map request for window: " + std::to_string(window), 7);
    mapWindow(window, false); // Map the window
}

void mapWindow(Window window, bool beforeInit) {
    // This is deffinitly going to be fun, kappa
    XWindowAttributes windowAttributes;
    XGetWindowAttributes(pointerToDisplay, window, &windowAttributes); // Get the window attributes

    if (beforeInit) {
        if (windowAttributes.override_redirect || windowAttributes.map_state != IsViewable) {
            log("Window already mapped", 7);
            return;
        }
    }
    // Create the frame
    const Window frame = XCreateSimpleWindow(pointerToDisplay, rootWindow, // Parent window
        windowAttributes.x, windowAttributes.y, // Position
        windowAttributes.width, windowAttributes.height, // Size
        readVarInt(config, "borderWidth", 1), // Border width
        readVarUnLong(config, "borderColor", 0xff0000), // Border color
        readVarUnLong(config, "backdropColor", 0x0000ff)); // Backdrop color
    
    // Select events from the frame
    XSelectInput(pointerToDisplay, frame, SubstructureRedirectMask | SubstructureNotifyMask);

    // Add client to save set so if we crash (which will probably happen with the code i write) itll be kept alive
    XAddToSaveSet(pointerToDisplay, window);

    // Putting the window in the frame
    XReparentWindow(pointerToDisplay, window, frame, 0, 0);

    // FINALLY WE GET TO MAP IT
    XMapWindow(pointerToDisplay, frame);

    // Save frame handle
    windowMap[window] = frame; // i dont get what this does but the tutorial says it does

    // WE GET TO MAP IT AGAIN (this time with the window)
    XMapWindow(pointerToDisplay, window);
}

void unmapWindow(XUnmapEvent& event) {
    log("Unmap request for window: " + std::to_string(event.window), 7);
    if (!windowMap.count(event.window)) {
        log("Ignored unmapNotify from non-client window", 7);
        return;
    }
    if (event.event == rootWindow) {
        log("Ignored unmapNotify from root window", 7);
        return;
    }
    // Now reverse the steps we took in mapRequest()
    const Window frame = windowMap[event.window];
    XUnmapWindow(pointerToDisplay, frame);
    XReparentWindow(pointerToDisplay, event.window, rootWindow, 0, 0);
    XRemoveFromSaveSet(pointerToDisplay, event.window);
    XDestroyWindow(pointerToDisplay, frame);
    windowMap.erase(event.window);
}
int onXError(Display *display, XErrorEvent *error) { // This is really unsafe because of no error handling just logging but ig it works?
    log("X Error: " + std::to_string(error->error_code) + " " + std::to_string(error->request_code) + " " + std::to_string(error->minor_code), 2);
    return 0; // xlib shenanigans
}

int returnHigher(int a, int b) {
    //condition ? true : false
    return a > b ? a : b;
}