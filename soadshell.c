#include <stdio.h>	// citiri, afisari
#include <stdlib.h>	// alocare de memorie
#include <string.h>	// siruri de catactere
#include <stdbool.h>	// pt. bool-uri

// #include <unistd.h> 			pentru fork-uri, copii, etc.
// #include <sys/wait.h>		tot pentru fork-uri


#define MAX_INPUT_SIZE 1024			// dimensiunea maxima a unei comenzi 
#define clear() printf("\033[H\033[J")		// pt stergerea ecranului

/*
		Logica programului:
	- Fiecare comanda citita va reprezenta un void** .
	- Pentru fiecare comanda introdusa (de tip void**), primul argument va 
	fi numele comenzii, iar restul argumentele (ultimul element va fi NULL).
	- De la void**, pt. fiecare comanda implementata, se fac conversiile necesare.
*/

void pwd_command(void **args) {
    printf("Executing pwd_command\n");
}

void ls_command(void **args) {
    printf("Executing ls_command\n");
}


// acesta o sa fie handler-ul de comenzi

void execute_command(void **command_args) {
	char * command = (char *)command_args[0];

	if (strcmp(command, "pwd") == 0) {
		pwd_command(command_args);
	} else if (strcmp(command, "ls") == 0) {
		ls_command(command_args);
	} else {
		printf("Unknown command: %s\n", command);
	}
    
}


// cu aceasta functie se vor citi comenzile 

void **read_command() {
    char *input = malloc(MAX_INPUT_SIZE);
    
    if (input == NULL) {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }

    printf("SoadShell> ");
    fgets(input, MAX_INPUT_SIZE, stdin);

    // eliminam \n de la finalul comenzii
    
    size_t length = strlen(input);
    if (length > 0 && input[length - 1] == '\n') {
        input[length - 1] = '\0';
    }

    // descompunem comanda in comanda si argumente
    
    char *token = strtok(input, " ");
    void **command_args = malloc(MAX_INPUT_SIZE); 
    
    if (command_args == NULL) {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (token != NULL) {
        command_args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    command_args[i] = NULL; 

    return command_args;
}

// functia main va citi comenzi pana la introducerea lui "exit"

int main() {

	clear();
	printf("\n \n \n              soadgang sHELL \n\n\n");
	while (true) {

		// citire comanda + argumente

		void **command_args = read_command();

		// iesirea din loop

		if (strcmp(command_args[0], "exit") == 0) {
		    free(command_args);
		    clear();
		    break;
		}

		// executia comenzii

		execute_command(command_args);

		// eliberarea memoriei

		free(command_args);
	}

	return 0;
}
