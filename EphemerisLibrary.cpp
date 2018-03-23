#include "EphemerisLibrary.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void CApp::Greeting() {
  std::cout << "This console app does laboratory work about ephemeris." << std::endl;
}

void CApp::Work() {
  StartUp();
  Preprocessing();
  ProcessFile();
}

void CApp::SetTargetTime(std::vector<int> &targetSpec) {
  targetTime = (targetSpec[0] * 1.) + (targetSpec[1] / 60.) + (targetSpec[2] / 3600.);
}

void CApp::SetTimes(std::vector<int> &timeSpec) {
  auto startTime = (timeSpec[0] * 1.) + (timeSpec[1] / 60.);
  auto endTime = (timeSpec[2] * 1.) + (timeSpec[3] / 60.);
  auto epochs = static_cast<int>((endTime - startTime) / 0.25);

  auto hour = timeSpec[0];
  auto minute = timeSpec[1];

  for (auto i = 0; i <= epochs; ++i) {
    times.push_back(startTime + i * 0.25);
    hours.push_back(hour);
    minutes.push_back(minute);

    minute += 15;
    if (minute >= 60) {
      minute %= 60;
      hour += 1;
    }
  }
}

void CApp::StartUp() {
  std::vector<int> targetSpec;
  std::cout << "Please specify your variant in format \'yy mm dd hh mm ss\'." << std::endl;
  std::cin >> varYear >> varMonth >> varDay;

  for (auto i = 0; i < 3; ++i) {
    int x;
    std::cin >> x;
    targetSpec.push_back(x);
  }
  SetTargetTime(targetSpec);

  std::vector<int> timeSpec;
  std::cout << "Great. Now please specify the time period in format \'hh mm hh mm\'." << std::endl;
  for (auto i = 0; i < 4; ++i) {
    int x;
    std::cin >> x;
    timeSpec.push_back(x);
  }
  SetTimes(timeSpec);

  std::cout << "Almost there. Please enter satellites numbers in format \'nn nn ...\'." << std::endl;
  for (auto i = 0; i < 8; ++i) {
    int x;
    std::cin >> x;
    satellites.push_back(x);
  }

  std::cout << "And finally please enter the path to the file with the data." << std::endl;
  std::cin >> dataPath;
}

void CApp::Preprocessing() {
  std::vector<std::vector<double>> matrix;
  std::vector<double> difference;
  std::vector<double> dividend;
  std::vector<double> divider;

  for (auto i = 0; i < 8; ++i) {
    std::vector<double> tmp;

    difference.push_back(targetTime - times[i]);

    for (auto j = 0; j < 8; ++j) {
      tmp.push_back(times[i] - times[j]);
    }

    matrix.push_back(tmp);
  }

  for (auto i = 0; i < 8; ++i) {
    double product = 1;
    double subProduct = 1;

    for (auto j = 0; j < 8; ++j) {
      if (i != j) {
        product *= difference[j];
        subProduct *= matrix[i][j];
      }
    }

    dividend.push_back(product);
    divider.push_back(subProduct);
  }

  for (auto i = 0; i < 8; ++i) {
    coefficients.push_back(dividend[i] / divider[i]);
  }
}

void CApp::ProcessFile() {
  std::ifstream infile(dataPath);
  std::string line;
  auto i = 0;
  while (std::getline(infile, line)) {
    std::cout << i << ' ' << line << std::endl;
    ++i;
  }
  infile.close();
}