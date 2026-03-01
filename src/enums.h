#pragma once

#include <string>
#include "colors.h"

// ─── Enumeraciones ───────────────────────────────────────
enum class Prioridad { BAJA = 1, MEDIA = 2, ALTA = 3, CRITICA = 4 };
enum class Estado    { PENDIENTE, EN_PROGRESO, COMPLETADA, CANCELADA };

// ─── Helpers de conversión ───────────────────────────────
std::string prioridadStr(Prioridad p);
std::string estadoStr(Estado e);
std::string colorPrioridad(Prioridad p);
std::string colorEstado(Estado e);
