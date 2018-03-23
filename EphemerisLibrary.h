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

  void SetTargetTime(std::vector<int> &targetSpec);
  void SetTimes(std::vector<int> &timeSpec);
  void StartUp();
  void Preprocessing();
  void ProcessFile();
public:
  void Greeting();
  void Work();

};

#endif //EPHEMERIS_EPHEMERISLIBRARY_H
