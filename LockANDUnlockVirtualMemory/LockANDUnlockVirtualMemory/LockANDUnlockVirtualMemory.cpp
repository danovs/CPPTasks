#include <iostream>
#include <Windows.h>

using namespace std;

void handleError(const string& message) {
    cout << message << " Error code: " << GetLastError() << endl;
    exit(EXIT_FAILURE);
}

int main() {
    const SIZE_T size = 4096;
    LPVOID vm = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);

    if (!vm) {
        handleError("Virtual allocation failed");
    }

    if (!VirtualLock(vm, size)) {
        handleError("Virtual lock failed");
    }

    if (!VirtualUnlock(vm, size)) {
        handleError("Virtual unlock failed");
    }

    if (!VirtualFree(vm, 0, MEM_RELEASE)) {
   
            handleError("Virtual free failed");
    }

    return 0;
}