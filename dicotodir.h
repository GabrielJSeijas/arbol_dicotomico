#ifndef DICOTODIR_H
#define DICOTODIR_H

#include <stdbool.h>

// Estructura que representa un rasgo (trait) de una especie.
// Por ejemplo: "Hojas como agujas", valor true/false.
typedef struct {
    char* name;     // Nombre del rasgo
    bool value;     // Valor del rasgo (true o false)
} Trait;

// Estructura que representa una especie.
// Incluye el nombre y un arreglo de rasgos.
typedef struct {
    char* name;     // Nombre de la especie
    Trait* traits;  // Arreglo de rasgos asociados a la especie
    int trait_count; // Número de rasgos
} Species;

// Estructura para manejar una lista de especies.
// Útil para procesar varias especies a la vez.
typedef struct {
    Species* species; // Arreglo de especies
    int count;        // Número total de especies
} SpeciesList;

// Función para leer un archivo JSON y obtener la lista de especies.
// Implementada en json_parser.c
SpeciesList* parse_json_file(const char* filename);

// Libera la memoria usada por la lista de especies.
// Implementada en json_parser.c
void free_species_list(SpeciesList* list);

// Crea un directorio en el sistema de archivos.
// Implementada en directory_utils.c
int create_directory(const char* path);

// Crea un archivo para una especie dentro de un directorio.
// Implementada en directory_utils.c
int create_species_file(const char* dir_path, const char* species_name);

// Construye el nombre de un directorio usando una pregunta y texto extra.
// Implementada en directory_utils.c
char* build_dir_name(const char* question, const char* text, 
                     bool use_pre, bool use_suf);

// Construye el árbol dicotómico usando la lista de especies.
// Implementada en tree_builder.c
void build_dichotomic_tree(SpeciesList* list, const char* root_dir, 
                           const char* true_text, const char* false_text, 
                           bool use_prefix, bool use_suffix);

// Verifica que el orden de los rasgos sea correcto en la lista de especies.
// Implementada en tree_builder.c
void verify_trait_order(SpeciesList* list);

// Procesa los argumentos de la línea de comandos.
// Implementada en main.c
void parse_arguments(int argc, char *argv[], char **json_file, char **root_dir,
                     char **true_text, char **false_text, 
                     bool *use_prefix, bool *use_suffix); 

#endif // DICOTODIR_H