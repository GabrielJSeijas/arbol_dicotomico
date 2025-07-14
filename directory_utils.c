#include "directory_utils.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h> 

// Crea un directorio en la ruta indicada con permisos específicos.
// Devuelve 1 si se creó correctamente o ya existe, 0 si hubo error.
int create_directory(const char* path) {
    int status = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status == -1) {
        // Si el error no es porque ya existe, muestra el error y retorna 0.
        if (errno != EEXIST) {
            fprintf(stderr, "Error al crear directorio %s: %s\n", path, strerror(errno));
            return 0;
        }
    }
    return 1;
}

// Crea un archivo de texto para una especie dentro de un directorio.
// El archivo se llama "<species_name>.txt".
// Devuelve 1 si se creó correctamente, 0 si hubo error.
int create_species_file(const char* dir_path, const char* species_name) {
    char file_path[1024];
    // Construye la ruta completa del archivo.
    snprintf(file_path, sizeof(file_path), "%s/%s.txt", dir_path, species_name);
    
    // Crea el archivo con permisos de escritura y lectura.
    int fd = open(file_path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        fprintf(stderr, "Error al crear archivo %s: %s\n", file_path, strerror(errno));
        return 0;
    }
    close(fd);
    return 1;
}

// Construye el nombre de un directorio usando una pregunta y un texto extra.
// Si use_pre es true, pone el texto antes de la pregunta.
// Si use_suf es true, pone el texto después de la pregunta.
// Devuelve el nombre construido como string dinámico (hay que liberar después).
char* build_dir_name(const char* question, const char* text, bool use_pre, bool use_suf) {
    // Calcula el tamaño necesario para el string final.
    size_t len = strlen(question);
    if (use_pre) len += strlen(text) + 1; // espacio extra si va antes
    if (use_suf) len += strlen(text) + 1; // espacio extra si va después
    len += 1; // para el '\0'
    
    char* dir_name = malloc(len);
    if (!dir_name) { // Si no hay memoria, retorna NULL.
        return NULL;
    }
    
    dir_name[0] = '\0'; // Inicializa el string vacío.

    // Si use_pre, agrega el texto antes de la pregunta.
    if (use_pre) {
        strcpy(dir_name, text);
        strcat(dir_name, " ");
    }
    
    // Agrega la pregunta.
    strcat(dir_name, question);

    // Si use_suf, agrega el texto después de la pregunta.
    if (use_suf) {
        strcat(dir_name, " ");
        strcat(dir_name, text);
    }
    
    return dir_name;
}