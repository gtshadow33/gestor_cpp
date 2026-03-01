#pragma once

#include <vector>
#include <string>
#include "tarea.h"

// ─── Clase principal GestorTareas ────────────────────────
class GestorTareas {
private:
    std::vector<Tarea> tareas;
    int nextId = 1;
    const std::string ARCHIVO = "tareas.dat";

    // ── Helpers de presentación
    void separador(int tipo = 0, int n = 60) const;
    void imprimirTarjeta(const Tarea& t) const;

    // ── Persistencia interna
    void guardarArchivo() const;
    void cargarArchivo();

public:
    GestorTareas();

    // ── CRUD
    void crearTarea();
    void listarTareas(const std::vector<Tarea>& lista) const;
    void listarTodas() const;
    void cambiarEstado();
    void eliminarTarea();

    // ── Filtros
    void filtrarPorEstado()    const;
    void filtrarPorPrioridad() const;
    void filtrarPorEtiqueta()  const;

    // ── Ordenamientos
    void ordenarPorPrioridad();
    void ordenarPorFechaCreacion();
    void ordenarPorVencimiento();

    // ── Estadísticas
    void estadisticas() const;

    // ── Bucle principal
    void menu();
};
