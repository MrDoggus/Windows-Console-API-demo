#include "console.h"

// https://learn.microsoft.com/en-us/windows/console/setconsolemode
// https://learn.microsoft.com/en-us/windows/console/setconsoledisplaymode
// https://learn.microsoft.com/en-us/windows/console/classic-vs-vt#exceptions-for-using-windows-console-apis
// https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences

const int COLOR_MAP[COLOR_MAP_LEN] = {
    15,     // WHITE
    9,      // RED
    208,    // ORANGE 
    11,     // YELLOW 
    10,     // GREEN
    14,     // CYAN
    12,     // BLUE
    13,     // MAGENTA
    0       // BLACK
};

console_info_t* setup_console()
{
    console_info_t* cinfo;

    // Allocate memory for struct

    HANDLE hHeap = GetProcessHeap();

    if (!hHeap) {
        return NULL;
    }

    cinfo = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(console_info_t));

    if (!cinfo) {
        return NULL;
    }


    // Get console handles

    cinfo->outHandle = GetStdHandle(STD_OUTPUT_HANDLE); 
    cinfo->errHandle = GetStdHandle(STD_ERROR_HANDLE); 
    cinfo->inHandle = GetStdHandle(STD_INPUT_HANDLE);

    if (cinfo->outHandle == INVALID_HANDLE_VALUE || 
        cinfo->errHandle == INVALID_HANDLE_VALUE || 
        cinfo->inHandle == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    // Set console state

    cinfo->isFocused = 1;

    CONSOLE_SCREEN_BUFFER_INFO buffinfo;
    BOOL retval = GetConsoleScreenBufferInfo(cinfo->outHandle, &buffinfo);
    if (!retval) return NULL;
    
    cinfo->c_height = buffinfo.dwSize.Y;
    cinfo->c_width = buffinfo.dwSize.X;

    

    return cinfo;
}

console_error_t handle_events(console_info_t* cinfo, char blocking)
{
    if (!cinfo) {
        return CONSOLE_ERR_NULL_CINFO;
    }

    console_error_t retval, event_error;

    const int LP_BUFFLEN = 16;
    INPUT_RECORD lpBuffer[LP_BUFFLEN];
    DWORD inputsRead;

    if (blocking) {
        ReadConsoleInput(cinfo->inHandle, lpBuffer, LP_BUFFLEN, &inputsRead);
    }
    else {
        DWORD numofevents;
        GetNumberOfConsoleInputEvents(cinfo->inHandle, &numofevents);

        if (numofevents == 0) {
            return CONSOLE_SUCCESS;
        }

        ReadConsoleInput(cinfo->inHandle, lpBuffer, LP_BUFFLEN, &inputsRead);
    }
    

        
    for (int i = 0; i < inputsRead; i++) {
        switch (lpBuffer[i].EventType)
        {
        case FOCUS_EVENT:
            if (!cinfo->focus_event) continue;

            event_error = cinfo->focus_event(cinfo, &lpBuffer[i].Event.FocusEvent);
            if (event_error) {
                retval |= CONSOLE_ERR_FOCUS_EVENT;
            }
            break;

        case WINDOW_BUFFER_SIZE_EVENT:
            if (!cinfo->window_event) continue;

            event_error = cinfo->window_event(cinfo, &lpBuffer[i].Event.WindowBufferSizeEvent);
            if (event_error) {
                retval |= CONSOLE_ERR_WINDOW_EVENT;
            }
            break;
        case KEY_EVENT: 
            if (!cinfo->key_event) continue;

            event_error = cinfo->key_event(cinfo, &lpBuffer[i].Event.KeyEvent);
            if (event_error) {
                retval |= CONSOLE_ERR_KEY_EVENT;
            }
            break;

        default: 
            if (!cinfo->other_event) continue;

            event_error = cinfo->other_event(cinfo, &lpBuffer[i]);
            if (event_error) {
                retval |= CONSOLE_ERR_OTHER_EVENT;
            }
            
            break;
        }
    }

    return retval;
}

console_error_t focus_event(console_info_t* cinfo, PFOCUS_EVENT_RECORD focus_event)
{
    if (focus_event->bSetFocus) {
        cinfo->isFocused = 1;
    }
    else {
        cinfo->isFocused = 0;
    }

    return CONSOLE_SUCCESS;
}

console_error_t window_event(console_info_t* cinfo, PWINDOW_BUFFER_SIZE_RECORD record)
{
    cinfo->c_width = record->dwSize.X;
    cinfo->c_height = record->dwSize.Y;

    return CONSOLE_SUCCESS;
}

console_error_t clear_display(console_info_t* cinfo)
{
    WriteConsoleA(cinfo->outHandle, "\x1b[2J\x1b[H", 8, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t clear_line(console_info_t* cinfo)
{
    WriteConsoleA(cinfo->outHandle, "\x1b[2K\x1b[G", 8, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t set_cursor_pos(console_info_t* cinfo, short x, short y)
{
    const int STR_BUFFLEN = 20;
    char strbuff[STR_BUFFLEN];

    snprintf(strbuff, STR_BUFFLEN, "\x1b[%d;%dH", y, x);
    WriteConsoleA(cinfo->outHandle, strbuff, strlen(strbuff), NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t hide_cursor(console_info_t* cinfo)
{
    WriteConsoleA(cinfo->outHandle, "\x1b[?25l", 7 , NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t show_cursor(console_info_t* cinfo)
{
    WriteConsoleA(cinfo->outHandle, "\x1b[?25h", 7 , NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t set_foreground_color(console_info_t* cinfo, console_color_t color)
{
    const int STR_BUFFLEN = 16;
    char strbuff[STR_BUFFLEN];

    snprintf(strbuff, STR_BUFFLEN, "\x1b[38;5;%dm", COLOR_MAP[color]);

    WriteConsoleA(cinfo->outHandle, strbuff, strlen(strbuff), NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t set_background_color(console_info_t* cinfo, console_color_t color)
{
    const int STR_BUFFLEN = 16;
    char strbuff[STR_BUFFLEN];

    snprintf(strbuff, STR_BUFFLEN, "\x1b[48;5;%dm", COLOR_MAP[color]);

    WriteConsoleA(cinfo->outHandle, strbuff, strlen(strbuff), NULL, NULL);

    return CONSOLE_SUCCESS;
}