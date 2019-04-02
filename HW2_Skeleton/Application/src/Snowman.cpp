#include <Snowman.hpp>
Snowman::Snowman()
{

}

Snowman::~Snowman()
{

}


void Snowman::rotate_body()
{
	body->SetOrientation(glm::rotate(body->GetOrientation(), glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Snowman::rotate_head(double x_angle, double y_angle)
{
	//auto currentPos = head
	//head->SetPosition(glm::vec3(0, 0, 0));
	head->SetOrientation(glm::rotate(head->GetOrientation(), glm::radians(static_cast<float>(x_angle)), glm::vec3(0 , 1, 0.0f)));
	head->SetOrientation(glm::rotate(head->GetOrientation(), glm::radians(static_cast<float>(y_angle)), glm::vec3(1,0,0)));
	
}

void Snowman::rotate_head()
{
	head->SetOrientation(glm::rotate(head->GetOrientation(), glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}