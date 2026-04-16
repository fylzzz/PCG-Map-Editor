#ifndef SAVE_DIALOG_H
#define SAVE_DIALOG_H

// Returns 1 on success, 0 on cancel
int ShowSaveDialog(char* outPath, int maxLen, const char* defaultName);

#endif