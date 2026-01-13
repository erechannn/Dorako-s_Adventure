#ifndef OPTION_SCENE_H_
#define OPTION_SCENE_H_

enum class Option {
	CameraOption,
	SoundOption
};


class OptionScene {
public:
	OptionScene();
	void initialize();
	void update(float delta_time);
	void draw()const;
	void camera_option_update();
	void sound_option_update();
	void camera_option_draw()const;
	void sound_option_draw()const;
	bool is_end();
private:
	Option option_{};
	bool is_end_{ false };

};



#endif // !OPTION_SCENE_H_
