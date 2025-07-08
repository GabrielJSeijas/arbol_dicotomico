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
    // Verificamos el orden de los traits
    for (int i = 0; i < list->count; i++) {
        
        // Empezamos la ruta desde el directorio raíz para CADA especie
        char current_path[4096] = {0};
        strcpy(current_path, root_dir);

        // Iteramos sobre los traits de la especie actual
        for (int j = 0; j < list->species[i].trait_count; j++) {
            const char* question = list->species[i].traits[j].name;
            bool answer = list->species[i].traits[j].value;
            
            char* dir_name = build_dir_name(question, 
                                          answer ? true_text : false_text, 
                                          use_prefix, use_suffix);
            if (!dir_name) {
                break; // Salir si falla la alocación de memoria
            }
            
            // Concatenamos el nombre del nuevo directorio a la ruta actual
            strcat(current_path, "/");
            strcat(current_path, dir_name);
            free(dir_name);
            
            // Creamos el directorio
            if (!create_directory(current_path)) {
                fprintf(stderr, "Error al crear el path para la especie '%s'. Abortando creación para esta especie.\n", list->species[i].name);
                break; 
            }
            
            // Si es el último trait de ESTA especie, creamos su archivo
            if (j == list->species[i].trait_count - 1) {
                create_species_file(current_path, list->species[i].name);
            }
        }
    }
}

