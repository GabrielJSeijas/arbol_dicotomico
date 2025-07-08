#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include "dicotodir.h" 

//Prototipos de funciones para parsear el archivo JSON
SpeciesList* parse_json_file(const char* filename);
void free_species_list(SpeciesList* list);

#endif