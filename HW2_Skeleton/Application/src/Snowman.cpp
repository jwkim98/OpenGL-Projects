#include <Snowman.hpp>
#include <cmath>

Snowman::Snowman()
{

}

Snowman::~Snowman()
{

}


void Snowman::rotate_body()
{
	Body->SetOrientation(glm::rotate(Body->GetOrientation(), glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Snowman::rotate_head(double x_angle, double y_angle)
{
	//auto currentPos = Head
	//Head->SetPosition(glm::vec3(0, 0, 0));
	//double gradient = -(x_angle / y_angle);
	glm::vec3 vect(y_angle, x_angle, 0);
	double distance = sqrt(pow(x_angle, 2) + pow(y_angle, 2));
	//Head->SetOrientation(glm::rotate(Head->GetOrientation(), glm::radians(static_cast<float>(x_angle)), glm::vec3(0 , 1, 0.0f)));
	//Head->SetOrientation(glm::rotate(Head->GetOrientation(), glm::radians(static_cast<float>(y_angle)), glm::vec3(1,0,0)));
	Head->SetOrientation(glm::rotate(Head->GetOrientation(), glm::radians(static_cast<float>(-distance)), vect));

}

void Snowman::rotate_head()
{
	Head->SetOrientation(glm::rotate(Head->GetOrientation(), glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Snowman::move_arm(double speed, float elapsed_time)
{
	static mat4 _rightInitialOrientation = RightArm->GetOrientation();
	static mat4 _leftInitialOrientation = LeftArm->GetOrientation();

	if(_armDirection)
		_armPosition += speed * elapsed_time;
	else
		_armPosition -= speed * elapsed_time;

	if (_armPosition > _armThreshold)
		_armDirection = false;

	if (_armPosition < -_armThreshold)
		_armDirection = true;

	RightArm->SetOrientation(glm::rotate(_rightInitialOrientation, glm::radians(static_cast<float>(_armPosition)), vec3(1.0f, 0.0f, 0.0f)));
	LeftArm->SetOrientation(glm::rotate(_leftInitialOrientation, glm::radians(static_cast<float>( _armPosition)), vec3(1.0f, 0.0f, 0.0f)));
}

void Snowman::move_nose(double speed, float elapsed_time)
{
	static mat4 _noseInitialOrientation = Nose->GetOrientation();

	if (_noseDirection)
		_nosePosition += speed * elapsed_time;
	else
		_nosePosition -= speed * elapsed_time;

	if (_nosePosition > _noseThreshold)
		_noseDirection = false;

	if (_nosePosition < -_noseThreshold)
		_noseDirection = true;

	Nose->SetOrientation(glm::rotate(_noseInitialOrientation, glm::radians(static_cast<float>(_nosePosition)), vec3(1.0f, 0.0f, 0.0f)));
		
}