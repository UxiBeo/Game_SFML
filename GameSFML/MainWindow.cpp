#include "MainWindow.h"
#include "imgui-SFML.h"
#include "imgui.h"
void MainWindow::ProcessEvent(Mouse& mouse, Keyboard& kbd, sf::RenderWindow& window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		switch (event.type)
		{
			/*----------------------Mouse Event---------------------*/
		case sf::Event::MouseMoved:
			mouse.OnMouseMove(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::MouseButtonPressed:
		{
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				mouse.OnLeftPressed();
				break;
			case sf::Mouse::Right:
				mouse.OnRightPressed();
				break;
			default:
				break;
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				mouse.OnLeftReleased();
				break;
			case sf::Mouse::Right:
				mouse.OnRightReleased();
				break;
			default:
				break;
			}
			break;
		}
		case sf::Event::MouseWheelScrolled:
		{
			if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
				mouse.OnWheel(event.mouseWheelScroll.delta);
			break;
		}
		break;
		//mouse cursor has entered the window
		case sf::Event::MouseEntered:
			break;
			//mouse cursor has left the window
		case sf::Event::MouseLeft:
			mouse.Flush();
			break;
			/*-------------------Mouse Event----------------------*/

			/*-------------------Keyboard Event-------------------*/
		case sf::Event::KeyPressed:
			if (ImGui::GetIO().WantCaptureKeyboard) 
			{
				break;
			}
			kbd.OnKeyPressed(event.key.code);
			break;
		case sf::Event::KeyReleased:
			if (ImGui::GetIO().WantCaptureKeyboard) 
			{
				break;
			}
			kbd.OnKeyReleased(event.key.code);
			break;

			/*-------------------Keyboard Event-------------------*/

			/*--------------------Window Event--------------------*/
			//happen when user closed the program by many way
		case sf::Event::Closed:
			window.close();
			break;
			//hapen when user change active window like alt+tab
		case sf::Event::LostFocus:
			mouse.Flush();
			break;
			//hapen when user comeback to program
		case sf::Event::GainedFocus:
			break;
			// catch the resize events
		case sf::Event::Resized:
		{
			// update the view to the new size of the window
			auto view = window.getView();
			view.setSize(sf::Vector2f((float)event.size.width, (float)event.size.height));
			window.setView(view);
			break;
		}
		/*---------------------Window Event-------------------*/
		default:
			break;
		}
	}
}
