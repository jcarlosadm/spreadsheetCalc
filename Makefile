############ início da configuração ############################

# diretórios dos objetos e do binário
# tenha certeza de configurar a localização do binário nas opções de execução do eclipse
OBJ_DIR= objects
#BIN_DIR= bin

# coloque aqui a lista de objetos do programa
_OBJ= main.o

# coloque as depedências de header files de cada objeto (exceto o header do próprio objeto)
# veja o header file de cada objeto para uma pista dessas depedências
DEP_MAIN= 

# as flags e opções usadas
CC= gcc
CFLAGS= -c -Wall

# nome do binário gerado
BIN_NAME= main

############ fim da configuração ###############################

# gera lista de objetos com caminhos relativos na pasta de objetos
OBJ= $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

# comando para criar diretórios
MK_DIR= mkdir -p

.PHONY: makedir_objects
.PHONY: makedir_bin

all: makedir_objects $(BIN_NAME)

makedir_objects:
	$(MK_DIR) $(OBJ_DIR)

$(BIN_NAME): $(OBJ)
	$(CC) -o $@ $^

# modelo para cada objeto diferente de main
#$(OBJ_DIR)/date.o: dateC/date.c $(DEP_DATE)
#	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/main.o: main.c $(DEP_MAIN)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_NAME)
