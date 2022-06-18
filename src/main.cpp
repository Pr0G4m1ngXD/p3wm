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
void createNotify(XConfigureRequestEvent& event);
void mapRequest(Window window);

int returnHigher(int a, int b);

Display* pointerToDisplay;
Window rootWindow;
XWindowAttributes windowAttributes;
XButtonEvent xEventButton;
XEvent xEvent;

int main(int argc, char *argv[]) {

    std::cout << logo << std::endl; // Print the logo
    std::cout << version << std::endl; // Print the version
    std::cout << disclaimer << std::endl; // Print the disclaimer

    log("Starting p3wm", 0); // Print a test message

    // test the readvar overloading
    readVar("/home/link/.p3wm/config.conf", "type", "undefined");
    readVar("/home/link/.p3wm/config.conf", "version", 0);

    if (!(pointerToDisplay = XOpenDisplay(0x0))) {
        log("Could not open display (is X running?)", 3);
        exit(1);
    }

    // Get home directory
    const char* home = getenv("HOME");
    // Make a new const char* for the path to the config files and set it to the home directory + .p3wm/config.conf
    const char* confighome = "/.p3wm/config.conf";
    // Make a new char* that is the home (const char*) + the config (const char*)
    char config[57]; // 6 bytes for home + 32 bytes for username + 18 bytes for .p3wm/config.conf + 1 bytes for null terminator = 57 bytes total so we wont be wasting memory
    strcpy(config, home);
    strcat(config, confighome);


    // Test if config can be read .p3wm/config.conf in home directory
    if (readVar(config, "type", "") != 1) {
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


    // Loop through the events
    for (;;) {

        XNextEvent(pointerToDisplay, &xEvent); // Get the next event

        // Check the event type
        switch (xEvent.type) {
            case KeyPress:
                return 0;
                break;
            case KeyRelease:
                if (xEvent.xkey.subwindow != None) {
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
                while (XCheckTypedEvent(pointerToDisplay, MotionNotify, &xEvent)); // make pc go brrrr (idk why this works)
                xdiff = xEvent.xbutton.x_root - xEventButton.x_root; // difference in x coordinates from start of drag
                ydiff = xEvent.xbutton.y_root - xEventButton.y_root; // difference in y coordinates from start of drag
                XMoveResizeWindow(pointerToDisplay, xEvent.xmotion.window, // move the window
                    windowAttributes.x + (xEventButton.button == 1 ? xdiff : 0), //New X position
                    windowAttributes.y + (xEventButton.button == 1 ? ydiff : 0), //New Y position
                    returnHigher(1, windowAttributes.width + (xEventButton.button == 3 ? xdiff : 0)),   //New width
                    returnHigher(1, windowAttributes.height + (xEventButton.button == 3 ? ydiff : 0))); //New height
                break;
            case ButtonRelease:
                XUngrabPointer(pointerToDisplay, CurrentTime);
                break;
            case CreateNotify:
                break;
            case ConfigureRequest:
                break;  
            case MapRequest:
                break;
            case UnmapNotify:
                break;
            case DestroyNotify:
                break;
            default:
                log("Event not recognized", 1);
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
    xEvent.type = ClientMessage;
    xEvent.xclient.window = window;
    xEvent.xclient.message_type = XInternAtom(pointerToDisplay, "_NET_WM_STATE", False);
    xEvent.xclient.format = 32;
    xEvent.xclient.data.l[0] = XInternAtom(pointerToDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    xEvent.xclient.data.l[1] = XInternAtom(pointerToDisplay, "_NET_WM_STATE_ADD", False);
    xEvent.xclient.data.l[2] = XInternAtom(pointerToDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    xEvent.xclient.data.l[3] = 0;
    xEvent.xclient.data.l[4] = 0;
    XSendEvent(pointerToDisplay, xEvent.xclient.window, False, NoEventMask, &xEvent);
}

void createNotify(XConfigureRequestEvent& event) {
}

void mapRequest(Window window) {
}

int returnHigher(int a, int b) {
    //condition ? true : false
    return a > b ? a : b;
}