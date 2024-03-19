#include <Windows.h>
#include <tlhelp32.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t szClassName[] = L"KellerTeste"; // Notice the 'L' prefix for wide-character string
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szClassName;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(szClassName, L"Keller Teste", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONERROR); // Notice the 'L' prefix for wide-character string
        return 0;
    }

    // Create "Ativar" button
    HWND btnAtivar = CreateWindow(L"BUTTON", L"Ativar", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 50, 50, 100, 30, hwnd, (HMENU)1, hInstance, NULL);

    // Create "Desativar" button
    HWND btnDesativar = CreateWindow(L"BUTTON", L"Desativar", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 50, 100, 100, 30, hwnd, (HMENU)2, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}

void OpenNotepad()
{
    ShellExecute(NULL, L"open", L"notepad.exe", NULL, NULL, SW_SHOWNORMAL);
}

void CloseNotepad()
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(pe32);
        if (Process32First(hSnap, &pe32))
        {
            do
            {
                if (_wcsicmp(pe32.szExeFile, L"notepad.exe") == 0)
                {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                    if (hProcess != NULL)
                    {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                    }
                }
            } while (Process32Next(hSnap, &pe32));
        }
        CloseHandle(hSnap);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED) // Check if the button was clicked
        {
            if (LOWORD(wParam) == 1) // Check if the "Ativar" button was clicked
            {
                OpenNotepad();
            }
            else if (LOWORD(wParam) == 2) // Check if the "Desativar" button was clicked
            {
                CloseNotepad();
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
