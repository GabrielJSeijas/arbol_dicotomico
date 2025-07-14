#include "dicotodir.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

// Definimos los textos por defecto para las condiciones
#define DEFAULT_TRUE_TEXT "sí tiene"
#define DEFAULT_FALSE_TEXT "no tiene"

// Muestra cómo se usa el programa y las opciones disponibles
void print_usage(const char *program_name) {
    fprintf(stderr, "Uso: %s <clave.json> [-d|--dir <directorio>] [-t|--true <texto>] [-f|--false <texto>] [-p|--pre] [-s|--suf]\n", program_name);
    fprintf(stderr, "Opciones:\n");
    fprintf(stderr, "  -d, --dir <directorio>  Directorio raíz para la estructura (por defecto: .)\n");
    fprintf(stderr, "  -t, --true <texto>      Texto para condiciones verdaderas (por defecto: \"%s\")\n", DEFAULT_TRUE_TEXT);
    fprintf(stderr, "  -f, --false <texto>     Texto para condiciones falsas (por defecto: \"%s\")\n", DEFAULT_FALSE_TEXT);
    fprintf(stderr, "  -p, --pre               Usar texto como prefijo (activado por defecto).\n");
    fprintf(stderr, "  -s, --suf               Usar texto como sufijo.\n");
}

// Esta función procesa los argumentos de la línea de comandos y asigna los valores a las variables correspondientes
void parse_arguments(int argc, char *argv[], char **json_file, char **root_dir,
                    char **true_text, char **false_text, bool *use_prefix, bool *use_suffix) {
    *root_dir = ".";
    *true_text = DEFAULT_TRUE_TEXT;
    *false_text = DEFAULT_FALSE_TEXT;
    
    bool p_flag_set = false;
    bool s_flag_set = false;

    static struct option long_options[] = {
        {"dir", required_argument, 0, 'd'},
        {"true", required_argument, 0, 't'},
        {"false", required_argument, 0, 'f'},
        {"pre", no_argument, 0, 'p'},
        {"suf", no_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    // Usamos getopt_long para leer las opciones de la línea de comandos
    int opt;
    while ((opt = getopt_long(argc, argv, "d:t:f:ps", long_options, NULL)) != -1) {
        switch (opt) {
            case 'd': *root_dir = optarg; break; // Directorio raíz
            case 't': *true_text = optarg; break; // Texto para condición verdadera
            case 'f': *false_text = optarg; break; // Texto para condición falsa
            case 'p': p_flag_set = true; break; // Prefijo activado
            case 's': s_flag_set = true; break; // Sufijo activado
            case '?':
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Decidimos si usar prefijo, sufijo, ambos o ninguno según los flags
    if (p_flag_set && s_flag_set) {
        *use_prefix = true;
        *use_suffix = true;
    } else if (s_flag_set) {
        *use_prefix = false;
        *use_suffix = true;
    } else {
        *use_prefix = true;
        *use_suffix = false;
    }

    // El archivo JSON debe ser el argumento posicional obligatorio
    if (optind >= argc) {
        fprintf(stderr, "Error: Se requiere archivo JSON\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    *json_file = argv[optind];
}

// Función principal del programa
int main(int argc, char *argv[]) {
    char *json_file = NULL;
    char *root_dir = NULL;
    char *true_text = NULL;
    char *false_text = NULL;
    
    bool use_prefix;
    bool use_suffix;

    // Procesamos los argumentos de la línea de comandos
    parse_arguments(argc, argv, &json_file, &root_dir, &true_text, &false_text, &use_prefix, &use_suffix);

    // Parseamos el archivo JSON para obtener la lista de especies
    SpeciesList *species_list = parse_json_file(json_file);
    if (!species_list) {
        fprintf(stderr, "Error: No se pudo parsear el archivo JSON\n");
        return EXIT_FAILURE;
    }

    // Si el directorio raíz no es el actual, lo creamos
    if (strcmp(root_dir, ".") != 0) {
        if (!create_directory(root_dir)) {
            fprintf(stderr, "Error: No se pudo crear el directorio raíz '%s'\n", root_dir);
            free_species_list(species_list);
            return EXIT_FAILURE;
        }
    }
    
    // Construimos el árbol dicotómico usando los parámetros dados
    build_dichotomic_tree(species_list, root_dir, true_text, false_text, use_prefix, use_suffix);

    // Liberamos la memoria usada por la lista de especies
    free_species_list(species_list);

    // Mostramos mensaje de éxito
    printf("Estructura dicotómica creada exitosamente en: %s\n", root_dir);
    return EXIT_SUCCESS;
}
