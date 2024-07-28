#include "gui-helper.h"
#include "Windows.h"

int ShowMessageBox(const char* title, const char* message)
{
  return MessageBox(NULL, title, message, MB_OK | MB_ICON_ERROR)
}

int SelectionDialog(const char* title, int count, const char** list, int selection)
{
  #error TODO
}