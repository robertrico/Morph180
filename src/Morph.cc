#include "Morph.h"

//Board
void Morph::Board::init(){
	int i,j;
	for(i=1;i<9;i++){
		std::vector<int> new_vec;
		for(j=1;j<7;j++){
			new_vec.push_back(0);
		}
		this->matrix.push_back(new_vec);
	}
}

void Morph::Board::print(){
	int i,j;
	for(i=8;i>0;i--){
		std::cout << " " << i <<"  ";
		for(j=1;j<7;j++){
			if((i%2 == 0 && j%2 == 0) || (i%2 != 0 && j%2 != 0)){
				std::cout << "\033[7;31m";
				std::cout << " " << this->getPieceChar(i,j);
				std::cout << " ";
			}else{
				std::cout << "\033[7;37m";
				std::cout << " " << this->getPieceChar(i,j);
				std::cout << " ";
			}
			std::cout << "\033[0m";
		}
		std::cout << std::endl;
	}

	std::cout << "\033[0m";
	std::cout << "                        " << std::endl;
	std::cout << "     A  B  C  D  E  F   " << std::endl;
}

char Morph::Board::getPieceChar(int x, int y){
	std::vector<Morph::Piece*>::iterator it;

	for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
		if((*it)->getPosition()[0] == x && (*it)->getPosition()[1] == y){
			return (*it)->getChar();
		}
	}
	return ' ';
}

void Morph::Board::addPiece(Morph::Piece *piece){
	piece->board = this;
	this->active_pieces.push_back(piece);
}

Morph::Piece* Morph::Board::getPiece(int x){
	return this->active_pieces.at(x);
}

Morph::Piece* Morph::Board::getPiece(int x, int y){
	std::vector<Morph::Piece*>::iterator it;

	for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
		if((*it)->getPosition()[0] == x && (*it)->getPosition()[1] == y){
			return (*it);
		}
	}

	return NULL;
}

bool Morph::Board::isEmpty(int x, int y){
	std::vector<Morph::Piece*>::iterator it;

	for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
		if((*it)->getPosition()[0] == x && (*it)->getPosition()[1] == y){
			return false;
		}
	}

	return true;
}

void Morph::Board::getNextMove(){
	std::random_shuffle(active_pieces.begin(), active_pieces.end());
	std::vector<Morph::Piece*>::iterator it;

	for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
		if(!(*it)->isPlayer()){
			(*it)->getMoves();
		}
	}

}
void Morph::Board::execute(){
	std::vector<Morph::Piece*>::iterator it;

	for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
		(*it)->showMoves();
	}
}

bool Morph::Board::moveParser(std::string move){
	int one,two,three,four;
	one = this->letterParser(move[0]);
	two = move[1] - '0';
	three = this->letterParser(move[2]);
	four = move[3] - '0';


	if(this->isEmpty(two,one)){
		std::cout << "\033[1m\033[031m";
		std::cout << "No Piece Exists in that space to move from" << std::endl;
		std::cout << "\033[0m" << std::endl;
	}else{
		Morph::Piece *current = this->getPiece(two,one);
		if(!current->isPlayer()){
			std::cout << "\033[1m\033[031m";
			std::cout << "This isn't your piece to move!" << std::endl;
			std::cout << "\033[0m" << std::endl;
			return false;
		}
		if((one == 0 || three == 0 || two > 8 || four > 8) || !current->validMove(three,four)){
			std::cout << "\033[1m\033[031m";
			std::cout << "Please Enter a valid move" << std::endl;
			std::cout << "\033[0m" << std::endl;
		}else if(this->isEmpty(four,three)){
			current->setPosition(four,three);
			return true;
		}
	}

	return false;
}

int Morph::Board::letterParser(char letter){
	if(letter == 'A' || letter == 'a'){
		return 1;
	}
	if(letter == 'B' || letter == 'b'){
		return 2;
	}
	if(letter == 'C' || letter == 'c'){
		return 3;
	}
	if(letter == 'D' || letter == 'd'){
		return 4;
	}
	if(letter == 'E' || letter == 'e'){
		return 5;
	}
	if(letter == 'F' || letter == 'f'){
		return 6;
	}
	return 0;
}

bool Morph::Board::capturablePiece(int x, int y){
	if(this->isEmpty(x,y)){
		return false;
	}else{
		return this->getPiece(x,y)->isPlayer();
	}
}

//Piece

void Morph::Piece::setPosition(int x, int y){
	this->position[0] = x;
	this->position[1] = y;
}

int* Morph::Piece::getPosition(){
	return this->position;
}

bool Morph::Piece::isPlayer(){
	return this->is_player;
}

void Morph::Piece::addMove(int x, int y){
	int* moves = new int[2];
	moves[0] = x;
	moves[1] = y;
	this->moves.push_back(moves);
}

void Morph::Piece::showMoves(){
	std::vector<int*>::iterator it;

	for(it=this->moves.begin(); it < this->moves.end(); it++){
		int x = *(*it);
		int y = *(*it+1);
		int *current = this->getPosition();
		std::cout << "Valid Moves: " << current[0]+x << "{}" << current[1]+y << std::endl;
	}

}

//Piece -- Bishop
void Morph::Bishop::getMoves(){
	std::cout << this->getPosition()[0] << " & " << this->getPosition()[1] << std::endl;
}
bool Morph::Bishop::validMove(int x, int y){
	return true;
}

char Morph::Bishop::getChar(){
	if(isPlayer()){
		return 'b';
	}
	return 'B';
}

//Piece -- Rook
void Morph::Rook::getMoves(){
	std::cout << "Rook Get Move" << std::endl;
}
bool Morph::Rook::validMove(int x, int y){
	return true;
}
char Morph::Rook::getChar(){
	if(isPlayer()){
		return 'r';
	}
	return 'R';
}
//Piece -- Knight
void Morph::Knight::getMoves(){
	int *current = this->getPosition();

	if(this->validMove(current[0]-2,current[1]+1)){
		this->addMove(-2,1);
	}
	if(this->validMove(current[0]-2,current[1]-1)){
		this->addMove(-2,-1);
	}
	if(this->validMove(current[0]-1,current[1]-2)){
		this->addMove(-1,-2);
	}
	if(this->validMove(current[0]-1,current[1]+2)){
		this->addMove(-1,2);
	}
	if(this->validMove(current[0]+1,current[1]+2) && this->board->capturablePiece(current[0]+1,current[1]+2)){
		this->addMove(1,2);
	}
	if(this->validMove(current[0]+1,current[1]-2) && this->board->capturablePiece(current[0]+1,current[1]-2)){
		this->addMove(1,-2);
	}
	if(this->validMove(current[0]+2,current[1]-1) && this->board->capturablePiece(current[0]+2,current[1]-1)){
		this->addMove(2,-1);
	}
	if(this->validMove(current[0]+2,current[1]+1) && this->board->capturablePiece(current[0]+2,current[1]+1)){
		this->addMove(2,1);
	}
}
bool Morph::Knight::validMove(int x, int y){
	if(this->isPlayer()){
		int *current = this->getPosition();
		int movex = current[1] -  x;
		int movey = current[0] - y;
		if( (movey == -2 && movex ==  1) ||
			(movey == -2 && movex == -1) ||
			(movey == -1 && movex == -2) ||
			(movey == -1 && movex ==  2) ||
			(movey ==  1 && movex ==  2) ||
			(movey ==  1 && movex == -2) ||
			(movey ==  2 && movex == -1) ||
			(movey ==  2 && movex ==  1)){
			return true;
		}else{
			return false;
		}
	}else{
		if(x > 8 || x < 1){
			return false;
		}
		if(y > 6 || y < 1){
			return false;
		}
		return true;
	}
}
char Morph::Knight::getChar(){
	if(isPlayer()){
		return 'n';
	}
	return 'N';
}
//Piece -- Pawn
void Morph::Pawn::getMoves(){
	std::cout << "Pawn Get Move" << std::endl;
}
bool Morph::Pawn::validMove(int x, int y){
	int *current = this->getPosition();
	if(x != current[1]){
		return false;
	}
	if(y - current[0] > 1){
		return false;
	}
	return true;
}
char Morph::Pawn::getChar(){
	if(isPlayer()){
		return 'p';
	}
	return 'P';
}

//Piece -- King
void Morph::King::getMoves(){
	if(!isPlayer()){
		int *current = this->getPosition();
		if(this->validMove(current[0],current[1]+1)){
			this->addMove(0,1);
		}
	}
}

bool Morph::King::validMove(int x, int y){
	int *current = this->getPosition();

	if(this->isPlayer() && (y > 1 || current[1] - x > 1 || x > current[1])){
		return false;
	}

	return true;
}

char Morph::King::getChar(){
	if(isPlayer()){
		return 'k';
	}
	return 'K';
}

