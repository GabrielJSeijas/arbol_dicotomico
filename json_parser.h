#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "dicotodir.h" // Incluye las definiciones necesarias para SpeciesList

// Función que recibe el nombre de un archivo JSON y devuelve una lista de especies
SpeciesList* parse_json_file(const char* filename);

// Función para liberar la memoria ocupada por la lista de especies
void free_species_list(SpeciesList* list);

#endif