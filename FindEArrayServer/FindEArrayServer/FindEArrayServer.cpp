#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

void CreateChildProcess(HANDLE hRead, HANDLE hWrite, const vector<int>& data) {
    STARTUPINFO si = { 0 };
    si.cb = sizeof(STARTUPINFO);
    si.hStdInput = hRead;  // Соединение с каналом чтения
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;

    // Записываем данные в канал
    DWORD bytesWritten;
    if (!WriteFile(hWrite, data.data(), data.size() * sizeof(int), &bytesWritten, NULL)) {
        cerr << "Error writing to pipe: " << GetLastError() << endl;
        return;
    }

    // Закрываем дескриптор записи, потому что он дальше не нужен
    CloseHandle(hWrite);

    // Запускаем дочерний процесс
    wchar_t childPath[] = L"C:\\Users\\217562\\Desktop\\CPPTasks\\FindEArrayClient\\x64\\Debug\\FindEArrayClient.exe"; // Убедитесь, что путь правильный
    if (!CreateProcess(NULL, childPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        cerr << "CreateProcess failed. Error: " << GetLastError() << endl;
        return;
    }

    // Ждем завершения дочернего процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Чтение результата из канала
    int results[2];
    DWORD bytesRead;
    if (ReadFile(hRead, results, sizeof(results), &bytesRead, NULL) && bytesRead == sizeof(results)) {
        cout << "Minimum element received from child process: " << results[0] << endl;
        cout << "Maximum element received from child process: " << results[1] << endl;
    }
    else {
        cerr << "Error reading from pipe: " << GetLastError() << endl;
    }

    // Закрываем дескрипторы
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    // Создаем анонимные каналы
    HANDLE hRead, hWrite;
    if (!CreatePipe(&hRead, &hWrite, NULL, 0)) {
        cerr << "CreatePipe failed. Error: " << GetLastError() << endl;
        return 1;
    }

    vector<int> data = { 3, 1, 4, 1, 5, 9, 2, 6, 5 }; // Пример массива
    CreateChildProcess(hRead, hWrite, data);

    // Закрываем дескриптор чтения в родительском процессе
    CloseHandle(hRead);
    return 0;
}