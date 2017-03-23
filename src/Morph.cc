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

void Morph::Board::replacePiece(int x, Morph::Piece* piece){
	Morph::Piece* toDelete = this->active_pieces.at(x);
	this->active_pieces.at(x) = piece;
	delete toDelete;
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

void Morph::Piece::freeze(){
	int *state = new int[2];
	int *current = this->getPosition();
	state[0] = current[0];
	state[1] = current[1];

	this->states.push_back(state);
}

void Morph::Piece::revert(){
	int* last = this->states.back();
	this->setPosition(last[0],last[1]);
	this->states.pop_back();
	delete last;

}

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
		std::cout << this->getChar() << " - " << "Valid Moves: {" << current[0]+x << "," << current[1]+y << "}" << std::endl;
	}

}

//Piece -- Bishop
void Morph::Bishop::getMoves(){
	int *current = this->getPosition();
	int x = 0;
	int y = 0;
	x += current[0];
	y += current[1];
	int count = -1;
	while((x > 0 && y > 0)){
		if(!this->board->isEmpty(current[0]+count,current[1]+count)){
 			if(this->board->capturablePiece(current[0]+count,current[1]+count)){
				this->addMove(count,count);
			}
			break;
		}
		if(this->validMove(current[0]+count,current[1]+count)){
			this->addMove(count,count);
		}
		x--;
		y--;
		count--;
	}
	x = 0;
	y = 0;
	x += current[0];
	y += current[1];
	count = 1;
	while((x < 9 && y < 7)){
		if(this->board->isEmpty(current[0]+count,current[1]+count)){
			break;
		}
		if(!this->board->isEmpty(current[0]+count,current[1]+count)){
 			if(this->board->capturablePiece(current[0]+count,current[1]+count)){
				this->addMove(count,count);
			}
			break;
		}
		if(this->validMove(current[0]+count,current[1]+count)){
			this->addMove(count,count);
		}
		x++;
		y++;
		count++;
	}
	x = 0;
	y = 0;
	x += current[0];
	y += current[1];
	count = 1;
	while((x < 9 && y > 0)){
		if(this->board->isEmpty(current[0]+count,current[1]-count)){
			break;
		}
		if(!this->board->isEmpty(current[0]+count,current[1]-count)){
 			if(this->board->capturablePiece(current[0]+count,current[1]-count)){
				this->addMove(count,-1*count);
			}
			break;
		}
		if(this->validMove(current[0]+count,current[1]-count)){
			this->addMove(count,-1*count);
		}
		x++;
		y--;
		count++;
	}
	x = 0;
	y = 0;
	x += current[0];
	y += current[1];
	count = 1;
	while((x > 0 && y < 7)){

		if(!this->board->isEmpty(current[0]-count,current[1]+count)){
 			if(this->board->capturablePiece(current[0]-count,current[1]+count)){
				this->addMove(-1*count,count);
			}
			break;
		}
		if(this->validMove(current[0]-count,current[1]+count)){
			this->addMove(-1*count,count);
		}
		x--;
		y++;
		count++;
	}
}
bool Morph::Bishop::validMove(int x, int y){
	if(!this->isPlayer()){
		if(x < 1 || y < 1){
			return false;
		}
		if(x > 8 || y > 6){
			return false;
		}
	}else{
		int *current = this->getPosition();
		if(x < current[0] && !this->board->capturablePiece(x,y)){
			return false;
		}
		int i = 0;
		int j = 0;
		i += current[1];
		j += current[0];
		int count = -1;

		if(x < i && y < j){
			return this->board->capturablePiece(x,y);
		}

		i = 0;
		j = 0;
		i += current[0];
		j += current[1];
		count = 1;

		if(x > i && y > j){
			while((i < 9 && j < 7)){
				if(this->board->isEmpty(current[0]+count,current[1]+count)){
					return false;
				}
				i++;
				j++;
				count++;
			}
		}

		i = 0;
		j = 0;
		i += current[1];
		j += current[0];
		count = 1;

		if(x < i && y > j){
			return this->board->capturablePiece(x,y);
		}

		i = 0;
		j = 0;
		i += current[1];
		j += current[0];
		count = 1;

		if(x > i && y < j){
			while((i > 0 && j < 7)){
				if(this->board->isEmpty(current[0]-count,current[1]+count)){
					return false;
				}
				i--;
				j++;
				count++;
			}
		}


	}

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
	int *current = this->getPosition();
	int j = 0;
	int i = 0;
	j += current[0];
	i += current[1];
	int count = j-1;

	while(count > 0){
		if(this->board->capturablePiece(count,i)){
			this->addMove(count-j,0);
			break;
		}
		if(!this->board->isEmpty(count,i)){
			break;
		}
		if(this->board->isEmpty(count,i)){
			this->addMove(count-j,0);
		}
		count--;
	} 

	count = i-1;
	while(count > 0){
		if(this->board->capturablePiece(j,count)){
			this->addMove(0,count-i);
			break;
		}
		if(!this->board->isEmpty(count,i)){
			break;
		}
		if(this->board->isEmpty(j,count)){
			this->addMove(0,count-i);
		}
		count--;
	} 

	count = i+1;
	while(count < 7){
		if(this->board->capturablePiece(j,count)){
			this->addMove(0,count-i);
			break;
		}
		if(!this->board->isEmpty(count,i)){
			break;
		}
		if(this->board->isEmpty(j,count)){
			this->addMove(0,count-i);
		}
		count++;
	} 

	count = j+1;
	while(count < 9){
		if(this->board->capturablePiece(count,i)){
			this->addMove(count-j,0);
			break;
		}
		count++;
	} 
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
		if(!this->board->isEmpty(y,x)){
			return false;
		}
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
	if(this->isPlayer()){
		return 'n';
	}
	return 'N';
}
//Piece -- Pawn
void Morph::Pawn::getMoves(){
	if(!this->isPlayer()){
		this->addMove(-1,0);
	}
}
bool Morph::Pawn::validMove(int x, int y){
	if(!this->board->isEmpty(y,x)){
		return false;
	}
	int *current = this->getPosition();
	if(x != current[1]){
		return false;
	}
	if(this->isPlayer()){
		if(y - current[0] > 1){
			return false;
		}
		if(current[0] > y){
			return false;
		}
	}else{
		if(x > 8 || x < 1){
			return false;
		}
		if(y > 6 || y < 1){
			return false;
		}
	}
	return true;
}
char Morph::Pawn::getChar(){
	if(this->isPlayer()){
		return 'p';
	}
	return 'P';
}

//Piece -- King
void Morph::King::getMoves(){
	if(!this->isPlayer()){
		int *current = this->getPosition();
		if(this->validMove(current[0],current[1]+1)){
			this->addMove(0,1);
		}
	}
}

bool Morph::King::validMove(int x, int y){
	if(!this->board->isEmpty(y,x)){
		return false;
	}
	int *current = this->getPosition();

	if(this->isPlayer() && (y > 1 || current[1] - x > 1 || x > current[1])){
		return false;
	}

	return true;
}

char Morph::King::getChar(){
	if(this->isPlayer()){
		return 'k';
	}
	return 'K';
}

