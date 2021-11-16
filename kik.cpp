#include <iostream>
#include <random>
#include <chrono>
using namespace std;

char NumToChar(int num){
    switch (num)
    {

    case 0:
        return ' ';
    case 1:
        return 'x';
    case 2:
        return 'o';
    case 11:
        return 'X';
    case 22:
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
    string field = "nx";
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

bool CheckIfWin(int currentBoard[9], int v){
    for(int i=0; i<3; i++){
        if(currentBoard[i]==v && currentBoard[i+3]==v && currentBoard[i+6]==v){
            currentBoard[i] = v*11;
            currentBoard[i+3] = v*11;
            currentBoard[i+6] = v*11;
            return true;
        }
        if(currentBoard[i*3]==v && currentBoard[i*3+1]==v && currentBoard[i*3+2]==v){
            currentBoard[i*3] = v*11;
            currentBoard[i*3+1] = v*11;
            currentBoard[i*3+2] = v*11;
            return true;
        }
    }
    if(currentBoard[0]==v && currentBoard[4]==v && currentBoard[8]==v){
        currentBoard[0] = v*11;
        currentBoard[4] = v*11;
        currentBoard[8] = v*11;
        return true;
    }
    if(currentBoard[2]==v && currentBoard[4]==v && currentBoard[6]==v){
        currentBoard[2] = v*11;
        currentBoard[4] = v*11;
        currentBoard[6] = v*11;
        return true;
    }
    return false;
}

int EasyGame()
{
    //random_device rd;
	//default_random_engine engine(rd()); //nie działa na windowsie
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine engine(seed);
    int free = 9;
    bool keepPlaying = true;
    bool won = false;
	int currentBoard[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    string cord;
    uniform_int_distribution<> coin(0,1);
    uniform_int_distribution<> field(0,9);
    int coinFlip = coin(engine);
    bool playerTurn = true;
    int playerValue;
    cout << "The dice landed on :" << coinFlip <<"\n"; 
    if(coinFlip){
        cout << "a---b---c\n";
        Wypisz(currentBoard);
        cout << "You start and play as 'x'. Pick a square from a1 to c3.\n\n";
        playerValue = 1;
    }
    else
    {
        int n = field(engine);
        currentBoard[n] = 1;
        cord = CoordsFromIndex(n);
        cout << "The computer started and drew an 'x' on " << cord << ".\n" << "a---b---c\n\n";
        Wypisz(currentBoard);
        cout << "It's your turn, you play as 'o'. Pick a square from a1 to c3.\n";
        playerValue = 2;
        free--;
    }
    
    int computerValue = (playerValue%2)+1;
    int currentValue;
    while(keepPlaying && free>0){
        //cout << "There are " << free << " free squares remaining.\n";
        if (playerTurn){
            bool keepAsking = true;
            currentValue = playerValue;
            while(keepAsking){
                cout << "Enter a free square:\n";
                string pick;
                cin >> pick;
                if(pick == "exit"){return -1;} //sekretny kod pozwalający na poddanie się :)
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
            currentValue = computerValue;
            uniform_int_distribution<> field(0,free-1);
            int n = field(engine);
            //cout << "Lets pick the " << n << "th free square.\n";
            int k = NthFreeIndex(currentBoard, n);
            //cout << "It shall be the " << k << "th square.\n";
            currentBoard[k] = computerValue;
            cord = CoordsFromIndex(k);
            playerTurn = true;
            cout << "The computer chose " << cord << ".\n";
        }
        Wypisz(currentBoard);
        free--;
        won = CheckIfWin(currentBoard, currentValue);
        if(won){
            cout << "Game over\n";
            Wypisz(currentBoard);
            return int(!playerTurn)*2-1; //!playerTurn daje 1 jeśli była tura gracza, else 0
        }
    }
    return 0;
}
int winningSpot(int currentBoard[9], int v){
    for(int i=0; i<3; i++){
        if(((currentBoard[i]==v) + (currentBoard[i+3]==v) + (currentBoard[i+6]==v)) == 2){
            for(int j=i; j<9; j+=3){
                if(currentBoard[j] == 0){return j;}
            }
        }
        if(((currentBoard[i*3]==v) + (currentBoard[i*3+1]==v) + (currentBoard[i*3+2]==v)) == 2){
            for(int j=i*3; j<i*3+3; j++){
                if(currentBoard[j] == 0){return j;}
            }
        }
    }
    if(((currentBoard[0]==v) + (currentBoard[4]==v) + (currentBoard[8]==v)) == 2){
        for(int j=0; j<9; j+= 4){
                if(currentBoard[j] == 0){return j;}
            }
    }
    if(((currentBoard[2]==v) + (currentBoard[4]==v) + (currentBoard[6]==v)) == 2){
        for(int j=2; j<7; j+= 2){
                if(currentBoard[j] == 0){return j;}
            }
    }
    return 9;
}
int cheeseSpot(int currentBoard[9], int v){
    for (int i=0; i<9; i++){
        if(currentBoard[i]==0 && ( (currentBoard[i + 1 - (i%3 == 2)*3]*currentBoard[i - 1 + (i%3 == 0)*3] == 0) && (currentBoard[i + 1 - (i%3 == 2)*3]+currentBoard[i - 1 + (i%3 == 0)*3] == v)) && 
        ((currentBoard[i + 3 - (i/3 == 2)*9]*currentBoard[i - 3 + (i/3 == 0)*9] == 0) && (currentBoard[i + 3 - (i/3 == 2)*9]+currentBoard[i - 3 + (i/3 == 0)*9] == v)) )
        {
            return i;
        }
    }
    return 9;
}
int FirstOneAvailable(int currentBoard[9]){
    for(int i = 0; i<9; i++){
        if(currentBoard[i] == 0){return i;}
    }
    return 9;
}
int PerfectPlayer(int currentBoard[9], int freeSquares, int myV, int hisV){
    //sprawdzamy czy można wygrać
    int w1 = winningSpot(currentBoard, myV);
    if(w1<9){return w1;}
    //sprawdzamy czy gdzieś "trzeba" zagrać
    int w2 = winningSpot(currentBoard, hisV);
    if(w2<9){return w2;}
    //magia
    if(freeSquares==6 && currentBoard[4] == myV){
        if((currentBoard[0]==hisV && currentBoard[8]==hisV) || (currentBoard[2]==hisV && currentBoard[6]==hisV))
        {
            return 1;
        }
        if(currentBoard[0] == hisV){return 8;}
        if(currentBoard[2] == hisV){return 6;}
        if(currentBoard[6] == hisV){return 2;}
        if(currentBoard[8] == hisV){return 0;}
    }

    //sprawdzamy czy gdzieś można "zcheesować"
    int w3 = cheeseSpot(currentBoard, myV);
    if(w3<9){return w3;}
    //albo czy przeciwnik może nas "zcheesować"
    //sprawdzamy czy gdzieś można "zcheesować"
    int w4 = cheeseSpot(currentBoard, hisV);
    if(w4<9){return w4;}
    //zawsze dobra taktyka:
    if(currentBoard[4] == 0){return 4;}
    
    if(freeSquares == 7){
        for(int i = 1; i<8; i+=2){
            if(currentBoard[i] == hisV){
                return (i%3 == 1)*(i+1) + (i/3 == 1)*(i-3);
            }
        }
        if(currentBoard[8] == hisV){return 1;}
        return FirstOneAvailable(currentBoard);
    }
    if(freeSquares == 8 && currentBoard[4] == hisV){return 0;}
    return FirstOneAvailable(currentBoard);
}


int HardGame()
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine engine(seed);
    int free = 9;
    bool keepPlaying = true;
    bool won = false;
	int currentBoard[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    string cord;
    uniform_int_distribution<> coin(0,1);
    uniform_int_distribution<> field(0,9);
    int coinFlip = coin(engine);
    //tu chyba są duuuże problemy z losowością. ale może już działa.
    bool playerTurn = true;
    int playerValue;
    if(coinFlip){
        cout << "a---b---c\n";
        Wypisz(currentBoard);
        cout << "You start and play as 'x'. Pick a square from a1 to c3.\n\n";
        playerValue = 1;
    }
    else
    {
        int n = 4;
        currentBoard[n] = 1;
        cord = CoordsFromIndex(n);
        cout << "The computer started and drew an 'x' on " << cord << ".\n" << "a---b---c\n\n";
        Wypisz(currentBoard);
        cout << "It's your turn, you play as 'o'. Pick a square from a1 to c3.\n";
        playerValue = 2;
        free--;
    }
    
    int computerValue = (playerValue%2)+1;
    int currentValue;
    while(keepPlaying && free>0){
        //cout << "There are " << free << " free squares remaining.\n";
        if (playerTurn){
            bool keepAsking = true;
            currentValue = playerValue;
            while(keepAsking){
                cout << "Enter a free square:\n";
                string pick;
                cin >> pick;
                if(pick == "exit"){return -1;} //sekretny kod pozwalający na poddanie się :)
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
            currentValue = computerValue;
            int k = PerfectPlayer(currentBoard, free, computerValue, playerValue);
            cord = CoordsFromIndex(k);
            if(currentBoard[k] != 0){
                cout << "Error: computer tried to play " << cord << " but it's occupied. He can't stand the humiliation and gives up.\n";
                return 1;
            }
            currentBoard[k] = computerValue;
            playerTurn = true;
            cout << "The computer chose " << cord << ".\n";
        }
        Wypisz(currentBoard);
        free--;
        won = CheckIfWin(currentBoard, currentValue);
        if(won){
            cout << "Game over\n";
            Wypisz(currentBoard);
            return int(!playerTurn)*2-1; //!playerTurn daje 1 jeśli była tura gracza, else 0
        }
    }
    return 0;
}
int PlayingLoop()
{
    int coinFlip;
    int score = 0;
    double ppoints = 0;
    double cpoints = 0;
    while(true){
        cout << "Please select difficulty 'easy' or 'hard':\n";
        string diff;
        int result;
        cin >> diff;
        if(diff == "easy" || diff == "Easy" || diff == "ez" || diff == "e"){
            cout << "Beginning game on easy mode.\n";
            result = EasyGame();
        }
        else
        {
            if(diff == "hard" || diff == "Hard" || diff == "h"){cout << "Beginning game on hard mode.\n";}
            else{cout << "Sorry, we don't have that difficulty yet. Beginning game on hard mode indstead.\n";}
            result = HardGame();
        }
        score += result;
        cout << result << " = result\n";
        switch (result)
        {
            case -1:
                cout <<"Sorry, the computer won.\n";
                cpoints++;
                break;
            case 0:
                cout <<"The game is a draw.\n";
                cpoints+=0.5;
                ppoints+=0.5;//chess style
                break;
            case 1:
                cout << "Congratulations! You won.\n";
                ppoints++;
                break;
        }
        cout << "Would you like to play again?\n";
        string WillToPlay;
        cin >> WillToPlay;
        if(!(WillToPlay == "yes" || WillToPlay == "YES" || WillToPlay == "y" || WillToPlay == "Y" || WillToPlay == "tak")){
            cout << "Thank you for playing. The score was " << ppoints << ":" << cpoints << "\n";
            return score;
        }
    }
    cout << "Error: truth is false";
    return 0;
}
int main () {
    cout << "Welcome to Tic Tac Toe by Kamil Dutkiewicz\nEnter 'START'to play. If you want to see the insturcion, type 'HELP'.";
    cout << "If you've changed your mind, type 'EXIT' to well... exit.\n";
    string start = "not-start";
    while(!(start == "start" || start == "START" || start == "Start"))
    {
        if(start == "HELP" || start == "help" || start == "h2p" || start == "Help" || start == "instruction")
        {
            cout << "You take turns with the computer, marking a square from:\na1|b1|c1\n--------\na2|b2|c2\n--------\na3|b3|c3\n";
            cout << "The goal of the game is to mark three of your symbols in a row (vertically, horizontally or diagonally), before the computer does so.\n";
            cout << "To mark a square, just write its coordinates and press 'ENTER'.\n";
            cout << "Remember to use lowercase and select a free square.\n";
            cout << "You will play randomly as either 'x' or 'o', 'x' is the one to start.\n";
            cout << "Good luck!\n";
        }
        if(start == "exit" || start == "EXIT" || start == "Exit" || start == "quit" || start == "Alt+F4"){return 0;}
        cin >> start;
    }

    int score;
    score = PlayingLoop();
    if(score>0){
        cout << "Well played!\n";
    }
    else
    {
        if(score == 0){
            cout << "A tie? Come back soon for another round.\n";
        }
        else{
            cout << "Better luck next time.\n";
        }
    }
    cout << "Thank you for playing. Exciting. Exiting. Goodbye." << endl;
    return 0;
}