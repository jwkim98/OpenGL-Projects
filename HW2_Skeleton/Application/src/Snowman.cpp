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

void Snowman::rotate_head()
{
	head->SetOrientation(glm::rotate(head->GetOrientation(), glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}