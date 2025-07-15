#ifndef BOUNDING_SPHERE_H_
#define BOUNDING_SPHERE_H_

#include<gslib.h>

class BoundingSphere {
public:
	BoundingSphere(float radius = 0.0f,
		const GSvector3& center = GSvector3{ 0.0f,0.0f,0.0f });
	BoundingSphere translate(const GSvector3& posititon)const;
	BoundingSphere transform(const GSmatrix4& matrix)const;
	bool intersects(const BoundingSphere& other)const;

	void draw()const;

public:
	float radius{ 0.0f };
	GSvector3 center{ 0.0f,0.0f,0.0f };
};


#endif // !BOUNDING_SPHERE_H_
