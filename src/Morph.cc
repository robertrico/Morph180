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
			if((*it)->isRemoved()){
				return ' ';
			}else{
				return (*it)->getChar();
			}
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

void Morph::Board::removePiece(int x, int y){
	std::vector<Morph::Piece*>::iterator it;
	//Morph::Piece* removed;

	for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
		if((*it)->getPosition()[0] == x && (*it)->getPosition()[1] == y){
			//this->active_pieces.erase(it);
		}
	}

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

std::vector<struct Morph::Move> Morph::Board::showMoves(){

	std::vector<struct Morph::Move> moves;

	std::vector<Morph::Piece*>::iterator it;

	for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
		std::vector<int*> piece_moves = (*it)->getMoves();
		if(piece_moves.size() == 0){
			continue;
		}

		std::vector<int*>::iterator mit;
		for(mit=piece_moves.begin(); mit < piece_moves.end(); mit++){
			struct Morph::Move move;
			move.piece = *it;
			move.moves = *mit;
			moves.push_back(move);
		}
	}

	return moves;

}

void Morph::Board::execute(){
	double best_score = -9999;
	Morph::Piece* moved = NULL;
	int* best_move = new int[2];
	best_move[0] = 0;
	best_move[1] = 0;

	int loop = 0;

	std::vector<struct Morph::Move>::iterator mit;
	std::vector<struct Morph::Move> pmoves = this->showMoves();

	for(mit=pmoves.begin(); mit < pmoves.end(); mit++){
		if((*mit).piece->isPlayer()){
			continue;
		}
		loop++;
		int* position = (*mit).piece->getPosition();

		(*mit).piece->freeze();
		(*mit).piece->setPosition(position[0]+(*mit).moves[0],position[1]+(*mit).moves[1],true);
		double score = this->min(0);
		if(score > best_score){
			moved = (*mit).piece;
			best_move[0] = position[0];
			best_move[1] = position[1];
			best_score = score;
		}
		(*mit).piece->revert();
	}

	if(best_score == 9999912){
		std::cout << "\033[7;37m";
		std::cout << " You win!";
		std::cout << "\033[0m" << std::endl;
		exit(1);
	}
	if(best_score == -9999912){
		std::cout << "\033[7;37m";
		std::cout << " I win!";
		std::cout << "\033[0m" << std::endl;
		exit(1);
	}

	std::cout << "I moved " << best_move[0] << " " << best_move[1] << std::endl;
	std::cout << "With Score " << best_score << std::endl;
	std::cout << "and used "<< loop << " loops" << std::endl;

	moved->setPosition(best_move[0],best_move[1],false);
	delete[] best_move;
}

double Morph::Board::max(int depth){
	if(endEval()){
		return -9999912;
	}else if(depth == MAXDEPTH){
		return eval();
	}else{
		double best_score = -9999;
		//Morph::Piece* moved = NULL;
		int* best_move = new int[2];
		best_move[0] = 0;
		best_move[1] = 0;

		std::vector<struct Morph::Move>::iterator mit;
		std::vector<struct Morph::Move> pmoves = this->showMoves();

		for(mit=pmoves.begin(); mit < pmoves.end(); mit++){
			int* position = (*mit).piece->getPosition();
			if((*mit).piece->isPlayer()){
				continue;
			}

			(*mit).piece->freeze();
			(*mit).piece->setPosition(position[0]+(*mit).moves[0],position[1]+(*mit).moves[1],true);
			double score = this->min(depth+1);
			if(score > best_score){
				best_move[0] = position[0];
				best_move[1] = position[1];
				best_score = score;
			}
			(*mit).piece->revert();
		}

		delete[] best_move;
		return best_score;
	}
}

double Morph::Board::min(int depth){
	if(endEval()){
		return 9999912;
	}else if(depth == MAXDEPTH){
		return eval();
	}else{
		double best_score = 9999;
		//Morph::Piece* moved = NULL;
		int* best_move = new int[2];
		best_move[0] = 0;
		best_move[1] = 0;

		std::vector<struct Morph::Move>::iterator mit;
		std::vector<struct Morph::Move> pmoves = this->showMoves();

		for(mit=pmoves.begin(); mit < pmoves.end(); mit++){
			int* position = (*mit).piece->getPosition();
			if(!(*mit).piece->isPlayer()){
				continue;
			}

			(*mit).piece->freeze();
			(*mit).piece->setPosition(position[0]+(*mit).moves[0],position[1]+(*mit).moves[1],true);
			double score = this->max(depth+1);
			if(score < best_score){
				best_move[0] = position[0];
				best_move[1] = position[1];
				best_score = score;
			}
			(*mit).piece->revert();
		}

		delete[] best_move;
		return best_score;
	}
}

bool Morph::Board::endEval(){
	std::vector<Morph::Piece*>::iterator it;

	int deadAIKing = false;
	int deadKing = false;

	for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
		if((*it)->getChar() == 'K' && !(*it)->isRemoved()){
			deadAIKing = true;
		}
		if((*it)->getChar() == 'k' && !(*it)->isRemoved()){
			deadKing = true;
		}
	}

	return (deadKing xor deadAIKing);
}

int Morph::Board::eval(){
	std::vector<Morph::Piece*>::iterator it;

	int aiCount = 0;
	int aiRookCount = 0;
	int rookCount = 0;
	int playerCount = 0;
	int score = 0;

	for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
		if((*it)->isPlayer() && !(*it)->isRemoved()){
			playerCount++;
		}else if(!(*it)->isRemoved()){
			aiCount--;
		}
		if((*it)->getChar() == 'r'){
			rookCount--;
		}
		if((*it)->getChar() == 'R'){
			aiRookCount++;
		}
	}

	double evalScore = ((0.95) * aiCount) + ((0.5)*playerCount) + (0.25)*rookCount + (0.85)*aiRookCount;

	return evalScore;
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
		return false;
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
			return false;
		}

		if(this->capturableAIPiece(four,three)){

			std::vector<Morph::Piece*>::iterator it;

			for(it=this->active_pieces.begin(); it < this->active_pieces.end(); it++){
				int* position = (*it)->getPosition();
				if(four == position[0] && three == position[1]){
					this->active_pieces.erase(it);
				}
			}

			delete this->getPiece(four,three);
		}

		current->setPosition(four,three,false);
	}

	return true;
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

bool Morph::Board::capturableAIPiece(int x, int y){
	if(this->isEmpty(x,y)){
		return false;
	}else{
		return !this->getPiece(x,y)->isPlayer();
	}
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
	if(this->states.size() != 0){
		int* last = this->states.back();
		this->setPosition(last[0],last[1],true);
		this->setRemoved(false);
		delete[] last;
		this->states.pop_back();
	}
}

void Morph::Piece::setRemoved(bool rem){
	this->is_removed = rem;
}

bool Morph::Piece::isRemoved(){
	return this->is_removed;
}

void Morph::Piece::setPosition(int x, int y,bool min_max_move){
	if(!this->board->isEmpty(x,y)){
		if(this->isPlayer()){
			if(this->isPlayer() != this->board->getPiece(x,y)->isPlayer()){
				this->board->removePiece(x,y);
			}
		}else if(!min_max_move){
			if(this->isPlayer() != this->board->getPiece(x,y)->isPlayer()){
				this->board->removePiece(x,y);
			}
		}else{
			if(this->isPlayer() != this->board->getPiece(x,y)->isPlayer()){
				this->setRemoved(true);
			}
		}
	}

	this->position[0] = x;
	this->position[1] = y;
}

int* Morph::Piece::getPosition(){
	return this->position;
}

bool Morph::Piece::isPlayer(){
	return this->is_player;
}

//Piece -- Bishop
std::vector<int*> Morph::Bishop::getMoves(){
	int *current = this->getPosition();
	std::vector<int*> moves;

	int x = 0;
	int y = 0;
	x += current[0];
	y += current[1];
	int count = -1;
	while((x > 0 && y > 0)){
		if(!this->board->isEmpty(current[0]+count,current[1]+count)){
 			if(this->board->capturablePiece(current[0]+count,current[1]+count)){
				int* move = new int[2];
				move[0] = count;
				move[1] = count;
				moves.push_back(move);
			}
			break;
		}
		if(this->validMove(current[0]+count,current[1]+count)){
			int* move = new int[2];
			move[0] = count;
			move[1] = count;
			moves.push_back(move);
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
				int* move = new int[2];
				move[0] = count;
				move[1] = count;
				moves.push_back(move);
			}
			break;
		}
		if(this->validMove(current[0]+count,current[1]+count)){
			int* move = new int[2];
			move[0] = count;
			move[1] = count;
			moves.push_back(move);
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
				int* move = new int[2];
				move[0] = count;
				move[1] = -1*count;
				moves.push_back(move);
			}
			break;
		}
		if(this->validMove(current[0]+count,current[1]-count)){
			int* move = new int[2];
			move[0] = count;
			move[1] = -1*count;
			moves.push_back(move);
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
				int* move = new int[2];
				move[0] = -1*count;
				move[1] = count;
				moves.push_back(move);
			}
			break;
		}
		if(this->validMove(current[0]-count,current[1]+count)){
			int* move = new int[2];
			move[0] = -1*count;
			move[1] = count;
			moves.push_back(move);
		}
		x--;
		y++;
		count++;
	}
	return moves;
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
		int i = current[1];	
		int j = current[0];	

		while(i != x && j != y){
			if(x > i && y > j){
				i++;
				j++;
			}
			if(x < i && y > j){
				i--;
				j++;
			}

			if((x < i && y < j) || (x > i && y < j)){
				return this->board->capturablePiece(x,y);
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
std::vector<int*> Morph::Rook::getMoves(){
	int *current = this->getPosition();
	std::vector<int*> moves;

	int j = 0;
	int i = 0;
	j += current[0];
	i += current[1];
	int count = j-1;

	while(count > 0){
		if(this->board->capturablePiece(count,i)){
			int* move = new int[2];
			move[0] = count-j;
			move[1] = 0;
			moves.push_back(move);
			break;
		}
		if(!this->board->isEmpty(count,i)){
			break;
		}
		if(this->board->isEmpty(count,i)){
			int* move = new int[2];
			move[0] = count-j;
			move[1] = 0;
			moves.push_back(move);
		}
		count--;
	} 

	count = i-1;
	while(count > 0){
		if(this->board->capturablePiece(j,count)){
			int* move = new int[2];
			move[0] = 0;
			move[1] = count-i;
			moves.push_back(move);
			break;
		}
		if(!this->board->isEmpty(count,i)){
			break;
		}
		if(this->board->isEmpty(j,count)){
			int* move = new int[2];
			move[0] = 0;
			move[1] = count-i;
			moves.push_back(move);
		}
		count--;
	} 

	count = i+1;
	while(count < 7){
		if(this->board->capturablePiece(j,count)){
			int* move = new int[2];
			move[0] = 0;
			move[1] = count-i;
			moves.push_back(move);
			break;
		}
		if(!this->board->isEmpty(count,i)){
			break;
		}
		if(this->board->isEmpty(j,count)){
			int* move = new int[2];
			move[0] = 0;
			move[1] = count-i;
			moves.push_back(move);
		}
		count++;
	} 

	count = j+1;
	while(count < 9){
		if(this->board->capturablePiece(count,i)){
			int* move = new int[2];
			move[0] = count-j;
			move[1] = 0;
			moves.push_back(move);
			break;
		}
		count++;
	} 
	return moves;
}

bool Morph::Rook::validMove(int x, int y){
	int* current = this->getPosition();
	int i = current[1];
	int j = current[0];
	if(x != i && y != j){
		return false;
	}

	Morph::Piece *cur = NULL;
	cur = this->board->getPiece(x,y);

	if(x < i){
		while(x != i){
			i--;
			if(!this->board->isEmpty(j,i) && this->board->getPiece(j,i)->isPlayer() && x != i && y != j){
				return false;
			}
		}
		if(this->board->getPiece(j,i)->isPlayer() && x != i && y != j){
			return false;
		}
	}
	if(x > i){
		while(x != i){
			i++;
			if(!this->board->isEmpty(j,i) && x != i && y != j){
				return false;
			}
		}
	}
	if(y < j){
		while(y != j){
			j--;
			if(!this->board->isEmpty(j,i) && this->board->getPiece(j,i)->isPlayer() && x != i && y != j){
				return false;
			}
		}
		if(this->board->getPiece(j,i)->isPlayer() && x != i && y != j){
			return false;
		}
	}
	if(y > j){
		while(y != j){
			j++;
			if(!this->board->isEmpty(j,i) && x != i && y != j){
				return false;
			}
		}
	}

	return true;
}

char Morph::Rook::getChar(){
	if(isPlayer()){
		return 'r';
	}
	return 'R';
}

//Piece -- Knight
std::vector<int*> Morph::Knight::getMoves(){
	int *current = this->getPosition();
	std::vector<int*> moves;

	if(this->validMove(current[0]-2,current[1]+1)){
		int* move = new int[2];
		move[0] = -2;
		move[1] = 1;
		moves.push_back(move);
	}
	if(this->validMove(current[0]-2,current[1]-1)){
		int* move = new int[2];
		move[0] = -2;
		move[1] = -1;
		moves.push_back(move);
	}
	if(this->validMove(current[0]-1,current[1]-2)){
		int* move = new int[2];
		move[0] = -1;
		move[1] = -2;
		moves.push_back(move);
	}
	if(this->validMove(current[0]-1,current[1]+2)){
		int* move = new int[2];
		move[0] = -1;
		move[1] = 2;
		moves.push_back(move);
	}
	if(this->validMove(current[0]+1,current[1]+2) && this->board->capturablePiece(current[0]+1,current[1]+2)){
		int* move = new int[2];
		move[0] = 1;
		move[1] = 2;
		moves.push_back(move);
	}
	if(this->validMove(current[0]+1,current[1]-2) && this->board->capturablePiece(current[0]+1,current[1]-2)){
		int* move = new int[2];
		move[0] = 1;
		move[1] = -2;
		moves.push_back(move);
	}
	if(this->validMove(current[0]+2,current[1]-1) && this->board->capturablePiece(current[0]+2,current[1]-1)){
		int* move = new int[2];
		move[0] = 2;
		move[1] = -1;
		moves.push_back(move);
	}
	if(this->validMove(current[0]+2,current[1]+1) && this->board->capturablePiece(current[0]+2,current[1]+1)){
		int* move = new int[2];
		move[0] = 2;
		move[1] = 1;
		moves.push_back(move);
	}
	return moves;
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
	if(this->isPlayer()){
		return 'n';
	}
	return 'N';
}
//Piece -- Pawn
std::vector<int*> Morph::Pawn::getMoves(){
	std::vector<int*> moves;
	if(!this->isPlayer()){
		int* move = new int[2];
		move[0] = -1;
		move[1] = 0;
		moves.push_back(move);
	}
	return moves;
}
bool Morph::Pawn::validMove(int x, int y){
	int *current = this->getPosition();

	int corner_1[2],corner_2[2],corner_3[2],corner_4[2];

	corner_1[0] = current[0] + 1;
	corner_1[1] = current[1] + 1;

	corner_2[0] = current[0] - 1;
	corner_2[1] = current[1] + 1;

	corner_3[0] = current[0] + 1;
	corner_3[1] = current[1] - 1;

	corner_4[0] = current[0] - 1;
	corner_4[1] = current[1] - 1;

	Morph::Piece *cur = NULL;
	cur = this->board->getPiece(x,y);

	if(cur != NULL){
		if(x == corner_1[0] && y == corner_1[1]){
			if(cur->isPlayer() == this->isPlayer()){
				return false;
			}
		}
		if(x == corner_2[0] && y == corner_2[1]){
			if(cur->isPlayer() == this->isPlayer()){
				return false;
			}
		}
		if(x == corner_3[0] && y == corner_3[1]){
			if(cur->isPlayer() == this->isPlayer()){
				return false;
			}
		}
		if(x == corner_4[0] && y == corner_4[1]){
			if(cur->isPlayer() == this->isPlayer()){
				return false;
			}
		}
	}else{
		if(x == corner_1[0] && y == corner_1[1]){
			return false;
		}
		if(x == corner_2[0] && y == corner_2[1]){
			return false;
		}
		if(x == corner_3[0] && y == corner_3[1]){
			return false;
		}
		if(x == corner_4[0] && y == corner_4[1]){
			return false;
		}
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
std::vector<int*> Morph::King::getMoves(){
	std::vector<int*> moves;
	int *current = this->getPosition();
	if(!this->isPlayer()){
		if(current[1] + 1 < 7){
			int* move = new int[2];
			move[0] = 0;
			move[1] = 1;
			moves.push_back(move);
		}
	}else{
		if(current[1] + 1 > 0 && current[1]+1 < 7){
			int* move = new int[2];
			move[0] = 0;
			move[1] = -1;
			moves.push_back(move);
		}
	}

	return moves;
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

