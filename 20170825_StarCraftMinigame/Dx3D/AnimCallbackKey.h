#pragma once

struct AnimCallbackKey {
	float animPhase; //0.0f~1.0f
	int userData;

	AnimCallbackKey(float cbPhase, int data) 
	:animPhase(cbPhase), userData(data){

	}
};