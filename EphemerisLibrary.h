#ifndef EPHEMERIS_EPHEMERISLIBRARY_H
#define EPHEMERIS_EPHEMERISLIBRARY_H

#include <string>
#include <vector>

class CApp {
private:
  int varYear;
  int varMonth;
  int varDay;
  double targetTime;

  std::string dataPath;

  std::vector<int> satellites;
  std::vector<int> hours;
  std::vector<int> minutes;
  std::vector<double> times;
  std::vector<double> coefficients;

  std::vector<std::vector<double>> x;
  std::vector<std::vector<double>> y;
  std::vector<std::vector<double>> z;

  std::vector<double> resX;
  std::vector<double> resY;
  std::vector<double> resZ;

  void StartUp();

  void Preprocessing();

  void ProcessFile();

  void Calculate();

  void PrintResults();

  void SetTargetTime(std::vector<int> &targetSpec);

  void SetTimes(std::vector<int> &timeSpec);

  void SetDataPath();

  std::string GetSatelliteNumber(int number);

  std::string GetDate(int i);

  std::string GetStopDate();

  double Sum(std::vector<double> &array);
public:
  CApp();

  void Greeting();

  void Work();
};

#endif //EPHEMERIS_EPHEMERISLIBRARY_H
