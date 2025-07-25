#ifndef STATE_NULL_H_
#define STATE_NULL_H_

#include"IState.h"

class StateNull :public IState {
public:
	void update(float delta_time)override {};
	void start()override {};
	void end()override {};
};



#endif // !STATE_NULL_H_
