#ifndef OPTION_SCENE_H_
#define OPTION_SCENE_H_

enum class Option {
	NullSelect,
	CameraOption,
	SoundOption
};


class OptionScene {
public:
	OptionScene();
	void initialize();
	void update(float delta_time);
	void draw()const;
	void null_select_update();
	void camera_option_update();
	void sound_option_update();
	void camera_option_draw()const;
	void sound_option_draw()const;
	bool is_end();
	bool is_reverse_camera_y();
	bool is_reverse_camera_x();
private:
	Option option_{};
	bool is_end_{ false };
	bool is_reverse_camera_y_{ false };
	bool is_reverse_camera_x_{ false };
	bool is_camera_option_{ true };
	float volume_BGM_{};
	float volume_SE_{};
	GSvector2 position_{};
	GSvector4 rect_{};

};



#endif // !OPTION_SCENE_H_
