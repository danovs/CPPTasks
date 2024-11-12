#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Получаем дескрипторы стандартного ввода (pipes)
    HANDLE hRead = GetStdHandle(STD_INPUT_HANDLE);

    // Получаем данные из канала
    vector<int> data(9); // Определяем размер массива
    DWORD bytesRead;
    if (!ReadFile(hRead, data.data(), data.size() * sizeof(int), &bytesRead, NULL)) {
        cerr << "Error reading from pipe: " << GetLastError() << endl;
        return 1;
    }

    // Находим максимальное и минимальное значения в данных
    int minElement = data[0];
    int maxElement = data[0];
    for (int num : data) {
        if (num < minElement) {
            minElement = num;
        }
        if (num > maxElement) {
            maxElement = num;
        }
    }

    // Отправляем результаты обратно через стандартный вывод (также на канал)
    HANDLE hWrite = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD bytesWritten;
    int results[2] = { minElement, maxElement };
    if (!WriteFile(hWrite, results, sizeof(results), &bytesWritten, NULL)) {
        cerr << "Error writing to pipe: " << GetLastError() << endl;
        return 1;
    }

    return 0;
}