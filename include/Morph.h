#include "main.h"

namespace Morph{

	class Piece{
		public:
			int position[2];
			Piece(int x, int y){
				position[0] = x;
				position[1] = y;
			}
			virtual ~Piece(){}
			virtual void getMoves() = 0;
			virtual char getChar() = 0;
			int* getPosition();
			void setPosition(int x, int y);
		private:
			
	};

	class Board{
		private:
			std::vector< std::vector<int> > matrix;
			std::vector< Morph::Piece* > active_pieces;
		public:
			void init();
			void print();
			void getNextMove();
			void addPiece(Morph::Piece *piece);
			char getPieceChar(int x,int y);
			Morph::Piece *getPiece(int x);
			Morph::Piece *getPiece(int x,int y);
			bool isEmpty(int x, int y);
			void moveParser(std::string move);
			int letterParser(char letter);
	};


	class Bishop: public Piece{
		public:
			Bishop(int x, int y) : Piece(x,y){ };
			void getMoves();
			char getChar();
	};

	class Rook: public Piece{
		public:
			Rook(int x, int y) : Piece(x,y){ };
			void getMoves();
			char getChar();
	};

	class Knight: public Piece{
		public:
			Knight(int x, int y) : Piece(x,y){ };
			void getMoves();
			char getChar();
	};

	class Pawn: public Piece{
		public:
			Pawn(int x, int y) : Piece(x,y){ };
			void getMoves();
			char getChar();
	};

	class King: public Piece{
		public:
			King(int x, int y) : Piece(x,y){ };
			void getMoves();
			char getChar();
	};
}
