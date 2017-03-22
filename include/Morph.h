#include "main.h"
#include <algorithm>

namespace Morph{

	class Board;

	class Piece{
		public:
			Piece(int x, int y,bool is_player){
				this->position[0] = x;
				this->position[1] = y;
				this->is_player = is_player;
			}
			virtual ~Piece(){
				std::vector<int*>::iterator it;
				for(it=this->moves.begin(); it < this->moves.end(); it++){
					delete *it;
				}
				for(it=this->moves.begin(); it < this->moves.end(); it++){
					delete *it;
				}

			}
			void freeze();
			void revert();
			virtual void getMoves() = 0;
			virtual char getChar() = 0;
			void showMoves();
			void addMove(int x, int y);
			virtual bool validMove(int x, int y) = 0;
			int* getPosition();
			bool isPlayer();
			void setPosition(int x, int y);
			Morph::Board *board;
		private:
			bool is_player;
			int position[2];
			std::vector<int*> moves;
			std::vector<int*> states;
			
	};

	class Board{
		private:
			std::vector< std::vector<int> > matrix;
			std::vector< Morph::Piece* > active_pieces;
		public:
			void init();
			void print();
			void getNextMove();
			void execute();
			void addPiece(Morph::Piece *piece);
			void replacePiece(int x, Morph::Piece* piece);
			char getPieceChar(int x,int y);
			Morph::Piece *getPiece(int x);
			Morph::Piece *getPiece(int x,int y);
			bool isEmpty(int x, int y);
			bool moveParser(std::string move);
			int letterParser(char letter);
			bool capturablePiece(int x, int y);
	};


	class Bishop: public Piece{
		public:
			Bishop(int x, int y, bool is_player) : Piece(x,y,is_player){ };
			void getMoves();
			bool validMove(int x, int y);
			char getChar();
	};

	class Rook: public Piece{
		public:
			Rook(int x, int y, bool is_player) : Piece(x,y,is_player){ };
			void getMoves();
			bool validMove(int x, int y);
			char getChar();
	};

	class Knight: public Piece{
		public:
			Knight(int x, int y, bool is_player) : Piece(x,y,is_player){ };
			void getMoves();
			bool validMove(int x, int y);
			char getChar();
	};

	class Pawn: public Piece{
		public:
			Pawn(int x, int y, bool is_player) : Piece(x,y,is_player){ };
			void getMoves();
			bool validMove(int x, int y);
			char getChar();
	};

	class King: public Piece{
		public:
			King(int x, int y, bool is_player) : Piece(x,y,is_player){ };
			void getMoves();
			bool validMove(int x, int y);
			char getChar();
	};
}
