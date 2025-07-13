#include "tree_builder.h"
#include "directory_utils.h" 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Este archivo contiene la lógica para construir el árbol dicotómico
// a partir de la lista de especies y sus rasgos.
void build_dichotomic_tree(SpeciesList* list, const char* root_dir, 
                           const char* true_text, const char* false_text, 
                           bool use_prefix, bool use_suffix) {
    if (!list || list->count == 0) {
        return;
    }

    // Iteramos sobre todas las especies en la lista.
    for (int i = 0; i < list->count; i++) {
        
        char current_path[4096] = {0};
        strcpy(current_path, root_dir);
        
        bool path_creation_ok = true; // Bandera para saber si la ruta se creó sin errores.

        // Iteramos sobre los traits de la especie actual para crear su ruta de directorios.
        for (int j = 0; j < list->species[i].trait_count; j++) {
            const char* question = list->species[i].traits[j].name;
            bool answer = list->species[i].traits[j].value;
            
            char* dir_name = build_dir_name(question, 
                                          answer ? true_text : false_text, 
                                          use_prefix, use_suffix);
            if (!dir_name) {
                path_creation_ok = false;
                break; // Salir si falla la alocación de memoria
            }
            
            strcat(current_path, "/");
            strcat(current_path, dir_name);
            free(dir_name);
            
            if (!create_directory(current_path)) {
                fprintf(stderr, "Error al crear el path para la especie '%s'. Abortando creación para esta especie.\n", list->species[i].name);
                path_creation_ok = false;
                break; 
            }
        }

        // Crear el archivo de la especie solo si la ruta se generó correctamente.
        if (path_creation_ok) {
            create_species_file(current_path, list->species[i].name);
        }
    }
}