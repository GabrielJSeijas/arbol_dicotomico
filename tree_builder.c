#include "tree_builder.h"
#include "directory_utils.h" 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Esta función construye un árbol dicotómico usando una lista de especies y sus rasgos.
// Para cada especie, crea una ruta de directorios basada en sus traits y guarda un archivo con su nombre.
void build_dichotomic_tree(SpeciesList* list, const char* root_dir, 
                           const char* true_text, const char* false_text, 
                           bool use_prefix, bool use_suffix) {
    // Si la lista está vacía, no hacemos nada.
    if (!list || list->count == 0) {
        return;
    }

    // Recorremos todas las especies en la lista.
    for (int i = 0; i < list->count; i++) {
        
        char current_path[4096] = {0}; // Aquí vamos armando la ruta para cada especie.
        strcpy(current_path, root_dir); // Empezamos desde el directorio raíz.
        
        bool path_creation_ok = true; // Esta bandera nos dice si todo salió bien al crear la ruta.

        // Para cada trait de la especie, vamos agregando subdirectorios según la respuesta.
        for (int j = 0; j < list->species[i].trait_count; j++) {
            const char* question = list->species[i].traits[j].name;
            bool answer = list->species[i].traits[j].value;
            
            // Creamos el nombre del directorio según el trait y la respuesta (true/false).
            char* dir_name = build_dir_name(question, 
                                          answer ? true_text : false_text, 
                                          use_prefix, use_suffix);
            if (!dir_name) {
                path_creation_ok = false; // Si no se pudo crear el nombre, marcamos error.
                break;
            }
            
            strcat(current_path, "/");
            strcat(current_path, dir_name);
            free(dir_name); // Liberamos la memoria del nombre del directorio.
            
            // Intentamos crear el directorio. Si falla, mostramos un error y salimos del loop.
            if (!create_directory(current_path)) {
                fprintf(stderr, "Error al crear el path para la especie '%s'. Abortando creación para esta especie.\n", list->species[i].name);
                path_creation_ok = false;
                break; 
            }
        }

        // Si la ruta se creó bien, guardamos el archivo de la especie en el último directorio.
        if (path_creation_ok) {
            create_species_file(current_path, list->species[i].name);
        }
    }
}