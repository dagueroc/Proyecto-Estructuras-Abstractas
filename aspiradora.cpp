#include <iostream>

using namespace std;

void JugarPartida();
void JugarContraAspi();
void LimpiarTablero();
void LimpiarPantalla();
void MostrarTablero();
void UbicarFicha();
void UbicarFichaAspi();
bool RevisarHorizontal(int, int, char);
bool RevisarVertical(int, int, char);
bool RevisarDiagonal(int, int, char);
bool RevisarDiagonalInvertida(int, int, char);

//Para Linux
/*char tablero[8][9]={{{'#'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} , {'#'}},
					{{'#'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} , {'#'}},
					{{'#'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} , {'#'}},
					{{'#'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} , {'#'}},
					{{'#'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} , {'#'}},
					{{'#'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} , {'#'}},
					{{'#'},{'#'},{'#'},{'#'}, {'#'}, {'#'}, {'#'}, {'#'} , {'#'}},
					{{' '},{'1'},{'2'},{'3'}, {'4'}, {'5'}, {'6'}, {'7'} , {' '}},};*/
//Para Windows
char tablero[8][9]={{'#',' ',' ',' ', ' ', ' ', ' ', ' ' , '#'},
			{'#',' ',' ',' ', ' ', ' ', ' ', ' ' , '#'},
			{'#',' ',' ',' ', ' ', ' ', ' ', ' ' , '#'},
			{'#',' ',' ',' ', ' ', ' ', ' ', ' ' , '#'},
			{'#',' ',' ',' ', ' ', ' ', ' ', ' ' , '#'},
			{'#',' ',' ',' ', ' ', ' ', ' ', ' ' , '#'},
			{'#','#','#','#', '#', '#', '#', '#' , '#'},
			{' ','1','2','3', '4', '5', '6', '7' , ' '}};

char eleccion;
char turno;
int colT, colD;
int indD;
int cont = 0;
int h , k;
bool listo = false;
bool jugandoContraAspi = false; 

int main(){
    srand(time(0));
	while(true){
		LimpiarPantalla();
		cout<<"\t:::CUATRO EN LINEA:::"<<endl;
		cout<<"\t\t1) Jugar contra otro jugador."<<endl;
        cout<<"\t\t2) Jugar contra Aspi."<<endl;
		cout<<"\t\t3) Salir."<<endl;
		cout<<"\tEleccion: ";
		cin>>eleccion;
		switch(eleccion){
			case '1':
                jugandoContraAspi = false;
				turno = 'O';
				JugarPartida();
				LimpiarTablero();
			    break;
			case '2':
                jugandoContraAspi = true; 
				turno = 'O';
                JugarContraAspi(); 
                LimpiarTablero();
                break;
            case '3': 
				 return 0;
			default:
			cout<<"Opcion no disponible. Ingrese algun caracter para continuar: ";
			cin>>eleccion;
			break;
		}
	}
	return 0;
}

void JugarContraAspi(){
    LimpiarPantalla();
    MostrarTablero();
    if(RevisarHorizontal(indD, colD, turno) || RevisarVertical(indD, colD, turno) || RevisarDiagonal(indD, colD, turno) || RevisarDiagonalInvertida(indD, colD, turno)){
        cout << "\tIngrese algun caracter para continuar: ";
        cin >> eleccion;
        return;
    }
    if(turno == 'X'){
        turno = 'O';
    }else{
        turno = 'X';
    }
    if(turno == 'O'){
        cout << "\n\tTurno del jugador" << endl;
        cout << "\tElija una columna: ";
        cin >> colT;
        if(colT > 7 || colT < 1){
            cout << "\n\tColumna Inexistente." << endl;
            cout << "\tIngrese algun caracter para continuar: ";
            cin >> eleccion;
            JugarContraAspi();
        }else{
            colD = colT;
            UbicarFicha();
        }
    } else {
		UbicarFichaAspi();
    }
    JugarContraAspi();
}

void UbicarFichaAspi(){
    do {
        colD = rand() % 7 + 1;
    } while(tablero[0][colD] != ' ');
    for(int i = 5; i >= 0; i--){
        if(tablero[i][colD] == ' '){
            tablero[i][colD] = turno;
            indD = i;
            break;
        }
    }
    cout << "\n\tAspi jugó en la columna: " << colD << endl;
}


void LimpiarPantalla(){
	/*Para Linux
	if(system("clear") == -1){
		cout<<"Error al limpiar la pantalla."<<endl;
		LimpiarPantalla();
	}*/
	//Para Windows
	if(system("cls") == -1){
		cout<<"Error al limpiar la pantalla."<<endl;
		LimpiarPantalla();
	}
}

void LimpiarTablero(){
	for(int i = 0; i < 6; i++){
		for(int j = 1; j < 8; j++){
			tablero[i][j] = ' ';
		}
	}
}

void JugarPartida(){
	LimpiarPantalla();
	MostrarTablero();
	
	if(RevisarHorizontal(indD, colD, turno) || RevisarVertical(indD, colD, turno)  || RevisarDiagonal(indD,colD,turno) || RevisarDiagonalInvertida(indD,colD,turno)){
		cout<<"\tIngrese algun caracter para continuar: ";
		cin>>eleccion;
		return;
	}
	
	if(turno == 'X'){
		turno = 'O';
	}else{
		turno = 'X';
	}
	
	cout<<"\n\tTurno de "<<turno<<endl;
	cout<<"\tElija una columna: ";
	cin>>colT;
	if(colT > 7 || colT < 1){
		cout<<"\n\tColumna Inexistente."<<endl;
		cout<<"\tIngrese algun caracter para continuar: ";
		cin>>eleccion;
		JugarPartida();
	}else{
		colD = colT;
		UbicarFicha();
	}
	JugarPartida();
}

void MostrarTablero(){
	cout<<"\t:::CUATRO EN LINEA:::"<<endl;
	for(int i = 0; i < 8; i ++){
		cout<<endl<<"\t";
		for(int j = 0; j < 9; j++){
			cout<<tablero[i][j]<<" ";
		}
	}
}

void UbicarFicha(){
	for(int i = 0; i < 6; i++){
		if(tablero[i][colD] == ' '){
			if(i > 0){
				tablero[i-1][colD] = ' ';
			}
			tablero[i][colD] = turno;
			indD = i;
		}
	}
}

bool RevisarHorizontal(int i, int j, char t){
	cont = 1;
	h = j;
	listo = false;
	//Izquierda
	while(!listo){
		h --;
		if(h > 0){
			if(tablero[i][h] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	h = j;
	listo = false;
	//Derecha
	while(!listo){
		h ++;
		if(h < 8){
			if(tablero[i][h] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	if(cont >= 4){
		cout<<"\n\tGano el jugador "<<t<<endl;
		return true;
	}
	return false;
}

bool RevisarVertical(int i, int j , char t){
	cont = 1;
	h = i;
	listo = false;
	while(!listo){
		h++;
		if(h < 6){
			if(tablero[h][j] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	if(cont >= 4){
		cout<<"\n\tGano el jugador "<<t<<endl;
		return true;
	}
	return false;
}

bool RevisarDiagonal(int i, int j, char t){
	cont = 1;
	h = i;
	k = j;
	listo = false;
	//Izquierda
	while(!listo){
		h ++;
		k --;
		if(h < 6 && k > 0){
			if(tablero[h][k] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	h = i;
	k = j;
	listo = false;
	//Derecha
	while(!listo){
		h --;
		k ++;
		if(h > 0 && k < 8){
			if(tablero[h][k] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	if(cont >= 4){
		cout<<"\n\tGano el jugador "<<t<<endl;
		return true;
	}
	return false;
}


bool RevisarDiagonalInvertida(int i, int j, char t){
	cont = 1;
	h = i;
	k = j;
	listo = false;
	//Derecha
	while(!listo){
		h ++;
		k ++;
		if(h < 6 && k < 8){
			if(tablero[h][k] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	h = i;
	k = j;
	listo = false;
	//Izquierda
	while(!listo){
		h --;
		k --;
		if(h > 0 && k > 0){
			if(tablero[h][k] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	if(cont >= 4){
		cout<<"\n\tGano el jugador "<<t<<endl;
		return true;
	}
	return false;
}