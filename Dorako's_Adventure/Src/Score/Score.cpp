#include "Score.h"
#include <gslib.h>
#include <algorithm>

Score::Score(int score) :
	score_{ score } {

}

void Score::initialize(int score) {
	score_ = score;
}
void Score::add(int score) {
	score_ = std::min(score_ + score, 99999);
}
void Score::draw()const {

}
int Score::get() const{
	return score_;
}
void Score::clear() {
}