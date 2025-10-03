#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include"IState.h"
#include<unordered_map>

class StateMachine {
public:
	StateMachine();
	~StateMachine();

	void update(float delta_time);
	void change_state(int next_state_name);
	void add_state(int state_id, IState* state);
private:
	int now_state_{ 0 };
	IState* current_state_;
	//ƒyƒA‚Ì§ì
	std::unordered_map<int, IState*>states_;
	friend class Player;
};



#endif // !STATE_MACHINE_H_
