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
	
}

char Morph::King::getChar(){
	return 'K';
}

