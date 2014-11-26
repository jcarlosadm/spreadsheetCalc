############ início da configuração ############################

# diretório dos objeto
OBJ_DIR= objects

# coloque aqui a lista de objetos do programa
_OBJ= mainMenu.o spreadsheet.o load.o save.o graphics_select.o graphics_user.o graphics_instructions.o graphics_cells.o matrix.o stack_binExpTree.o binary_expression_tree.o undo_redo_cells.o stack_double.o functions.o main.o

# coloque as depedências de header files de cada objeto
# veja o header file de cada objeto para uma pista dessas depedências
DEP_MAIN= mainMenu.h
DEP_MAINMENU= mainMenu.h spreadsheet.h graphics_instructions.h graphics_select.h load.h
DEP_SPREADSHEET= spreadsheet.h matrix.h undo_redo_cells.h save.h graphics_cells.h graphics_instructions.h graphics_user.h graphics_select.h
DEP_LOAD= load.h matrix.h graphics_instructions.h graphics_select.h
DEP_SAVE= save.h matrix.h graphics_instructions.h graphics_select.h graphics_user.h
DEP_GRAPHICSSELECT= graphics_select.h
DEP_GRAPHICSUSER= graphics_user.h
DEP_GRAPHICSINST= graphics_instructions.h
DEP_GRAPHICSCELLS= graphics_cells.h
DEP_MATRIX= graphics_instructions.h graphics_cells.h matrix.h binary_expression_tree.h stack_binExpTree.h functions.h undo_redo_cells.h
DEP_STACKBINEXPTREE= stack_binExpTree.h binary_expression_tree.h
DEP_BINARYEXPRESSIONTREE= binary_expression_tree.h stack_double.h
DEP_UNDOREDOCELLS= undo_redo_cells.h
DEP_STACKDOUBLE= stack_double.h
DEP_FUNCTIONS= functions.h

# as flags e opções usadas
CC= gcc
CFLAGS= -c -Wall
CLIBS= -lncurses -lmxml -pthread

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
	$(CC) -o $@ $^ $(CLIBS)

$(OBJ_DIR)/mainMenu.o: mainMenu.c $(DEP_MAINMENU)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/spreadsheet.o: spreadsheet.c $(DEP_SPREADSHEET)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/load.o: load.c $(DEP_LOAD)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/save.o: save.c $(DEP_SAVE)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/graphics_select.o: graphics_select.c $(DEP_GRAPHICSSELECT)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/graphics_user.o: graphics_user.c $(DEP_GRAPHICSUSER)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/graphics_instructions.o: graphics_instructions.c $(DEP_GRAPHICSINST)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/graphics_cells.o: graphics_cells.c $(DEP_GRAPHICSCELLS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/stack_binExpTree.o: stack_binExpTree.c $(DEP_STACKBINEXPTREE)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/matrix.o: matrix.c $(DEP_MATRIX)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/binary_expression_tree.o: binary_expression_tree.c $(DEP_BINARYEXPRESSIONTREE)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/undo_redo_cells.o: undo_redo_cells.c $(DEP_UNDOREDOCELLS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/stack_double.o: stack_double.c $(DEP_STACKDOUBLE)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/functions.o: functions.c $(DEP_FUNCTIONS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/main.o: main.c $(DEP_MAIN)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_NAME)
