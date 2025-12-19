#include "Score.h"
#include "../UI/Number/NumberTexture.h"
#include "../Assets.h"
#include <gslib.h>
#include <algorithm>

Score::Score(int score) :
	score_{ score } {

}

void Score::initialize(int score) {
	score_ = score;
}
void Score::add(int score) {
	score_ = std::min(score_ + score, 99);
}
void Score::draw()const {
	static const NumberTexture number{ Texture_Number, 64, 128 };
	number.draw(GSvector2{ 0, 0 }, score_, 1);
}
int Score::get() const{
	return score_;
}
void Score::clear() {
	score_ = 0;
}