#ifndef DIRECTORY_UTILS_H
#define DIRECTORY_UTILS_H

#include <stdbool.h>

// Prototipos de funciones para manejar directorios y archivos
int create_directory(const char* path);
int create_species_file(const char* dir_path, const char* species_name);
char* build_dir_name(const char* question, const char* text, bool use_pre, bool use_suf);

#endif