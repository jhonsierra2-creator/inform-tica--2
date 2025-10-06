#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // std::min, std::count
using namespace std;

int main() {
    string nombreEntrada, nombreSalida;
    size_t n; // tamaño del bloque (semilla)

    cout << "Ingrese el valor de n (tamaño del bloque, entero > 0): ";
    cin >> n;

    if(n == 0) {
        cout<< "el valor ingresado es incorrecto";
        cout<<"por favor buelva a ingresar el numero";
        cin>>n;
        return 1;
    }

    cout << "Nombre del archivo de entrada: ";
    cin >> nombreEntrada;
    cout << "Nombre del archivo de salida: ";
    cin >> nombreSalida;

    // Intentar abrir el archivo de entrada en modo binario (para leer bytes exactamente)
    ifstream fin(nombreEntrada, ios::binary);
    if (!fin.is_open()) {
        cout << "El archivo '" << nombreEntrada << "' no existe.\n";
        cout << "¿Deseas crearlo vacío? (s/n): ";
        char opcion;
        cin >> opcion;
        if (opcion == 's' || opcion == 'S') {
            // crearlo vacío y volver a abrir para lectura (será vacío)
            ofstream crear(nombreEntrada, ios::binary);
            if (!crear.is_open()) {
                cerr << "Error al crear el archivo.\n";
                return 1;
            }
            crear.close();
            // reabrir para lectura (será un ifstream vacío, no falla)
            fin.open(nombreEntrada, ios::binary);
            if (!fin.is_open()) {
                cerr << "Error al abrir el archivo recién creado.\n";
                return 1;
            }
            cout << "Archivo '" << nombreEntrada << "' creado correctamente (vacío).\n";
        } else {
            cout << "No se puede continuar sin archivo de entrada.\n";
            return 1;
        }
    } else {
        cout << "Archivo '" << nombreEntrada << "' encontrado correctamente.\n";
    }
}
