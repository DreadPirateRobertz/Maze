#include "vertex.h"

int readFile(int, char*[], int&, int&, vector<vector<vector<vertex>>>&);
void findAdjacencies(int, int, vector<vector<vector<vertex>>>&);
void printer(deque<pair<int,int>>);
void breadthFirstSearch(vector<vector<vector<vertex>>>&, deque<pair<int,int>>&);

int main(int argc, char *argv[]) {
    int rows = 0, cols = 0;
    deque<pair<int,int>> pathThruTheMaze;
    vector<vector<vector<vertex>>> gameBoard;

    readFile(argc, argv, rows, cols, gameBoard);
    findAdjacencies(rows, cols, gameBoard);
    breadthFirstSearch(gameBoard, pathThruTheMaze);
    printer(pathThruTheMaze);
    return 0;
}

int readFile(int argc, char *argv[], int &rows, int &cols, vector<vector<vector<vertex>>> &gameBoard){
    if (argc < 2) {
        cerr << "Please Provide Input File Name";
        return 666;
    }
    cout << "Input File >>> " << argv[1] << endl;
    ifstream in(argv[1]);
    if(!in){
        cerr << "Unable to open " << argv[1] << " for input\n";
        return 666;
    }
    in >> rows >> cols;
    gameBoard.resize(2, vector<vector<vertex>>(rows, vector<vertex>(cols)));
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            // Load vertices for each side of this maze
            //Can be done without these temporary pointers
            vertex *normal = &gameBoard[1][i][j];
            vertex *reverse = &gameBoard[0][i][j];

            //Read data normally
            string stringDirection;
            in >> normal->row >> normal->col >> normal->arrowColor >>
               normal->circle >> stringDirection;

            //Set the color from NONE
            normal->color = Color::WHITE;
            reverse->color = Color::WHITE;


            if (stringDirection == "N") normal->direction = Direction::N;
            if (stringDirection == "S") normal->direction = Direction::S;
            if (stringDirection == "E") normal->direction = Direction::E;
            if (stringDirection == "W") normal->direction = Direction::W;
            if (stringDirection == "NE") normal->direction = Direction::NE;
            if (stringDirection == "NW") normal->direction = Direction::NW;
            if (stringDirection == "SE") normal->direction = Direction::SE;
            if (stringDirection == "SW") normal->direction = Direction::SW;



            reverse->row = normal->row;
            reverse->col = normal->col;
            reverse->arrowColor = normal->arrowColor;
            reverse->circle = normal->circle;
            reverse->side = "<---"; //For debugging

            // Flip the directions for reverse side
            switch(normal->direction){
                case Direction::N:
                    reverse->direction = Direction::S;
                    break;
                case Direction::S:
                    reverse->direction = Direction::N;
                    break;
                case Direction::E:
                    reverse->direction = Direction::W;
                    break;
                case Direction::W:
                    reverse->direction = Direction::E;
                    break;
                case Direction::NE:
                    reverse->direction = Direction::SW;
                    break;
                case Direction::NW:
                    reverse->direction = Direction::SE;
                    break;
                case Direction::SE:
                    reverse->direction = Direction::NW;
                    break;
                case Direction::SW:
                    reverse->direction = Direction::NE;
                    break;
                case Direction::NONE:
                    //Should Only Be the Last tile (7,7)
                    break;
                default:
                    cerr << "PANIC -> ERROR ON DIRECTIONAL INPUT\n";
            }
        }
    }
    in.close(); //Close File Stream
    return 0;
}
void printer(deque<pair<int,int>> path){

    int row, col;
    cout << "This is the path:" << endl;
    while(!path.empty()) {
        //Tie will unpack this pair
        tie(row,col) = path.front();
        printf("(%i,%i) ", row, col);
        path.pop_front();
    }
}

void findAdjacencies(int rows, int cols, vector<vector<vector<vertex>>> &gameBoard){
    // Variables
    int dy, dx, rtemp, ctemp;
    bool side = false;

    //Loop For each side
    for (int thisSide = 0; thisSide < gameBoard.size(); thisSide++) {
        //Flip the side
        //Side is a bool so I can flip it back and forth in one bit of code down below
        if (thisSide == 1) side = true;
        rtemp = 0;
        ctemp = 0;

        //These for-loops could have iterates to rows/cols but since 3D vector...
        for (int row = 0; row < gameBoard[thisSide].size(); row++){
            for (int col = 0; col < gameBoard[thisSide][row].size(); col++){
                //Temporary Pointer to vertex
                vertex *vertex = &gameBoard[thisSide][row][col];

                //Which Way?
                dy = 0;
                if (vertex->direction == Direction::N ||
                    vertex->direction == Direction::NE ||
                    vertex->direction == Direction::NW) dy = -1;
                if (vertex->direction == Direction::S ||
                    vertex->direction == Direction::SE ||
                    vertex->direction == Direction::SW) dy = 1;
                dx = 0;
                if (vertex->direction == Direction::W ||
                    vertex->direction == Direction::SW ||
                    vertex->direction == Direction::NW) dx = -1;
                if (vertex->direction == Direction::E ||
                    vertex->direction == Direction::NE ||
                    vertex->direction == Direction::SE) dx = 1;


                rtemp = row + dy;
                ctemp = col + dx;

                //Add all adjacent tiles with opposite color and make sure doesn't fall off board
                while(
                      rtemp >= 0 &&
                      rtemp < rows &&
                      ctemp >= 0 &&
                      ctemp < cols &&
                      vertex->circle != 'X' //If target stop
                      )
                {

                    //Is opposite color?
                    if (gameBoard[thisSide][rtemp][ctemp].arrowColor != vertex->arrowColor) {

                        //If circle go to the other side of the board:)
                        if (gameBoard[thisSide][rtemp][ctemp].circle == 'C')
                            //Bool for side here b/c I was being silly and didn't want to flip bit back around
                            //XOR gate or NAND gate logic could be used here
                            vertex->adjList.push_back(&gameBoard[!side][rtemp][ctemp]);
                        else
                            vertex->adjList.push_back(&gameBoard[side][rtemp][ctemp]);
                    }
                    //Next
                    ctemp += dx;
                    rtemp += dy;

                }
            }
        }
    }
}


void breadthFirstSearch(vector<vector<vector<vertex>>> &gameBoard, deque<pair<int,int>> &path){
    deque<vertex*> vertexChecker; //Double Ended Queues are Awesome!
    vertex *thisVertex;

    //Add first tile -> (1,1)
    vertexChecker.push_back(&gameBoard[1][0][0]);

    while(!vertexChecker.empty()){
        thisVertex = vertexChecker.front();
        for(auto vertex : thisVertex->adjList) {
            if(vertex->color == Color::WHITE) {
                vertex->color = Color::GREY;
                vertex->parentPtr = thisVertex;
                vertexChecker.push_back(vertex);
                if(vertex->circle == 'X') {
                    while(vertex->parentPtr != nullptr) {
                        path.push_front(
                                make_pair(vertex->row, vertex->col));
                        vertex = vertex->parentPtr; //Keep going back up the path
                    }
                    path.push_front(make_pair(1, 1));
                    return;
                }
            }
            thisVertex->color = Color::BLACK;
        }
        vertexChecker.pop_front();
    }
}
