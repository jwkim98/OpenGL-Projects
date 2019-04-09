#include <Snowman.hpp>
#include <PickingMaterial.hpp>
#include <cmath>

Snowman::Snowman(int headIndex, int bodyIndex)
{
	material.CreateMaterial();

	picking_material.CreateMaterial();

	geometry.GenerateSphere(&sphere_mesh1, 1.0);
	geometry.GenerateSphere(&sphere_mesh2, 2.0);
	geometry.GenerateCone(&cone_mesh, 0.5, 1.0);
	geometry.GenerateCylinder(&cylinder_mesh, 0.3, 0.8);

	_head = PickableObject(&sphere_mesh1, &material);
	_head.SetPickingMat(&picking_material);
	_head.SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	_head.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	_head.SetIndex(headIndex);


	_body = PickableObject(&sphere_mesh2, &material);
	_body.SetPickingMat(&picking_material);
	_body.SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	_body.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	_body.SetIndex(bodyIndex);
	// TODO: Add parent (cube2) to cube1 (Slide No. 14)
	_body.AddParent(&_head);

	_nose = PickableObject(&cone_mesh, &material);
	_nose.SetPickingMat(&picking_material);
	_nose.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	_nose.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	_nose.SetPosition(glm::vec3(-0.5f, 0.0f, 0.0f));
	_nose.SetIndex(3);
	// TODO: Add parent (cube2) to cube1 (Slide No. 14)
	_nose.AddParent(&_head);

	_rightArm = PickableObject(&cylinder_mesh, &material);
	_rightArm.SetPickingMat(&picking_material);
	_rightArm.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	_rightArm.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	_rightArm.SetOrientation(glm::rotate(_rightArm.GetOrientation(), glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	_rightArm.SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
	_rightArm.SetIndex(4);
	_rightArm.AddParent(&_body);

	_leftArm = PickableObject(&cylinder_mesh, &material);
	_leftArm.SetPickingMat(&picking_material);
	_leftArm.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	_leftArm.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	_leftArm.SetOrientation(glm::rotate(_leftArm.GetOrientation(), glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	_leftArm.SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	_leftArm.SetIndex(5);
	_leftArm.AddParent(&_body);

}

Snowman::~Snowman()
{

}


void Snowman::RotateBody(float speed)
{
	_body.SetOrientation(glm::rotate(_body.GetOrientation(), glm::radians(speed), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Snowman::RotateHead(double x_angle, double y_angle)
{
	//auto currentPos = _head
	//_head->SetPosition(glm::vec3(0, 0, 0));
	//double gradient = -(x_angle / y_angle);
	glm::vec3 vect(y_angle, x_angle, 0);
	double distance = sqrt(pow(x_angle, 2) + pow(y_angle, 2));
	//_head->SetOrientation(glm::rotate(_head->GetOrientation(), glm::radians(static_cast<float>(x_angle)), glm::vec3(0 , 1, 0.0f)));
	//_head->SetOrientation(glm::rotate(_head->GetOrientation(), glm::radians(static_cast<float>(y_angle)), glm::vec3(1,0,0)));
	_head.SetOrientation(glm::rotate(_head.GetOrientation(), glm::radians(static_cast<float>(-distance)), vect));

}

void Snowman::RotateHead()
{
	_head.SetOrientation(glm::rotate(_head.GetOrientation(), glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Snowman::MoveArm(double speed, float elapsed_time)
{
	static mat4 _rightInitialOrientation = _rightArm.GetOrientation();
	static mat4 _leftInitialOrientation = _leftArm.GetOrientation();

	if(_armDirection)
		_armPosition += speed * elapsed_time;
	else
		_armPosition -= speed * elapsed_time;

	if (_armPosition > _armThreshold)
		_armDirection = false;

	if (_armPosition < -_armThreshold)
		_armDirection = true;

	_rightArm.SetOrientation(glm::rotate(_rightInitialOrientation, glm::radians(static_cast<float>(_armPosition)), vec3(1.0f, 0.0f, 0.0f)));
	_leftArm.SetOrientation(glm::rotate(_leftInitialOrientation, glm::radians(static_cast<float>( _armPosition)), vec3(1.0f, 0.0f, 0.0f)));
}

void Snowman::MoveNose(double speed, float elapsed_time)
{
	static mat4 _noseInitialOrientation = _nose.GetOrientation();

	if (_noseDirection)
		_nosePosition += speed * elapsed_time;
	else
		_nosePosition -= speed * elapsed_time;

	if (_nosePosition > _noseThreshold)
		_noseDirection = false;

	if (_nosePosition < -_noseThreshold)
		_noseDirection = true;

	_nose.SetOrientation(glm::rotate(_noseInitialOrientation, glm::radians(static_cast<float>(_nosePosition)), vec3(1.0f, 0.0f, 0.0f)));
		
}

void Snowman::Render(Engine::Camera* camera)
{

	material.UpdateColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_body.Render(camera);
	material.UpdateColor(glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
	_head.Render(camera);
	material.UpdateColor(glm::vec4(1.0f, 0.2f, 1.0f, 0.0f));
	_leftArm.Render(camera);
	_rightArm.Render(camera);
	_nose.Render(camera);
}
void Snowman::RenderPicking(Engine::Camera* camera)
{
	_body.RenderPicking(camera);
	_head.RenderPicking(camera);
}

void Snowman::SetPosition(vec3 position)
{
	_head.SetPosition(position);
}

void Snowman::Move(vec3 position)
{
	_head.SetPosition(_head.GetPosition() + position);
}

bool Snowman::Jump(float elapsed_time, float velocity, bool start)
{

	if (start)
	{
		_totalTime = 0.1;
		_initialPos = _head.GetPosition();
	}
	else
	{
		_totalTime += elapsed_time;

		const float gravity = 5.0;
		float distance = velocity * _totalTime - (1.0f / 2.0f)*gravity*_totalTime*_totalTime;
		//std::cout << "distance: "<< distance << std::endl;
		if (distance <= 0)
		{
			//std::cout << "Jumped! " << std::endl;
			return false;
		}
		vec3 jumpVec(0, distance, 0);
		_head.SetPosition(_initialPos + jumpVec);
	}

	return true;
}

vec3 Snowman::GetLocation()
{
	return _head.GetPosition();
}