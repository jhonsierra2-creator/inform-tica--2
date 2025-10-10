#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/*
 * MÉTODO 2 - DECODIFICAR
 * Cada bloque de n bits fue rotado una posición a la izquierda.
 * Para revertirlo, rotamos una posición a la derecha.
 */
string decodificarMetodo2(const string &bits, size_t n) {
    string resultado;

    // Recorremos la cadena en bloques de tamaño n
    for (size_t i = 0; i < bits.size(); i += n) {
        string bloque = bits.substr(i, n);

        // Si el bloque tiene más de un bit, rotamos a la derecha
        if (bloque.size() > 1) {
            char ultimo = bloque.back();
            bloque.pop_back();
            bloque.insert(bloque.begin(), ultimo);
        }

        resultado += bloque;
    }

    return resultado;
}

int main() {
    size_t n;
    int metodo;
    string nombreEntrada, nombreSalida;

    cout << "=== DECODIFICADOR DE ARCHIVOS ===\n";
    cout << "Ingrese el valor de n (semilla): ";
    cin >> n;
    cout << "Ingrese el método usado para codificar (1 o 2): ";
    cin >> metodo;
    cout << "Ingrese el nombre del archivo codificado: ";
    cin >> nombreEntrada;
    cout << "Ingrese el nombre del archivo de salida: ";
    cin >> nombreSalida;

    // Abrir el archivo codificado
    ifstream entrada(nombreEntrada, ios::binary);
    if (!entrada.is_open()) {
        cout << "Error: no se pudo abrir el archivo de entrada.\n";
        cout << "¿Deseas crearlo vacío? (s/n): ";
        char crear_op;
        cin >> crear_op;
        if (crear_op == 's' || crear_op == 'S') {
            ofstream crear(nombreEntrada, ios::binary);
            crear.close();
            entrada.open(nombreEntrada, ios::binary);
            cout << "Archivo '" << nombreEntrada << "' creado correctamente (vacío).\n";
        } else {
            cout << "No se puede continuar sin archivo de entrada.\n";
            return 1;
        }
    }


    // Leer todos los bits del archivo (como texto)
    string bits((istreambuf_iterator<char>(entrada)), istreambuf_iterator<char>());
    entrada.close();

    if (bits.empty()) {
        cout << "El archivo está vacío.\n";
        return 0;
    }

    string resultado;

    // Solo implementamos el método 2 para claridad
    if (metodo == 2) {
        resultado = decodificarMetodo2(bits, n);
    } else {
        cout << "Solo se implementa el método 2 en este programa.\n";
        return 0;
    }

    // Guardar el resultado en el archivo de salida
    ofstream salida(nombreSalida, ios::binary);
    if (!salida.is_open()) {
        cout << "Error: no se pudo crear el archivo de salida.\n";
        return 1;
    }

    salida << resultado;
    salida.close();

    cout << "Archivo decodificado correctamente.\n";
    cout << "Se guardó en: " << nombreSalida << endl;

    return 0;
}
