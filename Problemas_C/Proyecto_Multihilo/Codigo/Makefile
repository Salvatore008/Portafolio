# Makefile para compilar archivos en el directorio actual 

# Nombre del ejecutable. Deseable en mayusculas
TARGET = proyecto
# Bibliotecas incluidas, la biblioteca math.h es una muy comun
LIBS = -lm
# Compilador utilizado, por ejemplo icc, pgcc , gcc
CC = gcc
# Banderas del compilador, por ejemplo -DDEBUG -02 -03 -wall -g
CFLAGS = -std=c17

# Palabras que usa el Makefile que podrian ser el nombre de un Programa
.PHONY: default all clean

# Compilacion por defecto
default: $(TARGET)
all: default

# Incluye los archivos .o y .c que estan en el directorio actual
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

# Incluye los archivos.h que estan en el directorio actual
HEADERS = $(wildcard *.hpp) **/*.hpp

# compila automaticamente solo archivos fuente que se han Modificado
# $< es el primer prerrequisito, generalmente el archivofuente
# $@ nombre del archivo que se esta generando, archivo objeto
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Preserva archivos intermedios
.PRECIOUS: $(TARGET) $(OBJECTS)

# Enlaza objetos y crea el ejecutable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) $(CFLAGS) -o $@

# Borra archivos .o
clean:
	-rm -f *.o core

# Borra archivos .o y el ejecutable
cleanall: clean
	-rm -f $(TARGET)
