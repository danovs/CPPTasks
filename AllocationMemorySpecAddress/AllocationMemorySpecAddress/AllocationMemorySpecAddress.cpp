
#include <iostream>
#include <Windows.h>

using namespace std;

void handleError(const string& message) {
    cout << message << " Error code: " << GetLastError() << endl;
    exit(EXIT_FAILURE);
}

int main()
{
    LPVOID lp;
    const int size = 1000;

    lp = VirtualAlloc(
        (LPVOID)0x00007ffC246C,
        size,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE
    );

    if (!lp) {
        handleError("Virtual allocation failed");
    }

    cout << "Virtual memory address: " << lp << endl;

    if (!VirtualFree(lp, 0, MEM_RELEASE)) {
        handleError("Memory release failed.");
    }
    return 0;
}
