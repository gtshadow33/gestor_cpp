#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "enums.h"

// ─── Estructura Tarea ────────────────────────────────────
struct Tarea { 
    int         id;
    std::string titulo;
    std::string descripcion;
    std::string responsable;
    Prioridad   prioridad;
    Estado      estado;
    std::string fechaCreacion;
    std::string fechaVencimiento;
    std::string etiquetas;       // separadas por comas

    // Serialización a CSV (separador '|')
    std::string toCSV() const;

    // Deserialización desde CSV
    static Tarea fromCSV(const std::string& linea);
};
