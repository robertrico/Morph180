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
	std::vector<Morph::Piece*>::iterator it;

	for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
		(*it)->getMoves();
	}

}

void Morph::Board::moveParser(std::string move){
	std::vector<int> m;
	m.push_back(1);

	int one,two,three,four;
	one = this->letterParser(move[0]);
	two = move[1] - '0';
	three = this->letterParser(move[2]);
	four = move[3] - '0';

	if(one == 0 || three == 0 || two > 8 || four > 8){
		std::cout << "\033[1m\033[031m";
		std::cout << "Please Enter a valid move" << std::endl;
		std::cout << "\033[0m" << std::endl;
	}else if(this->isEmpty(two,one)){
		std::cout << "\033[1m\033[031m";
		std::cout << "No Piece Exists in that space to move from" << std::endl;
		std::cout << "\033[0m" << std::endl;
	}else if(this->isEmpty(four,three)){
		this->getPiece(two,one)->setPosition(four,three);
	}
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

//Piece

void Morph::Piece::setPosition(int x, int y){
	this->position[0] = x;
	this->position[1] = y;
}
int* Morph::Piece::getPosition(){
	return this->position;
}
//Piece -- Bishop
void Morph::Bishop::getMoves(){
	std::cout << this->getPosition()[0] << " & " << this->getPosition()[1] << std::endl;
}

char Morph::Bishop::getChar(){
	return 'B';
}

//Piece -- Rook
void Morph::Rook::getMoves(){
	std::cout << "Rook Get Move" << std::endl;
}
char Morph::Rook::getChar(){
	return 'R';
}
//Piece -- Knight
void Morph::Knight::getMoves(){
	std::cout << "Knight Get Move" << std::endl;
}
char Morph::Knight::getChar(){
	return 'N';
}
//Piece -- Pawn
void Morph::Pawn::getMoves(){
	std::cout << "Pawn Get Move" << std::endl;
}
char Morph::Pawn::getChar(){
	return 'P';
}

//Piece -- King
void Morph::King::getMoves(){
	std::cout << "King Get Move" << std::endl;
}

char Morph::King::getChar(){
	return 'K';
}

