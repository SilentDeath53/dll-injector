#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

using namespace std;

bool injectDll(DWORD processId, const char* dllPath) {
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (processHandle == NULL) {
        cerr << "Failed to open process." << endl;
        return false;
    }

    LPVOID dllPathAddress = VirtualAllocEx(processHandle, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (dllPathAddress == NULL) {
        cerr << "Failed to allocate memory in target process." << endl;
        CloseHandle(processHandle);
        return false;
    }

    if (!WriteProcessMemory(processHandle, dllPathAddress, dllPath, strlen(dllPath) + 1, NULL)) {
        cerr << "Failed to write DLL path into target process." << endl;
        VirtualFreeEx(processHandle, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(processHandle);
        return false;
    }

    LPVOID loadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    if (loadLibraryAddress == NULL) {
        cerr << "Failed to get address of LoadLibrary function." << endl;
        VirtualFreeEx(processHandle, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(processHandle);
        return false;
    }

    HANDLE remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddress, dllPathAddress, 0, NULL);
    if (remoteThread == NULL) {
        cerr << "Failed to create remote thread in target process." << endl;
        VirtualFreeEx(processHandle, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(processHandle);
        return false;
    }

    WaitForSingleObject(remoteThread, INFINITE);

    VirtualFreeEx(processHandle, dllPathAddress, 0, MEM_RELEASE);
    CloseHandle(remoteThread);
    CloseHandle(processHandle);

    return true;
}

int main() {
    DWORD processId;
    cout << "Enter process ID: ";
    cin >> processId;

    char dllPath[MAX_PATH];
    cout << "Enter DLL path: ";
    cin >> dllPath;

    if (injectDll(processId, dllPath)) {
        cout << "DLL injected successfully." << endl;
    } else {
        cout << "Failed to inject DLL." << endl;
    }

    return 0;
}
