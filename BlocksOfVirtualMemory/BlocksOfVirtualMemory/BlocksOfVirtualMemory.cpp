#include <iostream>
#include <Windows.h>

using namespace std;

void handleError(const string& message) {

    cout << message << " Error code: " << GetLastError() << endl;
    exit(EXIT_FAILURE);
}

int main()
{
    BYTE* a, * b;
    const int size = 1000;

    a = (BYTE*)VirtualAlloc(

        NULL,
        size,
        MEM_COMMIT,
        PAGE_READWRITE

    );

    if (!a) {
        handleError("Virtual allocation failed.");
    }

    b = (BYTE*)VirtualAlloc(

        NULL,
        size,
        MEM_COMMIT,
        PAGE_READWRITE

    );

    FillMemory(a, size, 'X');
    CopyMemory(b, a, size);

    cout << "b[10] = " << b[10] << endl;
    if (!VirtualFree(a, 0, MEM_RELEASE)) {

        handleError("Memory release failed.");
    }
    return 0;
}