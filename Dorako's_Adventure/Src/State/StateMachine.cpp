#include"StateMachine.h"
#include"StateNull.h"

static StateNull null_state;

StateMachine::StateMachine()
	:current_state_{ &null_state } {

}
StateMachine::~StateMachine() {
	//ì‚Á‚½ó‘Ô‚ğ‚·‚×‚Äíœ
	current_state_ = &null_state;
	for (auto state : states_) {
		delete state.second;
	}
	states_.clear();
}
void StateMachine::update(float delta_time) {
	current_state_->update(delta_time);
}
//ó‘Ô‚Ì•Ï‰»
void StateMachine::change_state(int next_state_name) {
	//Ÿ‚Ìó‘Ô‚ÌŒŸõ
	IState* next_state = states_[next_state_name];
	if (next_state != nullptr) {
		//Œ»İ‚Ìó‘Ô‚ğI‚í‚ç‚¹‚é
		current_state_->end();
		//Ÿ‚Ìó‘Ô‚É‚·‚é
		current_state_ = next_state;
		current_state_->start();
		//Œ»İ‚Ìó‘Ô‚ÌXV
		now_state_ = next_state_name;

	}
}
//ó‘Ô‚Ì’Ç‰Á
void StateMachine::add_state(int state_id, IState* state) {
	states_.insert(std::pair<int, IState*>(state_id, state));
}