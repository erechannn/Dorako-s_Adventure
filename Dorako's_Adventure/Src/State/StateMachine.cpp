#include"StateMachine.h"
#include"StateNull.h"

static StateNull null_state;

StateMachine::StateMachine()
	:current_state_{ &null_state } {

}
StateMachine::~StateMachine() {
	current_state_ = &null_state;
	for (auto state : states_) {
		delete state.second;
	}
	states_.clear();
}
void StateMachine::update(float delta_time) {
	current_state_->update(delta_time);
}
void StateMachine::change_state(int next_state_name) {
	IState* next_state = states_[next_state_name];

	current_state_->end();
	current_state_ = next_state;
	current_state_->start();
	now_state_ = next_state_name;
}
void StateMachine::add_state(int state_id, IState* state) {
	states_.insert(std::pair<int, IState*>(state_id, state));
}