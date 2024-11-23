#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <windows.h>
#include <dirent.h>

const long double h = pow(1, -5);
using namespace std;


// Estructura que representa cada dato con fecha y número de casos
struct Dato {
    string fecha;  // Fecha del dato
    int casos;     // Número de casos confirmados en esa fecha
};

void cambiarColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // se le permite cambiar el color al codigo
    SetConsoleTextAttribute(hConsole, color); // se le asigna el color a la consola

    // color azul = 1
    // color rojo = 4
    // color verde = 2
    // color purpura = 5
    // color predeterminado gris = 7
    
}



// Función para leer el archivo CSV y almacenar los datos en un vector
vector<Dato> leerCSV(const string& nombreArchivo) {
    vector<Dato> datos;  // Vector que almacena los datos leídos
    ifstream archivo(nombreArchivo);  // Abrimos el archivo CSV
    string linea;  // Variable para leer cada línea del archivo

    // Verificamos si el archivo se abrió correctamente
    if (!archivo.is_open()) {
        cambiarColor(4);
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return datos; // regresamos un vector vacio para que no se ejecute el resto del programa
    }

    // Leemos cada línea del archivo
    while (getline(archivo, linea)) {
        stringstream ss(linea);  // Creamos un stringstream para dividir la línea
        string fecha, casosStr;  // Variables para almacenar los campos

        // Separamos la fecha y el número de casos usando el delimitador ';'
        if (getline(ss, fecha, ';') && getline(ss, casosStr)) {
            try {
                Dato dato;  // Creamos un objeto de tipo Dato
                dato.fecha = fecha;  // Asignamos la fecha
                dato.casos = stoi(casosStr);  // Convertimos el número de casos de string a int
                datos.push_back(dato);  // Añadimos el dato al vector
            } catch (invalid_argument&) {
                cerr << "Error: valor no numérico en casos: " << casosStr << endl;  // Si no se puede convertir, mostramos un error
            }
        }
    }

    archivo.close();  // Cerramos el archivo
    return datos;  // Retornamos el vector con los datos leídos
}

// Función que calcula las diferencias centradas de primer orden
vector<int> diferenciasCentradasPrimerOrden(const vector<int>& casos, long double h) {
    vector<int> derivada(casos.size(), 0.0);  // Vector para almacenar las derivadas calculadas

    // Calculamos las diferencias centradas para cada punto, excluyendo los extremos
    for (size_t i = 1; i < casos.size() - 1; ++i) {
        // Fórmula de diferencias finitas centradas de primer orden
        derivada[i] = (casos[i + 1] - casos[i - 1]) / (2 * h);
    }

    return derivada;  // Retornamos el vector con las derivadas
}

// Función que calcula las diferencias adelantadas de primer orden
vector<int> diferenciasAdelantadasPrimerOrden(const vector<int>& casos, long double h) {
    vector<int> derivada(casos.size() - 1, 0.0);  // Vector para almacenar las derivadas calculadas

    // Calculamos las diferencias adelantadas para cada punto
    for (size_t i = 0; i < casos.size() - 1; ++i) {
        // Fórmula de diferencias finitas adelantadas de primer orden
        derivada[i] = (casos[i + 1] - casos[i]) / h;
    }

    return derivada;  // Retornamos el vector con las derivadas
}

// Función que calcula las diferencias atrasadas de primer orden
vector<int> diferenciasAtrasadasPrimerOrden(const vector<int>& casos, long double h) {
    vector<int> derivada(casos.size() - 1, 0.0);  // Vector para almacenar las derivadas calculadas

    // Calculamos las diferencias atrasadas para cada punto
    for (size_t i = 1; i < casos.size(); ++i) {
        // Fórmula de diferencias finitas atrasadas de primer orden
        derivada[i - 1] = (casos[i] - casos[i - 1]) / h;
    }

    return derivada;  // Retornamos el vector con las derivadas
}

// Función que calcula las diferencias centradas de segundo orden
vector<int> diferenciasCentradasSegundoOrden(const vector<int>& casos, long double h) {
    vector<int> derivada(casos.size(), 0.0);  // Vector para almacenar las derivadas calculadas

    // Calculamos las diferencias centradas para cada punto, excluyendo los extremos
    for (size_t i = 1; i < casos.size() - 1; ++i) {
        // Fórmula de diferencias finitas centradas de segundo orden
        derivada[i] = (casos[i + 1] - 2 * casos[i] + casos[i - 1]) / (h * h);
    }

    return derivada;  // Retornamos el vector con las derivadas
}

// Función que calcula las diferencias adelantadas de segundo orden
vector<int> diferenciasAdelantadasSegundoOrden(const vector<int>& casos,long double h) {
    vector<int> derivada(casos.size() - 2, 0.0);  // Vector para almacenar las derivadas calculadas

    // Calculamos las diferencias adelantadas para cada punto
    for (size_t i = 0; i < casos.size() - 2; ++i) {
        // Fórmula de diferencias finitas adelantadas de segundo orden
        derivada[i] = (-3 * casos[i] + 4 * casos[i + 1] - casos[i + 2]) / (2 * h);
    }

    return derivada;  // Retornamos el vector con las derivadas
}

// Función que calcula las diferencias atrasadas de segundo orden
vector<int> diferenciasAtrasadasSegundoOrden(const vector<int>& casos, long double h) {
    vector<int> derivada(casos.size() - 2, 0.0);  // Vector para almacenar las derivadas calculadas

    // Calculamos las diferencias atrasadas para cada punto
    for (size_t i = 2; i < casos.size(); ++i) {
        // Fórmula de diferencias finitas atrasadas de segundo orden
        derivada[i - 2] = (3 * casos[i] - 4 * casos[i - 1] + casos[i - 2]) / (2 * h);
    }

    return derivada;  // Retornamos el vector con las derivadas
}

// Función para encontrar los índices de los máximos y mínimos en el vector de derivadas
void encontrarMaximos(const vector<int>& segundaDerivada, size_t& maxIndex, size_t& minIndex) {
    maxIndex = 0;  // Inicializamos el índice del valor máximo
    minIndex = 0;  // Inicializamos el índice del valor mínimo

    double maxVal = segundaDerivada[0], minVal = segundaDerivada[0];  // Inicializamos los valores máximos y mínimos

    // Recorremos el vector para encontrar los índices de los valores máximo y mínimo
    for (size_t i = 1; i < segundaDerivada.size(); ++i) {
        // Si encontramos un valor mayor, actualizamos el índice del máximo
        if (segundaDerivada[i] > maxVal) {
            maxVal = segundaDerivada[i];
            maxIndex = i;
        }
        // Si encontramos un valor menor, actualizamos el índice del mínimo
        if (segundaDerivada[i] < minVal) {
            minVal = segundaDerivada[i];
            minIndex = i;
        }
    }
}
vector<string> buscarArchivosCsv(const string& ruta) {
    vector<string> archivosCSV;
    DIR* lugar;                   // Puntero al directorio
    struct dirent* entrada;     // Estructura para almacenar información de cada archivo

    lugar = opendir(ruta.c_str()); // Abrimos el directorio
    if (lugar == nullptr) {
        cerr << "Error: no se pudo abrir el directorio." << endl;
        return archivosCSV;
    }

    while ((entrada = readdir(lugar)) != nullptr) { // Leemos cada entrada del directorio
        string nombreArchivo = entrada->d_name;

        // Filtrar archivos con extensión .csv
        if (nombreArchivo.size() > 4 && 
            nombreArchivo.substr(nombreArchivo.size() - 4) == ".csv") {
            archivosCSV.push_back(nombreArchivo.substr(0, nombreArchivo.size() - 4)); // se le quita el .csv al nombre
        }
    }

    closedir(lugar); // Cerramos el directorio
    return archivosCSV;
}
vector<Dato> obtenerArchivo() {
    string nombreArchivo;  // se pide que ingrese el nombre del archivo
    cout << "+------------------------------------------------------------------------------+" << endl;
    cout << "IMPORTANTE: El archivo debe estar en la misma carpeta que el programa y asegurarse que sea de extension .csv," << endl << "tambien aseguerese que no tenga espacios y si lo tiene porfavor rellenarlos con '_' ni signos especiales" << endl;
    cout << "+------------------------------------------------------------------------------+" << endl;
    string ruta = "."; // Ruta actual
    vector<string> archivosCSV = buscarArchivosCsv(ruta);

    cout << "Archivos .csv encontrados en: " << ruta << endl;
    for (const auto& archivo : archivosCSV) {
        cout << "- " << archivo << endl;
    }

    cout << "+------------------------------------------------------------------------------+" << endl;
    cout << "Ingrese le nombre del archivo CSV Ejemplo (datos_ejemplos): " ;
    cin >> nombreArchivo;

    nombreArchivo = nombreArchivo + ".csv";
    // Leemos los datos desde el archivo
    vector<Dato> datos = leerCSV(nombreArchivo);
    system("cls");
    return datos;
}



void imprimirResultado(const string& tipo, const string& fecha, double valor) {
    // Se usa setw para alinear las columnas
    cout << " | " << left << setw(45) << tipo 
         << " | " << setw(12) << fecha 
         << " | " << left << setw(12) << valor 
         << " |" << endl;
}

void imprimirLineaSeparadora() {
    cout << "+-------------------------------------------------+--------------+------------+" << endl;
}

int main() {
    
    vector<Dato> datos = obtenerArchivo();
    // Si no se encuentran datos, mostramos un mensaje y terminamos
    if (datos.empty()) {
        cerr << "No se encontraron datos en el archivo." << endl;
        return 1;
    }

    // Extraemos los casos confirmados en un vector
    vector<int> casos;
    for (const auto& dato : datos) {
        casos.push_back(dato.casos);  // Llenamos el vector con los casos
    }

    // Calculamos las tres diferencias de primer orden
    vector<int> centradasPrimerOrden = diferenciasCentradasPrimerOrden(casos, h);
    vector<int> adelantadasPrimerOrden = diferenciasAdelantadasPrimerOrden(casos, h);
    vector<int> atrasadasPrimerOrden = diferenciasAtrasadasPrimerOrden(casos, h);

    // Calculamos las tres diferencias de segundo orden
    vector<int> centradasSegundoOrden = diferenciasCentradasSegundoOrden(casos, h);
    vector<int> adelantadasSegundoOrden = diferenciasAdelantadasSegundoOrden(casos, h);
    vector<int> atrasadasSegundoOrden = diferenciasAtrasadasSegundoOrden(casos, h);

    // Variables para almacenar los índices de los máximos y mínimos
    size_t maxIndex, minIndex;

     cout << "+-------------------------------------------------+--------------+------------+" << endl;
    cout << "|                  Resultados                    |    Fecha     |    Valor   |" << endl;
    cout << "+-------------------------------------------------+--------------+------------+" << endl;
    
    // Imprimir resultados de las tres derivadas de primer orden
    cout << "Resultados de Derivadas de Primer Orden" << endl;
  
    imprimirLineaSeparadora();
    cambiarColor(2);
    encontrarMaximos(centradasPrimerOrden, maxIndex, minIndex);
    imprimirResultado("Mayor aceleracion (centrada 1er orden)", datos[maxIndex].fecha, centradasPrimerOrden[maxIndex]);
    imprimirResultado("Mayor desaceleracion (centrada 1er orden)", datos[minIndex].fecha, centradasPrimerOrden[minIndex]);
    cambiarColor(1);
    encontrarMaximos(adelantadasPrimerOrden, maxIndex, minIndex);
    imprimirResultado("Mayor aceleracion (adelantada 1er orden)", datos[maxIndex].fecha, adelantadasPrimerOrden[maxIndex]);
    imprimirResultado("Mayor desaceleracion (adelantada 1er orden)", datos[minIndex].fecha, adelantadasPrimerOrden[minIndex]);
    cambiarColor(3);
    encontrarMaximos(atrasadasPrimerOrden, maxIndex, minIndex);
    imprimirResultado("Mayor aceleracion (atrasada 1er orden)", datos[maxIndex].fecha, atrasadasPrimerOrden[maxIndex]);
    imprimirResultado("Mayor desaceleracion (atrasada 1er orden)", datos[minIndex].fecha, atrasadasPrimerOrden[minIndex]);
    cambiarColor(7);
    // Imprimir resultados de las tres derivadas de segundo orden
    imprimirLineaSeparadora();
    cout << "Resultados de Derivadas de Segundo Orden" << endl;
    imprimirLineaSeparadora();
    cambiarColor(2);
    encontrarMaximos(centradasSegundoOrden, maxIndex, minIndex);
    imprimirResultado("Mayor aceleracion (centrada 2do orden)", datos[maxIndex].fecha, centradasSegundoOrden[maxIndex]);
    imprimirResultado("Mayor desaceleracion (centrada 2do orden)", datos[minIndex].fecha, centradasSegundoOrden[minIndex]);
    cambiarColor(1);
    encontrarMaximos(adelantadasSegundoOrden, maxIndex, minIndex);
    imprimirResultado("Mayor aceleracion (adelantada 2do orden)", datos[maxIndex].fecha, adelantadasSegundoOrden[maxIndex]);
    imprimirResultado("Mayor desaceleracion (adelantada 2do orden)", datos[minIndex].fecha, adelantadasSegundoOrden[minIndex]);
    cambiarColor(3);
    encontrarMaximos(atrasadasSegundoOrden, maxIndex, minIndex);
    imprimirResultado("Mayor aceleracion (atrasada 2do orden)", datos[maxIndex].fecha, atrasadasSegundoOrden[maxIndex]);
    imprimirResultado("Mayor desaceleracion (atrasada 2do orden)", datos[minIndex].fecha, atrasadasSegundoOrden[minIndex]);
    cambiarColor(7);
    // Imprimir línea final
    imprimirLineaSeparadora();
}


    

