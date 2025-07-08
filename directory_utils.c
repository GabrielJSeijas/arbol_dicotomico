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

//Este archivo contiene la lógica para crear directorios y archivos,
// construir nombres de directorios y manejar errores de creación.

// Función para crear un directorio con permisos específicos
int create_directory(const char* path) {
    int status = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status == -1) {
        if (errno != EEXIST) {
            fprintf(stderr, "Error al crear directorio %s: %s\n", path, strerror(errno));
            return 0;
        }
    }
    return 1;
}

// Función para crear un archivo de especie dentro de un directorio específico
int create_species_file(const char* dir_path, const char* species_name) {
    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/%s.txt", dir_path, species_name);
    
    int fd = open(file_path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        fprintf(stderr, "Error al crear archivo %s: %s\n", file_path, strerror(errno));
        return 0;
    }
    close(fd);
    return 1;
}

// Función para construir el nombre del directorio basado en la pregunta y el texto
char* build_dir_name(const char* question, const char* text, bool use_pre, bool use_suf) {
    // Calcular tamaño necesario
    size_t len = strlen(question);
    if (use_pre) len += strlen(text) + 1; // +1 para el espacio
    if (use_suf) len += strlen(text) + 1; // +1 para el espacio
    len += 1; // para el '\0'
    
    char* dir_name = malloc(len);
    if (!dir_name) { /* ... error ... */ return NULL; }
    
    dir_name[0] = '\0'; // Iniciar como string vacío

    if (use_pre) {
        strcpy(dir_name, text);
        strcat(dir_name, " ");
    }
    
    strcat(dir_name, question);

    if (use_suf) {
        strcat(dir_name, " ");
        strcat(dir_name, text);
    }
    
    return dir_name;
}