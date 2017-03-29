#include "main.h"
#include <algorithm>
#define MAXDEPTH 4

namespace Morph{

	class Board;
	class Piece;

	struct Move{
		Morph::Piece* piece;
		int* moves;
	};

	class Piece{
		public:
			Piece(int x, int y,bool is_player){
				this->position[0] = x;
				this->position[1] = y;
				this->is_player = is_player;
				this->is_removed = false;
			}
			virtual ~Piece(){
				std::vector<int*>::iterator it;
				if(this->states.size() != 0){
					for(it=this->states.begin(); it < this->states.end(); it++){
						delete[] *it;
						this->states.erase(it);
					}
				}

			}
			void freeze();
			void revert();
			virtual std::vector<int*> getMoves() = 0;
			virtual char getChar() = 0;
			virtual bool validMove(int x, int y) = 0;
			int* getPosition();
			bool isPlayer();
			void setPosition(int x, int y, bool min_max_move);
			void setRemoved(bool rem);
			bool isRemoved();
			Morph::Board *board;
		private:
			bool is_player;
			bool is_removed;
			int position[2];
			std::vector<int*> states;
			
	};

	class Board{
		private:
			std::vector< std::vector<int> > matrix;
			std::vector< Morph::Piece* > active_pieces;
		public:
			void init();
			void print();
			std::vector<struct Move> showMoves();
			void execute();
			int eval();
			bool endEval();
			void removePiece(int x, int y);
			double min(int depth);
			double max(int depth);
			void addPiece(Morph::Piece *piece);
			void replacePiece(int x, Morph::Piece* piece);
			char getPieceChar(int x,int y);
			Morph::Piece *getPiece(int x);
			Morph::Piece *getPiece(int x,int y);
			bool isEmpty(int x, int y);
			bool moveParser(std::string move);
			int letterParser(char letter);
			bool capturablePiece(int x, int y);
			bool capturableAIPiece(int x, int y);
	};


	class Bishop: public Piece{
		public:
			Bishop(int x, int y, bool is_player) : Piece(x,y,is_player){ };
			std::vector<int*> getMoves();
			bool validMove(int x, int y);
			char getChar();
	};

	class Rook: public Piece{
		public:
			Rook(int x, int y, bool is_player) : Piece(x,y,is_player){ };
			std::vector<int*> getMoves();
			bool validMove(int x, int y);
			char getChar();
	};

	class Knight: public Piece{
		public:
			Knight(int x, int y, bool is_player) : Piece(x,y,is_player){ };
			std::vector<int*> getMoves();
			bool validMove(int x, int y);
			char getChar();
	};

	class Pawn: public Piece{
		public:
			Pawn(int x, int y, bool is_player) : Piece(x,y,is_player){ };
			std::vector<int*> getMoves();
			bool validMove(int x, int y);
			char getChar();
	};

	class King: public Piece{
		public:
			King(int x, int y, bool is_player) : Piece(x,y,is_player){ };
			std::vector<int*> getMoves();
			bool validMove(int x, int y);
			char getChar();
	};
}
