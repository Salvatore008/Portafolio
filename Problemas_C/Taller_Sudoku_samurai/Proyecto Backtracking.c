#include <stdio.h>


#define FILS 9
#define COLS 9

/*Proc. lee los sudokus de un archivo, en el archivo se encuentra en el siguiente orden:
	Sudoku superior izquierdo
	Sudoku superior derecho
	Sudoku inferior izquierdo
	Sudoku inferior derecho
	Sudoku central
*/
void Rellenar_Sudoks(int sudo[FILS][COLS], FILE* fp); 

/*Procs. resuelve el sudoku central*/
void Sol_Sudoku_Central(int sud[FILS][COLS], int sud_2[FILS][COLS], int sud_3[FILS][COLS], int sud_4[FILS][COLS], int sud_5[FILS][COLS], int fil, int col, int* sol_encontrada);
int Num_Valido_Compart(int sud[FILS][COLS], int sud_2[FILS][COLS], int sud_3[FILS][COLS], int sud_4[FILS][COLS], int sud_5[FILS][COLS], int num, int fil, int col);
void Es_Sol_Compart(int sud[FILS][COLS], int sud_2[FILS][COLS], int sud_3[FILS][COLS], int sud_4[FILS][COLS], int sud_5[FILS][COLS], int fil, int col, int* sol_encontrada);

/*Proc. copia las esquinas compartidas del sudoku central en los sudokus exteriores*/
void Rellenar_Esquinas(int sud[FILS][COLS], int sud_2[FILS][COLS], int sud_3[FILS][COLS], int sud_4[FILS][COLS], int sud_5[FILS][COLS]);

/*Procs. resuelve los sudokus exteriores*/
void Sol_Sudoku(int sud[FILS][COLS], int fil, int col, int* sol_encontrada, int esquina); //Resuelve los sudokus exteriores
void Celda_Vacia(int sud[FILS][COLS], int* fil, int* col, int* sol_encontrada, int esquina);
void Es_Sol_Ext(int sud[FILS][COLS], int fil, int col, int* sol_encontrada, int esquina);
int Num_Valido(int sud[FILS][COLS], int num, int fil, int col); //Valida el numero a colocar

/*Muestra un sudoku*/
void Mostrar_Sol(int sud[FILS][COLS]);

/*Muestra todos los sudokus*/
void Mostrar_Sol_Compart(int sud[FILS][COLS], int sud_2[FILS][COLS], int sud_3[FILS][COLS], int sud_4[FILS][COLS], int sud_5[FILS][COLS]);


int main(){
	FILE* fp;
	/*representa si un sudoku esta resuelto o no, despues de cada sudoku se restablece a 0*/
	int sol_parcial = 0;
	
	/*Matricez, representan los sudokus*/
	int sudoku_1[FILS][COLS];	
	int sudoku_2[FILS][COLS];
	int sudoku_3[FILS][COLS];
	int sudoku_4[FILS][COLS];
	int sudoku_5[FILS][COLS];
	
	/*Valida que el archivo se habra correctamente*/
	if((fp = fopen("Sudoku.bin", "r")) == NULL){
		printf("ERROR al abrir el archivo");
	}
	else{		
			
		/*Lee los sudokus de un archivo*/
		Rellenar_Sudoks(sudoku_1, fp);
		Rellenar_Sudoks(sudoku_2, fp);
		Rellenar_Sudoks(sudoku_3, fp);
		Rellenar_Sudoks(sudoku_4, fp);
		Rellenar_Sudoks(sudoku_5, fp);
		fclose(fp);
	

		/*Muestra los sudokus sin resolver*/
		Mostrar_Sol_Compart(sudoku_5, sudoku_1, sudoku_2, sudoku_3, sudoku_4); printf("\n");

		/*Resuelve el sudoku central*/
		Sol_Sudoku_Central(sudoku_5, sudoku_1, sudoku_2, sudoku_3, sudoku_4, 0, 0, &sol_parcial); 	sol_parcial = 0;
		Rellenar_Esquinas(sudoku_5, sudoku_1, sudoku_2, sudoku_3, sudoku_4);
	
		/*Resuelve los sudokus exteriores(partiendo de una esquina resulta)*/
		Sol_Sudoku(sudoku_1, 8, 8, &sol_parcial, 1); sol_parcial = 0; 
		Sol_Sudoku(sudoku_2, 8, 0, &sol_parcial, 2); sol_parcial = 0; 
		Sol_Sudoku(sudoku_3, 0, 8, &sol_parcial, 3); sol_parcial = 0; 
		Sol_Sudoku(sudoku_4, 0, 0, &sol_parcial, 4); sol_parcial = 0; 
		
		Mostrar_Sol_Compart(sudoku_5, sudoku_1, sudoku_2, sudoku_3, sudoku_4);	
	}

	printf("\nAlumnos de programacion I:\n Luigi Quero C.I: 30.009.785\n Alejandro Cerpa C.I: 30.334.870"); getchar();
	return 0;
}


/*Avanza celdas de los sudokus(dependiendo de cual sea) que tengan numeros*/
void Celda_Vacia(int sud[FILS][COLS], int* fil, int* col, int* sol_encontrada, int esquina){
	/*Esquina superior izquierda del sudoku central*/
	if(esquina == 1){
		while(sud[*fil][*col] != 0){	
			if(*col > 0){
				*col -= 1; 
			}
			else if(*fil > 0){
				*fil -= 1;
				*col = COLS - 1;
			}
			else{
				*sol_encontrada = 1;
			}
		}
	}
	/*Esquina superior derecha del sudoku central*/
	else if(esquina == 2){
		while(sud[*fil][*col] != 0){	
			if(*col < COLS - 1){
				*col += 1; 
			}
			else if(*fil > 0){
				*fil -= 1;
				*col = 0;
			}
			else{
				*sol_encontrada = 1;
			}
		}		
	}
	/*Esquina inferior izquierda del sudoku central*/
	else if(esquina == 3){
		while(sud[*fil][*col] != 0){	
			if(*col > 0){
				*col -= 1; 
			}
			else if(*fil < FILS - 1){
				*fil += 1;
				*col = COLS - 1;
			}
			else{
				*sol_encontrada = 1;
			}
		}			
	}
	/*Esquina inferior derecha del sudoku central*/
	else if(esquina == 4){
		while(sud[*fil][*col] != 0){	
			if(*col < COLS - 1){
				*col += 1; 
			}
			else if(*fil < FILS - 1){
				*fil += 1;
				*col = 0;
			}
			else{
				*sol_encontrada = 1;
			}
		}	
	}
}

/*Valida la solucion de un sudoku exterior(depende de cual sudoku sea)*/
void Es_Sol_Ext(int sud[FILS][COLS], int fil, int col, int* sol_encontrada, int esquina){
	/*Sudoku superior izquierdo*/
	if(esquina == 1){
		if(col > 0){
			Sol_Sudoku(sud, fil, col - 1, sol_encontrada, esquina);
		}
		else if(fil > 0){
			Sol_Sudoku(sud, fil - 1, COLS - 1, sol_encontrada, esquina);
		}
		else{
			*sol_encontrada = 1;
		}
	}
	/*Sudoku superior derecho*/
	else if(esquina == 2){
		if(col < COLS - 1){
			Sol_Sudoku(sud, fil, col + 1, sol_encontrada, esquina);
		}
		else if(fil > 0){
			Sol_Sudoku(sud, fil - 1, 0, sol_encontrada, esquina);
		}
		else{
			*sol_encontrada = 1;
		}		
	}
	/*Sudoku inferior izquierdo*/
	else if(esquina == 3){
		if(col > 0){
		Sol_Sudoku(sud, fil, col - 1, sol_encontrada, esquina);
		}
		else if(fil < FILS - 1){
			Sol_Sudoku(sud, fil + 1, COLS - 1, sol_encontrada, esquina);
		}
		else{
			*sol_encontrada = 1;
		}		
	}
	/*Sudoku inferior derecho*/
	else if(esquina == 4){
		if(col < COLS - 1){
		Sol_Sudoku(sud, fil, col + 1, sol_encontrada, esquina);
		}
		else if(fil < FILS - 1){
			Sol_Sudoku(sud, fil + 1, 0, sol_encontrada, esquina);
		}
		else{
			*sol_encontrada = 1;
		}		
	}
}

/*lee los sudokus de un archivo y los guarda en una matriz*/
void Rellenar_Sudoks(int sudo[FILS][COLS], FILE* fp){
	int i, j, aux;
	
	for(i = 0; i < FILS; i++){
		for(j = 0; j < COLS; j++){
			fscanf(fp, "%i", &aux);
			sudo[i][j] = aux;
		}
	}
}

/*Resuelve el sudoku central, el cual comparte esquinas con los 4 sudokus exteriores*/
void Sol_Sudoku_Central(int sud[FILS][COLS], int sud_2[FILS][COLS], int sud_3[FILS][COLS], int sud_4[FILS][COLS], int sud_5[FILS][COLS], int fil, int col, int* sol_encontrada){
	int num = 9;
	
	while(sud[fil][col] != 0){	
		if(col < COLS - 1){
			col++; 
		}
		else if(fil < FILS - 1){
			fil++;
			col = 0;
		}
		else{
			*sol_encontrada = 1;
		}
	}
	
	while(num > 0 && !*sol_encontrada){
		if(Num_Valido_Compart(sud, sud_2, sud_3, sud_4, sud_5, num, fil, col)){ 
			sud[fil][col] = num;
			
			Es_Sol_Compart(sud, sud_2, sud_3, sud_4, sud_5, fil, col, sol_encontrada);
				
			if(!*sol_encontrada){	
				sud[fil][col] = 0;
			}	
		}
		num--;	
	}				
}

/*Valida que el numero a colocar en el sudoku central sea valido tanto para el mismo como para el sudoku con el que comparte esquina*/
int Num_Valido_Compart(int sud[FILS][COLS], int sud_2[FILS][COLS], int sud_3[FILS][COLS], int sud_4[FILS][COLS], int sud_5[FILS][COLS], int num, int fil, int col){
	int ban = 1;

	if(!Num_Valido(sud, num, fil, col)){
		ban = 0;
	}
	else if(fil < 3 && col < 3 && !Num_Valido(sud_2, num, fil + 6, col + 6)){
			ban = 0;
	}
	else if(fil < 3 && col > COLS - 4 && !Num_Valido(sud_3, num, fil + 6, col - 6)){
			ban = 0;
	}
	else if(fil > FILS - 4 && col < 3 && !Num_Valido(sud_4, num, fil - 6, col + 6)){
			ban = 0;
	}
	else if(fil > FILS - 4 && col > COLS - 4 && !Num_Valido(sud_5, num, fil - 6, col - 6)){
			ban = 0;
	}
	
	return ban;
}

/*Valida la solucion del sudoku central*/
void Es_Sol_Compart(int sud[FILS][COLS], int sud_2[FILS][COLS], int sud_3[FILS][COLS], int sud_4[FILS][COLS], int sud_5[FILS][COLS], int fil, int col, int* sol_encontrada){
	if(col < COLS - 1){
		Sol_Sudoku_Central(sud, sud_2, sud_3, sud_4, sud_5, fil, col + 1, sol_encontrada);
	}
	else if(fil < FILS - 1){
		Sol_Sudoku_Central(sud, sud_2, sud_3, sud_4, sud_5, fil + 1, 0, sol_encontrada);
	}
	else{
		*sol_encontrada = 1;
	}	
}

/*Copia las esquinas ya resultas del sudoku central en los sudokus exteriores*/
void Rellenar_Esquinas(int sud[FILS][COLS], int sud_2[FILS][COLS], int sud_3[FILS][COLS], int sud_4[FILS][COLS], int sud_5[FILS][COLS]){
	int i, j, pos_x, pos_y;
	/*
		sud-sudoku central
		sud_2-sudoku superior izquierdo
		sud_3-sudoku superior derecho
		sud_4-sudoku inferior izquierdo
		sud_5-sudoku inferior derecho
	*/
	/*Esquina superior izquierda del sudoku central*/
	 pos_x = 0; pos_y = 0;
	for(i = FILS - 3; i < FILS; i++){
		for(j = COLS - 3; j < COLS; j++){
			sud_2[i][j] = sud[pos_y][pos_x];
			pos_x++;
		}
		pos_x = 0;
		pos_y++;
	}
	
	/*Esquina superior derecha del sudoku central*/
	pos_x = COLS - 3; pos_y = 0; 
	for(i = FILS - 3; i < FILS; i++){
		for(j = 0; j < 3; j++){
			sud_3[i][j] = sud[pos_y][pos_x];
			pos_x++;
		}
		pos_x = COLS - 3;
		pos_y++;
	}
	
	/*Esquina inferior izquierda del sudoku central*/
	pos_x = 0; pos_y = FILS - 3;
	for(i = 0; i < 3; i++){
		for(j = COLS - 3; j < COLS; j++){
			sud_4[i][j] = sud[pos_y][pos_x];
			pos_x++;
		}
		pos_x = 0;
		pos_y++;
	}
	
	/*Esquina inferior derecha del sudoku central*/
	pos_x = FILS - 3; pos_y = COLS - 3; 
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			sud_5[i][j] = sud[pos_y][pos_x];
			pos_x++;
		}
		pos_x = FILS - 3;
		pos_y++;
	}
}

/*Resuelve un sudoku exterior(cada sudoku inicia desde una esquina resuelta por el sudoku central)*/
void Sol_Sudoku(int sud[FILS][COLS], int fil, int col, int* sol_encontrada, int esquina){
	int num = 1;
	
	Celda_Vacia(sud, &fil, &col, sol_encontrada, esquina);
	
	while(num <= 9 && !*sol_encontrada){
		if(Num_Valido(sud, num, fil, col)){ 
			sud[fil][col] = num;
			
			Es_Sol_Ext(sud, fil, col, sol_encontrada, esquina);
				
			if(!*sol_encontrada){	
				sud[fil][col] = 0; 
			}	
		}
		num++;	
	}				
}

/*Valida que el numero sea valido dentro de su fila y su columna*/
int Num_Valido(int sud[FILS][COLS], int num, int fil, int col){
	int i;
	
	for(i = 0; i < FILS; i++){
		if(sud[fil][i] == num || sud[i][col] == num){
			return 0;
		}
	}
	
	return 1;
}

/*Muestra un sudoku en pantalla*/
void Mostrar_Sol(int sud[FILS][COLS]){
	int i, j;
	
	for(i = 0; i < FILS; i++){
		printf("\t    ");
		for(j = 0; j < COLS; j++){
			if(j < COLS - 1){
				printf(" %i", sud[i][j]);
			}
			else{
				printf(" %i\n", sud[i][j]);
			}
		}
	}
}

/*Muestra en pantalla todos los sudokus*/
void Mostrar_Sol_Compart(int sud[FILS][COLS], int sud_2[FILS][COLS], int sud_3[FILS][COLS], int sud_4[FILS][COLS], int sud_5[FILS][COLS]){
	int i, j;
	
	/*Imprime los sudokus superiores y la parte superior intermedia del sudoku central*/
	for(i = 0; i < FILS; i++){
		for(j = 0; j < COLS; j++){
			printf(" %i", sud_2[i][j]); // Sudoku superior izquierdo
		}
		if(i >= FILS - 3){
			for(j = 3; j < COLS - 3; j++){
				printf(" %i", sud[i - 6][j]); // Sudoku central, parte superior intermedia
			}
		}	
		else{
			printf("\t");
		}
		for(j = 0; j < COLS; j++){
			printf(" %i", sud_3[i][j]);	// Sudoku superior derecho	
		}
		printf("\n");
	}
	
	/*Imprime en pantalla la parte media del sudoku central*/
	for(i = 3; i < FILS - 3; i++){
		printf("\t    ");
		for(j = 0; j < COLS; j++){
			printf(" %i", sud[i][j]);
		}
		printf("\n");
	}
	
	/*Imprime en pantalla los sudokus inferiores y la parte inferior intermedia del sudoku central*/
	for(i = 0; i < FILS; i++){
		for(j = 0; j < COLS; j++){
			printf(" %i", sud_4[i][j]); // Sudoku inferior izquierdo
		}
		if(i < 3){
			for(j = 3; j < COLS - 3; j++){
				printf(" %i", sud[i + 6][j]); // Sudoku central, parte inferior intermedia
			}
		}	
		else{
			printf("\t");
		}
		for(j = 0; j < COLS; j++){
			printf(" %i", sud_5[i][j]);	// Sudoku inferior derecho
		}
		printf("\n");
	}
}