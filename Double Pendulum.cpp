#include "pch.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

float veclength(sf::Vector2f v) {
	return std::hypot(v.x, v.y);
}

double map(double x, double from, double to, double from2, double to2) {
	return (x - from) / (to - from) * (to2 - from2) + from2;
}

int main()
{
	sf::Clock clock;

	sf::Font font;
	if (!font.loadFromFile("font.otf"))
	{
		std::cout << "No font file found!\n";
	}

	const unsigned int W = 500;
	const unsigned int H = 500; // you can change this to full window size later

	const double pi = 3.14159265359;

	sf::RenderWindow window(sf::VideoMode(W, H), "Double Pendulum!", sf::Style::Default);
	window.setFramerateLimit(120);

	sf::View view;
	view.setSize(sf::Vector2f(200.f, 200.f));
	view.setCenter(sf::Vector2f(0.f, 50.f));
	window.setView(view);

	double m1 = 5, m2 = 5, l1 = 30, l2 = 40, q1 = 0, q2 = 0, g = 500;
	double v1 = 0, v2 = 0, w1 = 0, w2 = 0;

	std::vector<sf::Vector2f> path;
	
	sf::Vector2f pos1, pos2;

	sf::CircleShape ball1(m1);
	ball1.setPointCount(100);
	ball1.setFillColor(sf::Color::Blue);

	sf::CircleShape ball2(m2);
	ball2.setPointCount(100);
	ball2.setFillColor(sf::Color::Blue);

	double r1 = q1 / 180.f * pi, r2 = q2 / 180.f * pi;

	sf::Vector2f firstp(l1 * std::sin(r1) + l2 * std::sin(r2), l1 * std::cos(r1) + l2 * std::cos(r2));

	bool pressedPrev = false;
	bool paused = false;

	bool grabed = false;
	bool selectedFirst;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Time deltatime = clock.getElapsedTime();
		double dt = deltatime.asSeconds();
		clock.restart();

		sf::Text pausedText;
		pausedText.setFont(font);
		pausedText.setString(paused ? "Paused" : "");
		pausedText.setCharacterSize(10);
		pausedText.setFillColor(sf::Color::White);
		pausedText.setPosition(sf::Vector2f(-20.f, -40.f));

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !grabed) {
			grabed = true;

			path.clear();
			firstp = pos2;

			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			if (mousePos.x < 0) {
				mousePos.x = 0;
			}
			if (mousePos.y < 0) {
				mousePos.y = 0;
			}
			if (mousePos.x > W) {
				mousePos.x = W;
			}
			if (mousePos.y > H) {
				mousePos.y = H;
			}
			mousePos.x = map(mousePos.x, 0, W, -100, 100);
			mousePos.y = map(mousePos.y, 0, H, -50, 150);
			selectedFirst = veclength(sf::Vector2f(mousePos) - pos1) < veclength(sf::Vector2f(mousePos) - pos2);
		}
		else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			grabed = false;
			ball1.setFillColor(sf::Color::Blue);
			ball2.setFillColor(sf::Color::Blue);
		}

		if(grabed){
			path.clear();
			firstp = pos2;
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			if (mousePos.x < 0) {
				mousePos.x = 0;
			}
			if (mousePos.y < 0) {
				mousePos.y = 0;
			}
			if (mousePos.x > W) {
				mousePos.x = W;
			}
			if (mousePos.y > H) {
				mousePos.y = H;
			}
			mousePos.x = map(mousePos.x, 0, W, -100, 100);
			mousePos.y = map(mousePos.y, 0, H, -50,  150);
			v1 = 0;
			v2 = 0;
			if (selectedFirst) {
				ball1.setFillColor(sf::Color::Green);
				ball2.setFillColor(sf::Color::Blue);
				r1 = - std::atan2(mousePos.y, mousePos.x) + pi / 2;
			}
			else {
				ball2.setFillColor(sf::Color::Green);
				ball1.setFillColor(sf::Color::Blue);
				ball2.setPosition(sf::Vector2f(mousePos));
				r2 = - std::atan2(mousePos.y - pos1.y, mousePos.x - pos1.x) + pi/2;
			}
		}


		if (!paused) {
		
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			m1 += 0.1;
			if (m1 > 10) m1 = 1;
			ball1.setRadius(m1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			m1 -= 0.1;
			if (m1 < 1) {
				m1 = 9;
			}
			ball1.setRadius(m1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			l1++;
			l1 = int(l1) % 70;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			l1--;
			if (l1 < 5) {
				l1 = 69;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			m2 += 0.1;
			if (m2 > 10) m2 = 1;
			ball2.setRadius(m2);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			m2 -= 0.1;
			if (m2 < 1) {
				m2 = 9;
			}
			ball2.setRadius(m2);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			l2++;
			l2 = int(l2) % 70;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			l2--;
			if (l2 < 5) {
				l2 = 69;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if (!pressedPrev) {
				pressedPrev = true;
				paused = !paused;
			}
		}
		else pressedPrev = false;

		if (!paused && !grabed) {

			double num1 = -g * (2 * m1 + m2) * std::sin(r1);
			double num2 = -m2 * g * std::sin(r1 - 2 * r2);
			double num3 = -2 * sin(r1 - r2) * m2;
			double num4 = v2 * v2 * l2 + v1 * v1 * l1 * std::cos(r1 - r2);
			double den = l1 * (2 * m1 + m2 - m2 * std::cos(2 * r1 - 2 * r2));
			w1 = (num1 + num2 + num3 * num4) / den;

			num1 = 2 * std::sin(r1 - r2);
			num2 = (v1 * v1 * l1 * (m1 + m2));
			num3 = g * (m1 + m2) * std::cos(r1);
			num4 = v2 * v2 * l2 * m2 * std::cos(r1 - r2);
			den = l2 * (2 * m1 + m2 - m2 * std::cos(2 * r1 - 2 * r2));
			w2 = (num1 * (num2 + num3 + num4)) / den;

			v1 += w1 * dt;
			v2 += w2 * dt;

			v1 *= 0.9995;
			v1 *= 0.9995;

			r1 += v1 * dt;
			r2 += v2 * dt;
		}

		pos1.x = l1 * std::sin(r1);
		pos1.y = l1 * std::cos(r1);
		pos2.x = pos1.x + l2 * std::sin(r2);
		pos2.y = pos1.y + l2 * std::cos(r2);

		ball1.setPosition(pos1 - sf::Vector2f(m1, m1));
		ball2.setPosition(pos2 - sf::Vector2f(m2, m2));

		if (!paused && !grabed) path.push_back(pos2);

		window.clear();

		sf::Vertex line1[] =
		{
			sf::Vertex(sf::Vector2f(0.f, 0.f)),
			sf::Vertex(pos1)
		};

		window.draw(line1, 2, sf::Lines);

		sf::Vertex line2[] =
		{
			sf::Vertex(pos1),
			sf::Vertex(pos2)
		};

		sf::Vector2f lastp = firstp;
		for (auto p : path) {
			sf::Vertex line[] =
			{
				sf::Vertex(lastp),
				sf::Vertex(p)
			};
			window.draw(line, 2, sf::Lines);
			lastp = p;
		}

		window.draw(line2, 2, sf::Lines);

		window.draw(ball1);
		window.draw(ball2);
		window.draw(pausedText);
		window.display();
	}

	return 0;
}