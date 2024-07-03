
#include <stdio.h>
#include "console.h"


int main() {
    
    // BOOL allocret;

    // // Alloc console 
    // allocret = AllocConsole();
    // if (!allocret) {
    //     return 1;
    // }

    // Get handle
    HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE); 
    HANDLE errHandle = GetStdHandle(STD_ERROR_HANDLE); 
    HANDLE inHandle = GetStdHandle(STD_INPUT_HANDLE);
    if (outHandle == INVALID_HANDLE_VALUE || 
        errHandle == INVALID_HANDLE_VALUE || 
        inHandle == INVALID_HANDLE_VALUE) 
    {
        return 2;
    }

    // Write to console 
    BOOL writeret;
    DWORD charsWritten;
    writeret = WriteConsoleA(outHandle, "Hello World!\n", 14, &charsWritten, NULL);
    if (writeret == 0 || charsWritten != 14) 
    {
        return 3;
    }

    // 
    int numRead = 0; 
    DWORD BuffLength = 16;
    INPUT_RECORD lpBuffer[16];
    DWORD inputsRead;

    char strbuff[64];

    while (1) {
        // PeekConsoleInputA(inHandle, lpBuffer, BuffLength, &inputsRead);
        ReadConsoleInput(inHandle, lpBuffer, BuffLength, &inputsRead);
        

        for (int i = 0; i < inputsRead; i++) {

            switch (lpBuffer[i].EventType)
            {
            case FOCUS_EVENT:
                snprintf(strbuff, 64, "%d: FOCUS_EVENT\n", numRead + i);
                WriteConsoleA(outHandle, strbuff, strlen(strbuff), &charsWritten, NULL);
                break;
            case KEY_EVENT: 
                snprintf(strbuff, 64, "%d: KEY_EVENT\n", numRead + i);
                WriteConsoleA(outHandle, strbuff, strlen(strbuff), &charsWritten, NULL);

                KEY_EVENT_RECORD kreq = lpBuffer[i].Event.KeyEvent;
                snprintf(strbuff, 64, "kdown: %d | char: %d | kcode: %d | scode: %d\n", 
                        kreq.bKeyDown, kreq.uChar.AsciiChar, kreq.wVirtualKeyCode, kreq.wVirtualScanCode);
                WriteConsoleA(outHandle, strbuff, strlen(strbuff), &charsWritten, NULL);
                break;

            case MENU_EVENT: 
                snprintf(strbuff, 64, "%d: MENU_EVENT\n", numRead + i);
                WriteConsoleA(outHandle, strbuff, strlen(strbuff), &charsWritten, NULL);
                break;

            case MOUSE_EVENT:
                snprintf(strbuff, 64, "%d: MOUSE_EVENT\n", numRead + i);
                WriteConsoleA(outHandle, strbuff, strlen(strbuff), &charsWritten, NULL);
                break;

            case WINDOW_BUFFER_SIZE_EVENT: 
                snprintf(strbuff, 64, "%d: WINDOW_BUFFER_SIZE_EVENT\n", numRead + i);
                WriteConsoleA(outHandle, strbuff, strlen(strbuff), &charsWritten, NULL);
                break;
            default:
                break;
            }
            
        }

        numRead += inputsRead;

        if (numRead > 100) {
            snprintf(strbuff, 64, "%d inputs recieved.\n", numRead);
            WriteConsoleA(outHandle, strbuff, strlen(strbuff), &charsWritten, NULL);
            break;
        }

    }

    return 0;
}

/*
 * 
 * Functions to look at: 
 * 
 * FillConsoleOutputAttribute()
 * FillConsoleOutputCharacter()
 * FreeConsole()
 * GetConsoleAlias()
 * GenerateConsoleCtrlEvent()
 * GetConsoleDisplayMode()
 * 
*/