#ifndef I_STATE_H_
#define I_STATE_H_

class IState {
public:
	virtual ~IState() = default;
	virtual void start() = 0;
	virtual void update(float delta_time) = 0;
	virtual void end() = 0;
};
#endif // !I_STATE_H_
