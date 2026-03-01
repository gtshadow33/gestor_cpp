#include "enums.h"

std::string prioridadStr(Prioridad p) {
    switch (p) {
        case Prioridad::BAJA:    return "BAJA";
        case Prioridad::MEDIA:   return "MEDIA";
        case Prioridad::ALTA:    return "ALTA";
        case Prioridad::CRITICA: return "CRÍTICA";
    }
    return "?";
}

std::string estadoStr(Estado e) {
    switch (e) {
        case Estado::PENDIENTE:   return "PENDIENTE";
        case Estado::EN_PROGRESO: return "EN PROGRESO";
        case Estado::COMPLETADA:  return "COMPLETADA";
        case Estado::CANCELADA:   return "CANCELADA";
    }
    return "?";
}

std::string colorPrioridad(Prioridad p) {
    switch (p) {
        case Prioridad::BAJA:    return GREEN;
        case Prioridad::MEDIA:   return YELLOW;
        case Prioridad::ALTA:    return RED;
        case Prioridad::CRITICA: return MAGENTA;
    }
    return WHITE;
}

std::string colorEstado(Estado e) {
    switch (e) {
        case Estado::PENDIENTE:   return YELLOW;
        case Estado::EN_PROGRESO: return CYAN;
        case Estado::COMPLETADA:  return GREEN;
        case Estado::CANCELADA:   return RED;
    }
    return WHITE;
}
