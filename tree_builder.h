#ifndef TREE_BUILDER_H
#define TREE_BUILDER_H

#include <stdbool.h>
#include "json_parser.h"

// Prototipos de funciones para construir el árbol dicotómico a partir de la lista de especies y sus rasgos.
void build_dichotomic_tree(SpeciesList* list, const char* root_dir, 
                          const char* true_text, const char* false_text, 
                          bool use_prefix, bool use_suffix);
void verify_trait_order(SpeciesList* list);

#endif