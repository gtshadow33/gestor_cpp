#include "gestor_tareas.h"
#include "colors.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>

// ────────────────────────────────────────────────────────
//  Constructor
// ────────────────────────────────────────────────────────
GestorTareas::GestorTareas() {
    cargarArchivo();
}

// ────────────────────────────────────────────────────────
//  Helpers privados de presentación
// ────────────────────────────────────────────────────────
void GestorTareas::separador(int tipo, int n) const {
    std::cout << CYAN;
    const std::string s = (tipo == 1) ? "═" : "─";
    for (int i = 0; i < n; i++) std::cout << s;
    std::cout << RESET << "\n";
}

void GestorTareas::imprimirTarjeta(const Tarea& t) const {
    separador(0, 56);
    std::cout << BOLD << "  #" << t.id << "  " << t.titulo << RESET << "\n";
    if (!t.descripcion.empty())
        std::cout << "   " << t.descripcion << "\n";
    std::cout << "   " << (t.responsable.empty() ? "Sin asignar" : t.responsable) << "\n";
    std::cout << "   Prioridad : "
              << colorPrioridad(t.prioridad) << BOLD << prioridadStr(t.prioridad) << RESET << "\n";
    std::cout << "   Estado    : "
              << colorEstado(t.estado) << BOLD << estadoStr(t.estado) << RESET << "\n";
    std::cout << "   Creada    : " << t.fechaCreacion << "\n";
    if (!t.fechaVencimiento.empty())
        std::cout << "   Vence     : " << t.fechaVencimiento << "\n";
    if (!t.etiquetas.empty())
        std::cout << "    Etiquetas : " << MAGENTA << t.etiquetas << RESET << "\n";
}

// ────────────────────────────────────────────────────────
//  CRUD
// ────────────────────────────────────────────────────────
void GestorTareas::crearTarea() {
    Tarea t;
    t.id            = nextId++;
    t.fechaCreacion = timestampActual();
    t.estado        = Estado::PENDIENTE;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << CYAN << "\n  ┌─ Nueva Tarea ─────────────────────┐\n" << RESET;
    std::cout << "  Título      : "; std::getline(std::cin, t.titulo);
    std::cout << "  Descripción : "; std::getline(std::cin, t.descripcion);
    std::cout << "  Responsable : "; std::getline(std::cin, t.responsable);

    std::cout << "  Prioridad [1=Baja 2=Media 3=Alta 4=Crítica]: ";
    int p; std::cin >> p;
    t.prioridad = (p >= 1 && p <= 4) ? (Prioridad)p : Prioridad::MEDIA;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "  Fecha vencimiento (YYYY-MM-DD, vacío=ninguna): ";
    std::getline(std::cin, t.fechaVencimiento);
    std::cout << "  Etiquetas (ej: backend,urgente): ";
    std::getline(std::cin, t.etiquetas);

    tareas.push_back(t);
    guardarArchivo();
    std::cout << GREEN << "\n   Tarea #" << t.id << " creada correctamente.\n" << RESET;
}

void GestorTareas::listarTareas(const std::vector<Tarea>& lista) const {
    if (lista.empty()) {
        std::cout << YELLOW << "\n   No hay tareas que mostrar.\n" << RESET;
        return;
    }
    separador(1, 60);
    std::cout << BOLD << "  TABLERO DE TAREAS  (" << lista.size() << " tareas)\n" << RESET;
    for (const auto& t : lista) imprimirTarjeta(t);
    separador(1, 60);
}

void GestorTareas::listarTodas() const {
    listarTareas(tareas);
}

void GestorTareas::cambiarEstado() {
    std::cout << "  ID de tarea a actualizar: ";
    int id; std::cin >> id;
    auto it = std::find_if(tareas.begin(), tareas.end(),
                           [id](const Tarea& t){ return t.id == id; });
    if (it == tareas.end()) {
        std::cout << RED << "   Tarea no encontrada.\n" << RESET; return;
    }
    std::cout << "  Nuevo estado [0=Pendiente 1=En Progreso 2=Completada 3=Cancelada]: ";
    int e; std::cin >> e;
    if (e >= 0 && e <= 3) {
        it->estado = (Estado)e;
        guardarArchivo();
        std::cout << GREEN << "   Estado actualizado.\n" << RESET;
    } else {
        std::cout << RED << "   Opción inválida.\n" << RESET;
    }
}

void GestorTareas::eliminarTarea() {
    std::cout << "  ID de tarea a eliminar: ";
    int id; std::cin >> id;
    auto it = std::remove_if(tareas.begin(), tareas.end(),
                             [id](const Tarea& t){ return t.id == id; });
    if (it == tareas.end()) {
        std::cout << RED << "   Tarea no encontrada.\n" << RESET; return;
    }
    tareas.erase(it, tareas.end());
    guardarArchivo();
    std::cout << GREEN << "   Tarea eliminada.\n" << RESET;
}

// ────────────────────────────────────────────────────────
//  Filtros
// ────────────────────────────────────────────────────────
void GestorTareas::filtrarPorEstado() const {
    std::cout << "  Estado [0=Pendiente 1=En Progreso 2=Completada 3=Cancelada]: ";
    int e; std::cin >> e;
    Estado estado = (Estado)e;
    std::vector<Tarea> filtradas;
    std::copy_if(tareas.begin(), tareas.end(), std::back_inserter(filtradas),
                 [estado](const Tarea& t){ return t.estado == estado; });
    listarTareas(filtradas);
}

void GestorTareas::filtrarPorPrioridad() const {
    std::cout << "  Prioridad [1=Baja 2=Media 3=Alta 4=Crítica]: ";
    int p; std::cin >> p;
    Prioridad prio = (Prioridad)p;
    std::vector<Tarea> filtradas;
    std::copy_if(tareas.begin(), tareas.end(), std::back_inserter(filtradas),
                 [prio](const Tarea& t){ return t.prioridad == prio; });
    listarTareas(filtradas);
}

void GestorTareas::filtrarPorEtiqueta() const {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "  Etiqueta a buscar: ";
    std::string etiq; std::getline(std::cin, etiq);
    std::vector<Tarea> filtradas;
    std::copy_if(tareas.begin(), tareas.end(), std::back_inserter(filtradas),
                 [&etiq](const Tarea& t){
                     return t.etiquetas.find(etiq) != std::string::npos;
                 });
    listarTareas(filtradas);
}

// ────────────────────────────────────────────────────────
//  Ordenamientos
// ────────────────────────────────────────────────────────
void GestorTareas::ordenarPorPrioridad() {
    std::sort(tareas.begin(), tareas.end(),
              [](const Tarea& a, const Tarea& b){
                  return (int)a.prioridad > (int)b.prioridad;
              });
    std::cout << GREEN << "   Ordenado por prioridad (mayor → menor).\n" << RESET;
    listarTodas();
}

void GestorTareas::ordenarPorFechaCreacion() {
    std::sort(tareas.begin(), tareas.end(),
              [](const Tarea& a, const Tarea& b){
                  return a.fechaCreacion < b.fechaCreacion;
              });
    std::cout << GREEN << "   Ordenado por fecha de creación.\n" << RESET;
    listarTodas();
}

void GestorTareas::ordenarPorVencimiento() {
    std::sort(tareas.begin(), tareas.end(),
              [](const Tarea& a, const Tarea& b){
                  if (a.fechaVencimiento.empty()) return false;
                  if (b.fechaVencimiento.empty()) return true;
                  return a.fechaVencimiento < b.fechaVencimiento;
              });
    std::cout << GREEN << "   Ordenado por fecha de vencimiento.\n" << RESET;
    listarTodas();
}

// ────────────────────────────────────────────────────────
//  Persistencia
// ────────────────────────────────────────────────────────
void GestorTareas::guardarArchivo() const {
    std::ofstream f(ARCHIVO);
    if (!f) { std::cerr << RED << "   Error al guardar archivo.\n" << RESET; return; }
    f << nextId << "\n";
    for (const auto& t : tareas) f << t.toCSV() << "\n";
    f.close();
}

void GestorTareas::cargarArchivo() {
    std::ifstream f(ARCHIVO);
    if (!f) return;
    std::string linea;
    std::getline(f, linea);
    try { nextId = std::stoi(linea); } catch (...) { nextId = 1; }
    while (std::getline(f, linea)) {
        if (!linea.empty()) tareas.push_back(Tarea::fromCSV(linea));
    }
    f.close();
}

// ────────────────────────────────────────────────────────
//  Estadísticas
// ────────────────────────────────────────────────────────
void GestorTareas::estadisticas() const {
    int pend = 0, prog = 0, comp = 0, canc = 0;
    int baja = 0, media = 0, alta = 0, critica = 0;
    for (const auto& t : tareas) {
        switch (t.estado) {
            case Estado::PENDIENTE:   pend++;  break;
            case Estado::EN_PROGRESO: prog++;  break;
            case Estado::COMPLETADA:  comp++;  break;
            case Estado::CANCELADA:   canc++;  break;
        }
        switch (t.prioridad) {
            case Prioridad::BAJA:    baja++;    break;
            case Prioridad::MEDIA:   media++;   break;
            case Prioridad::ALTA:    alta++;    break;
            case Prioridad::CRITICA: critica++; break;
        }
    }
    separador(1, 50);
    std::cout << BOLD << "   ESTADÍSTICAS\n" << RESET;
    separador(0);
    std::cout << "  Total tareas   : " << BOLD << tareas.size() << RESET << "\n\n";
    std::cout << "  Por estado:\n";
    std::cout << "    " << YELLOW << " Pendiente   : " << pend  << RESET << "\n";
    std::cout << "    " << CYAN   << " En progreso : " << prog  << RESET << "\n";
    std::cout << "    " << GREEN  << " Completada  : " << comp  << RESET << "\n";
    std::cout << "    " << RED    << " Cancelada   : " << canc  << RESET << "\n\n";
    std::cout << "  Por prioridad:\n";
    std::cout << "    " << GREEN   << " Baja    : " << baja    << RESET << "\n";
    std::cout << "    " << YELLOW  << " Media   : " << media   << RESET << "\n";
    std::cout << "    " << RED     << " Alta    : " << alta    << RESET << "\n";
    std::cout << "    " << MAGENTA << " Crítica : " << critica << RESET << "\n";
    separador(1, 50);
}

// ────────────────────────────────────────────────────────
//  Menú principal
// ────────────────────────────────────────────────────────
void GestorTareas::menu() {
    while (true) {
        std::cout << "\n" << BOLD << CYAN;
        std::cout << "  ╔══════════════════════════════════════╗\n";
        std::cout << "  ║         GESTOR DE TAREAS TRELLO      ║\n";
        std::cout << "  ╠══════════════════════════════════════╣\n";
        std::cout << "  ║  1.    Crear tarea                   ║\n";
        std::cout << "  ║  2.    Ver todas las tareas          ║\n";
        std::cout << "  ║  3.    Cambiar estado                ║\n";
        std::cout << "  ║  4.    Eliminar tarea                ║\n";
        std::cout << "  ╠══════════════════════════════════════╣\n";
        std::cout << "  ║  FILTROS                             ║\n";
        std::cout << "  ║  5.    Filtrar por estado            ║\n";
        std::cout << "  ║  6.    Filtrar por prioridad         ║\n";
        std::cout << "  ║  7.    Filtrar por etiqueta          ║\n";
        std::cout << "  ╠══════════════════════════════════════╣\n";
        std::cout << "  ║  ORDENAR                             ║\n";
        std::cout << "  ║  8.    Ordenar por prioridad         ║\n";
        std::cout << "  ║  9.    Ordenar por creación          ║\n";
        std::cout << "  ║  10.   Ordenar por vencimiento       ║\n";
        std::cout << "  ╠══════════════════════════════════════╣\n";
        std::cout << "  ║  11.   Estadísticas                  ║\n";
        std::cout << "  ║  0.    Salir                         ║\n";
        std::cout << "  ╚══════════════════════════════════════╝\n" << RESET;
        std::cout << "  Opción: ";

        int op;
        if (!(std::cin >> op)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (op) {
            case  1: crearTarea();              break;
            case  2: listarTodas();             break;
            case  3: cambiarEstado();           break;
            case  4: eliminarTarea();           break;
            case  5: filtrarPorEstado();        break;
            case  6: filtrarPorPrioridad();     break;
            case  7: filtrarPorEtiqueta();      break;
            case  8: ordenarPorPrioridad();     break;
            case  9: ordenarPorFechaCreacion(); break;
            case 10: ordenarPorVencimiento();   break;
            case 11: estadisticas();            break;
            case  0:
                std::cout << GREEN << "\n   ¡Hasta luego!\n\n" << RESET;
                return;
            default:
                std::cout << RED << "   Opción inválida.\n" << RESET;
        }
    }
}
