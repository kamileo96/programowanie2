#include <iostream>
#include <random>
#include <ctime>
using namespace std;

char NumToChar(int num){
    switch (num)
    {

    case 0:
        return ' ';
    case 1:
        return 'X';
    case 2:
        return 'O';
    default:
        return '?';
    }
}

void Wypisz(int Board[9]){
    int i = 0;
    for(int j = 0; j<3; j++){
        for(int k = 0; k<2; k++){
            cout << NumToChar(Board[i]) << " | ";
            i++;
        }
        cout << NumToChar(Board[i]) << "\n";
        i++;
        if(j<2){cout << "---------\n";}
    }
}
int CoordsToIndex(string crd){
    //Poprawne pola only!
    //a1 => 0
    //c3 => 8 etc
    int idx = 0;
    idx += (crd[1]-49)*3;
    idx += int(crd[0]) - 97;
    return idx;
}
string CoordsFromIndex(int idx){
    string field;
    field[0] = char(idx%3 + 97);
    field[1] = char(idx/3 + 49);
    return field;
}
bool CheckCoordinates(string crd){
    if(!islower(crd[0]) || !isdigit(crd[1])){
        cout << "Incorrect coordinate format. Please use a smaller case letter followed by a digit.\n";
        return false;
        //ten if jest zasadniczo nie potrzebny, bo i tak sprawdzamy czy ascii jest pomiędzy a-c, 1-3, 
        //ale może warto dać odpowiedni error (w formie couta bo nie opanowaliśmy error handling w c++)
    }
    int w = int(crd[0]) - 97;
    int k = crd[1]-49;
    if(w<0 || w>2 || k<0 || k>2){
        cout << "Thou hath entered a nonexistent coordinate.\n";
        return false; 
    }
    return true;
}
bool ChangeCoordValue(int BoardFields[], string crd, int v){
    if(!CheckCoordinates(crd)){
        return false;
    }
    int idx = CoordsToIndex(crd);
    if (BoardFields[idx] > 0){
        cout << "You can't just overwrite squares. Pick an empty one instead.\n";
        return false;
    }
    BoardFields[idx] = v;
    return true;
}

int NthFreeIndex(int currentBoard[9], int n){
    //0 <= n < liczba wolnych pól
    
    int k = 0;
    for(int i = 0; i<9; i++){
        if (currentBoard[i] == 0){
            if(k == n){
                return i;
            }
            else{
                k++;
            }
        }
    }
    cout << "Error: less free squares than predicted\n";
    return 0;
}

int EasyGame(){
    //random_device rd;
	//default_random_engine engine(rd()); nie działa na windowsie
    default_random_engine engine{static_cast<long unsigned int>(time(0))};
    int free = 9;
    bool keepPlaying = true;
	int currentBoard[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    string cord;
    uniform_int_distribution<> coin(0,1);
    uniform_int_distribution<> field(0,9);
    int coinFlip = coin(engine);
    bool playerTurn = true;
    int playerValue;
    if(coinFlip){
        cout << "a---b---c\n";
        Wypisz(currentBoard);
        cout << "You start and play as 'X'. Pick a square from a1 to c3.\n\n";
        playerValue = 1;
    }
    else
    {
        int n = field(engine);
        currentBoard[n] = 1;
        cord = CoordsFromIndex(n);
        cout << "The computer started and drew an 'X' on " << cord << ".\n" << "a---b---c\n\n";
        Wypisz(currentBoard);
        cout << "It's your turn, you play as 'O'. Pick a square from a1 to c3.\n";
        playerValue = 2;
        free--;
    }
    
    int computerValue = (playerValue%2)+1;
    
    while(keepPlaying && free>0){
        if (playerTurn){
            bool keepAsking = true;
            while(keepAsking){
                cout << "Enter a free square:\n";
                string pick;
                cin >> pick;
                if(pick == "exit"){return 0;} //sekretny kod pozwalający na poddanie się :)
                if(pick.size() > 2){cout << "Assuming you ment " << pick[0] << pick[1] << ".\n";}
                keepAsking = !CheckCoordinates(pick);
                if(!keepAsking){
                    keepAsking = !ChangeCoordValue(currentBoard, pick, playerValue);
                    //naraz sprawdzamy czy miejsce jest zajęte i jeśli nie to je podmieniamy
                    playerTurn = false;
                }
            }
            //wyjscie z pętli tylko jeśli podmieniliśmy
        }
        else
        {
            uniform_int_distribution<> field(0,free);
            int n = field(engine);
            int k = NthFreeIndex(currentBoard, n);
            currentBoard[k] = computerValue;
            cord = CoordsFromIndex(k);
            playerTurn = true;
            cout << "The computer chose" << cord << ".\n";
        }
        Wypisz(currentBoard);
        free--;
        cout << free <<"free\n";
    }
    return 0;
}
int main () {
    cout << "Welcome to Tic Tac Toe by Kamil Dutkiewicz" << endl;
    cout << "The difficulty is set to easy." << endl;

    default_random_engine engine{static_cast<long unsigned int>(time(0))};
    uniform_int_distribution<> abc(0,1);    
    //^do usunięcia 


    int testBoard[] = {0, 1, 1, 2, 2, 2, 1, 0, 2};
    EasyGame();
     
    cout << "Thank you. Exciting. Exiting. Goodbye." << endl;
    return 0;
}