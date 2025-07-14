#ifndef TREE_BUILDER_H
#define TREE_BUILDER_H

#include <stdbool.h>
#include "json_parser.h"

// Función para construir el árbol dicotómico usando la lista de especies y sus rasgos.
// Parámetros:
//   list: lista de especies con sus rasgos
//   root_dir: directorio raíz donde se guarda el árbol
//   true_text, false_text: textos para las ramas verdaderas y falsas
//   use_prefix, use_suffix: opciones para modificar los nombres de los nodos
void build_dichotomic_tree(SpeciesList* list, const char* root_dir, 
                          const char* true_text, const char* false_text, 
                          bool use_prefix, bool use_suffix);

// Función para verificar que el orden de los rasgos en la lista de especies sea correcto.
// Esto ayuda a asegurar que el árbol se construya de manera consistente.
void verify_trait_order(SpeciesList* list);

#endif