#ifndef DICOTODIR_H
#define DICOTODIR_H

#include <stdbool.h>

// Estructura para representar un rasgo/trait
typedef struct {
    char* name;     // Nombre del rasgo (Hojas como agujas)
    bool value;     // Valor booleano (true/false)
} Trait;

// Estructura para representar una especie
typedef struct {
    char* name;     // Nombre de la especie (Pino)
    Trait* traits;  // Array de rasgos
    int trait_count; // Cantidad de rasgos
} Species;

// Estructura para la lista de especies
typedef struct {
    Species* species; // Array de especies
    int count;       // Cantidad de especies
} SpeciesList;

// Prototipos de funciones de json_parser.c
SpeciesList* parse_json_file(const char* filename);
void free_species_list(SpeciesList* list);

// Prototipos de funciones de directory_utils.c
int create_directory(const char* path);
int create_species_file(const char* dir_path, const char* species_name);
char* build_dir_name(const char* question, const char* text, 
                     bool use_pre, bool use_suf);

// Prototipos de funciones de tree_builder.c
void build_dichotomic_tree(SpeciesList* list, const char* root_dir, 
                           const char* true_text, const char* false_text, 
                           bool use_prefix, bool use_suffix);
void verify_trait_order(SpeciesList* list);

// Prototipo para manejo de argumentos (de main.c)
void parse_arguments(int argc, char *argv[], char **json_file, char **root_dir,
                     char **true_text, char **false_text, 
                     bool *use_prefix, bool *use_suffix); 

#endif // DICOTODIR_H