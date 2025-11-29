#ifndef SCORE_H_
#define SCORE_H_

class Score {
public:
	Score(int score = 0);
	void initialize(int score = 0);
	void add(int score);
	void draw()const;
	int get()const;
	void clear();
private:
	int score_;
};





#endif // !SCORE_H_
