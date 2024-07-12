#include <windows.h>
#include <stdio.h>

#ifndef __DCONSOLE__
#define __DCONSOLE__

typedef enum console_error {
    CONSOLE_SUCCESS             = 0x00,
    CONSOLE_ERR_NULL_CINFO      = 0x01,
    CONSOLE_ERR_WINDOW_EVENT    = 0x02,
    CONSOLE_ERR_FOCUS_EVENT     = 0x04,
    CONSOLE_ERR_KEY_EVENT       = 0x08,
    CONSOLE_ERR_OTHER_EVENT     = 0x10,
    CONSOLE_ERR_GENERIC         = 0x20,
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

/// @brief Allocates and initializes a console_info struct
/// @return Allocated console_info struct
console_info_t* setup_console();

/// @brief Handles events from the console input buffer 
/// @param cinfo Console info struct
/// @param blocking True if should wait for a input to handle
/// @return Error code
console_error_t handle_events(console_info_t* cinfo, char blocking);

/// @brief Basic handling of a focus event
/// @param cinfo Console info struct
/// @param focus_event focus event record
/// @return Error code
console_error_t focus_event(console_info_t* cinfo, PFOCUS_EVENT_RECORD focus_event);

/// @brief Basic handling of window buffer record
/// @param cinfo Console info struct
/// @param record window buffer size record
/// @return Error code
console_error_t window_event(console_info_t* cinfo, PWINDOW_BUFFER_SIZE_RECORD record);

/// @brief Clears all characters on the display
/// @param cinfo Console info struct
/// @return Error code
console_error_t clear_display(console_info_t* cinfo);

/// @brief Clears the line located at the cursor position
/// @param cinfo Console info struct
/// @return Error code
console_error_t clear_line(console_info_t* cinfo);

/// @brief Sets the position of the console cursor
/// @param cinfo Console info struct
/// @param x Horizontal position
/// @param y Vertical position
/// @return Error code
console_error_t set_cursor_pos(console_info_t* cinfo, short x, short y);

/// @brief Hides the cursor
/// @param cinfo Console info struct
/// @return Error code
console_error_t hide_cursor(console_info_t* cinfo);

/// @brief Shows the cursor
/// @param cinfo Console info struct
/// @return Error code
console_error_t show_cursor(console_info_t* cinfo);

/// @brief Resets color to default
/// @param cinfo Console info struct
/// @return Error code
console_error_t reset_color(console_info_t* cinfo);

/// @brief Sets the text color
/// @param cinfo Console info struct
/// @param color Specified color
/// @return Error code
console_error_t set_foreground_color(console_info_t* cinfo, console_color_t color);

/// @brief Sets the text background color
/// @param cinfo Console info struct
/// @param color Specified color
/// @return Error code
console_error_t set_background_color(console_info_t* cinfo, console_color_t color);

#endif