
//----------
// includes
//----------
#include <cstddef>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "Life.h"

using namespace std;

//--------------
// AbstractCell
//--------------
AbstractCell::AbstractCell(){}

AbstractCell::AbstractCell(char state) : _state(state){}

AbstractCell::~AbstractCell(){}

bool AbstractCell::isAlive(){
    return false;
}

bool AbstractCell::countDiagonals(){
    return false;
}

void AbstractCell::updateLife(int neighbors){}

char AbstractCell::getState(){
    return _state;
}

AbstractCell * AbstractCell::clone() const{
    return nullptr;
}

//------------
// ConwayCell
//------------
ConwayCell::ConwayCell() : AbstractCell(), _life(false){}

ConwayCell::ConwayCell(char state) : AbstractCell(state){
    if (state == '*'){
        _life = true;
    }
    else if (state == '.'){
        _life = false;
    }
}

ConwayCell::~ConwayCell(){}

bool ConwayCell::isAlive(){
	return _life;
}

bool ConwayCell::countDiagonals(){
    return true;
}

void ConwayCell::updateLife(int neighbors){
    if (_life){
        if (neighbors < 2 || neighbors > 3){
            _life = false;
            _state = '.';
        }
    }
    else{
        if (neighbors == 3){
            _life = true;
            _state = '*';
        }
    }
}

ConwayCell * ConwayCell::clone() const{
	return new ConwayCell(*this);
}

//-------------
// FredkinCell
//-------------
FredkinCell::FredkinCell() : AbstractCell(), _life(false), _age(0){}

FredkinCell::FredkinCell(char state) : AbstractCell(state){
    if (state == '-'){
        _life = false;
        _age = 0;
    }
    else if (state == '+'){
        _life = true;
        _age = 10;
    }
    else if (state >= '0' && state <= '9'){
        _life = true;
        _age = state - '0';
    }
}

FredkinCell::~FredkinCell(){}

bool FredkinCell::isAlive(){
	return _life;
}

bool FredkinCell::countDiagonals(){
    return false;
}

void FredkinCell::updateLife(int neighbors){
	if (_life){
		if((neighbors%2) == 0){
			_life = false;
			_state = '-';
			_age = 0;
		}
		else{
			if(_age < 9){
				++_age;
				_state = _age + '0';
			}
			else{
				_age = 10;
				_state = '+';
			}
		}
	}
	else{
		if((neighbors%2) == 1){
			_life = true;
			_state = '0';
			_age = 0;
		}
	}
}

FredkinCell * FredkinCell::clone() const{
	return new FredkinCell(*this);
}

//
// Cell
//
Cell::Cell() : _handle_cell(new ConwayCell()) {}

Cell::Cell(char state){
	if (state == '.' || state == '*'){
		_handle_cell = new ConwayCell(state);
	}
	else if (state == '-' || (state >= '0' && state < '2')){
		_handle_cell = new FredkinCell(state);
	}
	else if (state == '+' || (state >= '2' && state <= '9')){
		_handle_cell = new ConwayCell('*');
	}
}

//This needs to be fixed
Cell::Cell(const Cell& rhs){
	_handle_cell = rhs->clone();
}

Cell& Cell::operator =(const Cell& rhs){
	delete _handle_cell;
	_handle_cell = rhs->clone();
	return *this;
}

Cell::~Cell(){
	delete _handle_cell;
}

AbstractCell * Cell::operator ->() const{
	return _handle_cell;
}

bool Cell::isAlive(){
	return _handle_cell->isAlive();
}

bool Cell::countDiagonals(){
	return _handle_cell->countDiagonals();
}

void Cell::updateLife(int neighbors){
	_handle_cell->updateLife(neighbors);
    if (_handle_cell->getState() == '+' || (_handle_cell->getState() >= '2' && _handle_cell->getState() <= '9')){
        _handle_cell = new ConwayCell('*');
    }
}

//------------------
// friend_functions
//------------------
ostream& operator <<(ostream& lhs, const AbstractCell& rhs){
	return lhs << rhs._state;
}

ostream& operator <<(ostream& lhs, const ConwayCell& rhs){
	return lhs << rhs._state;
}

ostream& operator <<(ostream& lhs, const FredkinCell& rhs){
	return lhs << rhs._state;
}

ostream& operator <<(ostream& lhs, const Cell& rhs){
	return lhs << *(rhs._handle_cell);
}
