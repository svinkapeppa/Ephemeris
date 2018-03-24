#include "EphemerisLibrary.h"

#include <iostream>
#include <fstream>
#include <sstream>

CApp::CApp() {
  x = std::vector<std::vector<double>> (8, std::vector<double>());
  y = std::vector<std::vector<double>> (8, std::vector<double>());
  z = std::vector<std::vector<double>> (8, std::vector<double>());
}

void CApp::Greeting() {
  std::cout << "This console app does laboratory work about ephemeris." << std::endl;
}

void CApp::Work() {
  StartUp();
  Preprocessing();
  ProcessFile();
  Calculate();
  PrintResults();
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
  std::ifstream file(dataPath);
  std::string line;
  bool correctDate = 0;

  while (std::getline(file, line)) {
    if (correctDate == 1) {
      for (auto i = 0; i < 8; ++i) {
        auto satelliteNumber = GetSatelliteNumber(satellites[i]);
        std::istringstream iss(line);

        auto found = line.find(satelliteNumber);

        if (found != std::string::npos) {
          std::string satellite;
          double xCoord;
          double yCoord;
          double zCoord;
          if ((iss >> satellite >> xCoord >> yCoord >> zCoord)) {
            x[i].push_back(xCoord);
            y[i].push_back(yCoord);
            z[i].push_back(zCoord);
          }
        }
      }
    }

    for (auto i = 0; i < 8; ++i) {
      auto date = GetDate(i);
      auto found = line.find(date);

      if (found != std::string::npos) {
        correctDate = 1;
      }
    }

    auto stopDate = GetStopDate();
    auto found = line.find(stopDate);

    if (found != std::string::npos) {
      correctDate = 0;
    }
  }

  file.close();
}

void CApp::Calculate() {
  for (auto i = 0; i < 8; ++i) {
    std::vector<double> tmpX;
    std::vector<double> tmpY;
    std::vector<double> tmpZ;

    for (auto j = 0; j < 8; ++j) {
      tmpX.push_back(x[i][j] * coefficients[j]);
      tmpY.push_back(y[i][j] * coefficients[j]);
      tmpZ.push_back(z[i][j] * coefficients[j]);
    }

    resX.push_back(Sum(tmpX));
    resY.push_back(Sum(tmpY));
    resZ.push_back(Sum(tmpZ));
  }
}

void CApp::PrintResults() {
  for (int i = 0; i < 8; ++i) {
    std::cout << resX[i] << " " << resY[i] << " " << resZ[i] << std::endl;
  }
}

void CApp::StartUp() {
  std::vector<int> targetSpec;
  std::cout << "Please specify your variant in format \'yyyy mm dd hh mm ss\'." << std::endl;
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

  SetDataPath();
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

void CApp::SetDataPath() {
  std::cout << "And finally please enter the path to the file with the data." << std::endl;
  std::cin >> dataPath;
}

std::string CApp::GetSatelliteNumber(int number) {
  std::string satelliteNumber = std::string("PG");

  if (number < 10) {
    satelliteNumber += std::string("0");
  }

  satelliteNumber += std::to_string(number);

  return satelliteNumber;
}

std::string CApp::GetDate(int i) {
  std::string date = std::to_string(varYear);

  date += std::string(" ");
  if (varMonth < 10) {
    date += std::string(" ");
  }
  date += std::to_string(varMonth);

  date += std::string(" ");
  if (varDay < 10) {
    date += std::string(" ");
  }
  date += std::to_string(varDay);

  date += std::string(" ");
  if (hours[i] < 10) {
    date += std::string(" ");
  }
  date += std::to_string(hours[i]);

  date += std::string(" ");
  if (minutes[i] < 10) {
    date += std::string(" ");
  }
  date += std::to_string(minutes[i]);

  return date;
}

std::string CApp::GetStopDate() {
  std::string stopDate = std::to_string(varYear);

  stopDate += std::string(" ");
  if (varMonth < 10) {
    stopDate += std::string(" ");
  }
  stopDate += std::to_string(varMonth);

  stopDate += std::string(" ");
  if (varDay < 10) {
    stopDate += std::string(" ");
  }
  stopDate += std::to_string(varDay);

  auto hour = hours[7];
  auto minute = minutes[7] + 15;
  if (minute >= 60) {
    minute %= 60;
    ++hour;
  }

  stopDate += std::string(" ");
  if (hour < 10) {
    stopDate += std::string(" ");
  }
  stopDate += std::to_string(hour);

  stopDate += std::string(" ");
  if (minute < 10) {
    stopDate += std::string(" ");
  }
  stopDate += std::to_string(minute);

  return stopDate;
}

double CApp::Sum(std::vector<double> &array) {
  double sum = 0;

  for(auto &elem : array) {
    sum += elem;
  }

  return sum;
}