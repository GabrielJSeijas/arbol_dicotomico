#define _DEFAULT_SOURCE
#include "json_parser.h"
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_parser.h" 

//Este archivo contiene la lógica para parsear el archivo JSON,
// crear la lista de especies y liberar la memoria utilizada.   

SpeciesList* parse_json_file(const char* filename) {
    // Verificar si el nombre del archivo es válido
    json_object* root = json_object_from_file(filename);
    if (!root) {
        fprintf(stderr, "Error al leer el archivo JSON\n");
        return NULL;
    }

    // Verificar si el objeto raíz es un objeto JSON
    SpeciesList* list = malloc(sizeof(SpeciesList));
    if (!list) {
        fprintf(stderr, "Error de memoria\n");
        json_object_put(root);
        return NULL;
    }
    list->count = 0;
    list->species = NULL;

    // Iterar sobre las claves del objeto JSON
    json_object_object_foreach(root, key, val) {
         (void)key;
        if (json_object_is_type(val, json_type_array)) {
            int array_len = json_object_array_length(val);
            list->species = malloc(array_len * sizeof(Species));
            if (!list->species) {
                fprintf(stderr, "Error de memoria\n");
                json_object_put(root);
                free(list);
                return NULL;
            }

            // Iterar sobre cada especie en el array
            for (int i = 0; i < array_len; i++) {
                json_object* species_obj = json_object_array_get_idx(val, i);
    
                json_object_object_foreach(species_obj, species_name, traits_array) {
                    list->species[i].name = strdup(species_name);

                    int traits_count = json_object_array_length(traits_array);
                    list->species[i].traits = malloc(traits_count * sizeof(Trait));
                    list->species[i].trait_count = traits_count;

                    for (int j = 0; j < traits_count; j++) {
                        json_object* trait_obj = json_object_array_get_idx(traits_array, j);
                        json_object_object_foreach(trait_obj, trait_name, trait_value_obj) {
                            list->species[i].traits[j].name = strdup(trait_name);
                            list->species[i].traits[j].value = json_object_get_boolean(trait_value_obj);
                        }
                    }
                }
            }
            list->count = array_len;
        }
    }
    json_object_put(root);
    return list;
}

// Función para liberar la memoria utilizada por la lista de especies
// Esta función recorre la lista de especies y libera la memoria
// asignada a cada especie, sus rasgos y finalmente la lista misma.
// Es importante llamar a esta función después de haber terminado de usar la lista
// para evitar fugas de memoria.
void free_species_list(SpeciesList* list) {
    if (!list) return;
    
    for (int i = 0; i < list->count; i++) {
        free(list->species[i].name);
        for (int j = 0; j < list->species[i].trait_count; j++) {
            free(list->species[i].traits[j].name);
        }
        free(list->species[i].traits);
    }
    free(list->species);
    free(list);
}