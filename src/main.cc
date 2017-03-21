#include "main.h"
#include "Morph.h"

void testIsEmpty(Morph::Board *board);

int main(int argc, char** argv){
	Morph::Board *board = new Morph::Board();
	board->init();

	//Morph::Bishop *b1 = new Morph::Bishop(6,6);
	Morph::King *k1 = new Morph::King(1,5,true);

	Morph::King *k2 = new Morph::King(8,2,false);

	Morph::Knight *n1 = new Morph::Knight(7,1,false);
	Morph::Knight *n2 = new Morph::Knight(7,6,false);
	Morph::Knight *n3 = new Morph::Knight(2,1,true);
	Morph::Knight *n4 = new Morph::Knight(2,6,true);

	Morph::Pawn *p1 = new Morph::Pawn(7,3,false);
	Morph::Pawn *p2 = new Morph::Pawn(7,4,false);
	Morph::Pawn *p3 = new Morph::Pawn(2,3,true);
	Morph::Pawn *p4 = new Morph::Pawn(2,4,true);

	//board->addPiece(b1);
	board->addPiece(k1);
	board->addPiece(k2);
	board->addPiece(n1);
	board->addPiece(n2);
	board->addPiece(n3);
	board->addPiece(n4);
	board->addPiece(p1);
	board->addPiece(p2);
	board->addPiece(p3);
	board->addPiece(p4);

	//testIsEmpty(board);


	std::cout << std::endl << std::endl;

	board->print();
	std::cout << std::endl;
	while(1){
		std::cout << "Enter a move" << std::endl;
		std::string move;
		std::cin >> move;
		std::cout << std::endl;
		if(board->moveParser(move)){
			board->getNextMove();
			board->execute();
		}

		board->print();
		std::cout << std::endl;
	}

}

//(dynamic_cast <Morph::Bishop*> (board->getPiece(0)))->getMoves();
void testIsEmpty(Morph::Board *board){
	if(board->isEmpty(6,6)){
		std::cout << "Empty";
	}else{
		std::cout << "Filled";
	}

	if(board->isEmpty(6,1)){
		std::cout << "Empty";
	}else{
		std::cout << "Filled";
	}
	std::cout << std::endl;
}
