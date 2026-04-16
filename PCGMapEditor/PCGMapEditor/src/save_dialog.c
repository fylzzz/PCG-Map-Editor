#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <string.h>
#include "save_dialog.h"

int ShowSaveDialog(char* outPath, int maxLen, const char* defaultName) {
    if (defaultName != NULL)
        strncpy(outPath, defaultName, maxLen - 1);

    OPENFILENAMEA ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Map Files (*.fyl)\0*.fyl\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = outPath;
    ofn.nMaxFile = maxLen;
    ofn.lpstrDefExt = "fyl";
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

    return GetSaveFileNameA(&ofn) ? 1 : 0;
}