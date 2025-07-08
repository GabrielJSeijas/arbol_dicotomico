// Angel Valero 18-10436 y Gabriel Seijas 19-00036

#include "dicotodir.h" // Asegúrate que este .h está actualizado
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h> // Necesario para 'bool'
#include <string.h>

// Definiciones de texto por defecto
#define DEFAULT_TRUE_TEXT "si tiene"
#define DEFAULT_FALSE_TEXT "no tiene"

// Este archivo contiene la lógica para manejar los argumentos de línea de comandos,
// parsear el archivo JSON y construir la estructura dicotómica.
void print_usage(const char *program_name) {
    fprintf(stderr, "Uso: %s <clave.json> [-d|--dir <directorio>] [-t|--true <texto>] [-f|--false <texto>] [-p|--pre] [-s|--suf]\n", program_name);
    fprintf(stderr, "Opciones:\n");
    fprintf(stderr, "  -d, --dir <directorio>  Directorio raíz para la estructura (por defecto: .)\n"); // Cambiado a . por consistencia con enunciado
    fprintf(stderr, "  -t, --true <texto>      Texto para condiciones verdaderas (por defecto: \"%s\")\n", DEFAULT_TRUE_TEXT);
    fprintf(stderr, "  -f, --false <texto>     Texto para condiciones falsas (por defecto: \"%s\")\n", DEFAULT_FALSE_TEXT);
    fprintf(stderr, "  -p, --pre               Usar texto como prefijo (activado por defecto).\n");
    fprintf(stderr, "  -s, --suf               Usar texto como sufijo.\n");
}

//Pasamos punteros a dos booleanos 
void parse_arguments(int argc, char *argv[], char **json_file, char **root_dir,
                    char **true_text, char **false_text, bool *use_prefix, bool *use_suffix) {
    *root_dir = "."; // El directorio actual es un mejor defecto según enunciado
    *true_text = DEFAULT_TRUE_TEXT;
    *false_text = DEFAULT_FALSE_TEXT;
    
    // Banderas para detectar la presencia explícita de -p y -s
    bool p_flag_set = false;
    bool s_flag_set = false;

    // Opciones largas
    static struct option long_options[] = {
        {"dir", required_argument, 0, 'd'},
        {"true", required_argument, 0, 't'},
        {"false", required_argument, 0, 'f'},
        {"pre", no_argument, 0, 'p'},
        {"suf", no_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    // Parsear los argumentos de línea de comandos
    // Usamos getopt_long para manejar opciones largas y cortas
    // getopt_long devuelve -1 cuando no hay más opciones
    // optind se usa para saber el índice del primer argumento posicional
    // (el archivo JSON en este caso)
    int opt;
    while ((opt = getopt_long(argc, argv, "d:t:f:ps", long_options, NULL)) != -1) {
        switch (opt) {
            case 'd': *root_dir = optarg; break;
            case 't': *true_text = optarg; break;
            case 'f': *false_text = optarg; break;
            case 'p': p_flag_set = true; break;
            case 's': s_flag_set = true; break;
            case '?':
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Lógica para determinar el comportamiento de prefijo/sufijo
    if (p_flag_set && s_flag_set) {
        //Ambos flags están presentes: se activan ambos
        *use_prefix = true;
        *use_suffix = true;
    } else if (s_flag_set) {
        //Solo -s está presente: se activa sufijo, se desactiva prefijo
        *use_prefix = false;
        *use_suffix = true;
    } else {
        //Solo -p está presente o ninguno está: se activa prefijo (comportamiento por defecto)
        *use_prefix = true;
        *use_suffix = false;
    }

    // El archivo JSON es el argumento posicional
    if (optind >= argc) {
        fprintf(stderr, "Error: Se requiere archivo JSON\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    *json_file = argv[optind];
}

// Función principal del programa
// Esta función es responsable de parsear los argumentos, leer el archivo JSON,
// crear el directorio raíz y construir la estructura dicotómica.
// También maneja la liberación de memoria y muestra mensajes de éxito o error.
int main(int argc, char *argv[]) {
    char *json_file = NULL;
    char *root_dir = NULL;
    char *true_text = NULL;
    char *false_text = NULL;
    
    // Dos booleanos para controlar prefijo y sufijo
    bool use_prefix;
    bool use_suffix;

    // Parsear argumentos de línea de comandos (pasamos las direcciones de los dos bools)
    parse_arguments(argc, argv, &json_file, &root_dir, &true_text, &false_text, &use_prefix, &use_suffix);

    // Parsear el archivo JSON
    SpeciesList *species_list = parse_json_file(json_file);
    if (!species_list) {
        fprintf(stderr, "Error: No se pudo parsear el archivo JSON\n");
        return EXIT_FAILURE;
    }

    // Crear directorio raíz (si no es '.')
    if (strcmp(root_dir, ".") != 0) {
        if (!create_directory(root_dir)) {
            fprintf(stderr, "Error: No se pudo crear el directorio raíz '%s'\n", root_dir);
            free_species_list(species_list);
            return EXIT_FAILURE;
        }
    }
    
    // Construir el árbol dicotómico
    // Pasamos la lista de especies, el directorio raíz y los textos para condiciones true/false
    build_dichotomic_tree(species_list, root_dir, true_text, false_text, use_prefix, use_suffix);

    // Liberar memoria
    free_species_list(species_list);

    // Mensaje de éxito
    printf("Estructura dicotómica creada exitosamente en: %s\n", root_dir);
    return EXIT_SUCCESS;
}

