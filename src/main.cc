#include "main.h"
#include "Morph.h"

void testIsEmpty(Morph::Board *board);

int main(int argc, char** argv){
	Morph::Board *board = new Morph::Board();
	board->init();

	//Morph::Bishop *b1 = new Morph::Bishop(6,6);
	Morph::King *k1 = new Morph::King(1,5,true);

	Morph::King *k2 = new Morph::King(8,2,false);

	//board->addPiece(b1);
	board->addPiece(k1);
	board->addPiece(k2);

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
