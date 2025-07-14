#ifndef DIRECTORY_UTILS_H
#define DIRECTORY_UTILS_H

#include <stdbool.h>

// Función para crear un directorio en la ruta especificada
int create_directory(const char* path);

// Función para crear un archivo de especie dentro de un directorio dado
int create_species_file(const char* dir_path, const char* species_name);

// Función para construir el nombre de un directorio usando una pregunta y un texto.
// Los parámetros use_pre y use_suf indican si se usan prefijos o sufijos en el nombre.
char* build_dir_name(const char* question, const char* text, bool use_pre, bool use_suf);

#endif