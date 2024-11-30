#pragma once


class IDamage{
public:
	void ApplyDamage(float damage) {
		TakeDamage(damage);
	}

protected:
	virtual void TakeDamage(float damage) = 0;
};

