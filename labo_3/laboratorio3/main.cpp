#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // std::min, std::count
#include <iostream>
using namespace std;

//  MÉTODO 2
string metodo2(const string &bits, size_t n) {
    // Dividir en bloques
    vector<string> bloques;
    for (size_t i = 0; i < bits.size(); i += n) {
        bloques.push_back(bits.substr(i, n));
    }

    // Preparar resultado
    string resultado;
    resultado.reserve(bits.size()); // optimiza concatenación

    // Rotar cada bloque manualmente
    for (string bloque : bloques) {
        if (bloque.size() > 1) {
            char primero = bloque[0];
            for (size_t j = 0; j < bloque.size() - 1; ++j) {
                bloque[j] = bloque[j + 1];
            }
            bloque[bloque.size() - 1] = primero;
        }
        resultado += bloque;
    }

    return resultado;
}


int main() {
    int opcion;
    string nombreEntrada, nombreSalida;
    size_t n; // tamaño del bloque (semilla)

    cout << "Ingrese el valor de n: ";
    cin >> n;
    if (n == 0) {
        cout << "Valor de n inválido. Debe ser un entero mayor que 0.\n";
        return 1;
    }

    cout << "Ingrese el método de encriptación que quiere usar (1 o 2): ";
    cin >> opcion;
    cout << "Ingrese el nombre del archivo de entrada: ";
    cin >> nombreEntrada;
    cout << "Ingrese el nombre del archivo de salida: ";
    cin >> nombreSalida;

    // Intentar abrir el archivo de entrada
    ifstream fin(nombreEntrada, ios::binary);
    if (!fin.is_open()) {
        cout << "El archivo '" << nombreEntrada << "' no existe.\n";
        cout << "¿Deseas crearlo vacío? (s/n): ";
        char crear_op;
        cin >> crear_op;
        if (crear_op == 's' || crear_op == 'S') {
            ofstream crear(nombreEntrada, ios::binary);
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

    // Leer archivo y generar cadena binaria
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
    cout << "Total de bits generados: " << bitsTotales.size() << endl;

    if (bitsTotales.empty()) {
        cout << "No hay datos en el archivo de entrada.\n";
        return 0;
    }

    string resultado;

    //SELECCIÓN DE MÉTODO
     // MÉTODO 1
    if (opcion == 1) {

        vector<string> bloques;
        for (size_t i = 0; i < bitsTotales.size(); i += n)
            bloques.push_back(bitsTotales.substr(i, n));

        vector<string> bloquesCodificados;
        for (size_t i = 0; i < bloques.size(); ++i) {
            string bloque = bloques[i];
            string nuevo = bloque;

            if (i == 0) {
                for (char &b : nuevo) b = (b == '0') ? '1' : '0';
            } else {
                int count1 = static_cast<int>(count(bloques[i - 1].begin(), bloques[i - 1].end(), '1'));
                int count0 = static_cast<int>(bloques[i - 1].size()) - count1;

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

        for (const auto &b : bloquesCodificados)
            resultado += b;
    }
      // MÉTODO 2
    else if (opcion == 2) {

        resultado = metodo2(bitsTotales, n);
    }
    else {
        cout << "Opción inválida. Debe ser 1 o 2.\n";
        return 1;
    }

    // Guardar archivo de salida
    ofstream fout(nombreSalida, ios::binary);
    if (!fout.is_open()) {
        cout << "Error al crear el archivo de salida.\n";
        return 1;
    }
    fout << resultado;
    fout.close();

    cout << "Archivo codificado correctamente en '" << nombreSalida << "'.\n";
    cout << "Bits escritos: " << resultado.size() << endl;

    return 0;
}
