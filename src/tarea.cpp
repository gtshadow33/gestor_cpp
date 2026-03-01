#include "tarea.h"

std::string Tarea::toCSV() const {
    return std::to_string(id)             + "|" +
           titulo                         + "|" +
           descripcion                    + "|" +
           responsable                    + "|" +
           std::to_string((int)prioridad) + "|" +
           std::to_string((int)estado)    + "|" +
           fechaCreacion                  + "|" +
           fechaVencimiento               + "|" +
           etiquetas;
}

Tarea Tarea::fromCSV(const std::string& linea) {
    Tarea t;
    std::istringstream ss(linea);
    std::string tok;
    std::vector<std::string> campos;
    while (std::getline(ss, tok, '|')) campos.push_back(tok);
    if (campos.size() < 9) return t;

    t.id               = std::stoi(campos[0]);
    t.titulo           = campos[1];
    t.descripcion      = campos[2];
    t.responsable      = campos[3];
    t.prioridad        = (Prioridad)std::stoi(campos[4]);
    t.estado           = (Estado)std::stoi(campos[5]);
    t.fechaCreacion    = campos[6];
    t.fechaVencimiento = campos[7];
    t.etiquetas        = campos[8];
    return t;
}
