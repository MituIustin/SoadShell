#include <stdio.h>	// citiri, afisari
#include <stdlib.h>	// alocare de memorie
#include <string.h>	// siruri de catactere
#include <stdbool.h>	// pt. bool-uri

// #include <unistd.h> 			pentru fork-uri, copii, etc.
// #include <sys/wait.h>		tot pentru fork-uri


#define MAX_INPUT_SIZE 1024			// dimensiunea maxima a unei comenzi 
#define clear() printf("\033[H\033[J")		// pt stergerea ecranului

bool last_command_executed = false;

/*
		Logica programului:
	- Fiecare comanda citita va reprezenta un void** .
	- Pentru fiecare comanda introdusa (de tip void**), primul argument va 
	fi numele comenzii, iar restul argumentele (ultimul element va fi NULL).
	- De la void**, pt. fiecare comanda implementata, se fac conversiile necesare.
	- Fiecare functie va seta vatiabila globala last_command_executed false sau true.
*/


// TODO
void pwd_command(void **args) {
	printf("Executing pwd_command\n");
	last_command_executed = true;
	printf("Command executed!\n");

}

// TODO
void ls_command(void **args) {
    	printf("Executing ls_command\n");
	last_command_executed = false;
	printf("There was a problem!\n");
}


// acesta o sa fie handler-ul de comenzi

void execute_command(void **commands_args) {
	
	/* 
		commands_args va arata astfel : "nume_comanda1 arg1 arg2 ... && nume_comanda2 arg1 arg2 ...    etc.  ......)
		
		offsetul merge practic din comanda in comanda (separate de && si || )
		
		continue_execute este un bool logic care va spune daca trebuie sa se execute urmatoarele comenzi
			- in cazul in care ultima comanda s-a realizat cu succes si avem &&
			- in cazul in care nu s-a realizat cu succes ultima comanda si avem ||
	*/

	int args_offset = 0;
	bool continue_execute;

	// cu siguranta se va apela macar o functie

	do {
		// command este comanda care va trebui executata (ex: pwd, ls, exit)
		
		char *command = (char *)commands_args[args_offset];
		continue_execute = false;
		
		// args_num este numarul de argumente folosit pt. comanda de mai sus
		
		int args_num;
		
		/*
			apelam comanda corespunzatoare, folosind offset-ul si setand pentru 
		fiecare functie nr de argumente folosite
		*/
		
		if (strcmp(command, "pwd") == 0) {
			pwd_command(commands_args + args_offset);
			args_num = 2;
		} else if (strcmp(command, "ls") == 0) {
			ls_command(commands_args + args_offset);
			args_num = 1;
		} else {
			printf("Unknown command: %s\n", command);
			args_num = 0;
		}

		// adaugam la offset numarul de argumente folosite 

		args_offset += args_num;
		
		char * possible_operator = (char *)commands_args[args_offset];

		if ( possible_operator != NULL) {
			if (strcmp(possible_operator, "&&") == 0 && last_command_executed == true) {
				continue_execute = true;
			}

			if (strcmp(possible_operator, "||") == 0 && last_command_executed == false) {
				continue_execute = true;
			}
			
			// crestem offset-ul pentru a trece de && sau ||

			args_offset++;
		}

	} while (continue_execute == true);
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
