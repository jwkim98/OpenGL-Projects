#include <Animation.hpp>

Animation::Animation()
{
    _snowflakes = std::vector<Snowflake*>();
}

Animation::~Animation()
{
    _snowflakes.clear();
}

void Animation::AddSnowflake(Snowflake* snowflake)
{
    _snowflakes.emplace_back(snowflake);
}

void Animation::Animate(float deltaTime)
{
	float restartThreshold = -5;
	float restartReverseThreshold = 4.9;

	for (auto snowflake : _snowflakes)
	{
		float verticalSpeed = snowflake->_verticalSpeed*0.1f ;
		float horizontalSpeed = snowflake->_horizontalSpeed*0.1f;
		glm::vec3 startPosition = glm::vec3(snowflake->_startPositionX, snowflake->_startPositionY, 0);
		///Position will be differed by speed of this snowflake
		glm::vec3 currentPosition = snowflake->GetPosition();

		if (Mode) {
			if (currentPosition.y < restartThreshold)
			{
				snowflake->SetPosition(startPosition);
				snowflake->_previousElapsedTime = deltaTime;

			}
			else
			{
				// Time elpased since initialized (Set to original position)
				float elapsedTime = deltaTime - snowflake->_previousElapsedTime;
				snowflake->SetPosition(startPosition + glm::vec3(elapsedTime*verticalSpeed, 0.5f*pow(elapsedTime, 2)*horizontalSpeed, 0));
				snowflake->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(50.0f), glm::vec3(currentPosition.x, currentPosition.y, 0.1)));
			}
		}
		else 
		{
			if (currentPosition.y > restartReverseThreshold)
			{
				snowflake->SetPosition(glm::vec3(startPosition.x, -5.0f, 0.0f));
				snowflake->_previousElapsedTime = deltaTime;

			}
			else
			{
				float elapsedTime = deltaTime - snowflake->_previousElapsedTime;

				snowflake->SetPosition(glm::vec3(startPosition.x, -5.0f, 0.0f) + glm::vec3(elapsedTime*verticalSpeed, -0.5f*pow(elapsedTime, 2)*horizontalSpeed, 0));
				snowflake->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(50.0f), glm::vec3(currentPosition.x, currentPosition.y, 0.1)));
			}

		}
	}
}

void Animation::Render(Engine::Camera& camera)
{
	for (Snowflake* snowflakePtr : _snowflakes)
	{
		snowflakePtr->Render(&camera);
	}
}