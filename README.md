# Windows Keylogger

This project implements a low-level keyboard hook using the Windows API to log keystrokes along with associated metadata such as timestamps and active window titles. It is developed in C++ and is intended for use on Windows-based systems.

## Overview

The keylogger records:

- Every keypress detected globally on the system.
- The title of the currently active window at the time of each keypress.
- Timestamps associated with changes in focus and input activity.

The log is written to a local file named `keylog.txt`.

## Features

- Global keystroke monitoring via `SetWindowsHookEx`.
- Logging of contextual window titles using `GetForegroundWindow` and `GetWindowTextA`.
- Timestamped entries formatted via the C standard library's time functions.
- Special handling of keys such as ENTER, TAB, BACKSPACE, and modifier keys.
- Detection and translation of typed characters respecting Shift and Caps Lock states.
- Silent execution through console window hiding using `ShowWindow`.

## Technical Details

- Language: C++
- Platform: Windows
- API Dependencies: WinAPI (`Windows.h`, `User32.lib`)
- Output: `keylog.txt` (appended log file in working directory)

## Execution

Upon launching the compiled executable:

- The console window will automatically be hidden.
- The program will begin monitoring keyboard input system-wide.
- Logged data will be appended to `keylog.txt` located in the same directory as the executable.
- To stop the keylogger, terminate the process manually.

## Notes

- All timestamps are logged in local system time.
- The keylogger operates in the background and does not require user interaction once started.
