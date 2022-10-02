#Our CC and CFlags
ARGS = gcc -g -ansi -Wall -pedantic

UTILS = source/utilities
TABLES = source/tables

UTILSRC = converters.o formatter.o input.o textverify.o errors.o writer.o
TABLESRC = labelTable.o LabelReferenceTable.o dataTable.o codeTable.o
MAINSRC = main.o preprocessor.o compiler.o

#All our .o files
OBJS = $(MAINSRC) $(UTILSRC) $(TABLESRC)


run: assembler clean

assembler:$(OBJS)
	$(ARGS)  $(OBJS) -lm -o assembler 

#Compile external files
#UTILITIES
formatter.o: $(UTILS)/formatter.c 
	$(ARGS) -c $(UTILS)/formatter.c 

input.o: $(UTILS)/input.c 
	$(ARGS) -c $(UTILS)/input.c

converters.o: $(UTILS)/converters.c 
	$(ARGS) -c $(UTILS)/converters.c 

textverify.o: $(UTILS)/textverify.c
	$(ARGS) -c $(UTILS)/textverify.c

errors.o: $(UTILS)/errors.c
	$(ARGS) -c $(UTILS)/errors.c

writer.o: $(UTILS)/writer.c
	$(ARGS) -c $(UTILS)/writer.c

#TABLES
labelTable.o: $(TABLES)/labelTable.c
	$(ARGS) -c $(TABLES)/labelTable.c

LabelReferenceTable.o: $(TABLES)/LabelReferenceTable.c
	$(ARGS) -c $(TABLES)/LabelReferenceTable.c

dataTable.o: $(TABLES)/dataTable.c
	$(ARGS) -c $(TABLES)/dataTable.c

codeTable.o: $(TABLES)/codeTable.c
	$(ARGS) -c $(TABLES)/codeTable.c


#MAIN FILES
preprocessor.o: source/preprocessor.c
	$(ARGS) -c source/preprocessor.c

compiler.o: source/compiler.c 
	$(ARGS) -c source/compiler.c

main.o: source/main.c 
	$(ARGS) -c source/main.c

#Clean all .o files
clean:
	rm -f $(OBJS) 

