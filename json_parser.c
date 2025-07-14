#define _DEFAULT_SOURCE
#include "json_parser.h"
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_parser.h" 

// Esta función lee un archivo JSON y lo convierte en una lista de especies.
// Cada especie tiene un nombre y una lista de rasgos (traits).
SpeciesList* parse_json_file(const char* filename) {
    // Crea el objeto raíz a partir del archivo JSON
    json_object* root = json_object_from_file(filename);
    if (!root) {
        fprintf(stderr, "Error al leer el archivo JSON\n");
        return NULL;
    }
    
    // Reserva memoria para la lista de especies
    SpeciesList* list = malloc(sizeof(SpeciesList));
    if (!list) {
        fprintf(stderr, "Error de memoria\n");
        json_object_put(root);
        return NULL;
    }
    list->count = 0;
    list->species = NULL;

    // Recorre las claves del objeto raíz (por ejemplo, "especies")
    json_object_object_foreach(root, key, val) {
        (void)key; // No usamos la clave directamente
        if (json_object_is_type(val, json_type_array)) {
            int array_len = json_object_array_length(val);
            // Reserva memoria para el arreglo de especies
            list->species = malloc(array_len * sizeof(Species));
            if (!list->species) {
                fprintf(stderr, "Error de memoria\n");
                json_object_put(root);
                free(list);
                return NULL;
            }

            // Recorre cada especie en el arreglo
            for (int i = 0; i < array_len; i++) {
                json_object* species_obj = json_object_array_get_idx(val, i);

                // Cada especie tiene un nombre y un arreglo de rasgos
                json_object_object_foreach(species_obj, species_name, traits_array) {
                    // Copia el nombre de la especie
                    list->species[i].name = strdup(species_name);

                    int traits_count = json_object_array_length(traits_array);
                    // Reserva memoria para los rasgos de la especie
                    list->species[i].traits = malloc(traits_count * sizeof(Trait));
                    list->species[i].trait_count = traits_count;

                    // Recorre cada rasgo de la especie
                    for (int j = 0; j < traits_count; j++) {
                        json_object* trait_obj = json_object_array_get_idx(traits_array, j);
                        json_object_object_foreach(trait_obj, trait_name, trait_value_obj) {
                            // Copia el nombre del rasgo
                            list->species[i].traits[j].name = strdup(trait_name);
                            // Guarda el valor del rasgo (booleano)
                            list->species[i].traits[j].value = json_object_get_boolean(trait_value_obj);
                        }
                    }
                }
            }
            list->count = array_len;
        }
    }
    // Libera el objeto raíz de JSON
    json_object_put(root);
    return list;
}

// Libera toda la memoria reservada para la lista de especies y sus rasgos.
// Es importante llamar a esta función para evitar fugas de memoria.
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