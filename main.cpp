#include<bits/stdc++.h>
#include<Windows.h>
#include <Psapi.h>
using namespace std;

DWORD GetBaseAddress(const HANDLE hProcess) {
    if (hProcess == NULL)
        return NULL; 

    HMODULE lphModule[1024]; 
    DWORD lpcbNeeded(NULL); 

    if (!EnumProcessModules(hProcess, lphModule, sizeof(lphModule), &lpcbNeeded))
        return NULL; 

    TCHAR szModName[MAX_PATH];
    if (!GetModuleFileNameEx(hProcess, lphModule[0], szModName, sizeof(szModName) / sizeof(TCHAR)))
        return NULL; 

    return (DWORD)lphModule[0];
}

int main()
{
    while (1) {
        string str, msg;
        int count = 0, delay;

        cout << "도배할 채팅방 이름 : ";
        Sleep(100);
        getline(cin, str);
        cout << "\n도배할 메시지 : ";
        getline(cin, msg);
        cout << "\n도배 횟수 : ";
        cin >> count;
        cout << "\n딜레이 : ";
        cin >> delay;

        HWND hd01 = FindWindowA(NULL, str.c_str());
        if (hd01 == NULL) {
            cout << "error!";
            return 0;
        }
        HWND hd02 = FindWindowExA(hd01, 0, "RICHEDIT50W", "");
        SetForegroundWindow(hd02);

        DWORD procID;
        GetWindowThreadProcessId(hd01, &procID);
        HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
        byte value[] = { 0x90, 0x90, 0x90 };
        DWORD baseAddress = GetBaseAddress(handle);
        if (WriteProcessMemory(handle, (PBYTE*)(baseAddress + 0xA9F399), &value, sizeof(value), 0))
            cout << "success";
        else
            return  0;

        for (int i = 0; i < count; i++) {
            SendMessageA(hd02, 0x000c, 0, (LPARAM)msg.c_str());
            PostMessage(hd02, 0x0100, 0x9, 0xF0001);
            PostMessage(hd02, 0x0101, 0x9, 0xF0001);
            PostMessage(hd02, 0x0100, 0xD, 0x1C001);
            PostMessage(hd02, 0x0101, 0x9, 0xF0001);
            Sleep(delay);
        }
        cout << "\n계속 하시겠습니까? Y/N ";
        char answer;
        cin >> answer;
        cin.ignore();
        if (answer == 'N')
            break;
    }

    return 0;
}
