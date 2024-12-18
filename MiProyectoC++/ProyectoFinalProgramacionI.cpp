#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <string>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

// Estructura para productos en el inventario -----------------------------------------------------------------------------------------------------------------
struct Producto {
    string nombre;
    float precio;
    int cantidad;
};

// Estructura para registrar cambios ------------------------------------------------------------------------------------------------------------------------
struct Cambio {
    string tipo;        // Si elimina o agrega
    Producto producto;  // Producto afectado
};

// Declaración de funciones --------------------------------------------------------------------------------------------------------------------------------
void menuPrincipal();
void menuInventario(list<Producto>&, stack<Cambio>&);
void menuSolicitudes(queue<string>&);
void menuClientesEspera(queue<string>&);
void menuRegistroCambios(list<Producto>&, stack<Cambio>&);

// Funciones para Inventario ------------------------------------------------------------------------------------------------------------------------------
void agregarProducto(list<Producto>&, stack<Cambio>&);
void eliminarProducto(list<Producto>&, stack<Cambio>&);
void consultarProducto(list<Producto>&);
void listarProductos(list<Producto>&);

// Funciones para Solicitudes -----------------------------------------------------------------------------------------------------------------------------
void registrarSolicitud(queue<string>&);
void procesarSolicitud(queue<string>&);
void consultarSolicitud(queue<string>&);
void listarSolicitudes(queue<string>&);

// Funciones para Clientes en Espera -----------------------------------------------------------------------------------------------------------------------
void registrarCliente(queue<string>&);
void atenderCliente(queue<string>&);
void listarClientes(queue<string>&);

// Funciones para Registro de Cambios -----------------------------------------------------------------------------------------------------------------------
void deshacerCambio(list<Producto>&, stack<Cambio>&);

// Función para validar que la entrada sea numérica --------------------------------------------------------------------------------------------------------
int leerOpcion() {
    int opcion;
    while (true) {
        cin >> opcion;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta la entrada incorrecta
            cout << "Por favor, ingrese un numero valido: ";
        } else {
            return opcion;
        }
    }
}
 
// Main que Llama al Menú Principar -----------------------------------------------------------------------------------------------------------------------
int main() {
    menuPrincipal();
    return 0;
}


// Menú Principal -----------------------------------------------------------------------------------------------------------------------------------------
void menuPrincipal() {
    list<Producto> inventario;
    stack<Cambio> cambios;
    queue<string> solicitudes;
    queue<string> clientes;
    int opcion;
    do {
        cout << "\n--- Menu Principal ---\n";
        cout << "1. Gestion de Inventario\n";
        cout << "2. Gestion de Solicitudes de Compra\n";
        cout << "3. Gestion de Clientes en Espera\n";
        cout << "4. Gestion de Registro de Cambios\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                menuInventario(inventario, cambios);
                break;
            case 2:
                menuSolicitudes(solicitudes);
                break;
            case 3:
                menuClientesEspera(clientes);
                break;
            case 4:
                menuRegistroCambios(inventario, cambios);
                break;
            case 5:
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 5);
}

// ------------------------------------------------------- Gestion de Inventario ---------------------------------------------------------------------
void menuInventario(list<Producto>& inventario, stack<Cambio>& cambios) {
    int opcion;
    do {
        cout << "\n--- Gestion de Inventario ---\n";
        cout << "1. Registrar nuevo producto\n";
        cout << "2. Eliminar producto\n";
        cout << "3. Consultar producto\n";
        cout << "4. Listar productos\n";
        cout << "5. Regresar al menu principal\n";
        cout << "Seleccione una opcion: ";
        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                agregarProducto(inventario, cambios);
                break;
            case 2:
                eliminarProducto(inventario, cambios);
                break;
            case 3:
                consultarProducto(inventario);
                break;
            case 4:
                listarProductos(inventario);
                break;
            case 5:
                return;
            default:
                cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 5);
}

void agregarProducto(list<Producto>& inventario, stack<Cambio>& cambios) {
    Producto nuevo;
    cout << "Ingrese el nombre del producto: ";
    cin.ignore();
    getline(cin, nuevo.nombre);

    
    if (!nuevo.nombre.empty()) {
        nuevo.nombre[0] = toupper(nuevo.nombre[0]);  // Primera letra en mayúscula
        transform(nuevo.nombre.begin() + 1, nuevo.nombre.end(), nuevo.nombre.begin() + 1, ::tolower); // Resto en minúsculas
    }

    
    for (const auto& producto : inventario) {
        string nombreProducto = producto.nombre;
        if (!nombreProducto.empty()) {
            nombreProducto[0] = toupper(nombreProducto[0]);  // Primera letra en mayúscula
            transform(nombreProducto.begin() + 1, nombreProducto.end(), nombreProducto.begin() + 1, ::tolower); // Resto en minúsculas
        }

        if (nombreProducto == nuevo.nombre) {
            cout << "Error: El producto '" << nuevo.nombre << "' ya existe en el inventario.\n";
            return; 
        }
    }

    while (true) {
        cout << "Ingrese el precio del producto: ";
        cin >> nuevo.precio;
        if (nuevo.precio > 0) break;
        cout << "El precio debe ser positivo.\n";
    }

    while (true) {
        cout << "Ingrese la cantidad del producto: ";
        cin >> nuevo.cantidad;
        if (nuevo.cantidad > 0) break;
        cout << "La cantidad debe ser positiva.\n";
    }

    inventario.push_back(nuevo);

    // Registrar el cambio para gestión de cambios
    Cambio cambio = {"Agregar", nuevo};
    cambios.push(cambio);

    cout << "Producto '" << nuevo.nombre << "' agregado exitosamente.\n";
}



void eliminarProducto(list<Producto>& inventario, stack<Cambio>& cambios) {
    string nombre;
    cout << "Ingrese el nombre del producto a eliminar: ";
    cin.ignore();
    getline(cin, nombre);

    for (auto it = inventario.begin(); it != inventario.end(); ++it) {
        if (it->nombre == nombre) {
            // Registrar el cambio
            Cambio cambio = {"Eliminar", *it};
            cambios.push(cambio);

            inventario.erase(it);
            cout << "Producto '" << nombre << "' eliminado exitosamente.\n";
            return;
        }
    }

    cout << "Producto no encontrado.\n";
}

void consultarProducto(list<Producto>& inventario) {
    string nombre;
    cout << "Ingrese el nombre del producto a consultar: ";
    cin.ignore();
    getline(cin, nombre);

    for (auto& producto : inventario) {
        if (producto.nombre == nombre) {
            cout << "Producto encontrado:\n";
            cout << "Nombre: " << producto.nombre
                 << ", Precio: " << producto.precio
                 << ", Cantidad: " << producto.cantidad << endl;
            return;
        }
    }
    cout << "Producto no encontrado.\n";
}

void listarProductos(list<Producto>& inventario) {
    if (inventario.empty()) {
        cout << "El inventario esta vacio.\n";
        return;
    }

    cout << "\nProductos en inventario:\n";
    for (auto& producto : inventario) {
        cout << "Nombre: " << producto.nombre
             << ", Precio: " << producto.precio
             << ", Cantidad: " << producto.cantidad << endl;
    }
}

// --------------------------------------------------------------- Gestion de Solicitudes ---------------------------------------------------------------
void menuSolicitudes(queue<string>& solicitudes) {
    int opcion;
    do {
        cout << "\n--- Gestion de Solicitudes de Compra ---\n";
        cout << "1. Registrar solicitud\n";
        cout << "2. Procesar solicitud\n";
        cout << "3. Consultar solicitud en proceso\n";
        cout << "4. Listar solicitudes pendientes\n";
        cout << "5. Regresar al menu principal\n";
        cout << "Seleccione una opcion: ";
        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                registrarSolicitud(solicitudes);
                break;
            case 2:
                procesarSolicitud(solicitudes);
                break;
            case 3:
                consultarSolicitud(solicitudes);
                break;
            case 4:
                listarSolicitudes(solicitudes);
                break;
            case 5:
                return;
            default:
                cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 5);
}

void registrarSolicitud(queue<string>& solicitudes) {
    string solicitud;
    cout << "Ingrese la solicitud: ";
    cin.ignore();
    getline(cin, solicitud);
    solicitudes.push(solicitud);
    cout << "Solicitud registrada exitosamente.\n";
}

void procesarSolicitud(queue<string>& solicitudes) {
    if (!solicitudes.empty()) {
        cout << "Procesando solicitud: " << solicitudes.front() << endl;
        solicitudes.pop();
    } else {
        cout << "No hay solicitudes pendientes.\n";
    }
}

void consultarSolicitud(queue<string>& solicitudes) {
    if (!solicitudes.empty()) {
        cout << "Solicitud en proceso: " << solicitudes.front() << endl;
    } else {
        cout << "No hay solicitudes pendientes.\n";
    }
}

void listarSolicitudes(queue<string>& solicitudes) {
    if (solicitudes.empty()) {
        cout << "No hay solicitudes pendientes.\n";
        return;
    }

    cout << "Solicitudes pendientes:\n";
    queue<string> temp = solicitudes;
    while (!temp.empty()) {
        cout << temp.front() << endl;
        temp.pop();
    }
}

// ------------------------------------------------- Gestion de Clientes en Espera -------------------------------------------------------------------------
void menuClientesEspera(queue<string>& clientes) {
    int opcion;
    do {
        cout << "\n--- Gestion de Clientes en Espera ---\n";
        cout << "1. Registrar cliente\n";
        cout << "2. Atender cliente\n";
        cout << "3. Listar clientes en espera\n";
        cout << "4. Regresar al menu principal\n";
        cout << "Seleccione una opcion: ";
        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                registrarCliente(clientes);
                break;
            case 2:
                atenderCliente(clientes);
                break;
            case 3:
                listarClientes(clientes);
                break;
            case 4:
                return;
            default:
                cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 4);
}

void registrarCliente(queue<string>& clientes) {
    string cliente;
    cout << "Ingrese el nombre del cliente: ";
    cin.ignore();
    getline(cin, cliente);

    // Usamos una cola temporal para verificar si el cliente ya está en la cola
    queue<string> temp = clientes;
    while (!temp.empty()) {
        if (temp.front() == cliente) {
            cout << "El cliente '" << cliente << "' ya esta en la cola.\n";
            return;
        }
        temp.pop();
    }

    // Si no está en la cola, agregar el cliente
    clientes.push(cliente);
    cout << "Cliente registrado exitosamente.\n";
}

void atenderCliente(queue<string>& clientes) {
    if (!clientes.empty()) {
        cout << "Atendiendo a: " << clientes.front() << endl;
        clientes.pop();
    } else {
        cout << "No hay clientes en espera.\n";
    }
}

void listarClientes(queue<string>& clientes) {
    if (clientes.empty()) {
        cout << "No hay clientes en espera.\n";
        return;
    }

    cout << "Clientes en espera:\n";
    queue<string> temp = clientes;
    while (!temp.empty()) {
        cout << temp.front() << endl;
        temp.pop();
    }
}


// ------------------------------------------------------------------- Gestion de Registro de Cambios ---------------------------------------------------
void menuRegistroCambios(list<Producto>& inventario, stack<Cambio>& cambios) {
    int opcion;
    do {
        cout << "\n--- Gestion de Registro de Cambios ---\n";
        cout << "1. Deshacer ultima modificacion\n";
        cout << "2. Regresar al menu principal\n";
        cout << "Seleccione una opcion: ";
        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                deshacerCambio(inventario, cambios);
                break;
            case 2:
                return;
            default:
                cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 2);
}

void deshacerCambio(list<Producto>& inventario, stack<Cambio>& cambios) {
    if (cambios.empty()) {
        cout << "No hay cambios para deshacer.\n";
        return;
    }

    Cambio ultimo = cambios.top();  // Llamada correcta al método top()
    cambios.pop();

    if (ultimo.tipo == "Agregar") {
        // Deshacer agregar (eliminar producto)
        inventario.remove_if([&](const Producto& p) { return p.nombre == ultimo.producto.nombre; });
        cout << "Se deshizo el agregar del producto: " << ultimo.producto.nombre << endl;
    } else if (ultimo.tipo == "Eliminar") {
        // Deshacer eliminar (restaurar producto)
        inventario.push_back(ultimo.producto);
        cout << "Se deshizo la eliminacion del producto: " << ultimo.producto.nombre << endl;
    }
}