#ifndef SceneNull_H_
#define SceneNull_H_

#include "IScene.h"

//ヌルシーン
class SceneNull : public IScene {
public:
	//開始
	virtual void start() override {}
	//更新
	virtual void update(float delta_time) override {}
	//描画
	virtual void draw() const override {}
	//終了しているか？
	virtual bool is_end() const override {
		return false;
	}
	//次のシーンを返す
	virtual std::string next() const override {
		return "SceneNull";
	}
	//終了
	virtual void end() override {}
};

#endif // !1

