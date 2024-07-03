#include <windows.h>
#include <stdio.h>

#ifndef __DCONSOLE__
#define __DCONSOLE__

typedef enum console_error {
    CONSOLE_SUCCESS             = 0,
    CONSOLE_ERR_NULL_CINFO      = 1,
    CONSOLE_ERR_WINDOW_EVENT    = 2,
    CONSOLE_ERR_FOCUS_EVENT     = 4,
    CONSOLE_ERR_KEY_EVENT       = 8,
    CONSOLE_ERR_OTHER_EVENT     = 16,
    CONSOLE_ERR_GENERIC         = 32,
} console_error_t;

typedef enum console_color {
    CONSOLE_COL_WHITE   = 0,
    CONSOLE_COL_RED     = 1,
    CONSOLE_COL_ORANGE  = 2,
    CONSOLE_COL_YELLOW  = 3,
    CONSOLE_COL_GREEN   = 4,
    CONSOLE_COL_CYAN    = 5,
    CONSOLE_COL_BLUE    = 6,
    CONSOLE_COL_MAGENTA = 7,
    CONSOLE_COL_BLACK   = 8,
} console_color_t;

#define COLOR_MAP_LEN 9
extern const int COLOR_MAP[COLOR_MAP_LEN];



typedef struct console_info {

    HANDLE outHandle; 
    HANDLE errHandle; 
    HANDLE inHandle;

    int c_width;
    int c_height;

    // int cursor_x;
    // int cursor_y;

    char isFocused;

    console_error_t (*window_event)(struct console_info*, PWINDOW_BUFFER_SIZE_RECORD);
    console_error_t (*focus_event)(struct console_info*, PFOCUS_EVENT_RECORD);
    console_error_t (*key_event)(struct console_info*, PKEY_EVENT_RECORD);
    console_error_t (*other_event)(struct console_info*, PINPUT_RECORD);

} console_info_t;

console_info_t* setup_console();

console_error_t handle_events(console_info_t* cinfo, char blocking);

console_error_t focus_event(console_info_t* cinfo, PFOCUS_EVENT_RECORD focus_event);

console_error_t window_event(console_info_t* cinfo, PWINDOW_BUFFER_SIZE_RECORD record);

console_error_t fetch_consolestate(console_info_t* cinfo);

console_error_t clear_display(console_info_t* cinfo);

console_error_t clear_line(console_info_t* cinfo);

#endif