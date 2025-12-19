#ifndef PAUSE_SCENE_H_
#define PAUSE_SCENE_H_


enum class SelectState {
	SelectNull=0,
	RestartGame,
	Option,
	ResetStage,
	StageSelect,
	Title,
	Max
};

class PauseScene {
public:
	PauseScene();
	void initialize();
	void update(float delta_time);
	void draw()const;
	bool is_end();
private:
	SelectState to_state(int value);
	void pause_update(float delta_time);
	void restart_game_update(float delta_time);
	void option_game_update(float delta_time);
	void reset_stage_update(float delta_time);
	void stage_select_update(float delta_time);
	void title_update(float delta_time);
	void draw_back_ground()const;
	void draw_pause_menu()const;
	void draw_option()const;
	void draw_confirm_window()const;
private:
	SelectState select_state_{};
	bool is_end_{false};
	int select_num_{ 0 };
};






#endif
