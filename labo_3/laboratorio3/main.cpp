#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    // --- Entrada de datos ---
    string nombreEntrada, nombreSalida;
    size_t n;

    cout << "Ingrese el valor de n: ";
    cin >> n;
    while (n == 0) {
        cout << "Valor inválido. Ingrese un número mayor que 0: ";
        cin >> n;
    }

    cout << "Nombre del archivo de entrada: ";
    cin >> nombreEntrada;
    cout << "Nombre del archivo de salida: ";
    cin >> nombreSalida;

    // --- Verificar archivo de entrada ---
    ifstream fin(nombreEntrada, ios::binary);
    if (!fin.is_open()) {
        cout << "El archivo '" << nombreEntrada << "' no existe.\n";
        cout << "¿Deseas crearlo vacío? (s/n): ";
        char opcion;
        cin >> opcion;
        if (opcion == 's' || opcion == 'S') {
            ofstream crear(nombreEntrada, ios::binary);
            if (!crear.is_open()) {
                cerr << "Error al crear el archivo.\n";
                return 1;
            }
            crear.close();
            fin.open(nombreEntrada, ios::binary);
            cout << "Archivo '" << nombreEntrada << "' creado correctamente (vacío).\n";
        } else {
            cout << "No se puede continuar sin archivo de entrada.\n";
            return 1;
        }
    } else {
        cout << "Archivo '" << nombreEntrada << "' encontrado correctamente.\n";
    }

    // --- Leer bytes y generar bits ---
    string bitsTotales;
    fin.seekg(0, ios::end);
    streampos tam = fin.tellg();
    fin.seekg(0, ios::beg);
    if (tam > 0) bitsTotales.reserve(static_cast<size_t>(tam) * 8);

    char c;
    while (fin.get(c)) {
        unsigned char uc = static_cast<unsigned char>(c);
        for (int i = 7; i >= 0; --i)
            bitsTotales += ((uc >> i) & 1) ? '1' : '0';
    }
    fin.close();

    cout << "Cadena binaria generada (primeros 64 bits): "
         << bitsTotales.substr(0, min((size_t)64, bitsTotales.size()))
         << (bitsTotales.size() > 64 ? "..." : "") << endl;

    if (bitsTotales.empty()) {
        cout << "Archivo vacío. No se generó salida.\n";
        return 0;
    }

    // --- Dividir en bloques ---
    vector<string> bloques;
    for (size_t i = 0; i < bitsTotales.size(); i += n)
        bloques.push_back(bitsTotales.substr(i, n));

    // --- Codificar bloques ---
    vector<string> bloquesCodificados;
    for (size_t i = 0; i < bloques.size(); ++i) {
        string nuevo = bloques[i];
        if (i == 0) {
            for (char &b : nuevo) b = (b == '0') ? '1' : '0';
        } else {
            int count1 = count(bloques[i - 1].begin(), bloques[i - 1].end(), '1');
            int count0 = bloques[i - 1].size() - count1;
            if (count1 == count0) {
                for (char &b : nuevo) b = (b == '0') ? '1' : '0';
            } else if (count0 > count1) {
                for (size_t j = 0; j + 1 < nuevo.size(); j += 2)
                    swap(nuevo[j], nuevo[j + 1]);
            } else {
                for (size_t j = 0; j + 2 < nuevo.size(); j += 3)
                    swap(nuevo[j], nuevo[j + 2]);
            }
        }
        bloquesCodificados.push_back(nuevo);
    }

    // --- Guardar resultado ---
    string resultado;
    for (const auto &b : bloquesCodificados) resultado += b;

    ofstream fout(nombreSalida, ios::binary);
    if (!fout.is_open()) {
        cerr << "Error al crear el archivo de salida.\n";
        return 1;
    }
    fout << resultado;
    fout.close();

    cout << "Archivo codificado correctamente en '" << nombreSalida << "'.\n";
    cout << "Bits escritos: " << resultado.size() << endl;

    return 0;

    // prueva de modificación en el codigo

}
