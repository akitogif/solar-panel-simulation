#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <algorithm>

std::vector<double> readIrradianceCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<double> irradiance(24, 0.0);
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string hourStr, irradianceStr;
        std::getline(ss, hourStr, ',');
        std::getline(ss, irradianceStr);
        int hour = std::stoi(hourStr);
        double value = std::stod(irradianceStr);
        if (hour >= 0 && hour < 24)
            irradiance[hour] = value;
    }
    return irradiance;
}

void drawGraph(const std::vector<double>& powerOutput) {
    unsigned int width = 800, height = 600;
    sf::RenderWindow window(sf::VideoMode({width, height}), "Solar Power Output");
    std::cout << "Window created\n";
    while (window.isOpen()) {

      while (const::std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
          window.close();
        }
      }
      
      window.clear(sf::Color::White);

      double maxPower = *std::max_element(powerOutput.begin(), powerOutput.end());

      for (int i = 0; i < 23; ++i) {
        float x1 = (i) * (width / 24.0f);
        float y1 = height - (powerOutput[i] / maxPower) * height * 0.9;
        float x2 = (i + 1) * (width / 24.0f);
        float y2 = height - (powerOutput[i + 1] / maxPower) * height * 0.9;

        sf::Vertex line[] = {
            sf::Vertex{sf::Vector2f{x1,y1}, sf::Color::Blue},
            sf::Vertex{sf::Vector2f{x2, y2}, sf::Color::Blue}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
      }
      std::cout << "Running loop\n";
      window.display();
    }
}

int main() {
    std::vector<double> irradiance = readIrradianceCSV("data.csv");

    const double panelArea = 1.6;
    const double efficiency = 0.18;

    std::vector<double> powerOutput(24);
    for (int i = 0; i < 24; ++i) {
        powerOutput[i] = irradiance[i] * panelArea * efficiency;
    }

    drawGraph(powerOutput);
    return 0;
}
