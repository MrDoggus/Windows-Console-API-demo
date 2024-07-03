#include <stdio.h>
#include <stdlib.h>
#include "console.h"

console_info_t* cinfo;

BOOL ctrlhandler(DWORD dwCtrlType);

int main() {
    
    // --- Setup console --- //

    cinfo = setup_console();
    if (!cinfo) {
        return 1;
    }

    cinfo->window_event = &window_event;
    cinfo->focus_event = &focus_event;

    int retval;
    retval = SetConsoleMode(cinfo->inHandle, ENABLE_PROCESSED_INPUT | /*ENABLE_MOUSE_INPUT |*/ ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS );
    if (!retval) return 2;
    retval = SetConsoleMode(cinfo->outHandle, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    if (!retval) return 2;

    // const int STR_BUFFLEN = 64;
    // char strbuff[STR_BUFFLEN];

    // set CTRL handler
    SetConsoleCtrlHandler(&ctrlhandler, 1);
    
    hide_cursor(cinfo);

    clear_display(cinfo);
    

    // --- Check for text to display from file --- //
    char fstrbuff[24];
    char* PSTR = fstrbuff;
    int PSTR_len = 0;

    HANDLE dvdfileH;
    dvdfileH = CreateFileA(".\\dvd.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (dvdfileH == INVALID_HANDLE_VALUE) 
    {
        PSTR = "Hello World!";
    }
    else 
    {
        if (! ReadFile(dvdfileH, PSTR, 24, NULL, NULL)) {
            PSTR = "Hello World!";
        }
    }
    CloseHandle(dvdfileH);
    PSTR_len = strlen(PSTR);

    // --- Display text --- //
    int x = 1;
    int y = 1;
    int x_vel = 2;
    int y_vel = 1;


    int z = rand()%7 + 1;

    while (1) {

        handle_events(cinfo, 0);
        
        set_cursor_pos(cinfo, x, y);
        set_foreground_color(cinfo, z%8);
        WriteConsoleA(cinfo->outHandle, PSTR, PSTR_len, NULL, NULL);
        // snprintf(strbuff, STR_BUFFLEN, "\x1b[%d;%dH\x1b[38;5;%dm%s", y, x, COLOR_MAP[z%8], PSTR);
        // WriteConsoleA(cinfo->outHandle, strbuff, strlen(strbuff), NULL, NULL);
        
        Sleep(200);

        x += x_vel;
        y += y_vel;

        // Check if hit left or right edge
        if (1 > x) {
            x_vel = -x_vel;
            x = 1 + x_vel;

            z = rand()%7 + 1;
        }
        else if (x > cinfo->c_width-PSTR_len+1)
        {
            x_vel = -x_vel;
            x = cinfo->c_width-PSTR_len+1 + x_vel;

            z = rand()%7 + 1;
        }

        // Check if hit top or bottom edge
        if (1 > y) {
            y_vel = -y_vel;
            y = 1 + y_vel;

            z = rand()%7 + 1;
        }
        else if (y > cinfo->c_height)
        {
            y_vel = -y_vel;
            y = cinfo->c_height-1 + y_vel;

            z = rand()%7 + 1;
        }

        clear_line(cinfo);

    }

    return 0;
}

BOOL ctrlhandler(DWORD dwCT)
{
    if (dwCT == CTRL_C_EVENT || dwCT == CTRL_BREAK_EVENT)
    {
        show_cursor(cinfo);
        ExitProcess(0);
    }
    else 
    {
        ExitProcess(0);
    }
}