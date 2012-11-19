#ifndef UI_H_
#define UI_H_
#include <Windows.h>

INT_PTR CALLBACK NewdiskProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AuthProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam);

#endif