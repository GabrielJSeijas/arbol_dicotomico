# Compilador que vamos a usar
CC = gcc

# Opciones para compilar: avisos, estándar C11 y depuración
CFLAGS = -Wall -Wextra -std=c11 -g

# Opciones para enlazar: usamos la librería json-c
LDFLAGS = -ljson-c

# Nombre del programa que se va a generar
TARGET = dicotodir

# Archivos fuente del proyecto
SOURCES = main.c json_parser.c directory_utils.c tree_builder.c

# Archivos objeto que se generan a partir de los fuentes
OBJECTS = $(SOURCES:.c=.o)

# Regla principal: si escribo 'make', se compila todo y se genera el ejecutable
all: $(TARGET)

# Para crear el ejecutable, primero se compilan todos los .o y después se enlazan juntos
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Regla para compilar cada archivo .c en su .o correspondiente
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Si quiero borrar todo lo generado (ejecutable y objetos), uso 'make clean'
clean:
	rm -f $(TARGET) $(OBJECTS)

# Esto indica que 'all' y 'clean' no son archivos, sino comandos especiales
.PHONY: all clean
