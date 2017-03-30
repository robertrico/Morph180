#include "main.h"
#include <algorithm>
#define MAXDEPTH 2

namespace Morph{

	class Board;
	class Piece;

	struct Move{
		Morph::Piece* piece;
		int* moves;
	};

	class Bishop;
	class Knight;
	class Rook;

	struct Transition{
		Morph::Bishop* bishop;
		Morph::Knight* knight;
		Morph::Rook* rook;
		int current;
	};

	class Piece{
		public:
			Piece(int x, int y,bool is_player){
				this->position[0] = x;
				this->position[1] = y;
				this->is_player = is_player;
				this->is_removed = false;
				this->child_piece = false;
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
			virtual void morph() = 0;
			void freeze();
			void revert();
			virtual std::vector<int*> getMoves() = 0;
			virtual char getChar() = 0;
			virtual bool validMove(int x, int y) = 0;
			int* getPosition();
			bool isPlayer();
			void setPosition(int x, int y);
			void setPosition(int x, int y, bool min_max_move);
			void setRemoved(bool rem);
			bool isRemoved();
			Morph::Board *board;
			Morph::Transition morph_states;
			bool morphable;
			bool child_piece;
			Morph::Piece* current_state;
		private:
			bool is_player;
			bool is_removed;
			int position[2];
			std::vector<int*> states;
			
	};

	class Board{
		private:
			std::vector< std::vector<int> > matrix;
		public:
			std::vector< Morph::Piece* > active_pieces;
			void init();
			void print();
			std::vector<struct Move> showMoves();
			void execute();
			int eval();
			bool endEval();
			void removePiece(int x, int y);
			double min(int depth,int alpha);
			double max(int depth, int beta);
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
			Bishop(int x, int y, bool is_player) : Piece(x,y,is_player){ 
				this->morphable = true;
			};
			std::vector<int*> getMoves();
			bool validMove(int x, int y);
			char getChar();
			void morph();
			void init(int x , int y, bool is_player);
	};

	class Rook: public Piece{
		public:
			Rook(int x, int y, bool is_player) : Piece(x,y,is_player){
				this->morphable = true;
			};
			std::vector<int*> getMoves();
			bool validMove(int x, int y);
			void init(int x , int y, bool is_player);
			char getChar();
			void morph();
	};

	class Knight: public Piece{
		public:
			Knight(int x, int y, bool is_player) : Piece(x,y,is_player){
				this->morphable = true;
			};
			std::vector<int*> getMoves();
			bool validMove(int x, int y);
			void init(int x , int y, bool is_player);
			char getChar();
			void morph();
	};

	class Pawn: public Piece{
		public:
			Pawn(int x, int y, bool is_player) : Piece(x,y,is_player){ 
				this->morphable = false;
			};
			std::vector<int*> getMoves();
			bool validMove(int x, int y);
			char getChar();
			void morph();
			bool morphable;
	};

	class King: public Piece{
		public:
			King(int x, int y, bool is_player) : Piece(x,y,is_player){
				this->morphable = false;
			};
			std::vector<int*> getMoves();
			bool validMove(int x, int y);
			char getChar();
			void morph();
			bool morphable;
	};
}
