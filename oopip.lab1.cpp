#include <iostream>
#include <vector>
#include <set>
#include <limits>
#include <stdexcept>
#include <fstream> 

using namespace std;

class Matrix {
private:
    int rows;
    int cols;
    int** data;

public:
    
    Matrix(int r, int c) : rows(r), cols(c) {
        if (rows <= 0 || cols <= 0) {
            throw invalid_argument("Ошибка: количество строк и столбцов должно быть положительным.");
        }
        data = new int* [rows];
        for (int i = 0; i < rows; ++i) {
            data[i] = new int[cols]();
        }
    }

    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        data = new int* [rows];
        for (int i = 0; i < rows; ++i) {
            data[i] = new int[cols];
            for (int j = 0; j < cols; ++j) {
                data[i][j] = other.data[i][j]; 
            }
        }
    }

    
    ~Matrix() {
        for (int i = 0; i < rows; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }

    void fill() {
        cout << "Введите элементы матрицы " << rows << "x" << cols << ":\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                while (true) {
                    cout << "Элемент [" << i << "][" << j << "]: ";
                    if (cin >> data[i][j]) {
                        break; 
                    }
                    else {
                        cin.clear(); //  флаг ошибки
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очищает буфер ввода
                        cout << "Ошибка ввода. Пожалуйста, введите целое число.\n";
                    }
                }
            }
        }
    }

    void display() const {
        cout << "Матрица " << rows << "x" << cols << ":\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << data[i][j] << " ";
            }
            cout << endl;
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Ошибка: не удалось открыть файл для записи.");
        }

        file << "Матрица " << rows << "x" << cols << ":\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                file << data[i][j] << " ";
            }
            file << endl;
        }

        file.close();
        cout << "Матрица успешно сохранена в файл: " << filename << endl;
    }

    Matrix intersection(const Matrix& other) const {
        if (cols != other.cols) {
            throw invalid_argument("Матрицы должны иметь одинаковое количество столбцов для пересечения.");
        }

        set<int> setData;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                setData.insert(data[i][j]);
            }
        }

        vector<int> intersectionData;
        for (int i = 0; i < other.rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                if (setData.find(other.data[i][j]) != setData.end()) {
                    intersectionData.push_back(other.data[i][j]);
                }
            }
        }

        Matrix result(1, intersectionData.size());
        for (int j = 0; j < intersectionData.size(); ++j) {
            result.data[0][j] = intersectionData[j];
        }

        return result;
    }

    Matrix unionMatrix(const Matrix& other) const {
        set<int> setData;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                setData.insert(data[i][j]);
            }
        }
        for (int i = 0; i < other.rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                setData.insert(other.data[i][j]);
            }
        }

        Matrix result(1, setData.size());
        int index = 0;
        for (int value : setData) {
            result.data[0][index++] = value;
        }

        return result;
    }
  
};


int main() {
    int rows1, cols1, rows2, cols2;

    cout << "Введите размеры первой матрицы (строки, столбцы): ";
    while (!(cin >> rows1 >> cols1) || rows1 <= 0 || cols1 <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Неверный ввод. Пожалуйста, введите положительные числа для строк и столбцов: ";
    }
    Matrix mat1(rows1, cols1);
    mat1.fill();

    cout << "Введите размеры второй матрицы (строки, столбцы): ";
    while (!(cin >> rows2 >> cols2) || rows2 <= 0 || cols2 <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Неверный ввод. Пожалуйста, введите положительные числа для строк и столбцов: ";
    }
    Matrix mat2(rows2, cols2);
    mat2.fill();

    int choice;
    do {
        cout << "Меню:\n";
        cout << "1. Вывод первой матрицы\n";
        cout << "2. Вывод второй матрицы\n";
        cout << "3. Пересечение матриц\n";
        cout << "4. Объединение матриц\n";
        cout << "5. Сохранить первую матрицу в файл\n";
        cout << "6. Сохранить вторую матрицу в файл\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод. Пожалуйста, введите число: ";
        }

        switch (choice) {
        case 1:
            mat1.display();
            break;
        case 2:
            mat2.display();
            break;
        case 3: {
            try {
                Matrix intersectionMatrix = mat1.intersection(mat2);
                intersectionMatrix.display();
            }
            catch (const invalid_argument& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
            break;
        }
        case 4: {
            Matrix unionMatrix = mat1.unionMatrix(mat2);
            unionMatrix.display();
            break;
        }
        case 5: {
            string filename;
            cout << "Введите имя файла для сохранения первой матрицы: ";
            cin >> filename;
            try {
                mat1.saveToFile(filename);
            }
            catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 6: {
            string filename;
            cout << "Введите имя файла для сохранения второй матрицы: ";
            cin >> filename;
            try {
                mat2.saveToFile(filename);
            }
            catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 0:
            cout << "Выход из программы.\n";
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 0);

    return 0;
}