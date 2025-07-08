# Compilador y flags
CC = gcc

# Flags del compilador
# -Wall: Muestra todas las advertencias
# -Wextra: Muestra advertencias adicionales
# -std=c11: Usa el estándar C11
# -g: Incluye información de depuración
CFLAGS = -Wall -Wextra -std=c11 -g

# Flags de enlace
# -ljson-c: Enlaza con la biblioteca json-c para manejar JSON
LDFLAGS = -ljson-c

# Nombre del ejecutable final
TARGET = dicotodir

# Lista de todos los archivos fuente .c
SOURCES = main.c json_parser.c directory_utils.c tree_builder.c

# Convierte la lista de .c a una lista de .o
OBJECTS = $(SOURCES:.c=.o)

# Regla por defecto (make)
all: $(TARGET)

# Regla para construir el ejecutable final
# Depende de TODOS los archivos objeto (.o)
# Enlaza todos los .o para crear el TARGET
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Regla genérica para construir un archivo .o a partir de un .c
# El flag -c es CRUCIAL. Le dice a gcc "solo compila, no enlaces".
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Regla para limpiar los archivos generados (make clean)
clean:
	rm -f $(TARGET) $(OBJECTS)

# Declara que 'all' y 'clean' no son archivos reales
.PHONY: all clean